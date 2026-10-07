// Decompiled by Space Bunny Free. Names are provisional.
// Stores the new game mode in g_game->mode (+0x391f1) and installs the state
// handler for it in g_game->handler (+0x391f5), through the 0x490c14 jump
// table. Mode 6 also gets a different quit callback from SetCloseHandler.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391f1];
    int mode;                          // +0x391f1
    void (*handler)();                 // +0x391f5
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);
void __cdecl FUN_004609a0(int param);
void InitFrame();
void ReturnToMainMenuFrame();
void MenuFrame();
void PreBattleFrame();
void CampaignSetupFrame();
void LoadingScreenFrame();
void BattleFrame();
void EndGameFrame();

// FUNCTION: 0x490b30
void __stdcall SetGameMode(int param)
{
    g_game->mode = param;
    switch (param) {
    case 0:
        g_game->handler = InitFrame;
        break;
    case 1:
        g_game->handler = ReturnToMainMenuFrame;
        break;
    case 2:
        g_game->handler = MenuFrame;
        break;
    case 3:
        g_game->handler = PreBattleFrame;
        break;
    case 4:
        g_game->handler = CampaignSetupFrame;
        break;
    case 5:
        g_game->handler = LoadingScreenFrame;
        break;
    case 6:
        g_game->handler = BattleFrame;
        break;
    case 7:
        g_game->handler = EndGameFrame;
        break;
    default:
        g_game->handler = 0;
        break;
    }
    if (param == 6) {
        SetCloseHandler(FUN_004609a0, 0);
    } else {
        SetCloseHandler(LeaveNetGameCallback, 0);
    }
}
