// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern void __stdcall FUN_004199b0(void*, void*);

// FUNCTION: 0x41c150
void __stdcall FUN_0041c150(void* param_1)
{
    void* g = g_game;
    unsigned short val1 = *(unsigned short*)((char*)g + 0x37e9c);
    unsigned short val2 = *(unsigned short*)((char*)param_1 + 0xa8);
    if (val1 == val2) {
        FUN_004199b0((char*)g + 0x519, param_1);
    }
}
