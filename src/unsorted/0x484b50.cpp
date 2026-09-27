// Decompiled by Space Bunny Free. Names are provisional.
// Maps a screen position (a, b) onto the map: walks z down from just above
// row b, 16 cells at a time, until the isometric projection row
// (z - ground / 2) drops to b, then reports the point and its ground height.
// Pos components are 20.12 fixed point, and the height map works on the 12.4
// value in the high half, so each component doubles as a plain short.

#pragma pack(push, 1)
struct Game_00484b50 {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x1427f - 0x1422b];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

extern Game_00484b50* g_game;

struct Fixed_00484b50 {
    unsigned short frac;               // +0x0
    short whole;                       // +0x2
};

union Coord_00484b50 {
    int value;
    Fixed_00484b50 parts;
};

struct Pos_00484b50 {
    Coord_00484b50 x;                  // +0x0
    Coord_00484b50 y;                  // +0x4
    Coord_00484b50 z;                  // +0x8
};

int __stdcall FUN_00485070(Pos_00484b50* pos);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x484b50
void __stdcall FUN_00484b50(int x, int y, Pos_00484b50* out)
{
    if (x < 0)
        x = 0;
    if (x >= g_game->baseX)
        x = g_game->baseX - 1;
    if (y < 0)
        y = 0;
    if (y >= g_game->baseY)
        y = g_game->baseY - 1;

    Pos_00484b50 p;
    Pos_00484b50 probe;
    int s1;
    int s2;
    p.x.value = x << 16;
    int z = ((y & ~15) + 0x80) << 16;
    for (int i = 0x80; i >= 0; i -= 0x10) {
        p.z.value = z;
        p.y.value = max(FUN_00485070(&p), g_game->seaLevel) << 16;
        s1 = p.z.parts.whole - (p.y.parts.whole >> 1);
        if (s1 <= y)
            goto found;
        z -= 0x100000;
    }
    goto done;

found:
    probe.x.value = p.x.value;
    probe.y.value = p.y.value;
    probe.z.value = p.z.value;
    probe.z.value += 0x100000;
    probe.y.value = max(FUN_00485070(&probe), g_game->seaLevel) << 16;
    s2 = probe.z.parts.whole - (probe.y.parts.whole >> 1);
    if (s1 >= s2 || y < s1) {
        if (y > s2)
            goto done;
    }
    probe.x.value = p.z.value + (((y - s1) << 20) / (s2 - s1));
    p.y.value = max(FUN_00485070(&p), g_game->seaLevel) << 16;

done:
    out->x.value = p.x.value;
    out->y.value = p.y.value;
    out->z.value = p.z.value;
}
