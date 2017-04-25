#include "main.h"

Vector tpangles;
Vector atTargets;
vector<int64_t> aimbotfriends = { };

bool* bSendPacket = nullptr;

enum VIEW_ANTIAIM_PITCH {
    Emotion = 1,
    Up = 2,
    Down = 3,
    Fakedown = 4,
    Custom = 5
};

enum VIEW_ANTIAIM_YAW {
    Backwards = 1,
    Jitter = 2,
    FJitter = 3,
    SlowSpin = 4,
    FastSpin = 5,
    LowerYaw = 6,
    SidewaysLeft = 7,
    SidewaysRight = 8,
    CustomAA = 9
};

void ownClanTag(std::string& skrt) {
    skrt.append("   ");
    skrt.erase(skrt.length() - 1, 1);
}

int last = 0;
auto MakeAntiAim(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, bool& bPacket) -> void {
    
    if(!vars.misc.enabled)
        return;
    
    static bool fakeswitch = false;
    static bool bFlip = false;
    bFlip = !bFlip;
    bool bAttack = true;
    
    if (weapon->GetNextPrimaryAttack() - pGlobals->interval_per_tick > local->GetTickBase() * pGlobals->interval_per_tick)
        bAttack = false;
        
        if(cmd->buttons & IN_ATTACK && (bAttack)) {
            
            bPacket = false;
            
        } else {
            
            bPacket = true;
            
            if (vars.misc.fakeaa) {
                
                fakeswitch = !fakeswitch;
                
                if (fakeswitch)
                    bPacket = false;
                    else bPacket = true;
                        
            }

            if(vars.misc.aaX > 0) {
                
                if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Emotion) {
                    cmd->viewangles.x = 82;
                }
                
                if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Up) {
                    cmd->viewangles.x = -89;
                }
                
                if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Down) {
                    cmd->viewangles.x = 179;
                }
                
                if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Fakedown) {
                    cmd->viewangles.x = 180;
                }
                
                if(vars.misc.aaX == VIEW_ANTIAIM_PITCH::Custom) {
                    if(bPacket)
                        cmd->viewangles.x = 89;
                    else
                        cmd->viewangles.x = vars.misc.customaa;
                }
                
            }
            
            if(vars.misc.aaY > 0) {
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::Jitter) {
                    static bool bjitter;
                    cmd->viewangles.y = bjitter ? 90 : -90;
                    bjitter = !bjitter;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FJitter) {
                    
                    if(fakeswitch) cmd->viewangles.y = 90; else cmd->viewangles.y = local->GetLowerBodyYawTarget();
                    
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SlowSpin) {
                    if(fakeswitch) cmd->viewangles.y = 46; else cmd->viewangles.y = atTargets.y;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::FastSpin) {
                    if(fakeswitch) cmd->viewangles.y = 46; else cmd->viewangles.y = atTargets.y;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::LowerYaw) { // Twist.
                    
                    static int fakeTick = 0;
                    fakeTick++;
                    
                    if (vars.misc.fakeaa) {
                        
                        if (!bSendPacket) {
                            
                            if (fakeTick < 300)
                                cmd->viewangles.y += 67.57f;
                            else if (fakeTick > 300)
                                cmd->viewangles.y -= 67.15f;
                            if (fakeTick < 300)
                                cmd->viewangles.y += 67.57f;
                            else if (fakeTick > 300)
                                cmd->viewangles.y -= 67.15f;
                            if (fakeTick > 600)
                                fakeTick = 0;
                                
                        } else {
                            cmd->viewangles.y += 180.f;
                        }
                    } else {
                        cmd->viewangles.y += 180.f;
                    }

                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SidewaysLeft) {
                    if(fakeswitch) cmd->viewangles.y = 90;
                    else cmd->viewangles.y = atTargets.y;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SidewaysRight) {
                    if(fakeswitch) cmd->viewangles.y = -90;
                    else cmd->viewangles.y = atTargets.y;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::SidewaysRight) {
                    if(fakeswitch) cmd->viewangles.y = -90; else cmd->viewangles.y = atTargets.y;
                }
                
                if(vars.misc.aaY == VIEW_ANTIAIM_YAW::CustomAA) {
                    if(bPacket) {
                        cmd->viewangles.y = vars.misc.fakeyaw;
                    }
                    else {
                        cmd->viewangles.y = vars.misc.realyaw;
                    }
                    
                }
            }
            
        }
    
    
}

/* No Recoil */
auto MakeNoRecoil(C_BaseEntity* local, CUserCmd* cmd) -> void {
    if(!vars.misc.enabled)
        return;
    
    if(!vars.misc.norecoil)
        return;
    
    if(cmd->buttons & IN_ATTACK) {
        Vector localVec = local->GetPunchAngles() * 2.f;
        
        cmd->viewangles.x -= localVec.x;
        cmd->viewangles.y -= localVec.y;
    }
}

std::string GetLocalName() {
    player_info_t localInfo;
    pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &localInfo);
    return localInfo.name;
}

void ChangeName(const char* szName) {
    ConVar* cvar_name = pCvar->FindVar("name");
    *(int*)((uintptr_t)&cvar_name->fnChangeCallback + 0x15) = 0;
    cvar_name->SetValue(szName);
}

Vector CalcAngle(Vector src, Vector dst) {
    
    Vector delta, angles;
    delta.x = (src.x - dst.x);
    delta.y = (src.y - dst.y);
    delta.z = (src.z - dst.z);
    
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
    
    angles.x = (float)(atanf(delta.z / hyp) * M_RADPI);
    angles.y = (float)(atanf(delta.y / delta.x) * M_RADPI);
    
    angles.z = 0.0f;
    if (delta.x >= 0.0) { angles.y += 180.0f; }
    return angles;
}


float GetFOV(Vector start, Vector src, Vector dst) {
    Vector ang, aim;
    ang = CalcAngle(src, dst);
    AngleVectors(start, &aim);
    AngleVectors(ang, &ang);
    
    float mag_s = std::sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
    float mag_d = mag_s;
    float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
    float fov = std::acos(u_dot_v / (mag_s * mag_d)) * (180.f / M_PI_F);
    return fov;
}

float GetFov(const Vector& viewAngle, const Vector& aimAngle) {
    Vector delta = aimAngle - viewAngle;
    NormalizeAngles(delta);
    
    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

auto MakeAutoPistolas(CUserCmd* cmd, C_BaseCombatWeapon* weapon) -> void {
    if(!vars.aimbot.autopistol) return;
    if(!weapon->IsPistol()) return;
    if (cmd->buttons & IN_ATTACK) {
        static bool bAttack = false;
        if (bAttack)
            cmd->buttons |= IN_ATTACK;
            else
            cmd->buttons &= ~IN_ATTACK;
            bAttack = !bAttack;
      }
}

struct FireBulletData {
    Vector          src;
    trace_t         enter_trace;
    Vector          direction;
    CTraceFilter    filter;
    float           trace_length;
    float           trace_length_remaining;
    float           current_damage;
    int             penetrate_count;
};

void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, C_BaseEntity * ignore, trace_t * ptr) {
    Ray_t ray;
    ray.Init(vecAbsStart, vecAbsEnd);
    CTraceFilter filter;
    filter.pSkip = ignore;
    
    pEngineTrace->TraceRay(ray, mask, &filter, ptr);
}


float GetHitgroupDamageMultiplier(int iHitGroup) {
    switch (iHitGroup) {
        case HITGROUP_GENERIC:
            return 1.0f;
            
        case HITGROUP_HEAD:
            return 2.0f;
            
        case HITGROUP_CHEST:
            return 1.0f;
            
        case HITGROUP_STOMACH:
            return 1.0f;
            
        case HITGROUP_LEFTARM:
        case HITGROUP_RIGHTARM:
            return 1.0f;
            
        case HITGROUP_LEFTLEG:
        case HITGROUP_RIGHTLEG:
            return 1.0f;
            
        default:
            return 1.0f;
    }
}

bool TraceToExit( Vector &end, trace_t *enter_trace, Vector start, Vector dir, trace_t *exit_trace )
{
    float distance = 0.0f;
    
    while( distance <= 90.0f )
    {
        distance += 4.0f;
        end = start + dir * distance;
        
        auto point_contents = pEngineTrace->GetPointContents( end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL );
        
        if( point_contents & MASK_SHOT_HULL && ( !( point_contents & CONTENTS_HITBOX ) ) )
            continue;
        
        auto new_end = end - ( dir * 4.0f );
        
        TraceLine( end, new_end, 0x4600400B, 0, exit_trace );
        
        // bool allsolid; @ TR + 54
        // bool startsolid; @ TR + 55
        
        if( exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX )
        {
            TraceLine( end, start, 0x600400B, exit_trace->m_pEnt, exit_trace );
            
            if( ( exit_trace->fraction < 1.0f || exit_trace->allsolid ) && !exit_trace->startsolid )
            {
                end = exit_trace->endpos;
                return true;
            }
            continue;
        }
        
        if( !( exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid ) || exit_trace->startsolid )
        {
            if( exit_trace->m_pEnt )
            {
                if( enter_trace->m_pEnt && enter_trace->m_pEnt == pEntList->GetClientEntity(0))
                    return true;
            }
            continue;
        }
        
        // checking for SURF_NODRAW perhaps
        if( ( ( exit_trace->surface.flags >> 7 ) & 1 ) && !( ( enter_trace->surface.flags >> 7 ) & 1 ) )
            continue;
        
        if( exit_trace->plane.normal.Dot( dir ) <= 1.0f )
        {
            auto fraction = exit_trace->fraction * 4.0f;
            end = end - ( dir * fraction );
            return true; 
        } 
    } 
    return false; 
}

void ScaleDamage(int hitgroup, C_BaseEntity * enemy, float weapon_armor_ratio, float & current_damage)
{
    current_damage *= GetHitgroupDamageMultiplier(hitgroup);
    
    int armor = enemy->GetArmor();
    
    if (armor > 0)
    {
        if (hitgroup == HITGROUP_HEAD)
        {
            if (enemy->HasHelmet())
                current_damage *= (weapon_armor_ratio);
        }
        else
        {
            current_damage *= (weapon_armor_ratio);
        }
    }
}

bool HandleBulletPenetration(CCSWeaponInfo* weaponInfo, FireBulletData &data)
{
    surfacedata_t *enter_surface_data = pPhysics->GetSurfaceData(data.enter_trace.surface.surfaceProps);
    int enter_material = enter_surface_data->game.material;
    float enter_surf_penetration_mod = *(float*)((uintptr_t)enter_surface_data + 76);
    
    data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
    data.current_damage *= pow((weaponInfo->GetRangeModifier()), (data.trace_length * 0.002));
    
    if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
        data.penetrate_count = 0;
    
    if (data.penetrate_count <= 0)
        return false;
    
    Vector dummy;
    trace_t trace_exit;
    if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
        return false;
    
    
    
    
    surfacedata_t *exit_surface_data = pPhysics->GetSurfaceData(trace_exit.surface.surfaceProps);
    int exit_material = exit_surface_data->game.material;
    
    float exit_surf_penetration_mod = *(float*)((uintptr_t)exit_surface_data + 76);
    float final_damage_modifier = 0.16f;
    float combined_penetration_modifier = 0.0f;
    
    if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
    {
        combined_penetration_modifier = 3.0f;
        final_damage_modifier = 0.05f;
    }
    else
    {
        combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
    }
    
    if (enter_material == exit_material)
    {
        if (exit_material == 87 || exit_material == 85)
            combined_penetration_modifier = 3.0f;
        else if (exit_material == 76)
            combined_penetration_modifier = 2.0f;
    }
    
    float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
    float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponInfo->GetPenetration()) * 1.25f);
    float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();
    
    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;
    
    float lost_damage = fmaxf(0.0f, v35 + thickness);
    
    if (lost_damage > data.current_damage)
        return false;
    
    if (lost_damage >= 0.0f)
        data.current_damage -= lost_damage;
    
    if (data.current_damage < 1.0f)
        return false;
    
    data.src = trace_exit.endpos;
    data.penetrate_count--;
    
    return true;
}

bool SimulateFireBullet(C_BaseEntity* pLocal, C_BaseCombatWeapon * pWeapon, FireBulletData & data)
{
    data.penetrate_count = 4;
    data.trace_length = 0.0f;
    CCSWeaponInfo* weaponData = pWeapon->GetCSWpnData();
    
    if (weaponData == NULL)
        return false;
    
    data.current_damage = (float)weaponData->GetDamage();
    
    while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
    {
        data.trace_length_remaining = weaponData->GetRange() - data.trace_length;
        
        Vector end = data.src + data.direction * data.trace_length_remaining;
        
        //data.enter_trace
        TraceLine(data.src, end, MASK_SHOT, pLocal, &data.enter_trace);
        
        Ray_t ray;
        ray.Init(data.src, end + data.direction*40.f);
        
        pEngineTrace->TraceRay(ray, MASK_SHOT, &data.filter, &data.enter_trace);
        
        TraceLine(data.src, end + data.direction*40.f, MASK_SHOT, pLocal, &data.enter_trace);
        
        if (data.enter_trace.fraction == 1.0f)
            break;
        
        if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (data.enter_trace.m_pEnt->GetTeam() != pLocal->GetTeam())) {
            data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
            data.current_damage *= pow(weaponData->GetRangeModifier(), data.trace_length * 0.002);
            ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, weaponData->GetWeaponArmorRatio(), data.current_damage);
            
            return true;
        }
        
        if (!HandleBulletPenetration(weaponData, data))
            break;
    }
    
    return false;
}


bool GetDamage(Vector & vecPoint, float* Damage) {
    C_BaseEntity* pLocal = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    Vector dst = vecPoint;
    FireBulletData data;
    data.src = pLocal->GetEyePosition();
    data.filter.pSkip = pLocal;
    
    Vector angles;
    angles = CalcAngle(data.src, dst);
    AngleVectors(angles, &data.direction);
    Vector dataNormalized;
    
    VectorNormalize(data.direction);
    
    C_BaseCombatWeapon* pWeap = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
    if (SimulateFireBullet(pLocal, pWeap, data)) {
        *Damage = data.current_damage;
        return true;
    }
    return false;
}

auto MakeHitscan(C_BaseEntity* entity, C_BaseCombatWeapon* weapon, CUserCmd* cmd) -> int {
    std::vector<int> hitboxes;
    
    hitboxes.push_back((int)CSGOBones::Head);
    hitboxes.push_back((int)CSGOBones::Neck);
    hitboxes.push_back((int)CSGOBones::Neck_Lower);
    hitboxes.push_back((int)CSGOBones::Pelvis);
    hitboxes.push_back((int)CSGOBones::Stomach);
    hitboxes.push_back((int)CSGOBones::Lower_Chest);
    hitboxes.push_back((int)CSGOBones::Chest);
    hitboxes.push_back((int)CSGOBones::Upper_Chest);
    hitboxes.push_back((int)CSGOBones::Right_Thigh);
    hitboxes.push_back((int)CSGOBones::Left_Thigh);
    hitboxes.push_back((int)CSGOBones::Right_Shin);
    hitboxes.push_back((int)CSGOBones::Left_Shin);
    hitboxes.push_back((int)CSGOBones::Right_Foot);
    hitboxes.push_back((int)CSGOBones::Left_Foot);
    
    if(vars.aimbot.hitscan) {
        for(auto hit : hitboxes) {
            Vector vDest = GetHitboxPosition(entity, hit);
            float flDmg = 0.f;
            if(GetDamage(vDest, &flDmg)) {
                return hit;
            }
        }
    } else {
        return vars.aimbot.bone;
    }
    return (int)CSGOBones::Head;
}

Vector ExtrapolateTick( Vector p0, Vector v0 ) {
    return p0 + ( v0 * pGlobals->interval_per_tick );
}

Vector C_BaseEntity::GetPredicted( Vector p0 ) {
    return ExtrapolateTick( p0, this->GetVelocity() );
}

Vector C_BaseEntity::GetPredicted(int Bone) {
    return ExtrapolateTick(this->GetBonePosition(Bone), this->GetVelocity());
}

Vector Smooth(Vector& angle) {
    
    if(!vars.aimbot.enabled)
        return;
    
    if(!vars.aimbot.smooth)
        return;
    
    if(vars.aimbot.silent)
        return;
    
    Vector viewAngles = Vector(0.f, 0.f, 0.f);
    pEngine->GetViewAngles(viewAngles);
    
    Vector delta = angle - viewAngles;
    NormalizeAngles(delta);
    
    float smooth = powf(vars.aimbot.smoothf, 0.4f);
    smooth = std::min(0.99f, smooth);
    
    Vector toChange = delta - delta * smooth;
    
    return angle = viewAngles + toChange;
}

C_BaseEntity* FindTarget(C_BaseEntity* LocalPlayer) {
    
    int fov = vars.aimbot.fov;
    C_BaseEntity* TargetEntity = nullptr;
    
    Vector LocalPlayerEyePosition = LocalPlayer->GetEyePosition();
    Vector LocalPlayerViewAngle = LocalPlayer->GetViewAngles();
    
    for(int i = 1; i < pEngine->GetMaxClients(); i++) {
        
        C_BaseEntity* PossibleTarget = pEntList->GetClientEntity(i);
        
        if(!PossibleTarget || PossibleTarget->GetHealth() < 1 || PossibleTarget->GetImmune()) {
            continue;
        }
        
        if(LocalPlayer->GetTeam() == PossibleTarget->GetTeam()) {
            continue;
        }
        
        Vector PossibleTargetHitbox = PossibleTarget->C_BaseEntity::GetPredicted(vars.aimbot.bone);
        auto vis = IsVisible(LocalPlayer, PossibleTarget);
        
        if(!vis) {
            continue;
        }
        
        float FieldOfViewToTarget = GetFOV(LocalPlayerViewAngle, LocalPlayerEyePosition, PossibleTargetHitbox);
        
        if(FieldOfViewToTarget < fov) {
            TargetEntity = PossibleTarget;
            fov = FieldOfViewToTarget;
        }
    }
    
    return TargetEntity;
}



auto MakeAimbot(CUserCmd* pCmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, float& flForward, float& flSide, bool& bSendPacket) -> void {
    Vector eyepos = local->GetEyePosition();
    if(!vars.aimbot.enabled) return;
    for(int i = 0; i < pEntList->GetHighestEntityIndex(); i++) {
        auto* entity = pEntList->GetClientEntity(i);
        if(vars.misc.findtarget)
            entity = FindTarget(local);
        
        if(!entity)
            continue;
        
        if(entity == local)
            continue;
        
        if(entity->GetTeam() == local->GetTeam())
            continue;
        
        if(entity->GetHealth() < 1)
            continue;
        
        if(entity->GetDormant())
            continue;
        
        if(entity->GetImmune())
            continue;
        
        if(weapon->GetAmmo() < 1)
            continue;
        
        if(weapon->IsKnife() || weapon->IsBomb())
            continue;
        
        if(gCorrections[entity->GetIndex()].whitelist)
            continue;
        
        Vector vFrom, vTo;
        
        vFrom = GetHitboxPosition(entity, MakeHitscan(entity, weapon, pCmd)); // 0 = head.
        
        if(vars.aimbot.selfpred)
        eyepos += entity->GetVelocity() * pGlobals->interval_per_tick;
            
        vTo = CalcAngle(eyepos, vFrom);
        
        auto isVISIBLE = IsVisible(local, entity);
        float getdmg = 0.f;
        
        atTargets = vTo;
        
        if(vars.aimbot.autowall ? GetDamage(vFrom, &getdmg) || isVISIBLE : isVISIBLE) {
            if(GetFOV(pCmd->viewangles, local->GetEyePosition(), vFrom) < vars.aimbot.fov) {
                if(vars.aimbot.autoshoot) {
                    pCmd->buttons |= IN_ATTACK;
                }
                if(vars.aimbot.autostop) {
                    flForward = 0.f;
                    flSide = 0.f;
                }
                if(vars.aimbot.autocrouch) {
                    pCmd->buttons |= IN_DUCK;
                }
                bool bAttack = true;
                
                if (weapon->GetNextPrimaryAttack() - pGlobals->interval_per_tick > local->GetTickBase() * pGlobals->interval_per_tick)
                    bAttack = false;
                
                if(vars.aimbot.psilent) {
                    if(pCmd->buttons & IN_ATTACK && (bAttack)) {
                        bSendPacket = false;
                    } else {
                        bSendPacket = true;
                    }
                }
                if(vars.aimbot.smooth) {
                    pCmd->viewangles = Smooth(vTo);
                }
                if(pCmd->buttons & IN_ATTACK) {
                    if(!vars.aimbot.silent) {
                        pCmd->viewangles = vTo;
                        pEngine->SetViewAngles(pCmd->viewangles);
                    } else {
                        pCmd->viewangles = vTo;
                    }
                }
            }
        }
    }
}

bool CircleDone = false, JustStarted = true;
void Prespeed(C_BaseEntity *localPlayer, CUserCmd *cmd, Vector &oldAngles)
{
    if(!vars.misc.enabled) return;
    if(!vars.misc.circlestrafe) return;
    
    static int Angle = 0;
    if (oldAngles.y - Angle > 360)
        Angle -= 360;
    static bool shouldspin = false;
    static bool enabled = false;
    static bool check = false;
    if (pInputSystem->IsButtonDown(MOUSE_4))
    {
        if (!check)
            enabled = !enabled;
        check = true;
    }
    else
        check = false;
    if (enabled)
    {
        shouldspin = true;
        cmd->buttons |= IN_JUMP;
        cmd->buttons |= IN_DUCK;
    }
    if (shouldspin)
    {
        Vector Dir;
        AngleVectors(Vector(0, Angle, 0), &Dir);
        Dir *= 8218;
        Ray_t ray;
        CTraceFilterWorldOnly filter;
        trace_t trace;
        ray.Init(localPlayer->GetEyePosition(), localPlayer->GetVecOrigin() + Dir);
        pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);
        auto temp = 3.4f / ((trace.endpos - localPlayer->GetVecOrigin()).Length() / 100.f);
        if (temp < 3.4f)
            temp = 3.4f;
        if (enabled)
        {
            Angle += temp;
            cmd->sidemove = -450;
        }
        else {
            if (oldAngles.y - Angle < temp) {
                Angle = oldAngles.y;
                shouldspin = false;
            }
            else
                Angle += temp;
        }
        oldAngles.y = Angle;
    }
    else
        Angle = oldAngles.y;
}
// Circle
auto RotateMovement(CUserCmd* pCmd, float rotation) -> void{
    
    rotation = DEG2RAD(rotation);
    
    float cos_rot = cos(rotation);
    float sin_rot = sin(rotation);
    
    float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
    float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);
    
    pCmd->forwardmove = new_forwardmove;
    pCmd->sidemove = new_sidemove;
}

int CircleFactor = 0;
auto cStrafe(CUserCmd* pCmd, C_BaseEntity* local, Vector& OriginalView) -> void {
    
    if (!vars.misc.circlestrafe)
        return;
    
    if (!local)
        return;
    
    CircleFactor++;
    if (CircleFactor > 361)
        CircleFactor = 0;
        
    int GetItDoubled = 3.0 * CircleFactor - pGlobals->interval_per_tick;
        
    Vector StoredViewAngles = pCmd->viewangles;
    
    pCmd->viewangles = OriginalView;
    RotateMovement(pCmd, GetItDoubled);
        
}

auto enginepred(CUserCmd* cmd, C_BaseCombatWeapon* weapon, C_BaseEntity* local) -> void {
    float curtime = local->GetTickBase() * pGlobals->interval_per_tick;
    float backup = pGlobals->frametime;
    
    pGlobals->frametime = pGlobals->interval_per_tick;
    
    weapon->UpdateAccuracyPenalty();
    
    CMoveData data;
    memset(&data, 0, sizeof(data));
    
    pPrediction->SetupMove(local, cmd, pMoveHelper, &data);
    pGameMovement->ProcessMovement(local, &data);
    pPrediction->FinishMove(local, cmd, &data);
    
    pGlobals->frametime = backup;
}

auto MakeAutoStrafer(CUserCmd* cmd, C_BaseEntity* local) -> void {
    if(!vars.misc.enabled) return;
    if(!vars.misc.autostrafe) return;
    
    if (local->GetFlags() & FL_ONGROUND)
        return;
    
    if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
        return;
    
    if (cmd->mousedx <= 1 && cmd->mousedx >= -1)
        return;
    
    cmd->sidemove = cmd->mousedx < 0.f ? 450.f : -450.f;
}

void ClampMoves(float& flForward, float& flSide, float& flUp) {
    if(flForward > 450) flForward = 450;
    if(flSide > 450) flSide = 450;
    if(flUp > 450) flUp = 450;
    if(flForward < -450) flForward = -450;
    if(flSide < -450) flSide = -450;
    if(flUp < -450) flUp = -450;
}

auto MakeBhop(CUserCmd* cmd, C_BaseEntity* local) -> void {
    if(!vars.misc.enabled) return;
    if(!vars.misc.bhop) return;
    
    static bool shouldJump = false;
    static bool shouldFake = false;
    
    if (shouldFake && !shouldJump) {
        cmd->buttons |= IN_JUMP;
        shouldFake = false;
    }
    else if (cmd->buttons & IN_JUMP) {
        if (local->GetFlags() & FL_ONGROUND) {
            shouldFake = false;
            shouldJump = false;
            
        }
        else {
            cmd->buttons &= ~IN_JUMP;
            shouldJump = false;
        }
    }
    else {
        shouldJump = false;
        shouldFake = false;
    }
}

void MakeAirstuck(CUserCmd* cmd) {
    if (!vars.misc.airstuck)
        return;
    
    if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
        return;
    
    if (pInputSystem->IsButtonDown(KEY_Z))
        cmd->tick_count = 16777216;
}

static int ticks = 0;
int ticksMax = 16;

void MakeFakeLag(CUserCmd* cmd) {
    
    if (!vars.misc.fakelag)
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!localplayer || !localplayer->GetAlive())
        return;
    
    if (localplayer->GetFlags() & FL_ONGROUND && vars.misc.adaptive)
        return;
    
    if (cmd->buttons & IN_ATTACK) {
        CreateMove::sendPacket = true;
        return;
    }
    
    if (ticks >= ticksMax) {
        CreateMove::sendPacket = true;
        ticks = 0;
    } else {
        
        if (vars.misc.adaptive) {
            
            int packetsToChoke;
            
            if (localplayer->GetVelocity().Length() > 0.f)
            {
                packetsToChoke = (int)((64.f / pGlobals->interval_per_tick) / localplayer->GetVelocity().Length()) + 1;
                if (packetsToChoke >= 15)
                    packetsToChoke = 14;
                if (packetsToChoke < vars.misc.fakelagfactor)
                    packetsToChoke = vars.misc.fakelagfactor;
            } else {
                packetsToChoke = 0;
            }
            
            CreateMove::sendPacket = ticks < 16 - packetsToChoke;
            
        } else {
            CreateMove::sendPacket = ticks < 16 - vars.misc.fakelagfactor;
        }
    }
    ticks++;
}


void MakeTrigger(CUserCmd *Cmd) {
    
    if(!vars.aimbot.trigger)
        return;
    
    if(pInputSystem->IsButtonDown(KEY_EQUAL)) {
        
        C_BaseEntity* LocalPlayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
        
        if (!LocalPlayer || LocalPlayer->GetHealth() < 0) {
            return;
        }
    
        C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
        if (!activeWeapon || activeWeapon->GetAmmo() == 0)
            return;
        
        
        Vector traceStart, traceEnd;
        
        Vector viewAngles;
        pEngine->GetViewAngles(viewAngles);
        Vector viewAngles_rcs = viewAngles + (LocalPlayer->GetPunchAngles() * 2.0f);
        
        AngleVectors(viewAngles_rcs, &traceEnd);
        
        traceStart = LocalPlayer->GetEyePosition();
        traceEnd = traceStart + (traceEnd * 8192.0f);
        
        Ray_t ray;
        trace_t trace;
        CTraceFilter filter;
        filter.pSkip = LocalPlayer;
        
        ray.Init(traceStart, traceEnd);
        pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);
        
        if (trace.allsolid || trace.startsolid) {
            return;
        }
        
        C_BaseEntity* player = (C_BaseEntity*)trace.m_pEnt;
        if(!player || player->GetHealth() < 0 || player->GetImmune()) {
            return;
        }
        
        if(player->GetClientClass()->m_ClassID != EClassIds::CCSPlayer) {
            return;
        }
        
        if(LocalPlayer->GetTeam() == player->GetTeam()) {
            return;
        }
        
        if (activeWeapon->GetNextPrimaryAttack() > pGlobals->curtime) {
            if (*activeWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER) {
                Cmd->buttons &= ~IN_ATTACK2;
            } else {
                Cmd->buttons &= ~IN_ATTACK;
            }
        } else {
            if (*activeWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER) {
                Cmd->buttons |= IN_ATTACK2;
            } else {
                Cmd->buttons |= IN_ATTACK;
            }
        }
    }
}

void MakeShowRanks(CUserCmd* cmd)
{
    if (!vars.misc.showrank)
        return;
    
    if (!(cmd->buttons & IN_SCORE))
        return;
    
    float input[3] = { 0.f };
    MsgFunc_ServerRankRevealAll(input);
}


auto hacks(CUserCmd* cmd, C_BaseEntity* local, C_BaseCombatWeapon* weapon, Vector& vOldAngles, float& flForwardmove, float& flSidemove, bool& sendpacket) -> void {
    
    MakeAutoStrafer(cmd, local);
    
    MakeBhop(cmd, local);
    
    enginepred(cmd, weapon, local);
    
    //Prespeed(local, cmd, vOldAngles); // Old circle strafe

    MakeAimbot(cmd, local, weapon, flForwardmove, flSidemove, sendpacket); // Add some black magic shit.
    
    MakeAntiAim(cmd, local, weapon, sendpacket); // Add antiaims.
    
    MakeAutoPistolas(cmd, weapon);
    
    MakeNoRecoil(local, cmd); // Add norecoil
    
    MakeAirstuck(cmd);
    
    MakeFakeLag(cmd); // Broken
    
    MakeShowRanks(cmd);
    
    MakeTrigger(cmd);
    
    if(pInputSystem->IsButtonDown(KEY_V))
        cStrafe(cmd, local, vOldAngles);
    
    if(draw->m_szChangedValue[2].length() > 0) // Name Changer.
        ChangeName(draw->m_szChangedValue[2].c_str());
        
    if(draw->m_szChangedValue[3].length() > 0){
        if(vars.misc.clantag)
            SetTag(draw->m_szChangedValue[3].c_str(), "");
    }
}
bool bOnce = false;


void ClampYaw(float& angle) {
    while(angle > 180) {
        angle -= 360;
    }
    while(angle < -180) {
        angle += 360;
    }
}


bool SendPacket = true;
bool hkCreateMove(void* thisptr, float flSampleInput, CUserCmd* cmd) {
    createmovehook->GetOriginalMethod<tCreateMove>(25)(thisptr, flSampleInput, cmd);
    
    if(!cmd->command_number)
        return true;
    
    auto* local = pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if(!local)
        return false;
    
    if(local->GetLifeState() != 0)
        return false;
    
    if(local->GetHealth() < 1)
        return false;
    
    if(local->GetDormant())
        return false;
    
    if(bSendPacket == nullptr)
        return false;
    
    if(!bOnce) {
        if(draw->m_szCurrentString[2].empty()) {
            draw->m_szCurrentString[2] = GetLocalName();
        }
        bOnce = true;
    }
    
    auto* weapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(local->GetActiveWeapon());
    
    if(!weapon)
        return false;

    SendPacket = true;
    *bSendPacket = SendPacket;
    
    
    Vector vOldAngles = cmd->viewangles;
    Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
    Vector qAimAngles;
    
    float forward = cmd->forwardmove;
    float sidemove = cmd->sidemove;
    float upmove = cmd->upmove;
    
    if(pEngine->IsInGame() && pEngine->IsConnected()) {
        
        hacks(cmd, local, weapon, vOldAngles, forward, sidemove, *bSendPacket);
        
       // if(!vars.misc.antiuntrust)
       // ClampYaw(cmd->viewangles.y);
        
        if(pInputSystem->IsButtonDown(MOUSE_4))
            *bSendPacket = false;
        else
            *bSendPacket = true;
        
        
        FixMovement(vOldAngles, cmd);
        ClampMoves(forward, sidemove, upmove);
        

        
        if(vars.misc.enabled && vars.misc.antiuntrust) {
            cmd->viewangles.ClampAngles();
        }
        
        if(*bSendPacket)
        tpangles = cmd->viewangles;
    }
    return false;
}
