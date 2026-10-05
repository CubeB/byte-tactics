// Decompiled by Claude Opus 5.5. Names are provisional.
// Two order handlers from the order table at 0x4fc490: "Repairing"
// (0x402430) and the "Nanolathing" handler of mobile builders (0x402640).
//
// 0x402430 is defined in this file, before 0x402640, as in the original
// source file: its 0.0f constant then comes first in the constant pool, so
// 0x402640's 1.0f is followed directly by the -0.7 double, as in the
// original (compiled alone, 1.0f gets four bytes of padding after it).
// Any header include is needed: without one, 0x402430 swaps eax and edx in
// the health compare and 0x402640 gets different register choices.
#include <stdio.h>

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

struct Unit;

class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
};

class Class_00439e80 {
public:
    void FUN_00439e80(int ticks);
};

class CobScript {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

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
    char unknown_200[0x241 - 0x200];
    unsigned char flags;               // +0x241
    char unknown_242[0x249 - 0x242];
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct Unit {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef* type;                     // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* script;                 // +0x9a
    char unknown_9e[0xb0 - 0x9e];
    int repairTime;                    // +0xb0
    char unknown_b4[0xd4 - 0xb4];
    float metal;                       // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player* player;                    // +0xec
    char unknown_f0[0xff - 0xf0];
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float buildLeft;                   // +0x104
    short health;                      // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char flags10e;            // +0x10e
    char unknown_10f[0x110 - 0x10f];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 16;
            unsigned int bits18 : 2;
            unsigned int bits20 : 2;
            unsigned int unknown_22 : 10;
        } bits;
    };
    void ClaimWeapons(int param);
    void SetStateBits(int which, int on);
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0x12 - 0xa];
    Class_004895c0 target;             // +0x12
    Vec3 pos;                          // +0x22
    char unknown_2e[0x36 - 0x2e];
    int unitType;                      // +0x36
    int count;                         // +0x3a
};

struct Game {
    char unknown_0[0x1439b];
    UnitDef* unitTypes;                // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x38a47 - 0x37ef2];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
void __stdcall FUN_0041c150(Unit* unit);
void __stdcall FinishConstruction(Unit* unit, Unit* target);
void __stdcall DamageUnit(Unit* unit, Unit* target, int n, int kind, int flag);
int __stdcall FUN_00438700(Unit* unit, Order* order, int flags);
Vec3 __stdcall GetPiecePosition(Unit* unit, int piece);
int __stdcall FUN_0047db70(UnitDef* type, short a, Point16 cell, int b);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short type, Vec3 pos, int a, int b, int c);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* builder, char piece, char p4);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner, Unit* id, int flags, int param_6, int param_7);
int __stdcall FUN_0041bd10(Unit* builder, Unit* unit, float amount);
int __stdcall AddBuildProgress(Unit* unit, Unit* target, float amount);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitNanoParticles(Vec3* from, Box* to, int count);

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
        if (!(order->target.owner->type->flags & 0x40))
            return 7;
        if (order->target.owner->buildLeft == 0.0f && (unit->flags10e & 1)) {
            ((Unit*)unit)->ClaimWeapons(3);
            return 1;
        }
        return 8;
    case 1:
        if ((unsigned int)unit->health >= unit->type->maxHealth)
            return 1;
        unit->repairTime = g_game->ticks + 0x96;
        if (FUN_0041bd10(order->target.owner, unit, (float)(order->target.owner->type->workerTime / 30))) {
            Vec3 nano;
            GetNanoPiecePosition(order->target.owner, &nano);
            Box box;
            box.hi = unit->pos;
            box.lo = unit->pos;
            box.lo.x += unit->type->bounds.lo.x;
            box.lo.z += unit->type->bounds.lo.z;
            box.hi.x += unit->type->bounds.hi.x;
            box.hi.z += unit->type->bounds.hi.z;
            box.hi.y += unit->type->bounds.hi.y;
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
static inline Point16 GridCell(Vec3 pos, Point16 size)
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
// The (double) casts on the full refund emit nothing; without them MSVC keeps
// `refund` on the FP stack in that path (fld; fadd st, st(1); fstp; fstp st(0))
// instead of adding it straight to the field.
// FUNCTION: 0x402640
int __stdcall BuildingBuildOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 2) {
        if (order->target.owner != 0) {
            float refund = (unsigned int)((1.0f - order->target.owner->buildLeft) * order->target.owner->type->metalCost);
            if (unit->player->active && unit->player->type == 2) {
                switch (g_game->difficulty) {
                case 1:
                    unit->metal += refund * 0.7;
                    break;
                case 0:
                    unit->metal += refund * 0.5;
                    break;
                default:
                    unit->metal += (double)refund;
                    break;
                }
            } else {
                unit->metal += (double)refund;
            }
            FinishConstruction(unit, order->target.owner);
            DamageUnit(unit, order->target.owner, 30000, 9, 0);
        }
        ((Unit*)unit)->SetStateBits(9, 0);
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
                ((Unit*)unit)->SetStateBits(1, 0);
                return 5;
            }
            ((Unit*)unit)->SetStateBits(1, 1);
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
        ((Unit*)unit)->SetStateBits(8, 1);
        FUN_0041c150(unit);
        return 1;
    }
    case 3:
        if (order->target.owner != 0) {
            if (AddBuildProgress(unit, order->target.owner, (float)(unit->type->workerTime / 30))) {
                Vec3 nano;
                GetNanoPiecePosition(unit, &nano);
                Box box;
                box.hi = order->target.owner->pos;
                box.lo = order->target.owner->pos;
                box.lo.x += order->target.owner->type->bounds.lo.x;
                box.lo.z += order->target.owner->type->bounds.lo.z;
                box.hi.x += order->target.owner->type->bounds.hi.x;
                box.hi.z += order->target.owner->type->bounds.hi.z;
                box.hi.y += order->target.owner->type->bounds.hi.y;
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
        ((Unit*)unit)->SetStateBits(8, 0);
        FinishConstruction(unit, order->target.owner);
        order->target.SetUnit(0);
        order->count--;
        FUN_0041c150(unit);
        return 0;
    }
    return 7;
}
