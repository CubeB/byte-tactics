// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: cases 0, 5 and 6 share a call tail that is separate for case 6
// in the original; vector arithmetic and scratch registers also differ.
#include <math.h>
#include <windows.h>
#include <memory.h>
#include <stdlib.h>
struct Vec3 {
    union { int x; struct { unsigned short xf; short xh; }; };
    union { int y; struct { unsigned short yf; short yh; }; };
    union { int z; struct { unsigned short zf; short zh; }; };
    Vec3 operator+(const Vec3& other) const {
        Vec3 r; r.x = x + other.x; r.y = y + other.y; r.z = z + other.z; return r;
    }
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};
class Class_00438880 { public: void FUN_00438880(int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438930 { public: void FUN_00438930(Vec3*, int); };
class Class_00438a00 { public: void FUN_00438a00(Vec3*, int, int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004897e0 { public: unsigned char FUN_004897e0(); };
class Class_00489800 { public: void FUN_00489800(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
#pragma pack(push, 1)
struct UnitDef { char pad[0x241]; unsigned int flags; };
struct Unit {
    int active;
    char pad4[0x6a-4];
    Vec3 pos;
    char pad76[0x92-0x76];
    UnitDef* def;
    char pad96[0x110-0x96];
    unsigned int flags;
};
struct Order {
    char pad0[5]; unsigned char state;
    unsigned int flags;
    char padA[0x16-0xa]; Unit* target;
    char pad1A[8]; Vec3 pos;
    short x, z;
    char pad32[4];
    int weapon, step, radius;
    int Advance(int distance) {
        ((Class_00438930*)this)->FUN_00438930(&target->pos, distance);
        step++;
        return 1;
    }
};
#pragma pack(pop)
int __stdcall FUN_0049abb0(Unit*, Unit*, int);
void __stdcall FUN_0048a060(Unit*, Unit*, int);
int __stdcall FUN_0049adf0(Unit*, int);
int __stdcall FUN_0048a980(Vec3*, Vec3*);
int __stdcall FUN_004b6c30(int);
int __cdecl FUN_004b70ef(int, int);
int __cdecl FUN_004b7123(int, int);
// FUNCTION: 0x4034a0
int __stdcall FUN_004034a0(Unit* unit, Order* order, unsigned int flags)
{
    int weapon = order->weapon;
    if ((flags & 0x800) || !order->target || (flags & 0x10008)) return 5;
    if (order->radius && (int)_hypot(unit->pos.xh - order->x,
                                     unit->pos.zh - order->z) >= order->radius)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (!unit->active || (unit->def->flags & 0x800) || !(unit->flags & 0x80000000)) break;
        ((Class_00438880*)order)->FUN_00438880(0);
        order->pos = unit->pos;
        order->step = 0;
        if (!weapon) order->weapon = ((Class_004897e0*)unit)->FUN_004897e0();
        return 1;
    case 1:
        ((Class_004388d0*)order)->FUN_004388d0(0);
        if (flags & 0x3000) return 1;
        if (!FUN_0049abb0(unit, order->target, weapon)) return 1;
        ((Class_004898b0*)unit)->FUN_004898b0(0);
        ((Class_004898b0*)unit)->FUN_004898b0(2);
        FUN_0048a060(unit, order->target, weapon);
        order->flags = 0x13808;
        return 2;
    case 2:
        weapon = FUN_0049adf0(unit, weapon);
        switch (order->step) {
        case 0:
            return order->Advance(weapon);
        case 1: case 2: case 3: case 4:
            if (abs(unit->pos.y - order->target->pos.y) > 0x80000) {
                ((Class_00438930*)order)->FUN_00438930(&order->target->pos, weapon / 2);
                order->step = 6;
                return 1;
            } else {
                int angle = FUN_0048a980(&order->target->pos, &unit->pos);
                angle += FUN_004b6c30(0x8000) - 0x4000;
                int distance = weapon << 16;
                int dx = -FUN_004b70ef(angle, distance);
                int dz = -FUN_004b7123(angle, distance);
                Vec3* target = &order->target->pos;
                Vec3 pos = *target + Vec3(dx, 0, dz);
                ((Class_00438930*)order)->FUN_00438930(&pos, weapon / 4);
                return 1;
            }
        case 5:
            return order->Advance(weapon / 2);
        case 6: {
            Vec3* target = &order->target->pos;
            ((Class_00438930*)order)->FUN_00438930(target, 0);
            order->step++;
            break;
        }
        case 7:
            ((Class_00438a00*)order)->FUN_00438a00(&order->target->pos, weapon, weapon / 2);
            order->step++;
            return 1;
        case 8:
            ((Class_00438a00*)order)->FUN_00438a00(&order->target->pos, weapon * 2, weapon);
            order->step = 0;
            return 1;
        default: return 7;
        }
        return 1;
    case 3:
        if (flags & 0x40e0) { order->state = 1; return 4; }
        if (FUN_0049abb0(unit, order->target, weapon)) {
            ((Class_004898b0*)unit)->FUN_004898b0(0);
            ((Class_004898b0*)unit)->FUN_004898b0(2);
            FUN_0048a060(unit, order->target, weapon);
            order->flags = 0x148e8;
            ((Class_00439e80*)order)->FUN_00439e80(30);
            return 2;
        }
        ((Class_00489800*)unit)->FUN_00489800(3);
        order->flags = 0x100e8;
        ((Class_00439e80*)order)->FUN_00439e80(30);
        return 2;
    }
    return 7;
}
