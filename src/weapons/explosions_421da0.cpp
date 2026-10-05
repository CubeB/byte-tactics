// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Vec3_00421da0 {
    int x;
    int y;
    int z;
};

struct Point16_00421da0 {
    short x;
    short z;
};

struct Cell_00421da0 {
    char unknown_0[8];
    unsigned short feature;          // +0x8
    unsigned char offsetY;           // +0xa
    unsigned char offsetX;           // +0xb
    char unknown_c;
};

struct Feature_00421da0 {
    char unknown_0[0x94];
    Point16_00421da0 footprint;      // +0x94
    char unknown_98[0x100 - 0x98];
};

struct Game {
    char unknown_0[0x1426f];
    Feature_00421da0* features;      // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;

Cell_00421da0* __stdcall GetMapCell(int x, int y);

// FUNCTION: 0x421da0
unsigned short __stdcall FindFeatureAtPos(Vec3_00421da0* pos, Point16_00421da0* cell, Point16_00421da0* size)
{
    Point16_00421da0 c;
    c.x = (short)(pos->x >> 20);
    c.z = (short)(pos->z >> 20);
    Cell_00421da0* p = GetMapCell(c.x, c.z);
    if (p == 0)
        return 0xffff;
    if (p->feature == 0xfffe) {
        c.x -= p->offsetX;
        c.z -= p->offsetY;
        p = GetMapCell(c.x, c.z);
    }
    if (p->feature >= 0xfffb)
        return 0xffff;
    if (cell)
        *cell = c;
    if (size)
        *size = g_game->features[p->feature].footprint;
    return p->feature;
}
