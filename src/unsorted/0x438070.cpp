// Decompiled by Opus. Names are provisional.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game_00438070 {
    char unknown_0[0x14233];
    int mapWidth;                    // +0x14233
    int mapHeight;                   // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    int field_38a47;                 // +0x38a47
};
#pragma pack(pop)

struct Point16_00438070 {
    short x;
    short y;
    Point16_00438070() {}
    Point16_00438070(int x_, int y_) : x(x_), y(y_) {}
    Point16_00438070& operator+=(const Point16_00438070& o)
    {
        x += o.x;
        y += o.y;
        return *this;
    }
};

extern Game_00438070* g_game;
extern int DAT_00512318;
extern int DAT_0051231c;
extern int DAT_00512324;
extern int DAT_00512338;
extern int DAT_005122e8;
extern int DAT_00512330;
extern Point16_00438070 DAT_00512334;
extern Point16_00438070 DAT_00512320;

// Only the tail differs: the original stores the x offset to the stack and
// reloads it before adding DAT_00512334.x; this version keeps it in eax.
// FUNCTION: 0x438070
void FUN_00438070()
{
    DAT_00512318 = 1;
    DAT_0051231c = DAT_00512324 + g_game->field_38a47;
    DAT_005122e8 = DAT_00512338 + DAT_0051231c;
    DAT_00512330 = g_game->field_38a47;
    DAT_00512334 = Point16_00438070((int)((__int64)rand() * g_game->mapWidth / 0x8000),
                                    (int)((__int64)rand() * g_game->mapHeight / 0x8000));
    Point16_00438070 d((int)((__int64)rand() * 30 / 0x8000) - 15,
                       (int)((__int64)rand() * 10 / 0x8000) - 15);
    d += DAT_00512334;
    DAT_00512320 = d;
}
