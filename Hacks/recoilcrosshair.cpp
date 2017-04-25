#include "recoilcrosshair.h"

void rCrosshair() {
    
    if (!vars.visuals.enabled)
        return;
    
    if (!vars.visuals.crosshair)
        return;
    
    if (!pEngine->IsInGame())
        return;
    
    C_BaseEntity* localplayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    
    if (!localplayer || !localplayer->GetAlive())
        return;
    
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    if (!activeWeapon)
        return;
    
    CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
    if (weaponType != CSWeaponType::WEAPONTYPE_RIFLE && weaponType != CSWeaponType::WEAPONTYPE_SUBMACHINEGUN && weaponType != CSWeaponType::WEAPONTYPE_MACHINEGUN)
        return;
    
    Vector punchAngle = localplayer->GetPunchAngles();
    
    int ScreenWidth, ScreenHeight;
    pEngine->GetScreenSize(ScreenWidth, ScreenHeight);
    
    int x = (int) (ScreenWidth * 0.5f);
    int y = (int) (ScreenHeight * 0.5f);
    int dx = ScreenWidth / vars.misc.fov;
    int dy = ScreenHeight / vars.misc.fov;
    
    int crosshairX = (int) (x - (dx * punchAngle.y));
    int crosshairY = (int) (y + (dy * punchAngle.x));
    
    // outline horizontal
    draw->FilledRectangle(Vector2D(crosshairX - 4, crosshairY - 1), Vector2D(crosshairX + 5, crosshairY + 2), Color(0, 0, 0, 170));
    // outline vertical
    draw->FilledRectangle(Vector2D(crosshairX - 1, crosshairY - 4), Vector2D(crosshairX + 2, crosshairY + 5), Color(0, 0, 0, 170));
    // line horizontal
    draw->Line(Vector2D(crosshairX - 3, crosshairY), Vector2D(crosshairX + 4, crosshairY), Color(255, 255, 255, 255));
    // line vertical
    draw->Line(Vector2D(crosshairX, crosshairY + 3), Vector2D(crosshairX, crosshairY - 4), Color(255, 255, 255, 255));
}
