#include "../main.h"

void HSequenceProxyFn(const CRecvProxyData *pDataConst, void *pStruct, void *pOut);

class cSkin{
public:
    
    cSkin(){}
    
    cSkin(int paintkit, int seed, int itemDefinition, int stattrack, char* szName, float flWear) :
    Paintkit(paintkit), Seed(seed), ItemDefinition(itemDefinition), StatTrack(stattrack), name(szName), Wear(flWear){}
    
    int Paintkit = 0;
    int Seed = 0;
    int ItemDefinition = 0;
    int StatTrack = -1;
    char* name = nullptr;
    float Wear = 0.0001f;
};

class cSkinchanger {
public:
    
    static unordered_map<int, cSkin> Skins;
    static unordered_map<int, const char*> Models;
    
    unordered_map<int, string> KnifeToModel = unordered_map<int, string>({
        {WEAPON_KNIFE_BAYONET, "models/weapons/v_knife_bayonet.mdl"},
        {WEAPON_KNIFE_FLIP, "models/weapons/v_knife_flip.mdl"},
        {WEAPON_KNIFE_GUT, "models/weapons/v_knife_gut.mdl"},
        {WEAPON_KNIFE_KARAMBIT, "models/weapons/v_knife_karam.mdl"},
        {WEAPON_KNIFE_M9_BAYONET, "models/weapons/v_knife_m9_bay.mdl"},
        {WEAPON_KNIFE_TACTICAL, "models/weapons/v_knife_tactical.mdl"},
        {WEAPON_KNIFE_FALCHION, "models/weapons/v_knife_falchion_advanced.mdl"},
        {WEAPON_KNIFE_SURVIVAL_BOWIE, "models/weapons/v_knife_survival_bowie.mdl"},
        {WEAPON_KNIFE_BUTTERFLY, "models/weapons/v_knife_butterfly.mdl"},
        {WEAPON_KNIFE_PUSH, "models/weapons/v_knife_push.mdl"}
    });
    
    unordered_map<int, string> GloveToModel = unordered_map<int, string>({
        {GLOVE_STUDDED_BLOODHOUND, "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"},
        {GLOVE_SPORTY, "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"},
        {GLOVE_SLICK, "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"},
        {GLOVE_LEATHER_WRAP, "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"},
        {GLOVE_MOTORCYCLE, "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"},
        {GLOVE_SPECIALIST, "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"}
    });
    
    void FindModels();
    void Init();
    void LoadSkins();
    void FrameStageNotify(ClientFrameStage_t stage);
    void FireEventClientSide(IGameEvent* event);
    
    C_BaseEntity* local = nullptr;
    
    bool bInit = false;
};

extern cSkinchanger* skinchanger;
