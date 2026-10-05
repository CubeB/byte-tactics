// Decompiled by Sonnet. Names are provisional.

extern void* g_game;

struct Flags_00416e30
{
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short rest : 12;
};

// FUNCTION: 0x416e30
void __stdcall CmdTreeDeath(int unused)
{
    Flags_00416e30* f = (Flags_00416e30*)((char*)g_game + 0x37f2f);
    f->b3 = !f->b3;
}
