// Decompiled by Space Bunny Free. Names are provisional.
// Height of the ground under a unit's footprint: scans the footprint rectangle
// of map cells, keeping the lowest cell height over the cells the unit's mask
// marks and the highest over the cells it does not. If nothing was marked, the
// water surface minus the type's draft is returned instead.
#pragma pack(push, 1)

struct Point {
    short x;
    short y;
};

struct Cell {
    char unknown_0[0x5];
    unsigned char field_5;              // +0x5
    unsigned char field_6;              // +0x6
    char unknown_7[0xd - 0x7];
};

struct Unit_0047d820 {
    char unknown_0[0x14a];
    Point origin;                       // +0x14a, footprint in map cells
    char* mask;                         // +0x14e, one byte per footprint cell
    char unknown_152[0x22c - 0x152];
    unsigned char draft;                // +0x22c
};

struct Game_0047d820 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    Cell* cells;                        // +0x14287
};
#pragma pack(pop)

extern Game_0047d820* g_game;

// FUNCTION: 0x47d820
int __stdcall FUN_0047d820(Unit_0047d820* unit, Point cell)
{
    Point fp = unit->origin;
    if (cell.x < 1 || cell.y < 1 || cell.x + fp.x >= g_game->width ||
        cell.y + fp.y >= g_game->height)
        return 0;
    int width = g_game->width;
    Cell* c = &g_game->cells[cell.y * width + cell.x];
    unsigned char low = 0xff;
    unsigned char high = 0;
    unsigned char high2 = 0;
    int i = 0;
    for (int row = fp.y; row > 0; row--) {
        for (int col = fp.x; col > 0; col--) {
            unsigned char f = unit->mask[i++];
            if (f & 8) {
                if (c->field_6 < low)
                    low = c->field_6;
                if (c->field_5 > high)
                    high = c->field_5;
            }
            if (f & 0x10) {
                if (c->field_5 > high2)
                    high2 = c->field_5;
            }
            c++;
        }
        c += width - fp.x;
    }
    unsigned char r = low;
    if (high < low)
        r = g_game->seaLevel - unit->draft;
    return r;
}
