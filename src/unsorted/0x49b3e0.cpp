// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <math.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

// 16.16 fixed point seen as the short above the short below.
union Fix_0049b3e0 {
    int whole;
    short half[2];
};

struct Vec3_0049b3e0 {
    int x, y, z;

    Vec3_0049b3e0 operator-(const Vec3_0049b3e0& o) const
    {
        Vec3_0049b3e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Fix_0049b3e0 Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        Fix_0049b3e0 r;
        r.whole = (int)sqrt(fx * fx + fy * fy + fz * fz);
        return r;
    }
};

#pragma pack(push, 1)
struct Weapon_0049b3e0 {
    char unknown_0[0x111];
    unsigned int flags;                // +0x111
};

struct Obj_0049b3e0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Proj_0049b3e0 {
    Weapon_0049b3e0* weapon;           // +0x0
    Vec3_0049b3e0 pos;                 // +0x4
    Vec3_0049b3e0 start;               // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3_0049b3e0 target;              // +0x28
    char unknown_34[0x4e - 0x34];
    Obj_0049b3e0* field_4e;            // +0x4e
    char unknown_52[0x56 - 0x52];
    int* field_56;                     // +0x56
};
#pragma pack(pop)

struct Game_0049b3e0 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};

extern Game_0049b3e0* g_game;
int __stdcall FUN_00485070(Vec3_0049b3e0* pos);

// FUNCTION: 0x49b3e0
Vec3_0049b3e0* __stdcall FUN_0049b3e0(Proj_0049b3e0* p)
{
    unsigned char flag = (unsigned char)((p->weapon->flags >> 0x19) & 1);
    if (flag) {
        Fix_0049b3e0 dist = (p->pos - p->target).Length();
        if (dist.half[1] > 0x400) {
            p->start = p->target;
            ((Fix_0049b3e0*)&p->start.y)->half[1] = 0x2bc;
            return &p->start;
        }
    } else {
        if (p->field_56 != 0)
            return (Vec3_0049b3e0*)(p->field_56 + 1);
        Obj_0049b3e0* q = p->field_4e;
        if (q != 0 && (q->flags & 0x10000000) != 0)
            return (Vec3_0049b3e0*)((char*)q + 0x6a);
    }
    if (flag) {
        p->start = p->target;
        p->start.y = max(FUN_00485070(&p->target), g_game->seaLevel) << 16;
        return &p->start;
    }
    return &p->target;
}
