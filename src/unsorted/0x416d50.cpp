// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14281];
    unsigned short bit0 : 1;
    unsigned short flag1 : 1; // +0x14281, bit 1 (mask 2)
    unsigned short rest : 14;
};
#pragma pack(pop)

extern Game* g_game;

extern void FUN_00430f00();
extern void __stdcall FUN_004816a0(int flag);

// FUNCTION: 0x416d50
void __stdcall FUN_00416d50(int unused)
{
    g_game->flag1 = !g_game->flag1;
    FUN_00430f00();
    FUN_004816a0(0);
}
