// Decompiled by Opus. Names are provisional.

extern char* g_game;

void __stdcall FreeSurface(void* param_1);

// FUNCTION: 0x466aa0
void FreeRadar()
{
    FreeSurface(*(void**)(g_game + 0x142e3));
    FreeSurface(*(void**)(g_game + 0x142df));
    FreeSurface(*(void**)(g_game + 0x142db));
    *(void**)(g_game + 0x142e3) = 0;
    *(void**)(g_game + 0x142df) = 0;
    *(void**)(g_game + 0x142db) = 0;
}
