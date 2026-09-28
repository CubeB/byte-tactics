// Decompiled by Space Bunny Free. Names are provisional.

#pragma pack(push, 1)

struct Vec3 {
    int x, y, z;
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Point {
    short x, y;
};

struct Short3 {
    short x, y, z;
};

struct Game_0043d6d0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;                         // +0x1427f
    char unknown_14280[0x38a47 - 0x14280];
    int field_38a47;                                // +0x38a47
};

struct UnitType_0043d6d0 {
    char unknown_0[0x192];
    int range;                                      // +0x192
    char unknown_196[0x22c - 0x196];
    unsigned char draft;                            // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int flags;                             // +0x241
};

struct Target_0043d6d0 {
    int field_0;                                    // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                             // +0x73
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
};

struct TargetVec_0043d6d0 {
    TargetData_0043d6d0* field_0;                   // +0x0
};

struct Unit_0043d6d0 {
    char unknown_0[0x64];
    Short3 f64;                                     // +0x64
    Vec3 pos;                                       // +0x6a
    Point cell;                                      // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                                     // +0x7e
    char unknown_82[0x86 - 0x82];
    TargetVec_0043d6d0* obj;                        // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043d6d0* type;                        // +0x92
    Target_0043d6d0* target;                        // +0x96
    char unknown_9a[0xa8 - 0x9a];
    short a8;                                       // +0xa8
    char unknown_aa[0xf9 - 0xaa];
    signed char index;                              // +0xf9
    char unknown_fa[0x110 - 0xfa];
    unsigned int flags;                             // +0x110
};

#pragma pack(pop)

extern Game_0043d6d0* g_game;

Vec3 __stdcall FUN_0043e060(void* obj, int param);
Short3 __stdcall FUN_0043e180(void* obj, int index);
void __stdcall FUN_0048a9f0(Unit_0043d6d0* unit, Vec3 pos, int mode);
int __stdcall FUN_0047db70(UnitType_0043d6d0* type, short a8, Point cell, int mode);
void __stdcall FUN_0047d0e0(Unit_0043d6d0* unit);
void __stdcall FUN_0047cc30(Unit_0043d6d0* unit);
void __stdcall FUN_004827b0(Unit_0043d6d0* unit);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

#pragma pack(push, 1)
class Class_0043d6d0 {
public:
    char unknown_0[8];
    Vec3 p1;                                        // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                                   // +0x20
    char unknown_24[0x2a - 0x24];
    int field_2a;                                   // +0x2a
    unsigned char mode : 2;                         // +0x2e
    unsigned char flag : 1;                         // bit 2
    unsigned char unknown_2f : 5;

    void FUN_0043d6d0(Unit_0043d6d0* unit);
};
#pragma pack(pop)

// FUNCTION: 0x43d6d0
void Class_0043d6d0::FUN_0043d6d0(Unit_0043d6d0* u)
{
    if (u->obj != 0) {
        Vec3 v = FUN_0043e060(u->obj, u->index);
        if (u->type->flags & 0x2000) {
            int lim = (u->type->draft * 0xffff + g_game->seaLevel) << 16;
            v.y = v.y > lim ? v.y : lim;
        }
        FUN_0048a9f0(u, v, mode);
        Short3 o = FUN_0043e180(u->obj, u->index);
        u->f64 = o;
        if (u->obj->field_0 != 0) {
            field_20 = u->obj->field_0->field_20;
            p1 = u->obj->field_0->p1;
        } else {
            field_20 = 0;
            p1 = Vec3(0, 0, 0);
        }
        u->flags &= 0xfffeffff;
        return;
    }

    int nx = p1.x + u->pos.x;
    int ny = p1.y + u->pos.y;
    int nz = p1.z + u->pos.z;
    int m = mode;
    Vec3 pos;
    pos.x = nx;
    pos.y = ny;
    pos.z = nz;
    if (nx == u->pos.x && nz == u->pos.z && ny == u->pos.y && m == (int)(u->flags & 3))
        return;

    field_2a = g_game->field_38a47;
    Point draft = u->draft;
    Point cell;
    cell.x = (nx - (draft.x << 19) + 0x80000) >> 20;
    cell.y = (nz - (draft.y << 19) + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == (int)(u->flags & 3)) {
        u->pos.x = nx;
        u->pos.y = ny;
        u->pos.z = nz;
        u->flags |= 0x10000;
        return;
    }

    if (u->target->field_0 != 0 && (u->target->type == 1 || u->target->type == 2)) {
        if (FUN_0047db70(u->type, u->a8, cell, m) == 0)
            flag = 1;
        else
            flag = 0;
    }

    if (flag) {
        Point c = u->cell;
        int cx = (draft.x + c.x * 2) << 19;
        int cz = (draft.y + c.y * 2) << 19;
        if (nx > cx + 0x7ffff)
            pos.x = cx + 0x7ffff;
        else if (nx < cx - 0x7ffff)
            pos.x = cx - 0x7ffff;
        if (nz > cz + 0x7ffff)
            pos.z = cz + 0x7ffff;
        else if (nz < cz - 0x7ffff)
            pos.z = cz - 0x7ffff;
        int half = u->type->range / 2;
        if (pos.x > half) {
            unsigned short angle = u->f64.y;
            field_20 = half;
            Vec3 w;
            w.x = -FUN_004b70ef(angle, half);
            w.y = 0;
            w.z = -FUN_004b7123(angle, half);
            p1 = w;
        }
        u->pos.x = pos.x;
        u->pos.y = pos.y;
        u->pos.z = pos.z;
        u->flags |= 0x10000;
        return;
    }

    FUN_0047d0e0(u);
    u->pos.x = nx;
    u->pos.y = ny;
    u->pos.z = nz;
    u->cell = cell;
    u->flags = (u->flags & 0xfffffffc) | (m & 3);
    FUN_0047cc30(u);
    u->flags |= 0x10000;
    FUN_004827b0(u);
}
