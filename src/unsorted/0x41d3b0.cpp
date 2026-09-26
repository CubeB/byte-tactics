// Decompiled by Sonnet. Names are provisional.

extern char* g_game;

// FUNCTION: 0x41d3b0
void __stdcall FUN_0041d3b0(int param_1)
{
    *(unsigned int*)(g_game + 0x142fb + param_1 * 4) = *(unsigned int*)(g_game + 0x1431f);
    *(unsigned int*)(g_game + 0x1430b + param_1 * 4) = *(unsigned int*)(g_game + 0x14323);
    *(unsigned char*)(g_game + 0x1431b + param_1) = 1;
}
