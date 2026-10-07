// Decompiled by Opus, GPT-6, Claude Opus 5.5, Sonnet, DeepSeek V4.1 Flash, Haiku, GPT-6.1-sol, space-bunny-free, deepseek-v4.1 and claude-sonnet-5-5. Names are provisional.
// The ground order handlers the table at 0x4fc490 names, from Stop (0x401c20)
// to Patrol (0x4033a0). The unit's order object (Order) carries the state
// machine, the target and the position; the unit (Unit) and its type
// (UnitDef) carry the fields the handlers read and write.
#include <vector>

struct Vec_00401c20 {
    int x, y, z;
};

struct Point16 {
    short x;
    short z;
};

struct Box {
    Vec_00401c20 lo;
    Vec_00401c20 hi;
};

struct Unit;
struct Order;
struct UnitDef;

class CobScript {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

class UnitResources {
public:
    char unknown_0[0x18];
    float metal;                       // +0x18
    char unknown_1c[0x28 - 0x1c];
    int RequestEnergyAndMetal(float energy, float metal);
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
    int operator==(Class_00438760 o) const { return index == o.index; }
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

class Class_00438880 {
public:
    void FUN_00438880(char* text);
};

class Class_00438930 {
public:
    void FUN_00438930(int* pos, int param);
};

class Class_004388d0 {
public:
    void FUN_004388d0(int param);
};

class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec_00401c20* pos, int c, int d, int e);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x14a];
    Point16 footprint;                 // +0x14a
    char unknown_14e[0x15e - 0x14e];
    Box bounds;                        // +0x15e
    char unknown_176[0x18a - 0x176];
    float metalCost;                   // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int maxHealth;            // +0x1fa
    unsigned short workerTime;         // +0x1fe
    char unknown_200[0x218 - 0x200];
    unsigned short radius;             // +0x218
    char unknown_21a[0x241 - 0x21a];
    union {
        unsigned int flags;            // +0x241
        unsigned char flags241;        // +0x241, the handlers that test a byte
    };
    union {
        unsigned char flags245;        // +0x245, AttackUTypeOrder's byte test
        struct {
            unsigned int low : 20;     // +0x245, SelfDestructOrder's countdown
            unsigned int countdown : 3;
            unsigned int high : 9;
        };
    };
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct WeaponType {
    char unknown_0[0xc0];
    float energyCost;                  // +0xc0 (TDF energypershot)
    float metalCost;                   // +0xc4 (TDF metalpershot)
    char unknown_c8[0xe4 - 0xc8];
    unsigned short buildTime;          // +0xe4
};

struct Weapon {
    WeaponType* type;                  // +0x0
    char unknown_4[0xe - 0x4];
    unsigned char stockpile;           // +0xe
    unsigned char flags;               // +0xf
    char unknown_10[0x1c - 0x10];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x1439b - 0x1435f];
    UnitDef* unitTypes;                // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    int ticks;                         // +0x38a47
};

struct Sub_403010 {
    char unknown_0[0x245];
    unsigned int unused_bits : 2;
    unsigned int flag : 1;             // +0x245, bit 2
    unsigned int unused_bits2 : 29;
};

struct Target {
    char unknown_0[0x245];
    unsigned int lowbits : 13;
    unsigned int flag : 1;             // +0x245, bit 13
};

class Class_00438b90 {
public:
    char unknown_0[0x16];
    Unit* target;                      // +0x16
    char unknown_1a[0x36 - 0x1a];
    int state;                         // +0x36

    void FUN_00438b90(Class_00438760 kind);
};

struct Order {
    char unknown_0[4];
    Class_00438760 kind;               // +0x4
    unsigned char state;               // +0x5
    union {
        unsigned int flags;            // +0x6
        struct {
            unsigned int low : 15;
            unsigned int waiting : 1;
            unsigned int high : 16;
        };
    };
    char unknown_a[0x12 - 0xa];
    Class_004895c0 target;             // +0x12, its owner is at +0x16
    Vec_00401c20 pos;                  // +0x22
    char unknown_2e[0x36 - 0x2e];
    union {
        int wait;                      // +0x36
        int id;
        int weapon;
        int unitType;
        int ticks;
        int mode;
        int done;
        int field_36;
    };
    union {
        int radius;                    // +0x3a
        int waitLimit;
        int count;
    };
    int progress;                      // +0x3e
    char unknown_42[0x4a - 0x42];
    Order* next;                       // +0x4a

    Unit* Target() { return target.owner; }
    void Wait() { waiting = 1; }
    int* Position() { return (int*)&pos; }
};

struct Unit {
    int active;                        // +0x0
    char unknown_4[0x10 - 0x4];
    // Two views of +0x10: the three weapons, or the order list at +0x5c.
    union {
        Weapon weapons[3];             // +0x10
        struct {
            char unknown_10[0x5c - 0x10];
            Order* orders;             // +0x5c
        };
    };
    char unknown_64[0x6a - 0x64];
    Vec_00401c20 pos;                  // +0x6a
    char unknown_76[0x86 - 0x76];
    int blocked;                       // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitDef* def;                      // +0x92
    Player* owner;                     // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short id;                 // +0xa6
    char unknown_a8[0xac - 0xa8];
    int value;                         // +0xac
    int repairTime;                    // +0xb0
    char unknown_b4[0xbc - 0xb4];
    UnitResources resources;           // +0xbc
    char unknown_e4[0xec - 0xe4];
    Player* player;                    // +0xec
    char unknown_f0[0xff - 0xf0];
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float buildLeft;                   // +0x104
    short health;                      // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char flags10e;            // +0x10e
    char unknown_10f[0x110 - 0x10f];
    // The unit's state flags: the handlers name their own bits.
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 16;
            unsigned int bits18 : 2;
            unsigned int bits20 : 2;
            unsigned int unknown_22 : 10;
        } bits;
        struct {
            unsigned int low : 18;
            unsigned int fire : 2;
            unsigned int move : 2;
            unsigned int high : 10;
        };
        struct {
            unsigned int unknown_bits : 18;
            unsigned int mode2 : 2;
            unsigned int unknown_bits2 : 12;
        };
        struct {
            unsigned int unknown_bits20 : 20;
            unsigned int mode : 2;
            unsigned int unknown_bits210 : 10;
        };
    };
    char unknown_114[0x118 - 0x114];   // sizeof(Unit) is the units array's stride
    void ReleaseWeapons(int param);
    void ClaimWeapons(int param);
    void SetStateBits(int which, int on);
    int Ready() { return (flags & 0x10000000) && !(flags & 0x4000); }
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RegisterOrderTypes(void* table, int id);
extern char DAT_004fc490[];

void __stdcall ClearWeaponTarget(Unit* unit, int weapon);
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);
int __stdcall RandomInt(int range);
void __stdcall GetVisibleEnemiesInRadius(int player, Vec_00401c20* pos, int radius, int flags,
                                         std::vector<Unit*>* out);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit, Unit* target, void* flags);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
void __stdcall DamageUnit(Unit* unit, Unit* target, int n, int kind, int flag);
void __stdcall SetWeaponTargetUnit(Unit* unit, Unit* target, int weapon);
Unit* __stdcall GetWeaponTargetUnit(Unit* unit, int index);
int __stdcall WeaponCanReachUnit(Unit* unit, Unit* target, int param_3);
void __stdcall FUN_0041c150(Unit* unit);
void __stdcall FinishConstruction(Unit* unit, Unit* target);
int __stdcall FUN_00438700(Unit* unit, Order* order, int flags);
Vec_00401c20 __stdcall GetPiecePosition(Unit* unit, int piece);
int __stdcall FUN_0047db70(UnitDef* type, short a, Point16 cell, int b);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short type, Vec_00401c20 pos,
                           int a, int b, int c);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* builder, char piece, char p4);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner, Unit* id, void* pos,
                        int param_6, int param_7);
int __stdcall FUN_0041bd10(Unit* builder, Unit* unit, float amount);
int __stdcall AddBuildProgress(Unit* unit, Unit* target, float amount);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec_00401c20* out);
void __stdcall EmitNanoParticles(Vec_00401c20* from, Box* to, int count);
void __stdcall FUN_0041c110(Unit* unit);
void __stdcall FUN_0041bcd0(Unit* unit, int param);
void __stdcall FUN_0043a020(Unit* unit, Order* order);
Order* __stdcall FUN_0043b700(Unit* unit);
int __stdcall FUN_0043b1f0(Unit* unit, Order* order, int param);

// Handler of the "Stopping" entry in the order table at 0x4fc490: stops the
// unit and, for a VTOL that is flying, queues a VTOL_LANDIFCAN order.
// FUNCTION: 0x401c20
int __stdcall StopOrder(Unit* unit, Class_00438880* order, int unused)
{
    order->FUN_00438880(0);
    ClearWeaponTarget(unit, 0);
    ClearWeaponTarget(unit, 1);
    ClearWeaponTarget(unit, 2);
    if ((unit->flags & 3) == 2 && (unit->def->flags & 0x800)) {
        AppendOrder(unit, new Class_0043a1f0("VTOL_LANDIFCAN", 0, &unit->pos, 0, 0, 0));
    }
    return 5;
}

// An order handler from the same table as 0x401c20: updates two unit flag
// bits and returns 5, like its neighbour.
// FUNCTION: 0x401cc0
int __stdcall MakeSelectableOrder(Unit* unit, Class_00438880* order, int unused)
{
    unit->flags = (unit->flags & ~0x8000) | 0x20;
    return 5;
}

// FUNCTION: 0x401ce0
int __stdcall WaitOrder(Unit* unit, Order* order, int flags)
{
    if(order->radius) {
        std::vector<Unit*> units;
        GetVisibleEnemiesInRadius(unit->playerIndex,&unit->pos,order->radius,0,&units);
        if(!units.empty()) return 5;
        if(order->wait<=0) return 5;
        int delay=RandomInt(30)+150;
        order->wait-=delay;
        ((Class_00439e80*)order)->FUN_00439e80(delay);
        return 2;
    }
    unsigned state=0;
    state=order->state;
    switch(state) {
    case 0: ((Class_00439e80*)order)->FUN_00439e80(order->wait); return 1;
    case 1: return 5;
    default: return 7;
    }
}

// Order handler: state 0 waits a random time if the unit's type allows it;
// state 1 picks the nearest non-allied unit with the order's id (the squared
// distance minus a random share of itself) and queues an order on it.
// FUNCTION: 0x401e00
int __stdcall AttackUTypeOrder(Unit* unit, Order* order, int unused)
{
    unsigned int s = 0;
    s = order->state;
    switch (s) {
    case 0:
        if (!(unit->def->flags245 & 0x10))
            return 7;
        ((Class_00439e80*)order)->FUN_00439e80(RandomInt(0x5a) + 1);
        return 1;
    case 1: {
        Unit* best = 0;
        int bestDist = 0x7fffffff;
        for (Unit* u = g_game->units + 1; u <= g_game->units_end; u++) {
            if (u->id == order->id && unit->owner->allied[u->owner->index] == 0) {
                int dz = u->pos.z - unit->pos.z;
                int dx = u->pos.x - unit->pos.x;
                int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
                d -= RandomInt(d / 2);
                if (d <= bestDist) {
                    best = u;
                    bestDist = d;
                }
            }
        }
        if (best) {
            Class_00438760 kind = GetOrderType(3, unit, best, 0);
            AppendOrder(unit, new Class_0043a1f0(kind, best, 0, 0, 0, 0));
            return 0;
        }
        return 5;
    }
    default:
        return 7;
    }
}

// FUNCTION: 0x401fd0
int __stdcall WaitForAttackOrder(int unused1, Order* obj, int unused3)
{
    if (obj->target.owner == 0) {
        return 5;
    }
    unsigned int s = 0;
    s = obj->state;
    switch (s) {
    case 0:
        obj->flags = 0x18;
        return 1;
    case 1:
        return 5;
    default:
        return 7;
    }
}

// FUNCTION: 0x402010
int __stdcall SelfDestructOrder(Unit* unit, Order* order, int flags)
{
    if(!(order->count&0xf0000000)) order->count=unit->def->countdown|0xf0000000;
    if(!order->done && unit->def->countdown>0) {
        int sounds[6]={22,21,20,19,18,17};
        if(flags&2) {
            if(!(unit->flags&0x4000)) { QueueUnitSpeech(unit,23,0); return 5; }
        } else {
            int count=order->count&0x0fffffff;
            if(!count) order->done=1;
            else order->count=(count-1)|0xf0000000;
            if(count>=0) {
                QueueUnitSpeech(unit,sounds[count],0);
                if(count>0) {
                    ((Class_00439e80*)order)->FUN_00439e80(30);
                    order->flags|=2; return 1;
                }
                if(count==0) {
                    ((Class_00439e80*)order)->FUN_00439e80(RandomInt(15));
                    order->flags|=2; return 1;
                }
            }
            DamageUnit(unit,unit,30000,3,0);
        }
    } else DamageUnit(unit,unit,30000,3,0);
    return 5;
}

// FUNCTION: 0x402160
int __stdcall AttackNoMoveOrder(Unit* unit, Order* order, int flags)
{
    if (order->target.owner == 0 || (flags & 0x10808) != 0)
        return 5;
    switch (order->state) {
    case 0:
        ((Class_00438880*)order)->FUN_00438880(0);
        return 1;
    case 1:
        unit->ClaimWeapons(0);
        SetWeaponTargetUnit(unit, order->target.owner, 0);
        order->flags = 0x11808;
        return 1;
    case 2:
        unit->ReleaseWeapons(3);
        return 9;
    default:
        return 7;
    }
}

// Order handler: follows the order's target unit, waits a random number of
// ticks near it and, when interrupted, picks a random unit around the
// remembered position (GetVisibleEnemiesInRadius fills a vector) as the new target.
// FUNCTION: 0x4021f0
int __stdcall GuardNoMoveOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 0x10008) {
        order->state = 3;
        return 2;
    }
    switch (order->state) {
    case 0:
        unit->ReleaseWeapons(3);
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 1;
    case 1: {
        order->target.SetUnit(GetWeaponTargetUnit(unit, 0));
        Unit* t = order->target.owner;
        if (t != 0 && (t->flags & 0x10000000)) {
            order->pos = t->pos;
            unit->ClaimWeapons(0);
            SetWeaponTargetUnit(unit, order->target.owner, 0);
            order->wait = 0;
            order->waitLimit = RandomInt(3) + 3;
            return 1;
        }
        ((Class_00439e80*)order)->FUN_00439e80(0x1e);
        return 2;
    }
    case 2:
        if (flags & 0x4000)
            order->wait = 0;
        else
            order->wait++;
        if (order->wait <= order->waitLimit && WeaponCanReachUnit(unit, order->target.owner, 0)) {
            order->flags |= 0x7008;
            return 2;
        }
        if (RandomInt(100) < 0x50) {
            order->wait = 0;
            return 1;
        }
        break;
    case 3: {
        std::vector<Unit*> units;
        GetVisibleEnemiesInRadius(unit->playerIndex, &order->pos, 0x280, 0, &units);
        if (!units.empty()) {
            order->target.SetUnit(units[RandomInt(units.size())]);
            SetWeaponTargetUnit(unit, order->target.owner, 0);
            order->state = 1;
            return 2;
        }
        break;
    }
    default:
        return 7;
    }
    return 0;
}

// 0x402430 stays defined before 0x402640: the constant pool order follows it.
// Keep a header include: without one the register choices change.
#include <stdio.h>

// Order handler "Repairing": the order's target unit (the builder) repairs
// `unit`, spending its worker time and drawing nano particles from its nano
// piece to the unit's bounding box.
// FUNCTION: 0x402430
int __stdcall SelfRepairOrder(Unit* unit, Order* order, int unused)
{
    if (order->target.owner == 0) {
        QueueUnitSpeech(unit, 7, "Repair aborted.");
        return 8;
    }
    switch (order->state) {
    case 0:
        if (!(order->target.owner->def->flags241 & 0x40))
            return 7;
        if (order->target.owner->buildLeft == 0.0f && (unit->flags10e & 1)) {
            unit->ClaimWeapons(3);
            return 1;
        }
        return 8;
    case 1:
        if ((unsigned int)unit->health >= unit->def->maxHealth)
            return 1;
        unit->repairTime = g_game->ticks + 0x96;
        if (FUN_0041bd10(order->target.owner, unit, (float)(order->target.owner->def->workerTime / 30))) {
            Vec_00401c20 nano;
            GetNanoPiecePosition(order->target.owner, &nano);
            Box box;
            box.hi = unit->pos;
            box.lo = unit->pos;
            box.lo.x += unit->def->bounds.lo.x;
            box.lo.z += unit->def->bounds.lo.z;
            box.hi.x += unit->def->bounds.hi.x;
            box.hi.z += unit->def->bounds.hi.z;
            box.hi.y += unit->def->bounds.hi.y;
            EmitNanoParticles(&nano, &box, 6);
        }
        ((Class_00439e80*)order)->FUN_00439e80(1);
        order->flags |= 8;
        return 2;
    case 2:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    default:
        return 7;
    }
}

// World position (16.16 fixed point) to the map cell of the top-left corner
// of a footprint centred there.
static inline Point16 GridCell(Vec_00401c20 pos, Point16 size)
{
    Point16 cell;
    cell.x = (pos.x - (size.x << 19) + 0x80000) >> 20;
    cell.z = (pos.z - (size.z << 19) + 0x80000) >> 20;
    return cell;
}

// Order handler "Nanolathing" of a mobile builder: places the unit to build
// at the script's build piece, then spends worker time on it. When the order
// is cancelled, the metal already spent is refunded (only half or 70% of it
// for AI players on some difficulty settings).
// FUNCTION: 0x402640
int __stdcall BuildingBuildOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 2) {
        if (order->target.owner != 0) {
            float refund = (unsigned int)((1.0f - order->target.owner->buildLeft) * order->target.owner->def->metalCost);
            // The (double) casts on the full refund stay: they keep refund off the FP stack.
            if (unit->player->active && unit->player->type == 2) {
                switch (g_game->difficulty) {
                case 1:
                    unit->resources.metal += refund * 0.7;
                    break;
                case 0:
                    unit->resources.metal += refund * 0.5;
                    break;
                default:
                    unit->resources.metal += (double)refund;
                    break;
                }
            } else {
                unit->resources.metal += (double)refund;
            }
            FinishConstruction(unit, order->target.owner);
            DamageUnit(unit, order->target.owner, 30000, 9, 0);
        }
        unit->SetStateBits(9, 0);
        FUN_0041c150(unit);
        return 5;
    }
    if (flags & 8) {
        QueueUnitSpeech(unit, 7, "Construction stopped");
        order->count--;
        FUN_0041c150(unit);
        return 0;
    }
    switch (order->state) {
    case 0:
        order->target.SetUnit(0);
        if (unit->flags & 0x20000000) {
            if (order->count <= 0) {
                unit->SetStateBits(1, 0);
                return 5;
            }
            unit->SetStateBits(1, 1);
            return 1;
        }
        break;
    case 1:
        return FUN_00438700(unit, order, 2);
    case 2: {
        int piece = -1;
        unit->script->QueryScript("QueryBuildInfo", &piece, 0, 0, 0);
        order->pos = GetPiecePosition(unit, piece);
        UnitDef* ut = &g_game->unitTypes[order->unitType];
        Point16 cell = GridCell(order->pos, ut->footprint);
        if (!FUN_0047db70(ut, 0, cell, unit->flags & 3)) {
            ((Class_00439e80*)order)->FUN_00439e80(15);
            order->flags |= 2;
            return 2;
        }
        order->target.SetUnit(CreateUnit(unit->playerIndex, order->unitType, order->pos, 0, 1, 0));
        if (order->target.owner == 0) {
            QueueUnitSpeech(unit, 7, "Unable to create any more units");
            ((Class_00439e80*)order)->FUN_00439e80(300);
            order->flags |= 2;
            return 2;
        }
        QueueUnitSpeech(unit, 9, "Starting construction");
        AttachUnitToPiece(order->target.owner, unit, piece, 1);
        order->target.owner->bits.bits18 = unit->bits.bits18;
        order->target.owner->bits.bits20 = unit->bits.bits20;
        AddOrder("getbuilt", 1, order->target.owner, unit, 0, 0, 0);
        unit->SetStateBits(8, 1);
        FUN_0041c150(unit);
        return 1;
    }
    case 3:
        if (order->target.owner != 0) {
            if (AddBuildProgress(unit, order->target.owner, (float)(unit->def->workerTime / 30))) {
                Vec_00401c20 nano;
                GetNanoPiecePosition(unit, &nano);
                Box box;
                box.hi = order->target.owner->pos;
                box.lo = order->target.owner->pos;
                box.lo.x += order->target.owner->def->bounds.lo.x;
                box.lo.z += order->target.owner->def->bounds.lo.z;
                box.hi.x += order->target.owner->def->bounds.hi.x;
                box.hi.z += order->target.owner->def->bounds.hi.z;
                box.hi.y += order->target.owner->def->bounds.hi.y;
                EmitNanoParticles(&nano, &box, 6);
            }
            if (order->target.owner->buildLeft != 0.0f) {
                ((Class_00439e80*)order)->FUN_00439e80(1);
                order->flags |= 0xa;
                return 2;
            }
            return 1;
        }
        break;
    case 4:
        QueueUnitSpeech(unit, 8, 0);
        unit->SetStateBits(8, 0);
        FinishConstruction(unit, order->target.owner);
        order->target.SetUnit(0);
        order->count--;
        FUN_0041c150(unit);
        return 0;
    }
    return 7;
}

// Order handler "Nanolathing" of a building that stockpiles weapons (the
// "BuildingBuild" entry of the order table at 0x4fc490): builds `count`
// rounds for weapon `weapon`, 5 ticks of build time per step, paying the
// energy and metal share of each step (energy first, as RequestEnergyAndMetal takes
// them), up to 200 stockpiled rounds.
// FUNCTION: 0x402b70
int __stdcall BuildWeaponOrder(Unit* unit, Order* order, int unused)
{
    // energyCharge's declaration stays at function scope: the frame slots of
    // the locals below follow it.
    int energyCharge;
    WeaponType* t = unit->weapons[order->weapon].type;
    switch (order->state) {
    case 0:
        if (order->count <= 0)
            return 5;
        if (unit->weapons[order->weapon].stockpile >= 200) {
            ((Class_00439e80*)order)->FUN_00439e80(300);
            return 2;
        }
        order->progress = 0;
        return 1;
    case 1: {
        // The float locals declared next, total, prev and the ints after them:
        // fixes the fild order; total stays after the ternary.
        float fnext, ftotal, fprev;
        int prev = order->progress;
        int next = prev + 5 < t->buildTime ? prev + 5 : t->buildTime;
        int total = t->buildTime;
        fnext = next;
        ftotal = total;
        fprev = prev;
        int metalCharge = (int)(fnext * t->metalCost / ftotal) - (int)(fprev * t->metalCost / ftotal);
        energyCharge = (int)(fnext * t->energyCost / ftotal) - (int)(fprev * t->energyCost / ftotal);
        if (unit->resources.RequestEnergyAndMetal(energyCharge, metalCharge)) {
            order->progress = next;
            if (next >= t->buildTime)
                return 1;
            ((Class_00439e80*)order)->FUN_00439e80(5);
            return 2;
        }
        ((Class_00439e80*)order)->FUN_00439e80(10);
        return 2;
    }
    case 2:
        unit->weapons[order->weapon].stockpile++;
        order->count--;
        FUN_0041c150(unit);
        return 0;
    default:
        return 7;
    }
}

// Order handler: waits for the order's time (at most 1800 ticks).
// A char loop counter gives the separate countdown register (ebx = 3).
// FUNCTION: 0x402d10
int __stdcall ParalyzeOrder(Unit* unit, Order* order, int unused)
{
    if (order->ticks == 0) {
        unit->SetStateBits(0x10, 0);
        return 5;
    }
    if (order->ticks > 0x708)
        order->ticks = 0x708;
    unit->ClaimWeapons(3);
    for (char i = 0; i < 3; i++)
        ClearWeaponTarget(unit, i);
    ((Class_004388d0*)order)->FUN_004388d0(0);
    ((Class_00439e80*)order)->FUN_00439e80(order->ticks);
    order->ticks = 0;
    unit->SetStateBits(0x10, 1);
    return 1;
}

// Class_00438760's operator== compares the index bytes, so the loop head is
// `cmp cl, dl` and `order` lands in ebp, `queued` in ebx. Order::Target() is
// a trivial inline accessor: every `order->Target()->X` in the flag-merge
// tail goes through it, and the temporary it adds shifts the rotation of the
// scratch registers back into phase.
// FUNCTION: 0x402da0
int __stdcall GetBuiltOrder(Unit* unit, Order* order, unsigned int flags)
{
    if (unit->buildLeft == 0.0f) {
        FUN_0041c110(unit);
        if (unit->active) {
            int queued = 0;
            if (order->target.owner) {
                Class_00438760 move("QMove");
                Class_00438760 patrol("QPatrol");
                for (Order* node = order->target.owner->orders; node; node = node->next) {
                    Class_00438760 kind;
                    if (node->kind == move)
                        kind = GetOrderType(2, unit, 0, node->Position());
                    else if (node->kind.index == patrol.index)
                        kind = GetOrderType(9, unit, 0, node->Position());
                    if (kind.index) {
                        AddOrder(kind, 1, unit, 0, node->Position(), 0, 0);
                        queued = 1;
                    }
                }
                if (unit->Ready() && order->Target()->Ready()) {
                    unit->fire = order->Target()->fire;
                    unit->move = order->Target()->move;
                    if (unit->owner->active && unit->owner->type == 1)
                        unit->value = order->Target()->value;
                }
            }
            if (!queued)
                AddOrder("PARK", 1, unit, 0, 0, 0, 0);
        }
        return 5;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        unit->ClaimWeapons(3);
        ((Class_00439e80*)order)->FUN_00439e80(300);
        order->Wait();
        return 1;
    case 1:
        ((Class_00439e80*)order)->FUN_00439e80(30);
        order->Wait();
        return 1;
    case 2:
        if (flags & 0x8000) {
            ((Class_00439e80*)order)->FUN_00439e80(30);
        } else if (flags & 1) {
            ((Class_00439e80*)order)->FUN_00439e80(11);
            FUN_0041bcd0(unit, 11);
        }
        order->Wait();
        return 2;
    default:
        return 7;
    }
}

// Order handler: state 0 calls ClaimWeapons(3) on the unit, state 1 waits
// ten ticks.
// FUNCTION: 0x402fc0
int __stdcall BeCarriedOrder(Unit* unit, Order* order, int unused)
{
    if (unit->blocked == 0) {
        return 5;
    }
    switch (order->state) {
    case 0:
        unit->ClaimWeapons(3);
        return 1;
    case 1:
        ((Class_00439e80*)order)->FUN_00439e80(10);
        return 2;
    default:
        return 7;
    }
}

// FUNCTION: 0x403010
int __stdcall ActivateOrder(Unit* param_1, int unused1, int unused2)
{
    if (((Sub_403010*)param_1->def)->flag) {
        param_1->SetStateBits(1, 1);
    }
    return 5;
}

// FUNCTION: 0x403040
int __stdcall DeactivateOrder(Unit* param_1, int unused1, int unused2)
{
    if (((Sub_403010*)param_1->def)->flag) {
        param_1->SetStateBits(1, 0);
    }
    return 5;
}

// FUNCTION: 0x403070
int __stdcall CloakOnOrder(char* param1, int unused1, int unused2)
{
    Target* p = *(Target**)(param1 + 0x92);
    if (p->flag) {
        *(unsigned int*)(param1 + 0x110) |= 0x800;
    }
    return 5;
}

// FUNCTION: 0x4030a0
int __stdcall CloakOffOrder(char* unit, int unused1, int unused2)
{
    Target* p = *(Target**)(unit + 0x92);
    if (p->flag) {
        *(unsigned int*)(unit + 0x110) &= ~0x800;
    }
    return 5;
}

// FUNCTION: 0x4030d0
int __stdcall StandingMoveOrder(Unit* unit, Order* order, int unused)
{
    unit->mode2 = order->mode;
    return 5;
}

// A char loop counter gives the separate countdown register (ebx = 3).
// FUNCTION: 0x403100
int __stdcall StandingFireOrder(Unit* unit, Order* order, int unused)
{
    unit->mode = order->mode;
    if (order->mode == 0 || order->mode == 1) {
        for (char i = 0; i < 3; i++) {
            if (unit->weapons[i].flags & 0x10) {
                ClearWeaponTarget(unit, i);
            }
        }
    }
    return 5;
}

// FUNCTION: 0x403160
int __stdcall QMoveQPatrolOrder(int param_1, void* param_2, int param_3)
{
    ((Class_00439e80*)param_2)->FUN_00439e80(0x3c);
    return 6;
}

// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.
// FUNCTION: 0x403180
void RegisterUnitOrders()
{
    RegisterOrderTypes(DAT_004fc490, 0x17);
}

// Order handler: asks GetOrderType for the next order kind (returned as a
// Class_00438760 by value), passes it on by value (the 0x406240 call site
// builds the same argument in place) and returns state 2.
// FUNCTION: 0x403190
int __stdcall AttackSpecialOrder(Unit* unit, Class_00438b90* order, int unused)
{
    order->FUN_00438b90(GetOrderType(3, unit, order->target, 0));
    order->state = 2;
    return 2;
}

// Order handler: state 0 (only when the unit's +0x86 is clear) resets the
// order and calls FUN_00438930 with its position; state 1 posts message 6
// when bit 0x20 of the third argument is set.
// FUNCTION: 0x4031d0
int __stdcall MoveGroundOrder(Unit* unit, Order* order, int flags)
{
    switch (order->state) {
    case 0:
        if (unit->blocked != 0)
            return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        ((Class_00438930*)order)->FUN_00438930((int*)&order->pos, order->field_36 + 4);
        order->flags = 0xe0;
        return 1;
    case 1:
        if (flags & 0x20) {
            QueueUnitSpeech(unit, 6, 0);
            return 5;
        }
        return 9;
    default:
        return 7;
    }
}

// FUNCTION: 0x403260
int __stdcall AttackKamikazeOrder(Unit* unit, Order* order, unsigned flags)
{
    if(flags&0x10008) return 5;
    if(order->target.owner) order->pos=order->target.owner->pos;
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(unit->blocked) return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        ((Class_00438930*)order)->FUN_00438930((int*)&order->pos,unit->def->radius<16?16:unit->def->radius);
        ((Class_00439e80*)order)->FUN_00439e80(60);
        order->flags|=0xe0; return 1;
    case 1:
        if(flags&0x20) {
            QueueUnitSpeech(unit,6,0);
            AppendOrder(unit,new Class_0043a1f0("SELFDESTRUCT",0,0,1,0,0));
            return 5;
        }
        if(flags&0x40) return 8;
        order->state=0; return 2;
    default: return 7;
    }
}

// FUNCTION: 0x4033a0
int __stdcall PatrolOrder(Unit* unit, Order* order, int flags)
{
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(!unit->active) return 7;
        ((Class_00438880*)order)->FUN_00438880(0);
        FUN_0043a020(unit,order);
        ((Class_00439e80*)order)->FUN_00439e80(1); return 1;
    case 1:
        unit->ReleaseWeapons(3);
        ((Class_00438930*)order)->FUN_00438930((int*)&order->pos,0);
        ((Class_00439e80*)order)->FUN_00439e80(15);
        order->flags|=0xe0; return 1;
    case 2:
        if(flags&0xe0) { order->state=1; return 6; }
        {
            Order* next=FUN_0043b700(unit);
            if(next && FUN_0043b1f0(unit,next,0)) { order->flags=0; order->state=1; return 3; }
        }
        ((Class_00439e80*)order)->FUN_00439e80(RandomInt(30)+30);
        order->state=1; return 4;
    default: return 7;
    }
}
