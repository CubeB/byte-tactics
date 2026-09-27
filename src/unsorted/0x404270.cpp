// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: health scaling reuses maxHealth instead of reloading it; the
// resulting register allocation and instruction layout differ.
#include <windows.h>
#include <math.h>
struct Vec3 { int x, y, z; };
struct Point { short x, y; };
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
struct Unit;
extern const float DAT_004fc930, DAT_004fc934, DAT_004fc938, DAT_004fc93c;
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x15e]; Vec3 min, max;
    char pad176[0x186-0x176]; float energy, metal;
    char pad18e[0x1fa-0x18e]; unsigned int maxHealth;
    char pad1fe[0x212-0x1fe]; unsigned short buildRange;
    char pad214[0x245-0x214];
    float EnergyCost() { return energy * DAT_004fc930; }
    float MetalCost() { return metal * DAT_004fc930; }
    unsigned int Health() { return maxHealth; }
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
short __stdcall FUN_0048a980(Vec3*, Vec3*);
void __stdcall FUN_00438590(Unit*, Order*, short);
void __stdcall FUN_004385f0(Unit*, Order*);
int __stdcall FUN_00438700(Unit*, Order*, int);
void __stdcall FUN_0043e400(Unit*, Vec3*);
void __stdcall FUN_00472200(Vec3*, Vec3*, int);
void __stdcall FUN_00488570(Unit*, void*, int);
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
        order->duration = (int)(order->target.Get()->def->EnergyCost() * DAT_004fc934 - order->target.Get()->def->MetalCost() * DAT_004fc938 - DAT_004fc93c);
        order->duration = order->duration < 1800 ? order->duration : 1800;
        Unit* captured = order->target.Get();
        UnitDef* capturedDef = captured->def;
        unsigned int health = capturedDef->maxHealth;
        order->duration = (captured->health + health) * order->duration / (capturedDef->Health() * 2);
        int experience = 0;
        experience = order->target.Get()->experience;
        order->duration = ((experience / 5 + 10) * order->duration * 10) / 100;
        ((Class_004898b0*)unit)->FUN_004898b0(3);
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
        FUN_00438590(unit, order, FUN_0048a980(position, &order->target.Get()->pos) - unit->angle);
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
        FUN_0043e400(order->source, &start);
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
        FUN_00488570(target, unit->owner, 0);
        FUN_0047f780(unit, 16, 0);
        return 5;
    }
    return 7;
}
