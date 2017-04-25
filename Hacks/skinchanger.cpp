#include "skinchanger.h"

int Knife = WEAPON_KNIFE_BUTTERFLY; // Chage knife model here
int Glove = GLOVE_SPECIALIST;

unordered_map<int, cSkin> cSkinchanger::Skins = unordered_map<int, cSkin>( {
    /*
     To add a new skin copy this :
     
     make_pair(WEAPON_NAME, cSkin(int paintkit, int seed, int itemDefinition, int stattrack, char* szName, float flWear));
     
     For paintkit replace with the skinID, look here https://github.com/adamb70/CSGO-skin-ID-dumper/blob/master/item_index.txt
     For seed if the skin has different patterns like case hardeneds replace this with the seed you want otherwise replace with 0
     For itemDefinition if the weapon is a gun replace it with -1 and if it's a knife replace it with WEAPON_KNIFE_NAME
     For stattrack if you dont want stattrack replace with -1 or if you do want stattrack replace with any number
     For szName if you want a name replace with (char*)("NAME") and if you dont want a name replace with nullptr
     For flWear replace with the float value that you want, 0.00000000000001f is the lowest and 1.f is the highest
     
     */
    
    make_pair( WEAPON_KNIFE,        cSkin( 568, 0, WEAPON_KNIFE_BUTTERFLY, -1, nullptr, 0.0001f ) ),    // Bowie saphire
    make_pair( GLOVE,               cSkin( 10016, 0, GLOVE_SLICK, -1, nullptr, 0.0001f)),               // Driver gloves
    make_pair( WEAPON_AK47,         cSkin( 639, 0, -1, -1, nullptr, 0.0001f ) ),    // BloodSport
    make_pair( WEAPON_AWP,          cSkin( 446, 0, -1, -1, nullptr, 0.0001f ) ),    // Medusa
    make_pair( WEAPON_AUG,          cSkin( 455, 0, -1, -1, nullptr, 0.0001f) ),     // Akihabara's Accept
    make_pair( WEAPON_GLOCK,        cSkin( 38, 33, -1, -1, nullptr, 0.0001f) ),     // Fade
    make_pair( WEAPON_USP_SILENCER, cSkin( 504, 0, -1, -1, nullptr, 0.0001f) ),     // Kill Confirmed
    make_pair( WEAPON_M4A1,         cSkin( 309, 0, -1, -1, nullptr, 0.0001f) ),     // Howl
    make_pair( WEAPON_DEAGLE,       cSkin( 37, 0, -1, -1, nullptr, 0.0001f) ),      // Blaze
    make_pair( WEAPON_UMP45,        cSkin( 37, 0, -1, -1, nullptr, 0.0001f) ),      // Blze
    make_pair( WEAPON_TEC9,         cSkin( 168, 0, -1, -1, nullptr, 0.061f) ),      // Nuclear Threat
    make_pair( WEAPON_SG556,        cSkin( 39, 0, -1, -1, nullptr, 0.061f) ),       // Bulldoze
    make_pair( WEAPON_SSG08,        cSkin( 222, 0, -1, -1, nullptr, 0.061f) ),      // Blood in the water
    make_pair( WEAPON_GALILAR,      cSkin( 379, 0, -1, -1, nullptr, 0.0001f) ),     // Cerberus
    make_pair( WEAPON_MP7,          cSkin( 102, 0, -1, -1, nullptr, 0.061f) ),      // Whiteout
    make_pair( WEAPON_P250,         cSkin( 102, 0, -1, -1, nullptr, 0.061f) ),      // Whiteout
    make_pair( WEAPON_FIVESEVEN,    cSkin( 44, 128, -1, -1, nullptr, 0.0001f) ),    // Case Hardened
    make_pair( WEAPON_P90,          cSkin( 182, 0, -1, -1, nullptr, 0.061f) ),      // Emerald Dragon
    make_pair( WEAPON_SCAR20,       cSkin( 639, 0, -1, -1, (char*)("splashgang"), 0.061f) ),   // Splash Jam
} );

unordered_map<int, const char*> cSkinchanger::Models;

cSkinchanger* skinchanger = new cSkinchanger;

void cSkinchanger::FrameStageNotify(ClientFrameStage_t stage) {
    
    if(stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START){
        
        local = (C_BaseEntity*)(pEntList->GetClientEntity(pEngine->GetLocalPlayer()));
        
        if(local && local->GetHealth() > 0){
            
            if(!bInit){
                Init();
                bInit = true;
            }
            
            LoadSkins();
        }
    }
}

void cSkinchanger::FindModels() {
    Models[pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl")] = KnifeToModel[Knife].c_str();
    Models[pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl")] = KnifeToModel[Knife].c_str();
    Models[pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl")] = GloveToModel[Glove].c_str();
    Models[pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl")] = GloveToModel[Glove].c_str();
}

void cSkinchanger::LoadSkins() {
    
    player_info_t player_info;
    pEngine->GetPlayerInfo(local->GetId(), &player_info);
    
    int* pWeapons = local->GetWeapons();
    
    C_BaseViewModel* LocalPlayerViewModel = (C_BaseViewModel*)pEntList->GetClientEntityFromHandle(local->GetViewModel());
    C_BaseAttributableItem* WeaponViewModel = nullptr;
    
    if(LocalPlayerViewModel)
        WeaponViewModel = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(LocalPlayerViewModel->GetWeapon());
    
    C_BaseCombatWeapon* localWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(local->GetActiveWeapon());
    
    if(pWeapons) {
        
        for(int i = 0; pWeapons[i]; i++) {
            
            C_BaseAttributableItem* attributableItem = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(pWeapons[i]);
            
            if(attributableItem) {
                
                int* ItemDef = attributableItem->GetItemDefinitionIndex();
                
                unordered_map<int, cSkin>::iterator SkinIter;
                
                if(*ItemDef == WEAPON_KNIFE_T)
                    SkinIter = Skins.find(WEAPON_KNIFE);
                else if(*ItemDef == GLOVE_T)
                    SkinIter = Skins.find(GLOVE);
                else
                    SkinIter = Skins.find(*ItemDef);
                
                if(SkinIter != Skins.end()) {
                    
                    if(*attributableItem->GetOriginalOwnerXuidLow() == player_info.xuidlow && *attributableItem->GetOriginalOwnerXuidHigh() == player_info.xuidhigh){
                        
                        int* model_index = attributableItem->GetModelIndex();
                        
                        unordered_map<int, const char*>::iterator model_iter = Models.find(*model_index);
                        
                        if(model_iter != Models.end())
                            *model_index = pModelInfo->GetModelIndex(model_iter->second);
                        
                        
                        cSkin skin = move(SkinIter->second);
                        
                        if(Knife && (*ItemDef == WEAPON_KNIFE || *ItemDef == WEAPON_KNIFE_T))
                            *attributableItem->GetItemDefinitionIndex() = Knife;
                        
                        if(Glove && (*ItemDef == GLOVE_T || *ItemDef == GLOVE_CT))
                            *attributableItem->GetItemDefinitionIndex() = Glove;
                        
                        if(skin.name)
                            sprintf(attributableItem->GetCustomName(), skin.name);
                        
                        
                        *attributableItem->GetItemIDHigh() = -1;
                        *attributableItem->GetFallbackPaintKit() = skin.Paintkit;
                        *attributableItem->GetFallbackStatTrak() = skin.StatTrack;
                        *attributableItem->GetEntityQuality() = 4;
                        *attributableItem->GetFallbackSeed() = skin.Seed;
                        *attributableItem->GetFallbackWear() = skin.Wear;
                        *attributableItem->GetAccountID() = player_info.xuidlow;
                    }
                }
                
                if (WeaponViewModel && WeaponViewModel == attributableItem) {
                    
                    int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();
                    
                    unordered_map<int, const char*>::iterator model_iter = Models.find(*model_index);
                    
                    if (model_iter != Models.end())
                        *model_index = pModelInfo->GetModelIndex(model_iter->second);
                }
                
            }
        }
        
        if(LocalPlayerViewModel && localWeapon){
            
            int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();
            
            unordered_map<int, const char*>::iterator model_iter = Models.find(*((C_BaseEntity*)localWeapon)->GetModelIndex());
            
            if(model_iter != Models.end())
                *model_index = pModelInfo->GetModelIndex(model_iter->second);
        }
    }
    
    
    C_BaseAttributableItem* glove = (C_BaseAttributableItem*)pEntList->GetClientEntity(local->GetWearables()[0] & 0xFFF);
    
    if (!glove)
        return;
    
    auto keyExists = local->GetTeam() == TEAM_COUNTER_TERRORIST ? Skins.find(GLOVE) : Skins.find(GLOVE);
    
    if (keyExists != (local->GetTeam() == TEAM_COUNTER_TERRORIST ? Skins.end() : Skins.end())) {
            
        if (*glove->GetItemDefinitionIndex() == GLOVE) {
            glove->SetModelIndex(pModelInfo->GetModelIndex(to_string((Skins.at(GLOVE).ItemDefinition)).c_str()));
            *glove->GetItemDefinitionIndex() = GLOVE;
        }
    
    }
    
    
}

void cSkinchanger::Init() {
    Models.clear();
    FindModels();
}

void cSkinchanger::FireEventClientSide(IGameEvent *event) {
    
    if (!strcmp(event->GetName(), "game_newmap"))
        Init();
}


// Models

inline const int RandomSequence(int low, int high) {
    return (rand() % (high - low + 1) + low);
}

void HSequenceProxyFn(const CRecvProxyData *pDataConst, void *pStruct, void *pOut) {
    
    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);
    C_BaseViewModel* pViewModel = (C_BaseViewModel*)pStruct;

    if(!pViewModel)
        return g_pSequence(pDataConst, pStruct, pOut);
    
    C_BaseEntity* pOwner = (C_BaseEntity*)pEntList->GetClientEntityFromHandle(pViewModel->GetOwner());
    
    if (pViewModel && pOwner) {
        if (pOwner->GetIndex() == pEngine->GetLocalPlayer()) {
            
            const model_t* knife_model = pModelInfo->GetModel(*pViewModel->GetModelIndex());
            const char* model_filename = pModelInfo->GetModelName(knife_model);
            
            int m_nSequence = (int)pData->m_Value.m_Int;
            
            if (!strcmp(model_filename, "models/weapons/v_knife_butterfly.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_DRAW:
                        m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                        break;
                    default:
                        m_nSequence++;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_falchion_advanced.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_FALCHION_IDLE1;
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                        break;
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence = RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                        break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence--;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_push.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_DAGGERS_IDLE1;
                        break;
                    case SEQUENCE_DEFAULT_LIGHT_MISS1:
                    case SEQUENCE_DEFAULT_LIGHT_MISS2:
                        m_nSequence = RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_MISS1:
                        m_nSequence = RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                        break;
                    case SEQUENCE_DEFAULT_HEAVY_HIT1:
                    case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                    case SEQUENCE_DEFAULT_LOOKAT01:
                        m_nSequence += 3;
                        break;
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    default:
                        m_nSequence += 2;
                }
                
            } else if (!strcmp(model_filename, "models/weapons/v_knife_survival_bowie.mdl")) {
                
                switch (m_nSequence) {
                    case SEQUENCE_DEFAULT_DRAW:
                    case SEQUENCE_DEFAULT_IDLE1:
                        break;
                    case SEQUENCE_DEFAULT_IDLE2:
                        m_nSequence = SEQUENCE_BOWIE_IDLE1;
                        break;
                    default:
                        m_nSequence--;
                }
                
            }
            
            pData->m_Value.m_Int = m_nSequence;
        }
    }
    
    return g_pSequence(pData, pStruct, pOut);
}






