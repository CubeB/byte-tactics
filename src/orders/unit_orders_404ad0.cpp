// Decompiled by Claude Opus 5.5. Names are provisional.
// <windows.h> is needed: without it the feature pointer is loaded straight
// into esi instead of being added up in eax and copied. The whole-struct
// copy `box.hi = box.lo` gives the early store of hi.x and puts order in
// edi and the feature in esi.
#include <windows.h>
struct Vec3 {
    int x, y, z;
};

struct Box {
    Vec3 lo;
    Vec3 hi;
};

struct Point16 {
    short x;
    short z;
};

#pragma pack(push, 1)
#include "../map/feature.h"

#include "../units/unit_def.h"

struct Unit {
    // +0x0 holds the unit's UnitMotion pointer; only its non-nullness is read
    // here, and an int spelling keeps the file's symbol count (docs/c2-regalloc.md).
    int motion;                        // +0x0
    char unknown_4[0x66 - 0x4];
    unsigned short heading;            // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef* type;                     // +0x92
    char unknown_96[0xb0 - 0x96];
    int workTime;                      // +0xb0
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0x22 - 0xa];
    Vec3 pos;                          // +0x22
    char unknown_2e[0x36 - 0x2e];
    int time;                          // +0x36
    void AttachBuildFootprintMarker(Point16 cell, Point16 size);
    void SetDeadlineTicks(int ticks);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    ~Order();
    Order(Unit* unit, void* file, char* name);
    void OrStatusFlags(unsigned int flags);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
};

struct Game {
    char unknown_0[0x1426f];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x38a47 - 0x14273];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
int __stdcall RandomInt(int range);
int __stdcall GetGroundHeight(Vec3* pos);
unsigned short __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
void __stdcall StartBuildingScript(Unit* unit, Order* order, short turn);
int __stdcall WaitIfNotInBuildStance(Unit* unit, Order* order, int flags);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitReverseNanoParticles(Box* from, Vec3* to, int count);
void __stdcall ReclaimFeature(Unit* unit, Vec3* pos);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
void __stdcall AddOrder(int kind, int remove, Unit* owner, void* id, Vec3* pos, int param_6, int param_7);
void __stdcall AdjustBuildCount(int kind, Unit* owner, int id, int amount);

// Order handler "Reclaiming" for a feature (wreck, tree, rock) at the order
// position.
// FUNCTION: 0x404ad0
int __stdcall ReclaimOrder(Unit* unit, Order* order, int flags)
{
    Point16 cell;
    Point16 size;
    unsigned short index = FindFeatureAtPos(&order->pos, &cell, &size);
    if (index == 0xffff) {
        QueueUnitSpeech(unit, 7, "Reclamation failed");
        return 8;
    }
    Feature* f = &g_game->features[index];
    if (!(f->flags & 0x80))
        return 8;
    switch (order->state) {
    case 0:
        if (unit->motion && (unit->type->flags2 & 0x400)) {
            order->AttachBuildFootprintMarker(cell, size);
            order->flags = 0xe0;
            return 1;
        }
        break;
    case 1: {
        if (flags & 0x40)
            return 8;
        order->time = (int)(15.0f - (f->metal + f->value) * -0.5f);
        Vec3 pos;
        pos.x = (size.x + cell.x * 2) << 19;
        pos.z = (size.z + cell.z * 2) << 19;
        pos.y = (RandomInt(f->height) + GetGroundHeight(&pos)) << 16;
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &pos) - unit->heading);
        return 1;
    }
    case 2:
        return WaitIfNotInBuildStance(unit, order, 0);
    case 3:
        QueueUnitSpeech(unit, 11, 0);
    case 4:
        order->SetDeadlineTicks(2);
        order->time -= 2;
        if (order->time <= 0)
            return 1;
        unit->workTime = g_game->ticks + 300;
        if (order->time > 15) {
            Vec3 nano;
            GetNanoPiecePosition(unit, &nano);
            Box box;
            box.lo.x = cell.x << 20;
            box.lo.z = cell.z << 20;
            box.lo.y = GetGroundHeight(&box.lo) << 16;
            box.hi = box.lo;
            box.hi.x += f->footprintX << 20;
            box.hi.z += f->footprintZ << 20;
            box.hi.y += f->height << 16;
            EmitReverseNanoParticles(&box, &nano, 6);
            EmitReverseNanoParticles(&box, &nano, 6);
        }
        return 2;
    case 5:
        ReclaimFeature(unit, &order->pos);
        return 5;
    }
    return 7;
}
