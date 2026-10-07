// Decompiled by Space Bunny Free. Names are provisional.
// The wind update. While the per-tick counter at +0x37ec4 is below the tick
// count at +0x38a47 it jitters the wind direction by a random amount, picks a
// random wind speed in the range at +0x1425b..+0x1425f and, when that speed is
// not zero, a fresh direction, then turns direction and speed into the two wind
// vector components and a 0..1 strength. Once the counter has caught up with the
// tick count the wind is switched off instead.
#include <stdlib.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1425b];
    int field_1425b;                       // +0x1425b
    int field_1425f;                       // +0x1425f
    char unknown_14263[0x37ec4 - 0x14263];
    unsigned int windCounter;              // +0x37ec4
    int field_37ec8;                       // +0x37ec8
    int windX;                             // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                             // +0x37ed4
    unsigned short windDirection;          // +0x37ed8
    int windSpeed;                         // +0x37eda
    float field_37ede;                     // +0x37ede
    int windEnabled;                       // +0x37ee2
    char unknown_37ee6[0x38a47 - 0x37ee6];
    unsigned int field_38a47;              // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// Fixed-point trig helpers, written in assembly: the angle is a short.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall RandomInt(int range);

// FUNCTION: 0x490c40
void __cdecl UpdateWind()
{
    if (g_game->windCounter < g_game->field_38a47) {
        // The __int64 cast keeps the _allmul/_alldiv calls; keep this one
        // expression with no temporary.
        g_game->windCounter += ((int)((__int64)rand() * 10 / 0x8000) + 5) * 30;

        // Separate statements: one expression changes how the low bound is added.
        int range = g_game->field_1425f - g_game->field_1425b;
        int n = RandomInt(range);
        g_game->windSpeed = g_game->field_1425b + n;
        if (g_game->windSpeed != 0)
            g_game->windDirection = RandomInt(0x10000);

        g_game->windX = -FUN_004b70ef(g_game->windDirection, g_game->windSpeed) * 2;
        g_game->windZ = -FUN_004b7123(g_game->windDirection, g_game->windSpeed) * 2;

        g_game->field_37ede = (float)g_game->windSpeed / (float)g_game->field_37ec8;
        if (1.0 < g_game->field_37ede)
            g_game->field_37ede = 1.0f;

        g_game->windEnabled = 1;
    } else {
        g_game->windEnabled = 0;
    }
}
