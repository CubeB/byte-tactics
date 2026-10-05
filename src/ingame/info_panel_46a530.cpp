// Decompiled by space-bunny-free. Names are provisional.
// Draws one unit's selection box: the four corners of its model bounding box,
// projected to the screen. The corners are the box footprint on the ground, so
// all four take the low y of the box, not the high one.

#pragma pack(push, 1)

struct Vec3_0046a530 {
    int x;
    int y;
    int z;
};

struct Object_004cb650;

struct Unit {
    char unknown_0[0x6a];
    int x;                          // +0x6a
    int y;                          // +0x6e
    int z;                          // +0x72
    char unknown_76[0xa6 - 0x76];
    unsigned short type;            // +0xa6
};

struct Game_0046a530 {
    char unknown_0[0x1431f];
    int scroll_x;                   // +0x1431f
    int scroll_y;                   // +0x14323
    char unknown_14327[0x14377 - 0x14327];
    Object_004cb650** types;        // +0x14377
    char unknown_1437b[0x37f2f - 0x1437b];
    unsigned short flags;           // +0x37f2f
};

#pragma pack(pop)

// Bit 2 of the flags word at g_game+0x37f2f (like 0x416e00).
struct Flags_0046a530 {
    unsigned short low : 2;
    unsigned short flag : 1;
    unsigned short rest : 13;
};

extern Game_0046a530* g_game;

void __stdcall FUN_004cb650(Object_004cb650* obj, Vec3_0046a530* lo,
                            Vec3_0046a530* hi, int arg);
void __stdcall FUN_00467a50(Vec3_0046a530* view, Vec3_0046a530* pos,
                            Vec3_0046a530* corners, void* unit);

// FUNCTION: 0x46a530
void __stdcall FUN_0046a530(Vec3_0046a530* view, Unit* unit)
{
    Flags_0046a530* f = (Flags_0046a530*)((char*)g_game + 0x37f2f);
    if (f->flag) {
        Vec3_0046a530 lo;
        Vec3_0046a530 hi;
        FUN_004cb650(g_game->types[unit->type], &lo, &hi, 0);
        Vec3_0046a530 corners[4];
        corners[0].x = lo.x;
        corners[0].y = lo.y;
        corners[0].z = lo.z;
        corners[1].x = hi.x;
        corners[1].y = lo.y;
        corners[1].z = lo.z;
        corners[2].x = hi.x;
        corners[2].y = lo.y;
        corners[2].z = hi.z;
        corners[3].x = lo.x;
        corners[3].y = lo.y;
        corners[3].z = hi.z;
        Vec3_0046a530 pos;
        pos.x = unit->x - (g_game->scroll_x << 16);
        pos.y = unit->y;
        pos.z = unit->z - (g_game->scroll_y << 16);
        FUN_00467a50(view, &pos, corners, (char*)unit + 0x64);
    }
}
