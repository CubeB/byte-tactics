// Decompiled by Claude Opus 5.5. Names are provisional.
struct Vec3 {
    int x, y, z;
    Vec3 operator-(const Vec3& other) const {
        Vec3 r; r.z = z - other.z; r.y = y - other.y; r.x = x - other.x; return r;
    }
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
};

struct Point {
    short x, y;
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
class Class_0044e6c0 { public: void FUN_0044e6c0(int); };
class Class_00439e80 { public: void FUN_00439e80(int); };
#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x15e];
    Vec3 min;                          // +0x15e
    Vec3 max;                          // +0x16a
    char unknown_176[0x212 - 0x176];
    unsigned short buildRange;         // +0x212
    char unknown_214[0x21c - 0x214];
    short field_21c;                   // +0x21c
    char unknown_21e[0x241 - 0x21e];
    unsigned int flags;                // +0x241
    unsigned int flags2;               // +0x245
};

struct Unit {
    UnitMotion* type;                  // +0x0
    char unknown_4[0x6a - 0x4];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x7e - 0x76];
    Point footprint;                   // +0x7e
    char unknown_82[0x86 - 0x82];
    int field_86;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitDef* def;                      // +0x92
    void ClaimWeapons(int);
    void SetStateBits(int, int);
    int CanReclaim(Unit*);
};

struct UnitRef {
    int vtable;
    Unit* ptr;
    Unit* Get() { return ptr; }
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    UnitRef target;                    // +0x12
    char unknown_1a[0x22 - 0x1a];
    Vec3 pos;                          // +0x22
    char unknown_2e[0x36 - 0x2e];
    int elapsed;                       // +0x36
    int duration;                      // +0x3a
};

class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};
#pragma pack(pop)

void __stdcall QueueUnitSpeech(Unit* unit, int kind, const char* text);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitReverseNanoParticles(Vec3* box, Vec3* from, int count);
int __stdcall FUN_00438650(Unit* unit, Unit* target, int n);
void __stdcall DamageUnit(Unit* unit, Unit* target, int a, int b, int c);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall FUN_00414350(Point cell, Vec3* out, Point origin);

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall FUN_0040f200(Unit* unit, Order* order, unsigned int flags)
{
    ((Unit*)unit)->ClaimWeapons(3);
    if (unit->field_86)
        AttachUnitToPiece(unit, 0, -1, 2);
    ((Unit*)unit)->SetStateBits(1, 1);
    if ((unit->type->field_2e & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->FUN_0044e6c0(unit->def->field_21c / 2);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Order handler "Reclaiming" for a unit target (the hovering variant of
// 0x404730): state 0 checks the builder can reclaim, state 1 snaps the order
// position to the target's cell and moves over it, state 2 drains the target
// while it stays in build range.
// FUNCTION: 0x414a80
int __stdcall VtolReclaimUnitOrder(Unit* unit, Order* order, unsigned int flags)
{
    Unit* target = order->target.Get();
    if (!target || (flags & 0x10008))
        return 5;
    switch (order->state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            if (!(unit->def->flags2 & 0x400)) {
                QueueUnitSpeech(unit, 7, "Reclamation failed");
                return 7;
            }
            if (!((Unit*)unit)->CanReclaim(target)) {
                QueueUnitSpeech(unit, 7, "That unit cannot be reclaimed");
                return 8;
            }
            ((Class_00438880*)order)->FUN_00438880("Reclaiming");
            FUN_0040f200(unit, order, 0);
            return 1;
        }
        return 7;
    case 1: {
        order->elapsed = FUN_00438650(unit, target, 15);
        order->duration = 0;
        Point origin = unit->footprint;
        Point cell = WorldToCell(order->pos, origin);
        FUN_00414350(cell, &order->pos, origin);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->target.Get()->pos);
        ((Class_004388d0*)order)->FUN_004388d0((int)obj);
        order->flags |= 0x100e8;
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    }
    case 2: {
        if (flags & 0x40)
            return 9;
        order->flags |= 0x10008;
        Vec3 delta = unit->pos - order->target.Get()->pos;
        int range = 0;
        range = unit->def->buildRange;
        int square = delta.Square();
        if (square <= range * range && ((Unit*)unit)->CanReclaim(order->target.Get())) {
            if (order->duration >= 15) {
                DamageUnit(unit, order->target.Get(), order->elapsed, 5, 0);
                order->duration = 0;
            }
            Vec3 start;
            GetNanoPiecePosition(order->source, &start);
            Vec3 bounds[2];
            bounds[1] = order->target.Get()->pos;
            bounds[0] = order->target.Get()->pos;
            bounds[0].x += order->target.Get()->def->min.x;
            bounds[0].z += order->target.Get()->def->min.z;
            bounds[1].x += order->target.Get()->def->max.x;
            bounds[1].z += order->target.Get()->def->max.z;
            bounds[1].y += order->target.Get()->def->max.y;
            EmitReverseNanoParticles(bounds, &start, 6);
            ((Class_00439e80*)order)->FUN_00439e80(2);
            order->duration += 2;
            return 2;
        }
        ((Class_00439e80*)order)->FUN_00439e80(30);
        return 0;
    }
    }
    return 7;
}
