// Decompiled by space-bunny-free. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int unknown_110_0 : 6;    // +0x110 bits 0-5
    unsigned int state : 2;            // +0x110 bits 6-7
    unsigned int unknown_110_8 : 24;   // +0x110 bits 8-31
    char unknown_114[0x118 - 0x114];
};

struct Player_0048d420 {               // 0x14b bytes
    char unknown_0[0x67];
    Unit* unitsBegin;                  // +0x67
    Unit* unitsEnd;                    // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d420 {
    char unknown_0[0x1b63];
    Player_0048d420 players[10];       // +0x1b63, stride 0x14b
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;              // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit* units;                       // +0x14357
    Unit* unitsEnd;                    // +0x1435b
};
#pragma pack(pop)

extern Game_0048d420* g_game;

// FUNCTION: 0x48d420
Unit* FUN_0048d420(void)
{
    Player_0048d420* player = &g_game->players[g_game->player];
    Unit* u;
    for (u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->field_a6 != 0) {
            if (u->state == 0)
                return u;
        }
    }
    for (Unit* p = g_game->units; p <= g_game->unitsEnd; p++) {
        p->state = 0;
    }
    for (u = player->unitsBegin; u <= player->unitsEnd; u++) {
        if (u->field_a6 != 0) {
            if (u->state == 0)
                return u;
        }
    }
    return 0;
}
