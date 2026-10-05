// Decompiled by Opus. Names are provisional.
// Toggles one flag bit in a 16-bit bitfield inside the game state.

extern void* g_game;

struct Flags_418ca0
{
    unsigned short low : 10;
    unsigned short flag : 1;
    unsigned short rest : 5;
};

// FUNCTION: 0x418ca0
void __stdcall CmdShootAll(int unused)
{
    Flags_418ca0* f = (Flags_418ca0*)((char*)g_game + 0x37f2f);
    f->flag = !f->flag;
}
