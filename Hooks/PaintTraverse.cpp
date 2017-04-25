#include "../main.h"
#include "../Hacks/esp.h"
#include "../Hacks/recoilcrosshair.h"

bool qOnce = false;
bool bPanelOnce = false;

void hkPaintTraverse(void* thisptr, VPANEL panel, bool forceRepaint, bool allowForce) {
    
    painthook->GetOriginalMethod<tPaintTraverse>(42)(thisptr, panel, forceRepaint, allowForce);
    
    static VPANEL last = 0;
    
    if(!last) {
        
        if(strstr(pPanel->GetName(panel), "FocusOverlayPanel")) {
            
            framefont = draw->createfont(framefont, "Tahoma", 14, FONTFLAG_NONE);
            espfont = draw->createfont(espfont, "Tahoma", 12, FONTFLAG_OUTLINE);
            
            fontMenu = draw->createfont(fontMenu, "Courier New", 14, FONTFLAG_OUTLINE);
            titleFont = draw->createfont(titleFont, "Banco", 20, FONTFLAG_OUTLINE);
            last = panel;
            
        }
    }
    
    if(!bPanelOnce) {
        
        vars.colors.ctchams = Color(40, 204, 67, 255);
        vars.colors.ctchams_ign = Color(0, 124, 255, 255);
        
        vars.colors.tchams = Color::Yellow();
        vars.colors.tchams_ign = Color::Magenta();
        
        vars.colors.tbox = Color(204, 173, 40, 255);
        vars.colors.tbox_ign = Color(255, 69, 0, 255);
        
        vars.colors.ctbox = Color(40, 204, 67, 255);
        vars.colors.ctbox_ign = Color(0, 124, 255, 255);
        
        vars.colors.hands = Color::Purple();
        vars.colors.hands_ign = Color::Blue();
        
        vars.colors.world = Color::Green(); // :^)
        
        bPanelOnce = true;
        
    }

    if(panel == last) {
        
        
        pwnmymenu(); // Gets keypress to diplay menu
        DrawPlayerESP();
        
        if(vars.menu) {
        
            
        //    gMenu.Menu();
            
            draw->drawmenu();
            draw->drawmouse();
            pEngine->ExecuteClientCmd("cl_mouseenable 0");
            
        }
        
        draw->DrawBasicString(10, 15, Color::Magenta(), espfont, "yEet");
        
    }
}
