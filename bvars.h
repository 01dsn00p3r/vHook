class ccombobox {
public:
    /* this class only handles all the open checks for the combo boxes, nothing more. */
    bool esp_opend;
    bool hitbox_opend;
    bool aaY_opend;
    bool aaX_opend;
    bool hands_opend;
    bool players_opend;
    bool playerlist_opend_x[64];
    bool playerlist_opend_y[64];
	bool clan_opend;
	bool colors_opend;
    
    bool misc_clantag, misc_name, misc_changename;
    int inputIndex = 0;
};

/* Change values for settings to be active on startup */
class cVariables : public ccombobox {
public:
    bool menu = false;
    int buttonPadding = 24;
    int tabIndex;
    int menueffect = 0;
    
    class cAimbot {
    public:
        bool enabled	= true;
        bool silent		= false;
        bool psilent	= false;
		bool smooth		= true;
		float smoothf	= .4f;
        int fov			= 1;                // 20
        int hitbox		= 6;
        int pointscale	= 73;
        int bone		= 6;
        bool hitscan	= false;
        bool selfpred	= false;
        bool autowall	= false;
        bool autoshoot	= false;
        bool autopistol = false;
        bool autocrouch = false;
        bool autostop	= false;
		bool trigger	= false;
		int selected;
    }aimbot;
    
    class cVisuals {
    public:
        bool enabled	= true;
        bool box		= false;
        bool name		= true;
        bool health		= false;	// Kinda broken
        bool healthtext = true;
        bool weapon		= true;
        bool snapline	= true;
        bool handchams	= true;
        bool skeleton	= false;
        bool chams		= true;
        bool headhitbox = false;
        bool enemyonly	= true;
		bool bombtimer	= false;
		bool skinc		= true;
		
		bool crosshair	= false;
		bool whenshooting = false;
        
        int handsType	= 3; // 0 - lit, 1 - texture, 2 - wireframe, 3 - nohands
        int playersType = 1;
        
        int handchams_alpha		= 255;
        int playerchams_alpha	= 255;
        
        int coltab;
        
        int tchams[4] = { 255, 255, 0, 255 };
        int ctchams[4] = { 0, 255, 0, 255 };
        
        int tchams_ignore[4] = { 236, 81, 63, 255 };
        int ctchams_ignore[4] = { 51, 124, 197, 255 }; // this color is soo fucking p100!
        
        int tbox[4] = { 255, 255, 0, 255 };
        int ctbox[4] = { 0, 255, 0, 255 };
        
        int tbox_ignore[4] = { 236, 81, 63, 255 };
        int ctbox_ignore[4] = { 51, 124, 197, 255 }; // this color is soo fucking p100!
        
    }visuals;
    
    class cMisc {
    public:
        bool enabled = true;
        bool antiuntrust = true;
        bool norecoil = false;
        bool novisualrecoil = false;
        bool nosky = false;
        bool worldpaint = false;
        bool bhop = true;
        bool autostrafe = false;
        bool circlestrafe = false;
		bool ragestrafe = false;
		bool silentStrafe = false;
        bool thirdpersonmode = false;
        bool antiscreenshot = false;
		int fov = 20;
		bool airstuck = true;
		bool showrank = true;
		int fakelagfactor = 9;
		bool fakelag = false;
		bool adaptive = false;
		bool noflash = true;
		float flashalpha = 160.f;
		bool findtarget = false;
		bool spammer = false;
		bool clantag = true;
		int clan = 0;
		// Anti Aim
        int aaY = 0;
        int aaX = 0;
        bool attargets = false;
        int customaa = 0;
        int fakeyaw = 180;
        int realyaw = 20;
		bool fakeaa = false;
    }misc;
    
    class cColors {
    public:
        int tab = 0;
        
        Color ctchams;
        Color tchams;
        
        Color ctchams_ign;
        Color tchams_ign;
        
        Color tbox;
        Color tbox_ign;
        
        Color ctbox;
        Color ctbox_ign;
        
        Color hands;
        Color hands_ign;
        
        Color world;
        
    }colors;
	
	class cMain {
	public:
		int tab = 0;
	}main;
	
    int playerTab = 0;
	bool canmodify;
};

struct Corrections {
    float x, y;
    bool whitelist;
    bool resolved;
    
    int dropdown_x = 0;
    int dropdown_y = 0;
};

extern Corrections gCorrections[64];

extern cVariables vars;
