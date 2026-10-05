// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler: state 0 prepares the order (FUN_0040f200 is defined here
// because /Ob2 inlined it), state 1 snaps the order's position to the map grid
// for the unit's footprint and heads there, state 2 finishes.

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
};

struct Unit;
class Class_0043d210 {
public:
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
    void SetFlightMode(Unit* unit, int state);
};
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_00489800 { public: void ReleaseWeapons(int); };
class Class_0048b090 { public: void SetStateBits(int, int); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_00438880 { public: void FUN_00438880(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x21c]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Unit {
    Class_0043d210* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[8]; Point footprint;
    int field_82; int field_86;
    char pad8a[8]; UnitDef* def;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x22 - 0xa]; Vec3 pos;
    char pad2e[0x4a - 0x2e]; int field_4a;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_0047f780(Unit* unit, int kind, const char* text);

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - origin.x * 0x80000 + 0x80000) >> 20;
    c.y = (v.z - origin.y * 0x80000 + 0x80000) >> 20;
    return c;
}
static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}
static inline void Snap(Vec3* v, Point size)
{
    Point c = WorldToCell(*v, size);
    CellToWorld(size, c, v);
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Class_004898b0*)unit)->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Class_0048b090*)unit)->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// FUNCTION: 0x40fa20
int __stdcall FUN_0040fa20(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        break;
    case 1:
        ((Class_00438880*)order)->FUN_00438880(0);
        ((Class_00489800*)unit)->ReleaseWeapons(3);
        Snap(&order->pos, unit->footprint);
        ((Class_004388d0*)order)->FUN_004388d0((int)new Class_0044e2d0(order, order->pos));
        order->flags = 0xe0;
        return 1;
    case 2:
        if (!order->field_4a)
            FUN_0047f780(unit, 6, 0);
        return 5;
    }
    return 7;
}
