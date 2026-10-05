// Decompiled by Claude Opus 5.5. Names are provisional.
// Picks a random build cell near a world position: up to 30 tries of a
// random direction and distance (within `range` cells) from `pos`, snapped
// to the class's placement grid (spacing, offset and a random jitter reduced
// by a margin; the second grid is used for types whose field_1c0 is
// non-negative). A cell is accepted when FUN_0047db70 allows the type there
// and the score FUN_0047c770 is at most the type's footprint area times
// twice net->field_d30.
//
// <windows.h> (or one of several other header sets) is needed: without it
// the distance and the direction's x swap esi and edi, and the add of pos.x
// loads pos.x first.
#include <windows.h>

struct Point16 {
    short x;
    short y;
};

struct Vec3 {
    int x;
    int y;
    int z;
    Vec3 operator+(const Vec3& o) const { Vec3 r; r.x = x + o.x; r.y = y + o.y; r.z = z + o.z; return r; }
};

#pragma pack(push, 1)
struct UnitType {
    char unknown_0[0x14a];
    Point16 origin;                    // +0x14a
    char unknown_14e[0x1c0 - 0x14e];
    short field_1c0;                   // +0x1c0
};

struct Net {
    char unknown_0[0xd30];
    int field_d30;                     // +0xd30
};

struct Game {
    char unknown_0[0x391e9];
    Net* net;                          // +0x391e9
};

class Class_0040a7b0 {
public:
    char unknown_0[0xf1];
    Point16 spacing0;                  // +0xf1
    Point16 offset0;                   // +0xf5
    int margin0;                       // +0xf9
    Point16 spacing1;                  // +0xfd
    Point16 offset1;                   // +0x101
    int margin1;                       // +0x105
    bool FUN_0040a5d0(UnitType* type, Vec3* pos, int range, Point16* out);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_004b6c30(int range);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
int __stdcall FUN_0047db70(UnitType* type, short a, Point16 cell, int b);
int FUN_0047c770(void);

static inline Point16 WorldToCell(Vec3 v, Point16 origin)
{
    Point16 c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// Inlined copy of FUN_004103a0 (see 0x44d720.cpp).
static inline Vec3 Direction(short angle, int scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// FUNCTION: 0x40a5d0
bool Class_0040a7b0::FUN_0040a5d0(UnitType* type, Vec3* pos, int range, Point16* out)
{
    int threshold = g_game->net->field_d30 * type->origin.y * type->origin.x * 2;
    Point16 spacing = type->field_1c0 < 0 ? spacing0 : spacing1;
    Point16 offset = type->field_1c0 < 0 ? offset0 : offset1;
    int margin = type->field_1c0 < 0 ? margin0 : margin1;
    for (int i = 0; i < 30; i++) {
        int dist = FUN_004b6c30(range) << 16;
        int angle = FUN_004b6c30(0x10000);
        Vec3 v = Direction(angle, dist) + *pos;
        Point16 cell = WorldToCell(v, type->origin);
        cell.x = cell.x / spacing.x * spacing.x + offset.x + FUN_004b6c30(spacing.x - margin - type->origin.x);
        cell.y = cell.y / spacing.y * spacing.y + offset.y + FUN_004b6c30(spacing.y - margin - type->origin.y);
        if (FUN_0047db70(type, 0, cell, 1) && FUN_0047c770() <= threshold) {
            if (out)
                *out = cell;
            return true;
        }
    }
    return false;
}
