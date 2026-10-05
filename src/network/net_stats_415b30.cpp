// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148e7];
    int count;          // +0x148e7
    void** items;       // +0x148eb
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

void __stdcall StepGafSequence(void* item);

// FUNCTION: 0x415b30
void FUN_00415b30(void)
{
    for (int i = g_game->count - 1; i >= 0; i--)
        StepGafSequence(g_game->items[i]);
}
