// Decompiled by Haiku. Names are provisional.
struct Game;
extern Game* g_game;

// FUNCTION: 0x4194c0
void __stdcall CmdShowRanges(int unused)
{
    int* ptr = (int*)((char*)g_game + 0x391bf);
    *ptr ^= 1;
}
