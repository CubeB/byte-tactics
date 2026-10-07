// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// VTOL patrol order handler ("Patrolling"). State 0 prepares the order
// (FUN_0040f200), state 1 clears the order's 0xe0 bits, state 2 flies to a
// point 0x140 units away along the heading to the order's position, lands on a
// free pad when damaged (VTOL_LANDING, as in 0x412710), or takes the next
// queued order.
// Kept: the flags/health block's register allocation depends on this include.
#include <windows.h>
#include <vector>

struct Vec3 {
    int x, y, z;
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

struct Unit;
class UnitMotion {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x1fa]; unsigned int field_1fa;
    char pad1fe[0x21c - 0x1fe]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Player {
    char pad0[0x146]; unsigned char index;
};
struct Unit {
    UnitMotion* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[0x86 - 0x76]; int field_86;
    char pad8a[8]; UnitDef* def;
    Player* player;
    char pad9a[0x108 - 0x9a]; short field_108;
    void ClaimWeapons(int);
    void ReleaseWeapons(int);
    void SetStateBits(int, int);
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x22 - 0xa]; Vec3 pos;
    char pad2e[0x4e - 0x2e]; unsigned int field_4e;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, int a, Vec3* b, int c, int d, int e);
};
#pragma pack(pop)

int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall FUN_0043a020(Unit*, Order*);
Unit* __stdcall FUN_0043b700(Unit*);
int __stdcall FUN_0043b1f0(Unit*, Unit*, int);
void __stdcall GetFactoriesInRadius(int player, Vec3* pos, int range, std::vector<Unit*>* out);

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// 0x40f790, matched in 0x40f790.cpp; /Ob2 inlines it into state 2.
Vec3 __stdcall FUN_0040f790(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

static inline void Dummy(void) {}
// FUNCTION: 0x410e70
int __stdcall VtolPatrolOrder(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 2: {
        if (flags & 0xe0)
            return 6;
        short angle = GetHeadingBetween(&unit->pos, &order->pos);
        Vec3 dest = FUN_0040f790(order->pos, Offset(angle, 0x1400000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->FUN_0044e730(0x150);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= 0xe0;
        // Landing block stays inline, no helper: the landed path returns 0
        // through the plain scope-exit destructor.
        if ((unsigned int)unit->field_108 < (unit->def->field_1fa >> 2) * 3) {
            std::vector<Unit*> v;
            GetFactoriesInRadius(unit->player->index, &unit->pos, 0xf00, &v);
            if (!v.empty()) {
                ((Class_004388d0*)order)->FUN_004388d0(0);
                Unit* target = v[RandomInt(v.size())];
                AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", (int)target, 0, 0, 0, 0));
                order->flags = 0;
                return 0;
            }
        }
        // Empty Dummy() calls use up the inline budget so both vector
        // destructor sites call _Destroy out of line.
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Dummy();
        Unit* next = FUN_0043b700(unit);
        if (next && FUN_0043b1f0(unit, next, 0)) {
            order->flags = 0;
            return 3;
        }
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 2;
    }
    case 1:
        order->field_4e &= ~0xe0;
        return 1;
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            FUN_0043a020(unit, order);
            ((Class_00438880*)order)->FUN_00438880("Patrolling");
            FUN_0040f200(unit, order, 0);
            unit->ReleaseWeapons(3);
            return 1;
        }
        break;
    }
    return 7;
}
