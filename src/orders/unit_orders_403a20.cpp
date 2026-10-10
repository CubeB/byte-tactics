// Decompiled by GPT-6 Astra. Names are provisional.
#include <windows.h>
#include <math.h>
struct Point { short x, y; };
// Kept local, not util/vec3.h: its free operators inline differently from these members and the code size changes.
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
#include "mission_type.h"

#include "path_order_attach.h"
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x14a]; Point origin;
    char pad14e[0x15e-0x14e]; Vec3 min, max;
    char pad176[0x1fe-0x176]; unsigned short buildRate;
    char pad200[0x212-0x200]; unsigned short buildRange;
    char pad214[0x249-0x214];
};
struct Unit {
    char pad0[0x66]; short heading;
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
    void AttachBuildFootprintMarker(Point, Point);
    void SetDeadlineTicks(int);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    Order(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    void AttachApproachRadiusGoal(Vec3* pos, int radius);
    Unit* Target();
    void Wait();
    Vec3* Position();
};
struct Game {
    char pad0[0x1439b]; UnitDef* unitDefs;
    char pad1439f[0x38a47-0x1439f]; int tick;
};
#pragma pack(pop)
extern Game* g_game;
static inline UnitDef* Definitions() { return g_game->unitDefs; }
union Fixed { int value; struct { unsigned short fraction; short whole; }; };
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall MarkSelectionOrdersDirty(Unit*);
int __stdcall CanPlaceUnitFootprint(UnitDef*, int, Point, int);
void __stdcall SnapWorldPosToFootprint(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall AddOrder(MissionType, int, Unit*, Unit*, Vec3*, int, int);
short __stdcall GetHeadingBetween(Vec3*, Vec3*);
void __stdcall StartBuildingScript(Unit*, Order*, short);
int __stdcall WaitIfNotInBuildStance(Unit*, Order*, int);
int __stdcall AddBuildProgress(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitNanoParticles(Vec3*, Vec3*, int);
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
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Feature;
// FUNCTION: 0x403a20
int __stdcall MobileBuildOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 8) {
        QueueUnitSpeech(unit, 7, "Construction terminated");
        MarkSelectionOrdersDirty(unit);
        return 8;
    }
    if (flags & 2) {
        MarkSelectionOrdersDirty(unit);
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
        order->AttachBuildFootprintMarker(cell, def->origin);
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
                QueueUnitSpeech(unit, 7, "I can't reach the construction site");
                return 8;
            }
        }
        if (!CanPlaceUnitFootprint(def, 0, WorldToCell(order->pos, Definitions()[order->type].origin), 1)) {
            if (!order->retries)
                QueueUnitSpeech(unit, 7, "Waiting for target area to clear");
            else if (order->retries > 10) {
                QueueUnitSpeech(unit, 7, "Target area was blocked");
                return 8;
            }
            order->retries++;
            order->SetDeadlineTicks(30);
            return 2;
        }
        unit->ClaimWeapons(3);
        SnapWorldPosToFootprint(def, &order->pos);
        ((PathOrderAttach*)((char*)order + 0x12))->SetUnit(
            CreateUnit(unit->player, (short)order->type, order->pos, 0, 1, 0));
        if (!order->target) {
            QueueUnitSpeech(unit, 7, "Unable to create any more units");
            order->SetDeadlineTicks(300);
            return 2;
        }
        QueueUnitSpeech(unit, 9, "Starting construction");
        MarkSelectionOrdersDirty(unit);
        AddOrder("getbuilt", 1, order->target, unit, 0, 0, 0);
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &order->target->pos) - unit->heading);
        return 1;
    }
    case 2:
        return WaitIfNotInBuildStance(unit, order, 10);
    case 3: {
        int rate = 0;
        rate = unit->def->buildRate;
        if (AddBuildProgress(unit, order->target, (float)(rate / 30))) {
            Vec3 start;
            GetNanoPiecePosition(unit, &start);
            Vec3 bounds[2];
            bounds[0] = order->target->pos + order->target->def->min;
            bounds[1] = order->target->pos + order->target->def->max;
            EmitNanoParticles(&start, bounds, 6);
        }
        unit->timeout = g_game->tick + 300;
        if (order->target->progress != 0.0f) {
            order->SetDeadlineTicks(1);
            order->flags |= 0xa;
            return 2;
        }
        return 1;
    }
    case 4:
        QueueUnitSpeech(unit, 8, "Building complete");
        return 5;
    }
    return 7;
}
