// Decompiled by Opus. Names are provisional.
// Advances the current player's cyclic counter (0..5), then refreshes.

extern char* g_game;

void RefreshAllyIcons();

// FUNCTION: 0x47ae30
void __cdecl CycleCurrentPlayerAllyGroup()
{
    char* base = *(char**)(g_game + 0x29a0);
    int* p = (int*)(base + *(int*)(base + 0x224) * 24 + 8);
    *p = (*p + 1) % 6;
    RefreshAllyIcons();
}
