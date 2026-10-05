// Decompiled by Opus. Names are provisional.

struct Sub_00496db0 {
    char unknown_0[0x96];
    char flag_96;                      // +0x96
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1cd5];
    Sub_00496db0* sub_1cd5;            // +0x1cd5
    char unknown_1cd9[0x2a3c - 0x1cd9];
    unsigned short state_2a3c;         // +0x2a3c
    char unknown_2a3e[0x391f1 - 0x2a3e];
    int mode;                          // +0x391f1
    void (*handler)();                 // +0x391f5
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00464290(unsigned char player, unsigned char kind);
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param);
void __cdecl LeaveNetGameCallback(int param);
void FUN_00497f40();

// FUNCTION: 0x496db0
void FUN_00496db0()
{
    g_game->state_2a3c = 2;
    FUN_00464290(0, 1);
    FUN_00464290(1, 2);
    g_game->sub_1cd5->flag_96 = 1;
    g_game->mode = 5;
    g_game->handler = FUN_00497f40;
    FUN_004b4fd0(LeaveNetGameCallback, 0);
}
