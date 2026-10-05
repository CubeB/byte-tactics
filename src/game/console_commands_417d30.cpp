// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Draws a line on the given surface between two map positions converted to
// screen coordinates: a start point (a 4-byte pair of shorts) and an end point
// formed by adding the dx/dz deltas to it.

struct Pos_00417bb0 {
    unsigned short x_frac;             // +0x0
    short x;                           // +0x2
    unsigned short y_frac;             // +0x4
    short y;                           // +0x6
    unsigned short z_frac;             // +0x8
    short z;                           // +0xa
};

struct Point16 {
    short x;                           // +0x0
    short z;                           // +0x2
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGroundHeight(Pos_00417bb0* pos);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);

static inline void WorldToScreen(Pos_00417bb0* pos, int* screen_x, int* screen_y)
{
    int height = GetGroundHeight(pos);
    *screen_x = pos->x - g_game->scroll_x + 0x80;
    *screen_y = pos->z - g_game->scroll_y - (height >> 1) + 0x20;
}

// FUNCTION: 0x417d30
void __stdcall FUN_00417d30(void* surface, Point16 from, short dx, short dz, int color)
{
    Pos_00417bb0 pos1, pos2;
    int x1 = from.x << 16;
    int z1 = from.z << 16;
    *(int*)&pos1.x_frac = x1;
    *(int*)&pos1.z_frac = z1;
    *(int*)&pos2.x_frac = x1 + (dx << 16);
    *(int*)&pos2.z_frac = z1 + (dz << 16);
    int sx1, sy1, sx2, sy2;
    WorldToScreen(&pos1, &sx1, &sy1);
    WorldToScreen(&pos2, &sx2, &sy2);
    DrawLine(surface, sx1, sy1, sx2, sy2, color & 0xff);
}
