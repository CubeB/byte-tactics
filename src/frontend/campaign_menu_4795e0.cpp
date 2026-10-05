// Decompiled by Opus. Names are provisional.
// Returns the lowest owner id in 0..9 that no item uses, or -1.

struct Item_004795e0 {
    int id;                            // +0x0
    char unknown_4[0x10];
    int owner;                         // +0x14
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x29a0];
    Item_004795e0* items;              // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int itemCount;                     // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4795e0
int FUN_004795e0()
{
    for (int owner = 0; owner < 10; owner++) {
        int i;
        for (i = 0; i < g_game->itemCount; i++) {
            if (g_game->items[i].owner == owner)
                break;
        }
        if (i == g_game->itemCount)
            return owner;
    }
    return -1;
}
