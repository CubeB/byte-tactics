// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Point16_004197d0 {
    short x;
    short y;
};

struct Vec3_004197d0 {
    int x;
    int y;
    int z;
};

struct Item_004197d0 {
    char unknown_0[0x14a];
    Point16_004197d0 origin;           // +0x14a
    char unknown_14e[0x249 - 0x14e];
};

struct Player_004197d0 {
    char unknown_0[0x14b];
};

struct BitFlags_004197d0 {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char b6:1;
    unsigned char b7:1;
};

union Flags_004197d0 {
    unsigned char value;
    BitFlags_004197d0 bits;
};

struct Game {
    char unknown_0[0x1b63];
    Player_004197d0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char local_player;        // +0x2a42
    char unknown_2a43[0x2c92 - 0x2a43];
    int field_2c92;                    // +0x2c92
    int field_2c96;                    // +0x2c96
    int field_2c9a;                    // +0x2c9a
    int field_2c9e;                    // +0x2c9e
    int field_2ca2;                    // +0x2ca2
    int field_2ca6;                    // +0x2ca6
    Vec3_004197d0 pos;                 // +0x2caa
    char unknown_2cb6[0x2cc4 - 0x2cb6];
    unsigned short index;              // +0x2cc4
    Flags_004197d0 flags;              // +0x2cc6
    char unknown_2cc7[0x1439b - 0x2cc7];
    Item_004197d0* items;              // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_0047d2e0(Item_004197d0* type, Point16_004197d0 cell, int a,
                           Player_004197d0* player);
int FUN_0047c780(void);
int __stdcall FUN_0047d820(Item_004197d0* unit, Point16_004197d0 cell);

static inline Point16_004197d0 WorldToCell(Vec3_004197d0 v, Point16_004197d0 origin)
{
    Point16_004197d0 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// FUNCTION: 0x4197d0
int FUN_004197d0(void)
{
    Item_004197d0* item = &g_game->items[g_game->index];
    Point16_004197d0 cell = WorldToCell(g_game->pos, item->origin);
    g_game->field_2c92 = cell.x << 4;
    g_game->field_2c9a = cell.y << 4;
    Point16_004197d0 origin = item->origin;
    g_game->field_2c9e = (origin.x << 4) + g_game->field_2c92;
    g_game->field_2ca6 = (origin.y << 4) + g_game->field_2c9a;
    g_game->flags.bits.b6 = FUN_0047d2e0(item, cell, 0, &g_game->players[g_game->local_player]);
    unsigned char r;
    if (g_game->flags.value & 0x40)
        r = FUN_0047c780();
    else
        r = FUN_0047d820(item, cell);
    g_game->field_2c96 = r;
    g_game->field_2ca2 = r;
    return (g_game->flags.value >> 6) & 1;
}
