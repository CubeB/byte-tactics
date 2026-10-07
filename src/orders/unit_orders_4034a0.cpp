// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler: attack-move style state machine. The inner step switch
// advances order->step after cases 6 and 7 in one `order->step++` after the
// switch; cases 0 and 5 go through the inline Advance(), so MSVC merges only
// those two (at the pushed distance) and keeps case 6 separate.
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
class Class_004897e0 { public: unsigned char ChooseWeapon(); };
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
    void ReleaseWeapons(int);
    void ClaimWeapons(int);
};
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
void __stdcall AlignUnitToGround(Unit*);
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
void __stdcall DetonateUnitWeapon(Unit*, int);

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
int __stdcall WeaponCanReachUnit(Unit*, Unit*, int);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
int __stdcall FUN_0049adf0(Unit*, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(int, int);
int __cdecl FUN_004b7123(int, int);
// FUNCTION: 0x4034a0
int __stdcall AttackChaseOrder(Unit* unit, Order* order, unsigned int flags)
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
        if (!weapon) order->weapon = ((Class_004897e0*)unit)->ChooseWeapon();
        return 1;
    case 1:
        ((Class_004388d0*)order)->FUN_004388d0(0);
        if (flags & 0x3000) return 1;
        if (!WeaponCanReachUnit(unit, order->target, weapon)) return 1;
        unit->ClaimWeapons(0);
        unit->ClaimWeapons(2);
        SetWeaponTargetUnit(unit, order->target, weapon);
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
                int angle = GetHeadingBetween(&order->target->pos, &unit->pos);
                angle += RandomInt(0x8000) - 0x4000;
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
        case 6:
            ((Class_00438930*)order)->FUN_00438930(&order->target->pos, 0);
            break;
        case 7:
            ((Class_00438a00*)order)->FUN_00438a00(&order->target->pos, weapon, weapon / 2);
            break;
        case 8:
            ((Class_00438a00*)order)->FUN_00438a00(&order->target->pos, weapon * 2, weapon);
            order->step = 0;
            return 1;
        default: return 7;
        }
        order->step++;
        return 1;
    case 3:
        if (flags & 0x40e0) { order->state = 1; return 4; }
        if (WeaponCanReachUnit(unit, order->target, weapon)) {
            unit->ClaimWeapons(0);
            unit->ClaimWeapons(2);
            SetWeaponTargetUnit(unit, order->target, weapon);
            order->flags = 0x148e8;
            ((Class_00439e80*)order)->FUN_00439e80(30);
            return 2;
        }
        unit->ReleaseWeapons(3);
        order->flags = 0x100e8;
        ((Class_00439e80*)order)->FUN_00439e80(30);
        return 2;
    }
    return 7;
}
