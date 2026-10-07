// Decompiled by space-bunny-free. Names are provisional.
//
// Rotates the four corners of a box by the given angles, projects each rotated
// corner to screen space (16.16 fixed point down to short) and outlines the
// resulting quad on the surface.

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

void __stdcall RotateByAngles(Vec3_00467a50* in, Vec3_00467a50* out, short* angles);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);

// FUNCTION: 0x467a50
void __stdcall FUN_00467a50(void* surface, Vec3_00467a50* offset,
                            Vec3_00467a50* corners, short* angles)
{
    Vec3_00467a50* scratch = g_game->scratch;
    Point_00467a50* points = g_game->points;
    unsigned char color = g_game->color;
    // corners is copied to c, which is the loop's induction variable: this
    // fixes the stack slot of the loop counter.
    Vec3_00467a50* c = corners;

    for (int i = 0; i < 4; i++) {
        RotateByAngles(c, scratch, angles);
        // Separate int locals, evaluated in the order y, z, x.
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
    DrawLine(surface, quad[0].x, quad[0].y, quad[1].x, quad[1].y, color);
    DrawLine(surface, quad[1].x, quad[1].y, quad[2].x, quad[2].y, color);
    DrawLine(surface, quad[2].x, quad[2].y, quad[3].x, quad[3].y, color);
    DrawLine(surface, quad[3].x, quad[3].y, quad[0].x, quad[0].y, color);
}
