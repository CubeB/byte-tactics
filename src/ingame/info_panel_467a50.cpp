// Decompiled by space-bunny-free. Names are provisional.
//
// Rotates the four corners of a box by the given angles, projects each rotated
// corner to screen space (16.16 fixed point down to short) and outlines the
// resulting quad on the surface.
//
// MATCH. Two things were needed on top of the obvious decompilation.
//
// 1. The three projected values must be separate int locals evaluated in the
//    order y (the height), z, x, the same idiom as the matched 0x4581e0,
//    because the original loads scratch->y and offset->y first and the other
//    two pairs after it.
//
// 2. `corners` must be copied into a local pointer that is then the loop's
//    induction variable. The loop counter is spilled to a stack slot, and with
//    `corners` used as the induction variable directly its argument home is
//    still considered occupied, so MSVC 5 drops the counter one slot lower,
//    onto `offset`'s home ([esp+0x1c]). Copying `corners` to a local `c`
//    makes that home dead from the prologue on, and the counter then lands on
//    `corners`' home, [esp+0x20], as in the original. The generated code is
//    otherwise unchanged (the copy is the same `mov ebp, [esp + 0x18]` the
//    direct use produced), so only the slot choice moves.

struct Vec3_00467a50 {
    int x;
    int y;
    int z;
};

struct Point_00467a50 {
    int x;
    int y;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0xdd5];
    unsigned char color;                  // +0xdd5
    char unknown_dd6[0x14383 - 0xdd6];
    Vec3_00467a50* scratch;               // +0x14383
    Point_00467a50* points;               // +0x14387
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004b6cc0(Vec3_00467a50* in, Vec3_00467a50* out, short* angles);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int color);

// FUNCTION: 0x467a50
void __stdcall FUN_00467a50(void* surface, Vec3_00467a50* offset,
                            Vec3_00467a50* corners, short* angles)
{
    Vec3_00467a50* scratch = g_game->scratch;
    Point_00467a50* points = g_game->points;
    unsigned char color = g_game->color;
    Vec3_00467a50* c = corners;

    for (int i = 0; i < 4; i++) {
        FUN_004b6cc0(c, scratch, angles);
        int y = (short)((scratch->y + offset->y) >> 16);
        int z = (short)((offset->z - scratch->z) >> 16);
        int x = (short)((scratch->x + offset->x) >> 16);
        points->x = x + 0x80;
        points->y = z - (y >> 1) + 0x20;
        c++;
        scratch++;
        points++;
    }

    Point_00467a50* quad = g_game->points;
    FUN_004be950(surface, quad[0].x, quad[0].y, quad[1].x, quad[1].y, color);
    FUN_004be950(surface, quad[1].x, quad[1].y, quad[2].x, quad[2].y, color);
    FUN_004be950(surface, quad[2].x, quad[2].y, quad[3].x, quad[3].y, color);
    FUN_004be950(surface, quad[3].x, quad[3].y, quad[0].x, quad[0].y, color);
}
