// Decompiled by space-bunny-free. Names are provisional.
// Bilinearly interpolates the map cell height under a 12.4 fixed-point
// position: returns -1 when the 2x2 cell block the sample needs is off the map.

#pragma pack(push, 1)
struct Cell_00485070 {
    char unknown_0[0x4];
    unsigned char height;              // +0x4
    char unknown_5[0xd - 0x5];
};

struct Game_00485070 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_00485070* cells;              // +0x14287
};
#pragma pack(pop)

struct Fixed_00485070 {
    unsigned short frac;
    short whole;
};

struct Pos_00485070 {
    Fixed_00485070 x;                  // +0x0
    Fixed_00485070 y;                  // +0x4
    Fixed_00485070 z;                  // +0x8
};

extern Game_00485070* g_game;

// FUNCTION: 0x485070
int __stdcall FUN_00485070(Pos_00485070* p)
{
    int x = p->x.whole >> 4;
    int z = p->z.whole >> 4;
    int xf = p->x.whole & 15;
    int zf = p->z.whole & 15;
    if (x < 0 || x + 1 >= g_game->width || z < 0 || z + 1 >= g_game->height)
        return -1;
    Cell_00485070* c = &g_game->cells[z * g_game->width + x];
    int a = c->height;
    int b = c[1].height;
    int e = c[g_game->width].height;
    int f = c[g_game->width + 1].height;
    int row0 = a + (b - a) * xf / 16;
    int row1 = e + (f - e) * xf / 16;
    return row0 + (row1 - row0) * zf / 16;
}
