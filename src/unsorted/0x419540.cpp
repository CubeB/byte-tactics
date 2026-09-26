// Decompiled by Haiku. Names are provisional.
struct Game;
extern Game* g_game;

// FUNCTION: 0x419540
void __stdcall FUN_00419540(int unused)
{
    *(int*)((char*)g_game + 0x391c3) ^= 1;
}
