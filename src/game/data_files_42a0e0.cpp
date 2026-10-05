// Decompiled by Opus. Names are provisional.
// Appends an item to the game's growable "Animplay Pointers" array (the
// array that 0x415b30 walks).

#pragma pack(push, 1)
struct GameState {
    char unknown_0[0x148e7];
    int count;          // +0x148e7
    void** items;       // +0x148eb
};
#pragma pack(pop)

extern GameState* g_game;

// Reallocates a named block (its own file returns void; the result is used here).
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x42a0e0
void __stdcall FUN_0042a0e0(void* item)
{
    g_game->items = (void**)FUN_004d84a0(g_game->items, "Animplay Pointers", (g_game->count + 1) * 4);
    g_game->items[g_game->count] = item;
    g_game->count++;
}
