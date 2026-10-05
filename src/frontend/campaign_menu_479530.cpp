// Decompiled by Opus. Names are provisional.

struct Item_479530 {
    int id;                            // +0x0, 0 = free slot
    char unknown_4[0x14];
};

#pragma pack(push, 1)
struct GameState_479530 {
    char unknown_0[0x29a0];
    Item_479530* items;                // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int itemCount;                     // +0x38d81
};
#pragma pack(pop)

extern GameState_479530* g_game;

// FUNCTION: 0x479530
int FUN_00479530()
{
    for (int i = 0; i < g_game->itemCount; i++) {
        if (g_game->items[i].id == 0) {
            return i;
        }
    }
    return -1;
}
