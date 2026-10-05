// Decompiled by Opus. Names are provisional.
// Converts a 16.16 fixed-point world position into screen coordinates.

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

// FUNCTION: 0x417bb0
void __stdcall FUN_00417bb0(Pos_00417bb0* pos, int* screen_x, int* screen_y)
{
    int height = FUN_00485070(pos);
    *screen_x = pos->x - g_game->scroll_x + 0x80;
    *screen_y = pos->z - g_game->scroll_y - (height >> 1) + 0x20;
}
