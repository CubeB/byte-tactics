// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a43];
    unsigned char playerIndex;          // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;     // +0x14273
};
#pragma pack(pop)

struct MapSize_00408090 {
    unsigned int width;                 // +0x0
    unsigned int height;                // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct Map_00408090 {
    char unknown_0[0x80];
    MapSize_00408090 size;              // +0x80
};

struct Position_00408090 {              // 16.16 fixed point; only high words read
    short xFrac;
    short x;                            // +0x2
    short yFrac;
    short y;                            // +0x6
    short zFrac;
    short z;                            // +0xa
};

extern Game* g_game;

// FUNCTION: 0x408090
int __stdcall IsPointVisible(Map_00408090* map, Position_00408090* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->size.Contains(tx, ty)) {
        return 0;
    }
    return (g_game->visibilityMask[map->size.width * ty + tx] & (1 << g_game->playerIndex)) != 0;
}
