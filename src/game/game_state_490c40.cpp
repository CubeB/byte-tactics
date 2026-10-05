// Decompiled by Space Bunny Free. Names are provisional.
// The wind update. While the per-tick counter at +0x37ec4 is below the tick
// count at +0x38a47 it jitters the wind direction by a random amount, picks a
// random wind speed in the range at +0x1425b..+0x1425f and, when that speed is
// not zero, a fresh direction, then turns direction and speed into the two wind
// vector components and a 0..1 strength. Once the counter has caught up with the
// tick count the wind is switched off instead.
//
// Three spellings here are load bearing:
//  - The __int64 cast. MSVC knows rand()'s range, so plain `rand() * 10 / 0x8000`
//    is folded into `and edx,0x7fff; add eax,edx; sar eax,0xf`; the 64-bit
//    product and division are what the original calls _allmul and _alldiv for.
//  - The counter update is one inline expression with no temporary: MSVC then
//    keeps the address of the field in esi (saved inside the guarded block) and
//    does the read-modify-write through it, instead of reloading g_game.
//  - The range and the random number are separate statements. Written as one
//    expression, the add of the low bound to the result is emitted as
//    `add edx, [eax+0x1425b]`; as two statements it is the original's load of
//    the low bound into edx followed by a register add.
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
int __stdcall FUN_004b6c30(int range);

// FUNCTION: 0x490c40
void __cdecl UpdateWind()
{
    if (g_game->windCounter < g_game->field_38a47) {
        g_game->windCounter += ((int)((__int64)rand() * 10 / 0x8000) + 5) * 30;

        int range = g_game->field_1425f - g_game->field_1425b;
        int n = FUN_004b6c30(range);
        g_game->windSpeed = g_game->field_1425b + n;
        if (g_game->windSpeed != 0)
            g_game->windDirection = FUN_004b6c30(0x10000);

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
