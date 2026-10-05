// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern void FUN_00430f00();

// FUNCTION: 0x416590
void __stdcall FUN_00416590(int unused)
{
    void* ecx = g_game;
    unsigned short ax = *(unsigned short*)((char*)ecx + 0x37f06);
    unsigned short edx = ~ax;
    edx ^= ax;
    edx &= 0x40;
    edx ^= ax;
    *(unsigned short*)((char*)ecx + 0x37f06) = edx;
    FUN_00430f00();
}
