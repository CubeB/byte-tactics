// Decompiled by Opus. Names are provisional.
// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x416e30 and 0x418ca0).

extern void* g_game;

struct Flags_00417060
{
    unsigned short low : 8;
    unsigned short flag : 1;
    unsigned short rest : 7;
};

// FUNCTION: 0x417060
void __stdcall FUN_00417060(int unused)
{
    Flags_00417060* f = (Flags_00417060*)((char*)g_game + 0x37f2f);
    f->flag = !f->flag;
}
