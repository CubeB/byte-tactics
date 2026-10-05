// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14281];
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short flag2 : 1; // +0x14281, bit 2 (mask 4)
    unsigned short rest : 13;
};
#pragma pack(pop)

extern Game* g_game;

extern void __stdcall FUN_004816a0(int flag);

// FUNCTION: 0x416690
void __stdcall CmdLOSType(int unused)
{
    g_game->flag2 = !g_game->flag2;
    FUN_004816a0(0);
}
