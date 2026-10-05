// Decompiled by Sonnet. Names are provisional.

extern void* g_game;

struct Flags_00417090
{
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short b5 : 1;
    unsigned short b6 : 1;
    unsigned short b7 : 1;
    unsigned short b8 : 1;
    unsigned short b9 : 1;
    unsigned short rest : 6;
};

// FUNCTION: 0x417090
void __stdcall FUN_00417090(int unused)
{
    Flags_00417090* f = (Flags_00417090*)((char*)g_game + 0x37f2f);
    f->b9 = !f->b9;
}
