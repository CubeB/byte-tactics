// Decompiled by Opus. Names are provisional.
// Toggles one flag bit in a 16-bit bitfield inside the game state.

extern void* g_game;

struct Flags_416630
{
    unsigned short low : 3;
    unsigned short flag : 1;
    unsigned short rest : 12;
};

// FUNCTION: 0x416630
void __stdcall FUN_00416630(int unused)
{
    Flags_416630* f = (Flags_416630*)((char*)g_game + 0x37f06);
    f->flag = !f->flag;
}
