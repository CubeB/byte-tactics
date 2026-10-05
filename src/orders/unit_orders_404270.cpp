// Decompiled by Claude Opus 5.5. Names are provisional.
// Capture order handler, a state machine on order->state.
//
// The two handlers before it in the original source file, 0x403a20 and
// 0x403f70 (matched in their own files), are defined again below, unannotated
// and in a namespace of their own, so that their float constants (8.0, -8.0,
// 0.0f and 16.0) come first in the constant pool, as in the original. Compiled
// alone, -150.0f is followed by padding before the 8.0 double instead of the
// original's next constant (-0.5f), and the checker rejects that reference.
//
// Matching notes:
// - EnergyCost() and MetalCost() cast their product to float; without the
//   cast MSVC folds the 30 into the reciprocals of 2000 and 140.
// - The health scaling reads maxHealth once through MaxHealth() and once as a
//   plain field; two identical reads are merged into a single load.
#include <windows.h>
#include <math.h>

namespace Build {
struct Point { short x, y; };
struct Vec3 {
    int x, y, z;
    Vec3 operator+(const Vec3& other) const {
        Vec3 r;
        r.x = x + other.x;
        r.y = y + other.y;
        r.z = z + other.z;
        return r;
    }
};
struct Unit;
class Class_00438760 { public: unsigned char index; Class_00438760(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
class Class_004895c0 { public: void SetUnit(Unit*); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x14a]; Point origin;
    char pad14e[0x15e-0x14e]; Vec3 min, max;
    char pad176[0x1fe-0x176]; unsigned short buildRate;
    char pad200[0x212-0x200]; unsigned short buildRange;
    char pad214[0x241-0x214]; unsigned char flags;
    char pad242[0x249-0x242];
};
struct Unit {
    int active; char pad4[0x66-4]; short angle;
    char pad68[2]; Vec3 pos;
    char pad76[8]; Point footprint;
    char pad82[0x92-0x82]; UnitDef* def;
    char pad96[0xb0-0x96]; int timeout;
    char padb4[0xff-0xb4]; unsigned char player;
    char pad100[4]; float progress;
};
struct Order {
    char pad0[5]; unsigned char state;
    unsigned int flags;
    char padA[0x16-0xa]; Unit* target;
    char pad1A[8]; Vec3 pos;
    char pad2E[8]; int type;
    int unused; int retries;
};
struct Game {
    char pad0[0x1439b]; UnitDef* defs;
    char pad1439f[0x38a47-0x1439f]; int tick;
};
#pragma pack(pop)
extern Game* g_game;
static inline UnitDef* Definitions() { return g_game->defs; }
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
void __stdcall FUN_0047f780(Unit*, int, const char*);
void __stdcall FUN_0041c110(Unit*);
int __stdcall FUN_0047db70(UnitDef*, int, Point, int);
void __stdcall FUN_0047ddc0(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall FUN_0043adc0(Class_00438760, int, Unit*, Unit*, Vec3*, int, int);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall FUN_00438590(Unit*, Order*, short);
int __stdcall FUN_00438700(Unit*, Order*, int);
int __stdcall FUN_0041ba60(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall FUN_004720d0(Vec3*, Vec3*, int);
class Class_00438a00 { public: void FUN_00438a00(Vec3*, int, int); };
static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}
static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}
// 0x403a20, matched in 0x403a20.cpp
int __stdcall FUN_00403a20(Unit* unit, Order* order, int flags)
{
    if (flags & 8) {
        FUN_0047f780(unit, 7, "Construction terminated");
        FUN_0041c110(unit);
        return 8;
    }
    if (flags & 2) {
        FUN_0041c110(unit);
        return 5;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0: {
        UnitDef* def = &Definitions()[order->type];
        Point cell = WorldToCell(order->pos, def->origin);
        CellToWorld(def->origin, cell, &order->pos);
        order->retries = 0;
        ((Class_00438ad0*)order)->FUN_00438ad0(cell, def->origin);
        order->flags = 0xe0;
        return 1;
    }
    case 1: {
        UnitDef* def = &Definitions()[order->type];
        if (flags & 0x40) {
            Fixed distance;
            distance.value = (int)_hypot(unit->pos.x - order->pos.x, unit->pos.z - order->pos.z);
            int gap = distance.whole - (int)(_hypot(unit->footprint.x, unit->footprint.y) * 8.0);
            gap += (int)(_hypot(def->origin.x, def->origin.y) * -8.0);
            unsigned int range = 0;
            range = unit->def->buildRange;
            if (gap > (int)range) {
                FUN_0047f780(unit, 7, "I can't reach the construction site");
                return 8;
            }
        }
        if (!FUN_0047db70(def, 0, WorldToCell(order->pos, Definitions()[order->type].origin), 1)) {
            if (!order->retries)
                FUN_0047f780(unit, 7, "Waiting for target area to clear");
            else if (order->retries > 10) {
                FUN_0047f780(unit, 7, "Target area was blocked");
                return 8;
            }
            order->retries++;
            ((Class_00439e80*)order)->FUN_00439e80(30);
            return 2;
        }
        ((Class_004898b0*)unit)->ClaimWeapons(3);
        FUN_0047ddc0(def, &order->pos);
        ((Class_004895c0*)((char*)order + 0x12))->SetUnit(
            CreateUnit(unit->player, (short)order->type, order->pos, 0, 1, 0));
        if (!order->target) {
            FUN_0047f780(unit, 7, "Unable to create any more units");
            ((Class_00439e80*)order)->FUN_00439e80(300);
            return 2;
        }
        FUN_0047f780(unit, 9, "Starting construction");
        FUN_0041c110(unit);
        FUN_0043adc0("getbuilt", 1, order->target, unit, 0, 0, 0);
        FUN_00438590(unit, order, GetHeadingBetween(&unit->pos, &order->target->pos) - unit->angle);
        return 1;
    }
    case 2:
        return FUN_00438700(unit, order, 10);
    case 3: {
        int rate = 0;
        rate = unit->def->buildRate;
        if (FUN_0041ba60(unit, order->target, (float)(rate / 30))) {
            Vec3 start;
            GetNanoPiecePosition(unit, &start);
            Vec3 bounds[2];
            bounds[0] = order->target->pos + order->target->def->min;
            bounds[1] = order->target->pos + order->target->def->max;
            FUN_004720d0(&start, bounds, 6);
        }
        unit->timeout = g_game->tick + 300;
        if (order->target->progress != 0.0f) {
            ((Class_00439e80*)order)->FUN_00439e80(1);
            order->flags |= 0xa;
            return 2;
        }
        return 1;
    }
    case 4:
        FUN_0047f780(unit, 8, "Building complete");
        return 5;
    }
    return 7;
}

// 0x403f70, matched in 0x403f70.cpp
int __stdcall FUN_00403f70(Unit* unit, Order* order, int flags)
{
    if (flags & 2) { FUN_0041c110(unit); return 5; }
    Unit* target = order->target;
    if (!target) {
        FUN_0047f780(unit, 7, "Construction terminated");
        return 8;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0: {
        if (!unit->active || !(unit->def->flags & 0x40)) break;
        double x = target->def->origin.x;
        double y = target->def->origin.y;
        int radius = (int)(sqrt(x * x + y + y) * 16.0) / 2;
        unsigned int range = 0;
        range = unit->def->buildRange;
        ((Class_00438a00*)order)->FUN_00438a00(&target->pos, range + radius, radius);
        order->flags = 0xe8;
        return 1;
    }
    case 1:
        if (flags & 0x40) {
            FUN_0047f780(unit, 7, "I can't get there");
            return 8;
        }
        if (target->progress == 0.0f) return 5;
        ((Class_004898b0*)unit)->ClaimWeapons(3);
        FUN_00438590(unit, order, GetHeadingBetween(&unit->pos, &order->target->pos) - unit->angle);
        FUN_0041c110(unit);
        return 1;
    case 2:
        return FUN_00438700(unit, order, 10);
    case 3: {
        int rate = 0;
        rate = unit->def->buildRate;
        if (FUN_0041ba60(unit, target, (float)(rate / 30))) {
            Vec3 start;
            GetNanoPiecePosition(unit, &start);
            Vec3 bounds[2];
            bounds[0] = order->target->pos + order->target->def->min;
            bounds[1] = order->target->pos + order->target->def->max;
            FUN_004720d0(&start, bounds, 6);
        }
        unit->timeout = g_game->tick + 300;
        if (order->target->progress != 0.0f) {
            ((Class_00439e80*)order)->FUN_00439e80(1);
            order->flags |= 0xa;
            return 2;
        }
        return 1;
    }
    case 4:
        FUN_0047f780(unit, 8, "Building complete");
        order->flags |= 2;
        return 5;
    }
    return 7;
}
} // namespace Build

struct Vec3 { int x, y, z; };
struct Point { short x, y; };
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
struct Unit;
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x15e]; Vec3 min, max;
    char pad176[0x186-0x176]; float energy, metal;
    char pad18e[0x1fa-0x18e]; unsigned int maxHealth;
    char pad1fe[0x212-0x1fe]; unsigned short buildRange;
    char pad214[0x245-0x214];
    float EnergyCost() { return (float)(energy * 30); }
    float MetalCost() { return (float)(metal * 30); }
    unsigned int MaxHealth() { return maxHealth; }
    union { unsigned int flags; struct { unsigned int lo:12; unsigned int capture:1; unsigned int hi:19; }; };
};
struct Unit {
    int active; char pad4[0x66-4]; short angle;
    char pad68[2]; Vec3 pos; Point cell;
    char pad7a[4]; Point footprint;
    char pad82[0x92-0x82]; UnitDef* def; void* owner;
    char pad9a[0xb0-0x9a]; int timeout;
    char padb4[4]; unsigned short experience;
    char padba[0x104-0xba]; float progress;
    short health; char pad10a[6]; unsigned int flags;
};
struct UnitRef { int vtable; Unit* ptr; Unit* Get() { return ptr; } };
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char pada[4]; Unit* source;
    UnitRef target;
    char pad1a[0x36-0x1a]; int elapsed, duration;
};
struct Game { char pad0[0x38a47]; int tick; };
#pragma pack(pop)
extern Game* g_game;
void __stdcall FUN_0047f780(Unit*, int, const char*);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall FUN_00438590(Unit*, Order*, short);
void __stdcall FUN_004385f0(Unit*, Order*);
int __stdcall FUN_00438700(Unit*, Order*, int);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall FUN_00472200(Vec3*, Vec3*, int);
void __stdcall GiveUnitToPlayer(Unit*, void*, int);
// FUNCTION: 0x404270
int __stdcall FUN_00404270(Unit* unit, Order* order, unsigned int flags)
{
    Unit* target = order->target.Get();
    if (!target || (flags & 0x10008)) {
        FUN_0047f780(unit, 7, "Capture failed");
        return 8;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0: {
        if (!unit->active) return 7;
        if (!(unit->def->flags & 0x1000)) return 7;
        if (target->def->capture) {
            FUN_0047f780(unit, 7, "That unit cannot be captured");
            return 8;
        }
        if (target->progress != 0.0f) {
            FUN_0047f780(unit, 7, "That unit is a cloud of vapor and cannot be captured");
            return 8;
        }
        ((Class_00438880*)order)->FUN_00438880("Capturing");
        order->duration = (int)(order->target.Get()->def->EnergyCost() / 2000 + order->target.Get()->def->MetalCost() / 140 + 150);
        order->duration = order->duration < 1800 ? order->duration : 1800;
        order->duration = (order->target.Get()->health + order->target.Get()->def->MaxHealth()) * order->duration / (order->target.Get()->def->maxHealth * 2);
        int experience = 0;
        experience = order->target.Get()->experience;
        order->duration = ((experience / 5 + 10) * order->duration * 10) / 100;
        ((Class_004898b0*)unit)->ClaimWeapons(3);
        ((Class_00438ad0*)order)->FUN_00438ad0(order->target.Get()->cell, order->target.Get()->footprint);
        order->flags = 0x100e8;
        return 1;
    }
    case 1: {
        if (flags & 0x40) return 8;
        Vec3* position = &unit->pos;
        Fixed distance;
        distance.value = (int)_hypot(unit->pos.x - target->pos.x, unit->pos.z - target->pos.z);
        int gap = distance.whole - (int)(_hypot(unit->footprint.x, unit->footprint.y) * 8.0);
        gap += (int)(_hypot(order->target.Get()->footprint.x, order->target.Get()->footprint.y) * -8.0);
        unsigned int range = 0;
        range = unit->def->buildRange;
        if (gap > (int)range) return 0;
        FUN_00438590(unit, order, GetHeadingBetween(position, &order->target.Get()->pos) - unit->angle);
        return 1;
    }
    case 2:
        return FUN_00438700(unit, order, 0x10008);
    case 3:
        FUN_0047f780(unit, 11, 0);
        return 1;
    case 4: {
        if (target->active && (target->flags & 0xc)) {
            FUN_004385f0(unit, order);
            ((Class_00439e80*)order)->FUN_00439e80(30);
            return 0;
        }
        if (order->elapsed >= order->duration) return 1;
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
        FUN_00472200(bounds, &start, 6);
        unit->timeout = g_game->tick + 900;
        order->elapsed += 2;
        ((Class_00439e80*)order)->FUN_00439e80(2);
        return 2;
    }
    case 5:
        GiveUnitToPlayer(target, unit->owner, 0);
        FUN_0047f780(unit, 16, 0);
        return 5;
    }
    return 7;
}
