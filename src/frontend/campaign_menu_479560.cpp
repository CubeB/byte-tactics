// Decompiled by Sonnet. Names are provisional.

struct Item_00479560 {
    int flag;            // +0x0
    char unknown_4[0x14]; // stride is 0x18 bytes
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x29a0];
    Item_00479560* items;      // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int count;                 // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x479560
int FUN_00479560()
{
    int result = 1;
    Game* g = g_game;
    int n = g->count;
    if (n > 0) {
        Item_00479560* p = g->items;
        do {
            if (p->flag != 0) {
                result = 0;
            }
            p++;
            n--;
        } while (n != 0);
    }
    return result;
}
