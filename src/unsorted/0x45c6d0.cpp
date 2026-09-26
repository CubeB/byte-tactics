// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct GameState_45c6d0 {
    char pad0[0x1434d];
    char f1434d;                      // +0x1434d
    char pad1[0x37efa - 0x1434d - 1];
    int f37efa;                       // +0x37efa
    char pad2[0x37f17 - 0x37efa - 4];
    char f37f17;                      // +0x37f17
    char f37f18;                      // +0x37f18
    char pad3[0x37f23 - 0x37f18 - 1];
    int f37f23;                       // +0x37f23
    int f37f27;                       // +0x37f27
    char pad4[0x38a4b - 0x37f27 - 4];
    short f38a4b;                     // +0x38a4b
    short f38a4d;                     // +0x38a4d
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern GameState_45c6d0* g_game;

// FUNCTION: 0x45c6d0
void FUN_0045c6d0()
{
    g_game->f37f23 = 10;
    g_game->f37f27 = 10;
    g_game->f38a4b = 10;
    g_game->f38a4d = 10;
    g_game->f1434d = 0x20;
    g_game->f37efa = 0;
    g_game->f37f17 = 10;
    g_game->f37f18 = 5;
}
