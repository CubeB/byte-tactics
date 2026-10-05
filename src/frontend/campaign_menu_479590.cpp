// Decompiled by Opus. Names are provisional.

struct Item_00479590 {
    int id;                            // +0x0, 0 = free slot
    char unknown_4[0x10];
    int owner;                         // +0x14
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x29a0];
    Item_00479590* items;              // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int itemCount;                     // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x479590
int __stdcall IsColorTaken(int owner, int skip)
{
    for (int i = 0; i < g_game->itemCount; i++) {
        if (g_game->items[i].owner == owner && g_game->items[i].id != 0 && i != skip) {
            return 1;
        }
    }
    return 0;
}
