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
void FUN_00496a60();
void FUN_00496b10();
void FUN_00496bb0();
void FUN_00496ce0();
void FUN_00496db0();
void FUN_00497f40();
void FUN_00499200();
void FUN_00499880();

// FUNCTION: 0x490b30
void __stdcall FUN_00490b30(int param)
{
    g_game->mode = param;
    switch (param) {
    case 0:
        g_game->handler = FUN_00496a60;
        break;
    case 1:
        g_game->handler = FUN_00496b10;
        break;
    case 2:
        g_game->handler = FUN_00496bb0;
        break;
    case 3:
        g_game->handler = FUN_00496ce0;
        break;
    case 4:
        g_game->handler = FUN_00496db0;
        break;
    case 5:
        g_game->handler = FUN_00497f40;
        break;
    case 6:
        g_game->handler = FUN_00499200;
        break;
    case 7:
        g_game->handler = FUN_00499880;
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
