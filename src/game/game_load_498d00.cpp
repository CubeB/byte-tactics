// Decompiled by Opus. Names are provisional.
// Converts the screen position stored at +0x2c76 into a 16.16 world position
// (scaled from the view origin), with the height taken from the ground there.
#include <string.h>

struct Fixed_00498d00 {
    unsigned short frac;               // +0x0
    short whole;                       // +0x2
};

struct Pos_00498d00 {
    Fixed_00498d00 x;                  // +0x0
    Fixed_00498d00 y;                  // +0x4
    Fixed_00498d00 z;                  // +0x8
};

struct Rect_00498d00 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2c76];
    Rect_00498d00 view;                // +0x2c76
    char unknown_2c8e[0x1422b - 0x2c8e];
    int world_w;                       // +0x1422b
    int world_h;                       // +0x1422f
    char unknown_14233[0x142e7 - 0x14233];
    short origin_x;                    // +0x142e7
    short origin_y;                    // +0x142e9
    short screen_w;                    // +0x142eb
    short screen_h;                    // +0x142ed
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00485070(Pos_00498d00* pos);

// FUNCTION: 0x498d00
void __stdcall FUN_00498d00(Pos_00498d00* out)
{
    Rect_00498d00 r = g_game->view;
    int dx = r.x - g_game->origin_x;
    int dy = r.y - g_game->origin_y;
    memset(out, 0, sizeof(Pos_00498d00));
    out->x.whole = g_game->world_w * dx / g_game->screen_w;
    out->z.whole = g_game->world_h * dy / g_game->screen_h;
    out->y.whole = FUN_00485070(out);
}
