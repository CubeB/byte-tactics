// Decompiled by Opus. Names are provisional.
// Toggles bit 4 of the flags word at g_game+0x37f2f (like 0x416e30, bit 3).

extern void* g_game;

struct Flags_00416e60
{
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short rest : 11;
};

// FUNCTION: 0x416e60
void __stdcall FUN_00416e60(int unused)
{
    Flags_00416e60* f = (Flags_00416e60*)((char*)g_game + 0x37f2f);
    f->b4 = !f->b4;
}
