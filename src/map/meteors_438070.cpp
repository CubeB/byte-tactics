// Decompiled by Opus. Names are provisional.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
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
};

extern Game* g_game;
extern int g_meteorActive;
extern int g_meteorStrikeEndTime;
extern int DAT_00512324;
extern int DAT_00512338;
extern int g_meteorNextStrikeTime;
extern int g_meteorNextHitTime;
extern Point16_00438070 g_meteorTarget;
extern Point16_00438070 g_meteorOrigin;

// C-style helpers returning the struct by value; with a constructor and
// operator+= the offset's x never goes through the stack slot as it does here.
static inline Point16_00438070 MakePoint(int x, int y)
{
    Point16_00438070 p;
    p.x = x;
    p.y = y;
    return p;
}

static inline Point16_00438070 AddPoints(Point16_00438070 a, Point16_00438070 b)
{
    Point16_00438070 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

// FUNCTION: 0x438070
void StartMeteorShower()
{
    g_meteorActive = 1;
    g_meteorStrikeEndTime = DAT_00512324 + g_game->field_38a47;
    g_meteorNextStrikeTime = DAT_00512338 + g_meteorStrikeEndTime;
    g_meteorNextHitTime = g_game->field_38a47;
    g_meteorTarget = MakePoint((int)((__int64)rand() * g_game->mapWidth / 0x8000),
                             (int)((__int64)rand() * g_game->mapHeight / 0x8000));
    g_meteorOrigin = AddPoints(MakePoint((int)((__int64)rand() * 30 / 0x8000) - 15,
                                       (int)((__int64)rand() * 10 / 0x8000) - 15),
                             g_meteorTarget);
}
