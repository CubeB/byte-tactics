// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xff];
    char owner;                        // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int unknown_110_0 : 6;    // +0x110
    unsigned int state : 2;            // +0x110 bits 6-7
    unsigned int unknown_110_8 : 24;
    char unknown_114[0x118 - 0x114];
};

struct Game_0048c320 {
    char unknown_0[0x2a42];
    char player;                       // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x1435f - 0x1435b];
    unsigned short* list;              // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                         // +0x14367
};
#pragma pack(pop)

extern Game_0048c320* g_game;

// FUNCTION: 0x48c320
void FUN_0048c320(void)
{
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* u = &g_game->units[list[i]];
        if (u->owner == g_game->player)
            u->state = 1;
    }
}
