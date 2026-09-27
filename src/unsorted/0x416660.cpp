// Decompiled by Sonnet. Names are provisional.
// Toggles one flag bit in a 16-bit bitfield inside the game state (the same
// flags word as 0x416630 and 0x417060).

extern char* g_game;

struct Flags_00416660
{
    unsigned short low : 4;
    unsigned short flag : 1;
    unsigned short rest : 11;
};

// FUNCTION: 0x416660
void __stdcall FUN_00416660(int unused)
{
    Flags_00416660* f = (Flags_00416660*)((char*)g_game + 0x37f06);
    f->flag = !f->flag;
}
