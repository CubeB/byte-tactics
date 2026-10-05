// Decompiled by GPT-6 Astra. Names are provisional.
#include <windows.h>
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
struct Point { short x, y; };
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
struct Unit;
extern const float DAT_004fc930, DAT_004fc934, DAT_004fc938, DAT_004fc93c;
class Class_00438880 { public: void FUN_00438880(const char*); };
class Class_00438ad0 { public: void FUN_00438ad0(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x15e]; Vec3 min, max;
    char pad176[0x184-0x176]; short radius; float energy, metal;
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
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall StartBuildingScript(Unit*, Order*, short);
void __stdcall StopBuildingScript(Unit*, Order*);
int __stdcall FUN_00438700(Unit*, Order*, int);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitReverseNanoParticles(Vec3*, Vec3*, int);
void __stdcall GiveUnitToPlayer(Unit*, void*, int);
class Class_00489960 { public: int CanReclaim(Unit*); };
int __stdcall FUN_00438650(Unit*, Unit*, int);
void __stdcall DamageUnit(Unit*, Unit*, int, int, int);
static inline int SquaredDistance(int dx, int dz)
{
    __int64 x = dx;
    __int64 z = dz;
    return (int)((x * x) >> 32) + (int)((z * z) >> 32);
}
// FUNCTION: 0x404730
int __stdcall ReclaimUnitOrder(Unit* unit, Order* order, unsigned int flags)
{
    Unit* target = order->target.Get();
    if (!target || (flags & 0x10008)) return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->active && (unit->def->flags & 0x400)) {
            if (!((Class_00489960*)unit)->CanReclaim(target)) {
                QueueUnitSpeech(unit, 7, "That unit cannot be reclaimed");
                QueueUnitSpeech(unit, 7, "Reclamation failed");
                return 8;
            }
            ((Class_00438880*)order)->FUN_00438880("Reclaiming");
            ((Class_004898b0*)unit)->ClaimWeapons(3);
            return 1;
        }
        QueueUnitSpeech(unit, 7, "Reclamation failed");
        return 7;
    case 1:
        if (flags & 0x20) return 1;
        ((Class_00438ad0*)order)->FUN_00438ad0(target->cell, target->footprint);
        order->flags |= 0x100e8;
        ((Class_00439e80*)order)->FUN_00439e80(15);
        order->elapsed = FUN_00438650(unit, order->target.Get(), 15);
        order->duration = 0;
        return 2;
    case 2:
        if (flags & 0x40) return 9;
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &target->pos) - unit->angle);
        return 1;
    case 3:
        return FUN_00438700(unit, order, 0x10008);
    case 4:
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    case 5: {
        Vec3 delta = unit->pos - target->pos;
        int range = 0;
        range = unit->def->buildRange;
        range += target->def->radius;
        int square = delta.Square();
        if (square <= range * range && ((Class_00489960*)unit)->CanReclaim(order->target.Get())) {
            if (order->duration >= 15) {
                DamageUnit(unit, order->target.Get(), order->elapsed, 5, 0);
                order->duration = 0;
            }
            unit->timeout = g_game->tick + 900;
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
        ((Class_00439e80*)order)->FUN_00439e80(15);
        StopBuildingScript(unit, order);
        return 0;
    }
    }
    return 7;
}
