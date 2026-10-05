// Decompiled by Claude Opus 5.5. Names are provisional.
// Meteor shower update, run once per game tick: starts a new shower when
// its time comes (an inline copy of FUN_00438070), and while one is active
// drops a meteor every DAT_00512314 ticks from a random point around the
// origin, high up, with a velocity that carries it to the target in 90
// ticks. The random offset is a struct copied into the position (which
// keeps the first stores of pos.x and pos.y), and the radius is shifted
// into 16.16 as its own statement; both decide the registers.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    int mapWidth;                    // +0x14233
    int mapHeight;                   // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    unsigned int ticks;              // +0x38a47
};
#pragma pack(pop)

struct Point16_00437de0 {
    short x;
    short y;
};

struct Vec3_00437de0 {
    int x;
    int y;
    int z;
};

extern Game* g_game;
extern unsigned int DAT_005122e8;      // next strike time
extern int DAT_00512310;               // strike radius
extern int DAT_00512314;               // ticks between meteors
extern int DAT_00512318;               // shower active
extern unsigned int DAT_0051231c;      // time the strike ends
extern Point16_00437de0 DAT_00512320;  // origin
extern int DAT_00512324;
extern void* DAT_00512328;             // owning player
extern int DAT_0051232c;               // enabled
extern unsigned int DAT_00512330;      // next meteor time
extern Point16_00437de0 DAT_00512334;  // target
extern int DAT_00512338;

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall FUN_0049df10(void* player, Vec3_00437de0* pos, Vec3_00437de0* vel, int count);

// C-style helpers returning the struct by value, as in 0x438070.cpp.
static inline Point16_00437de0 MakePoint(int x, int y)
{
    Point16_00437de0 p;
    p.x = x;
    p.y = y;
    return p;
}

static inline Point16_00437de0 AddPoints(Point16_00437de0 a, Point16_00437de0 b)
{
    Point16_00437de0 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

// Inline copy of FUN_00438070 (0x438070.cpp): starts a shower.
static inline void StartShower()
{
    DAT_00512318 = 1;
    DAT_0051231c = DAT_00512324 + g_game->ticks;
    DAT_005122e8 = DAT_00512338 + DAT_0051231c;
    DAT_00512330 = g_game->ticks;
    DAT_00512334 = MakePoint((int)((__int64)rand() * g_game->mapWidth / 0x8000),
                             (int)((__int64)rand() * g_game->mapHeight / 0x8000));
    DAT_00512320 = AddPoints(MakePoint((int)((__int64)rand() * 30 / 0x8000) - 15,
                                       (int)((__int64)rand() * 10 / 0x8000) - 15),
                             DAT_00512334);
}

// FUNCTION: 0x437de0
void FUN_00437de0()
{
    if (DAT_005122e8 <= g_game->ticks) {
        StartShower();
        if (DAT_0051232c == 0)
            DAT_00512318 = 0;
    }
    if (DAT_00512318 != 0) {
        if (DAT_00512330 <= g_game->ticks) {
            DAT_00512330 = DAT_00512314 + g_game->ticks;
            Vec3_00437de0 vel;
            vel.x = ((DAT_00512334.x - DAT_00512320.x) << 20) / 90;
            vel.y = -15 << 16;
            vel.z = ((DAT_00512334.y - DAT_00512320.y) << 20) / 90;
            int r = (int)((__int64)rand() * DAT_00512310 / 0x8000);
            int radius = r << 16;
            int angle = (int)((__int64)rand() * 0x10000 / 0x8000);
            Vec3_00437de0 offset;
            offset.x = -FUN_004b70ef(angle, radius);
            offset.y = 0;
            offset.z = -FUN_004b7123(angle, radius);
            Vec3_00437de0 pos = offset;
            pos.x += DAT_00512320.x << 20;
            pos.y = -vel.y * 90;
            pos.z += DAT_00512320.y << 20;
            FUN_0049df10(DAT_00512328, &pos, &vel, 1);
        }
        if (DAT_0051231c <= g_game->ticks)
            DAT_00512318 = 0;
    }
}
