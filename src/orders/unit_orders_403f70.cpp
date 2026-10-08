// Decompiled by GPT-6 Astra. Names are provisional.
#include <windows.h>
#include <math.h>
#include <memory.h>
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
class Class_00438ad0 { public: void AttachBuildFootprintMarker(Point, Point); };
class Class_00439e80 { public: void FUN_00439e80(int); };
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
    void ClaimWeapons(int);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void SetStateBits(int, int);
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
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall MarkSelectionOrdersDirty(Unit*);
int __stdcall FUN_0047db70(UnitDef*, int, Point, int);
void __stdcall FUN_0047ddc0(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall AddOrder(Class_00438760, int, Unit*, Unit*, Vec3*, int, int);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall StartBuildingScript(Unit*, Order*, short);
int __stdcall WaitIfNotInBuildStance(Unit*, Order*, int);
int __stdcall AddBuildProgress(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitNanoParticles(Vec3*, Vec3*, int);
class Class_00438a00 { public: void AttachRingApproachGoal(Vec3*, int, int); };
// The original radius expression adds the second dimension twice rather than
// squaring it: fld x; fld y; fld st(1); fmul st(2); fadd st(1); fadd st(1).
// FUNCTION: 0x403f70
int __stdcall HelpBuildOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 2) { MarkSelectionOrdersDirty(unit); return 5; }
    Unit* target = order->target;
    if (!target) {
        QueueUnitSpeech(unit, 7, "Construction terminated");
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
        ((Class_00438a00*)order)->AttachRingApproachGoal(&target->pos, range + radius, radius);
        order->flags = 0xe8;
        return 1;
    }
    case 1:
        if (flags & 0x40) {
            QueueUnitSpeech(unit, 7, "I can't get there");
            return 8;
        }
        if (target->progress == 0.0f) return 5;
        ((Unit*)unit)->ClaimWeapons(3);
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &order->target->pos) - unit->angle);
        MarkSelectionOrdersDirty(unit);
        return 1;
    case 2:
        return WaitIfNotInBuildStance(unit, order, 10);
    case 3: {
        int rate = 0;
        rate = unit->def->buildRate;
        if (AddBuildProgress(unit, target, (float)(rate / 30))) {
            Vec3 start;
            GetNanoPiecePosition(unit, &start);
            Vec3 bounds[2];
            bounds[0] = order->target->pos + order->target->def->min;
            bounds[1] = order->target->pos + order->target->def->max;
            EmitNanoParticles(&start, bounds, 6);
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
        QueueUnitSpeech(unit, 8, "Building complete");
        order->flags |= 2;
        return 5;
    }
    return 7;
}
