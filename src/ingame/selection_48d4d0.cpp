// Decompiled by space-bunny-free. Names are provisional.
// Picks a unit of the local player, scrolls the map to it and marks it (and
// every unit in g_game->list with the same owner) as selected. When the first
// search fails, the selected flag of every unit is cleared and the search is
// repeated, so search, clear and search form one inlined helper.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x6a];
    int pos_x;                           // +0x6a
    char unknown_6e[0xa6 - 0x6e];
    unsigned short field_a6;             // +0xa6
    unsigned short field_a8;             // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char owner;                 // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int unknown_110_0 : 6;      // +0x110
    unsigned int state : 2;              // +0x110 bits 6-7
    unsigned int unknown_110_8 : 24;
    char unknown_114[0x118 - 0x114];
};

struct Player_0048d4d0 {                 // 0x14b bytes
    char unknown_0[0x67];
    Unit* units_first;                   // +0x67
    Unit* units_last;                    // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0048d4d0 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units;                         // +0x14357
    Unit* units_last;                    // +0x1435b
    unsigned short* list;                // +0x1435f
    char unknown_14363[0x14367 - 0x14363];
    int count;                           // +0x14367
    char unknown_1436b[0x1436f - 0x1436b];
    unsigned short field_1436f;          // +0x1436f
};

struct Pos_0048d4d0 {
    int x;
    int y;
    int z;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall CenterCameraOnMapPosition(Pos_0048d4d0* p, int param_2);
void FUN_0048bae0(void);

static inline Unit* PickUnit(Player_0048d4d0* player)
{
    Unit* u = player->units_first;
    while (u <= player->units_last) {
        if (u->field_a6 != 0) {
            if (u->state == 0)
                return u;
        }
        u++;
    }
    Unit* q = g_game->units;
    while (q <= g_game->units_last) {
        q->state = 0;
        q++;
    }
    u = player->units_first;
    while (u <= player->units_last) {
        if (u->field_a6 != 0) {
            if (u->state == 0)
                return u;
        }
        u++;
    }
    return 0;
}

// FUNCTION: 0x48d4d0
void FUN_0048d4d0(void)
{
    Unit* u = PickUnit(&g_game->players[g_game->player]);
    if (u == 0)
        return;
    g_game->field_1436f = u->field_a8;
    CenterCameraOnMapPosition((Pos_0048d4d0*)&u->pos_x, 1);
    FUN_0048bae0();
    unsigned short* list = g_game->list;
    for (int i = 0; i < g_game->count; i++) {
        Unit* unit = &g_game->units[list[i]];
        if (unit->owner == g_game->player)
            unit->state = 1;
    }
    u->state = 1;
}
