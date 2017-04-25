#include "main.h"

ISurface* pSurface = nullptr;
IPanel* pPanel = nullptr;
ICvar* pCvar = nullptr;
IBaseClientDLL* pClient = nullptr;
IClientEntityList* pEntList = nullptr;
IVDebugOverlay* pOverlay = nullptr;
IEngineClient* pEngine = nullptr;
IVModelInfo* pModelInfo = nullptr;
IEngineTrace* pEngineTrace = nullptr;
IClientMode* pClientMode = nullptr;
CGlobalVarsBase* pGlobals = nullptr;
IInputSystem* pInputSystem = nullptr;
CInput* pInput = nullptr;
IMaterialSystem* pMatSystem = nullptr;
IVModelRender* pModelRender = nullptr;
IPrediction* pPrediction = nullptr;
IGameMovement* pGameMovement = nullptr;
IMoveHelper* pMoveHelper = nullptr;
IPhysicsSurfaceProps* pPhysics = nullptr;
//
IEngineVGui* pEngineVGui = nullptr;
IGameEventManager2* pGameEventManager = nullptr;
SendClanTagFn SetTag;
MsgFunc_ServerRankRevealAllFn MsgFunc_ServerRankRevealAll;
StartDrawingFn StartDrawing;
FinishDrawingFn FinishDrawing;


VMT* painthook;
VMT* createmovehook;
VMT* clientvmt;
VMT* modelhook;
VMT* predhook;
VMT* vguivmt;
VMT* materialVMT;
VMT* game_event_vmt;

RecvVarProxyFn g_pSequence = nullptr;
