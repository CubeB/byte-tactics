// Decompiled by Sonnet. Names are provisional.
// Advances the current player's cyclic counter: counter = (counter + 1) % n.

extern char* g_game;

// FUNCTION: 0x47a8e0
void __cdecl CycleCurrentPlayerSide()
{
    char* base = *(char**)(g_game + 0x29a0);
    int* p = (int*)(base + *(int*)(base + 0x224) * 24 + 4);
    *p = (*p + 1) % *(int*)(g_game + 0x37f39);
}
