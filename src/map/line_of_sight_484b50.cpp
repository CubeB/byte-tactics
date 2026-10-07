// Decompiled by Space Bunny Free, finished by space-bunny-free, finished by Sonnet 5.5. Names are provisional.
// Maps a map pixel (x, y) onto the terrain: walks z down from just above row y
// 16 cells at a time until the projected row (z - ground / 2) reaches y, then
// interpolates between that row and the next one.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x1427f - 0x1422b];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

extern Game* g_game;

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

int __stdcall GetGroundHeight(Pos_00484b50* pos);

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
    int s1;
    int s2;
    p.x.value = x << 16;
    int t = y & ~0xf;
    int i;
    for (i = 0x80; i >= 0; i -= 0x10) {
        // z comes from the counter, not from a variable of its own.
        p.z.value = (t + i) << 16;
        p.y.value = max(GetGroundHeight(&p), g_game->seaLevel) << 16;
        s1 = p.z.parts.whole - (p.y.parts.whole >> 1);
        if (s1 <= y)
            goto found;
    }
    goto done;

found:
    {
        Pos_00484b50 q = p;
        q.z.value = p.z.value + 0x100000;
        q.y.value = max(GetGroundHeight(&q), g_game->seaLevel) << 16;
        s2 = q.z.parts.whole - (q.y.parts.whole >> 1);
        if ((s1 < s2 && y >= s1) || y <= s2) {
            p.z.value = p.z.value + ((y - s1) << 20) / (s2 - s1);
            p.y.value = max(GetGroundHeight(&p), g_game->seaLevel) << 16;
        }
    }
done:
    *out = p;
}
