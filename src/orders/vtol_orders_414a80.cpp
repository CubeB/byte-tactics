// Decompiled by Claude Opus 5.5. Names are provisional.
#include "../util/vec3.h"

static inline Vec3 Sub(const Vec3& a, const Vec3& b)
{
    Vec3 r; r.z = a.z - b.z; r.y = a.y - b.y; r.x = a.x - b.x; return r;
}

static inline int Square(const Vec3& v)
{
    __int64 a = v.x, b = v.z;
    return (int)((a*a) >> 32) + (int)((b*b) >> 32);
}

struct Point {
    short x, y;
};

struct Unit;

#include "unit_motion.h"

class Class_0044e6c0 { public: void SetAltitude(int); };

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x15e];
    Vec3 min;                          // +0x15e
    Vec3 max;                          // +0x16a
    char unknown_176[0x212 - 0x176];
    unsigned short buildRange;         // +0x212
    char unknown_214[0x21c - 0x214];
    short altitude;                    // +0x21c
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
    int carrier;                      // +0x86
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
    void AnnounceStatusIfFlagged(const char*);
    void SetAttachedFx(int);
    void SetDeadlineTicks(int);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    Order(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    void AttachApproachRadiusGoal(Vec3* pos, int radius);
    void AttachBuildFootprintMarker(Point cell, Point size);
    Unit* Target();
    void Wait();
    Vec3* Position();
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
int __stdcall ComputeReclaimDamagePulse(Unit* unit, Unit* target, int n);
void __stdcall DamageUnit(Unit* unit, Unit* target, int a, int b, int c);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* target, char p3, char p4);
void __stdcall CellToWorldPos(Point cell, Vec3* out, Point origin);

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// 0x40f200, matched in 0x40f200.cpp; inlined into the state 0 case below.
void __stdcall PrepVtolClimb(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->carrier)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->flags & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude / 2);
        order->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Order handler "Reclaiming" for a unit target (the hovering variant of
// 0x404730): state 0 checks the builder can reclaim, state 1 snaps the order
// position to the target's cell and moves over it, state 2 drains the target
// while it stays in build range.
// Stays in a file of its own: it matches only in this file's symbol context.
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
            if (!unit->CanReclaim(target)) {
                QueueUnitSpeech(unit, 7, "That unit cannot be reclaimed");
                return 8;
            }
            order->AnnounceStatusIfFlagged("Reclaiming");
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        return 7;
    case 1: {
        order->elapsed = ComputeReclaimDamagePulse(unit, target, 15);
        order->duration = 0;
        Point origin = unit->footprint;
        Point cell = WorldToCell(order->pos, origin);
        CellToWorldPos(cell, &order->pos, origin);
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->target.Get()->pos);
        order->SetAttachedFx((int)obj);
        order->flags |= 0x100e8;
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    }
    case 2: {
        if (flags & 0x40)
            return 9;
        order->flags |= 0x10008;
        Vec3 delta = Sub(unit->pos, order->target.Get()->pos);
        int range = 0;
        range = unit->def->buildRange;
        int square = Square(delta);
        if (square <= range * range && unit->CanReclaim(order->target.Get())) {
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
            order->SetDeadlineTicks(2);
            order->duration += 2;
            return 2;
        }
        order->SetDeadlineTicks(30);
        return 0;
    }
    }
    return 7;
}
