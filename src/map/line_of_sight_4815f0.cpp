// Decompiled by Space Bunny Free. Names are provisional.
// Returns the map cell under a 16.16 world position (x at +0, z at +8), or
// null when it lies outside the map. A cell that is part of a feature footprint
// (0xfffe) is skipped: the position is corrected by the cell's own footprint
// offset and the lookup repeated.

#pragma pack(push, 1)
union SpotField_004815f0 {
    struct {
        unsigned char offsetY;           // +0xa
        unsigned char offsetX;           // +0xb
    };
    unsigned short spot;                 // +0xa
};

struct Cell_004815f0 {
    char unknown_0[8];
    unsigned short feature;              // +0x8
    SpotField_004815f0 sf;               // +0xa
    unsigned char flags;                 // +0xc
};

struct Game {
    char unknown_0[0x14233];
    int width;                           // +0x14233
    int height;                          // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_004815f0* cells;                // +0x14287
};
#pragma pack(pop)

struct Vec3_004815f0 {
    int x;
    int y;
    int z;
};

extern Game* g_game;

// FUNCTION: 0x4815f0
Cell_004815f0* __stdcall FUN_004815f0(Vec3_004815f0* pos)
{
    int x = pos->x >> 20;
    int z = pos->z >> 20;
    if (x < 0 || x >= g_game->width || z < 0 || z >= g_game->height)
        return 0;
    Cell_004815f0* cell = &g_game->cells[z * g_game->width + x];
    if (cell->feature == 0xfffe)
    {
        x -= cell->sf.offsetX;
        z -= cell->sf.offsetY;
        if (x < 0 || x >= g_game->width || z < 0 || z >= g_game->height)
            cell = 0;
        else
            cell = &g_game->cells[z * g_game->width + x];
    }
    return cell;
}
