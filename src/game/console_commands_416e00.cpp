// Decompiled by Sonnet. Names are provisional.
// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x417060, mask 4 = bit 2).

extern char* g_game;

struct Flags_00416e00
{
    unsigned short low : 2;
    unsigned short flag : 1;
    unsigned short rest : 13;
};

// FUNCTION: 0x416e00
void __stdcall FUN_00416e00(int unused)
{
    Flags_00416e00* f = (Flags_00416e00*)((char*)g_game + 0x37f2f);
    f->flag = !f->flag;
}
