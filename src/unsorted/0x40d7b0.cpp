// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Partial (67.5%): a map cell lookup. Everything matches except the prologue
// register allocation. The original keeps the first argument x in edx
// (`cmp edx, [edi+0x10]`, then `sar edx,1` in place), which cascades to
// cx in edx, cy in eax and w in esi; here x is allocated to eax, so cy takes
// esi, w takes eax and the final `imul` has its operands swapped. ~40
// variants (unsigned params, early `x >> 1`, separate ifs, reversed operand
// order, w declared before/after cx/cy, `map` re-read) never allocate x to
// edx.
#pragma pack(push, 1)
struct Map_0040d7b0 {
    char unknown_0[4];
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* data;                // +0x18
};

struct Game_0040d7b0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273
};
#pragma pack(pop)

extern Game_0040d7b0* g_game;

struct Class_0040d7b0 {
    char unknown_0[0x64];
    Map_0040d7b0* map;                 // +0x64
    char unknown_68[0x78 - 0x68];
    unsigned char field_78;            // +0x78

    int FUN_0040d7b0(int x, int y);
};

// FUNCTION: 0x40d7b0
int Class_0040d7b0::FUN_0040d7b0(int x, int y)
{
    Map_0040d7b0* map = this->map;
    if (x >= map->width || y >= map->height)
        return 0;
    int cx = (x >> 1) + (map->field_4 >> 2);
    int cy = (y >> 1) + (map->field_6 >> 2);
    int w = g_game->width >> 1;
    if (cx >= w || cy >= (g_game->height >> 1))
        return 0;
    if (!((1 << this->field_78) & g_game->visibilityMask[cy * w + cx]))
        return 2;
    Map_0040d7b0* map2 = this->map;
    return (map2->data[map2->width * (y >> 4) + x] >> ((y & 0xf) << 1)) & 3;
}
