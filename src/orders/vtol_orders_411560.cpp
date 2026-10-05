// Decompiled by Claude Opus 5.5. Names are provisional.
// Order handler that unloads the unit a transport carries ("Unloading").
// State 0 targets the cargo and heads for the order's position, state 1
// checks the cargo fits there and moves on, state 2 checks again, runs the
// script's "EndTransport" and drops the cargo, state 3 reports and ends.
// Match note: the cargo's field_170 goes through an int local (`int h`);
// passing the expression straight to FUN_0044e6c0 sign-extends into edx
// instead of eax.

struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
};

struct Unit;
class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc
    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
};
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_004388d0 { public: void FUN_004388d0(int); };
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_0044e730 { public: void FUN_0044e730(short); };
class Class_004b0940 { public: void StartScript(const char*, int, int); };

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x170]; short field_170;
    char pad172[0x21c - 0x172]; short field_21c;
    char pad21e[0x241 - 0x21e]; unsigned int flags;
};
struct Unit {
    void* type;
    char pad4[0x6a - 4]; Vec3 pos;
    char pad76[8]; Point footprint;
    char pad82[8]; Unit* cargo;
    char pad8e[4]; UnitDef* def;
    char pad96[4]; Class_004b0940* script;
};
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char padA[0x12 - 0xa]; Class_004895c0 target;
    Vec3 pos;
};
class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

int __stdcall FUN_0047db70(UnitDef*, int, Point, int);
void __stdcall FUN_0047f780(Unit*, int, const char*);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// FUNCTION: 0x411560
int __stdcall FUN_00411560(Unit* unit, Order* order, int flags)
{
    if (!unit->cargo)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->FUN_00438880("Unloading");
            order->target.SetUnit(unit->cargo);
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
            ((Class_0044e730*)obj)->FUN_0044e730(0x140);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags = 0xe8;
            return 1;
        }
        break;
    case 1: {
        Unit* cargo = order->target.owner;
        if (FUN_0047db70(cargo->def, 0, WorldToCell(order->pos, cargo->footprint), 1)) {
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            int h = unit->cargo->def->field_170;
            ((Class_0044e6c0*)obj)->FUN_0044e6c0(h);
            ((Class_004388d0*)order)->FUN_004388d0((int)obj);
            order->flags = 0xe8;
            return 1;
        }
        FUN_0047f780(unit, 7, "Unable to unload unit");
        return 9;
    }
    case 2: {
        if (flags & 0x40)
            return 9;
        Unit* cargo = order->target.owner;
        if (!FUN_0047db70(cargo->def, 0, WorldToCell(order->pos, cargo->footprint), 1)) {
            FUN_0047f780(unit, 7, "Unable to unload unit");
            return 9;
        }
        unit->script->StartScript("EndTransport", 0, 0);
        AttachUnitToPiece(unit->cargo, 0, -1, 1);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags = 0xe0;
        return 1;
    }
    case 3:
        FUN_0047f780(unit, 0xd, 0);
        return 5;
    }
    return 7;
}
