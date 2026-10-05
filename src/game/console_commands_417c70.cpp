// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Draws a line from a world position to the same position shifted by
// (dx, dz) whole map units, converting both ends to screen coordinates.
// <stdio.h> is included for the only reason that it makes MSVC 5's register
// allocator keep the shifted z value in ebx (as the original does); nothing
// from it is used.
#include <stdio.h>

struct Pos_00417bb0 {
    unsigned short x_frac;             // +0x0
    short x;                           // +0x2
    unsigned short y_frac;             // +0x4
    short y;                           // +0x6
    unsigned short z_frac;             // +0x8
    short z;                           // +0xa
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00485070(Pos_00417bb0* pos);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);

// FUNCTION: 0x417c70
void __stdcall FUN_00417c70(void* surface, Pos_00417bb0* p, short dx, short dz, int color)
{
    Pos_00417bb0 pos;
    *(int*)&pos.x_frac = (dx << 16) + *(int*)&p->x_frac;
    *(int*)&pos.z_frac = (dz << 16) + *(int*)&p->z_frac;

    int h1 = FUN_00485070(p);
    int x1 = p->x - g_game->scroll_x + 0x80;
    int y1 = p->z - g_game->scroll_y - (h1 >> 1) + 0x20;
    int h2 = FUN_00485070(&pos);
    int x2 = pos.x - g_game->scroll_x + 0x80;
    int y2 = pos.z - g_game->scroll_y - (h2 >> 1) + 0x20;

    FUN_004be950(surface, x1, y1, x2, y2, color & 0xff);
}
