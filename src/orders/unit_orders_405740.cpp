// Decompiled by GPT-6 Astra. Names are provisional.
#include <math.h>
struct Vec3 { int x, y, z; };
struct Unit;
class Class_00439e80 { public: void FUN_00439e80(int); };
#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x15e]; Vec3 min, max;
    char pad176[0x1fa-0x176]; unsigned int maxHealth;
    unsigned short buildRate;
    char pad200[0x241-0x200]; unsigned char flags;
};
struct Unit {
    char pad0[0x6a]; Vec3 pos;
    char pad76[0x92-0x76]; UnitDef* def;
    char pad96[0xb0-0x96]; int timeout;
    char padb4[0x104-0xb4]; float progress;
    short health; char pad10a[4]; unsigned short flags10e;
    unsigned int flags;
    void ClaimWeapons(int);
};
struct UnitRef { int vtable; Unit* ptr; Unit* Get() { return ptr; } };
struct Order {
    char pad0[5]; unsigned char state; unsigned int flags;
    char pada[4]; Unit* source; UnitRef target;
};
struct Game { char pad0[0x38a47]; int tick; };
#pragma pack(pop)
extern Game* g_game;
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
int __stdcall AddRepairProgress(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitNanoParticles(Vec3*, Vec3*, int);
// FUNCTION: 0x405740
int __stdcall RepairUnitNoMoveOrder(Unit* unit, Order* order, int unused)
{
    Unit* target = order->target.Get();
    if (!target) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (!(unit->def->flags & 0x40)) return 7;
        if (target->progress == 0.0f && (unit->flags10e & 1)) {
            ((Unit*)unit)->ClaimWeapons(3);
            return 1;
        }
        return 8;
    case 1: {
        if ((unsigned int)target->health >= target->def->maxHealth) return 1;
        if (target->flags & 0xc) return 1;
        unit->timeout = g_game->tick + 150;
        int rate = 0;
        rate = unit->def->buildRate;
        if (AddRepairProgress(unit, order->target.Get(), (float)(rate / 30))) {
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
            EmitNanoParticles(&start, bounds, 6);
        }
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 8;
        return 2;
    }
    case 2:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}
