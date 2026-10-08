// Decompiled by Claude Opus 5.5, GPT-6, GPT-6 Astra, GPT-6.1-sol, Opus, Sonnet,
// Claude Sonnet 5.5, claude-sonnet-5-5, space-bunny-free, DeepSeek V4.1 Flash,
// deepseek-v4.1-flash, deepseek-v4.1 and Haiku. Names are provisional.
// The ground order handlers the table at 0x4fc490 names, from Stop (0x401c20)
// to the std::vector<Unit*> destroy member at 0x406c00: the unit's order state
// machine, the mobile builders, capture, reclaim, resurrect, repair, standby,
// park, follow, transport and teleport handlers, with the order tables'
// registrations at the end. The unit's order object (Order) carries the state
// machine, the target and the position; the unit (Unit) and its type (UnitDef)
// carry the fields the handlers read and write.
// The handlers 0x401e00, 0x403a20, 0x403f70, 0x404ad0, 0x404db0, 0x405980,
// 0x406300 and 0x406aa0 stay in files of their own: their register allocation
// follows symbol ids this file's context cannot give them (0x401e00's address
// mode at the allied lookup flips when the merged views share one file). The
// vector members 0x406c10, 0x406c40 and 0x406c70 stay in theirs: the inline
// _Construct below makes the repair patrol call 0x406c70, which would then
// also change their own bytes, and 0x406c70 itself would be inlined into the
// patrol.

struct Unit;

void __stdcall CopyDwordIfNonNull(Unit** dest, Unit* const* src);
// The repair patrol's push_back inlines vector::insert, whose _Construct this
// overload replaces; it must be declared before <vector>.
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { CopyDwordIfNonNull(dest, &src); }
}

#include <vector>

struct Vec3 {
    union { int x; struct { unsigned short xf; short xh; }; };
    union { int y; struct { unsigned short yf; short yh; }; };
    union { int z; struct { unsigned short zf; short zh; }; };
    Vec3 operator+(const Vec3& other) const {
        Vec3 r; r.x = x + other.x; r.y = y + other.y; r.z = z + other.z; return r;
    }
    Vec3 operator-(const Vec3& other) const {
        Vec3 r; r.z = z - other.z; r.y = y - other.y; r.x = x - other.x; return r;
    }
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
};

struct Point16 {
    short x;
    short z;
};

struct Box {
    Vec3 lo;
    Vec3 hi;
};

struct UnitDef;

class CobScript {
public:
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
    int StartScriptWithArgs(char* name, void* param_2, int a, int b, int c, int d, int e, int f);
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
    void SetDeadlineTicks(int ticks);
};

class Class_00438880 {
public:
    void AnnounceStatusIfFlagged(const char* text);
};

class Class_00438930 {
public:
    void AttachApproachRadiusGoal(Vec3* pos, int param);
};

class Class_004388d0 {
public:
    void SetAttachedFx(int param);
};

class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
    Unit* Get() { return owner; }
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec3* pos, int c, int d, int e);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x14a];
    Point16 footprint;                 // +0x14a
    char unknown_14e[0x15e - 0x14e];
    Box bounds;                        // +0x15e
    char unknown_176[0x180 - 0x176];
    short height180;                   // +0x180
    char unknown_182[0x184 - 0x182];
    short radius184;                   // +0x184
    float energy;                      // +0x186
    float metal;                       // +0x18a
    char unknown_18e[0x1c0 - 0x18e];
    short height;                      // +0x1c0
    char unknown_1c2[0x1ea - 0x1c2];
    int buildTime;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    unsigned int maxHealth;            // +0x1fa
    unsigned short workerTime;         // +0x1fe
    char unknown_200[0x202 - 0x200];
    short range;                       // +0x202
    char unknown_204[0x212 - 0x204];
    unsigned short buildRange;         // +0x212
    char unknown_214[0x218 - 0x214];
    unsigned short radius;             // +0x218
    char unknown_21a[0x22a - 0x21a];
    unsigned char capacity;            // +0x22a
    char unknown_22b[0x231 - 0x22b];
    unsigned int* weaponCategories[3]; // +0x231
    unsigned int* categories;          // +0x23d
    union {
        unsigned int flags;            // +0x241
        unsigned char flags241;        // +0x241, the handlers that test a byte
        struct {
            unsigned int low : 11;
            unsigned int flying : 1;
            unsigned int high : 20;
        };
    };
    union {
        unsigned char flags245;        // +0x245, AttackUTypeOrder's byte test
        unsigned int flags245u;        // +0x245, the handlers that test a dword
        struct {
            unsigned int low : 20;     // +0x245, SelfDestructOrder's countdown
            unsigned int countdown : 3;
            unsigned int high : 9;
        };
        struct {
            unsigned int lo : 12;
            unsigned int capture : 1;
            unsigned int hi : 19;
        };
    };
    // The float casts stay: without them the 30 folds into the reciprocals.
    float EnergyCost() { return (float)(energy * 30); }
    float MetalCost() { return (float)(metal * 30); }
    // maxHealth is read once through MaxHealth() and once as a plain field:
    // two identical reads merge into one load.
    unsigned int MaxHealth() { return maxHealth; }
};

struct Player {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
    char unknown_9c[0xa4 - 0x9c];
    float energyCapacity;              // +0xa4
    float metalCapacity;               // +0xa8
    char unknown_ac[0x108 - 0xac];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct WeaponDef {
    char unknown_0[0xc0];
    float energyCost;                  // +0xc0 (TDF energypershot)
    float metalCost;                   // +0xc4 (TDF metalpershot)
    char unknown_c8[0xe4 - 0xc8];
    unsigned short buildTime;          // +0xe4
    char unknown_e6[0x111 - 0xe6];
    unsigned int flags;                // +0x111
};

struct Weapon {
    WeaponDef* type;                   // +0x0
    char unknown_4[0x8 - 0x4];
    WeaponDef* def;                    // +0x8, at +0x10 for weapon[0]
    char unknown_c[0xe - 0xc];
    unsigned char stockpile;           // +0xe, at +0x1e for weapon[0]
    unsigned char flags;               // +0xf, at +0x1f for weapon[0]
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

    void MergeFlagsFromTable(Class_00438760 kind);
};

// A 3-short rotation (the unit's +0x64).
struct Rot16 {
    short x, y, z;
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
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    Class_004895c0 target;             // +0x12, its owner is at +0x16
    Vec3 pos;                          // +0x22
    Point16 start;                     // +0x2e
    char unknown_32[0x36 - 0x32];
    union {
        int wait;                      // +0x36
        int id;
        int weapon;
        int type;
        int elapsed;
        int unitType;
        int ticks;
        int mode;
        int done;
        int attempts;
        int time;
        int guardRange;
        int field_36;
    };
    union {
        int radius;                    // +0x3a
        int step;
        int waitLimit;
        int count;
        int unused;
        int duration;
        int countdown;
    };
    union {
        int progress;                  // +0x3e
        int range;
        int retries;
        int capabilities;
    };
    char unknown_42[0x4a - 0x42];
    Order* next;                       // +0x4a

    Unit* Target() { return target.owner; }
    void Wait() { waiting = 1; }
    Vec3* Position() { return &pos; }
    int Advance(int distance);
};

struct Unit {
    int active;                        // +0x0
    char unknown_4[0x10 - 0x4];
    // Two views of +0x10: the three weapons, or the order list at +0x5c.
    union {
        Weapon weapons[3];             // +0x10
        struct {
            char unknown_10[0x5c - 0x10];
            Order* order;              // +0x5c
        };
    };
    Rot16 rot;                         // +0x64
    Vec3 pos;                          // +0x6a
    Point16 cell;                      // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point16 footprint;                 // +0x7e
    char unknown_82[0x86 - 0x82];
    Unit* transport;                   // +0x86
    Unit* cargo;                       // +0x8a
    char unknown_8e[0x92 - 0x8e];
    UnitDef* def;                      // +0x92
    Player* owner;                     // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short category;           // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0xac - 0xaa];
    int value;                         // +0xac
    int workTime;                      // +0xb0
    char unknown_b4[0xb8 - 0xb4];
    unsigned short experience;         // +0xb8
    char unknown_ba[0xbc - 0xba];
    UnitResources resources;           // +0xbc
    char unknown_e4[0xec - 0xe4];
    Player* player;                    // +0xec
    Unit* attacker;                    // +0xf0
    unsigned char orderPlayer;         // +0xf4
    unsigned char orderKind;           // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float progress;                    // +0x104
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
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void SetStateBits(int which, int on);
    int CanReclaim(Unit* unit);
    int Ready() { return (flags & 0x10000000) && !(flags & 0x4000); }
};
#pragma pack(pop)

inline int Order::Advance(int distance) {
    ((Class_00438930*)this)->AttachApproachRadiusGoal(&target.owner->pos, distance);
    step++;
    return 1;
}

extern Game* g_game;

void __stdcall RegisterOrderTypes(void* table, int id);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
extern char DAT_004fc490[];

void __stdcall ClearWeaponTarget(Unit* unit, int weapon);
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);
int __stdcall RandomInt(int range);
void __stdcall GetVisibleEnemiesInRadius(int player, Vec3* pos, int radius, int flags,
                                         std::vector<Unit*>* out);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit, Unit* target, void* flags);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, const char* text);
void __stdcall DamageUnit(Unit* unit, Unit* target, int n, int kind, int flag);
void __stdcall SetWeaponTargetUnit(Unit* unit, Unit* target, int weapon);
Unit* __stdcall GetWeaponTargetUnit(Unit* unit, int index);
int __stdcall WeaponCanReachUnit(Unit* unit, Unit* target, int param_3);
void __stdcall UpdateBuildMenuIfFocusUnit(Unit* unit);
void __stdcall FinishConstruction(Unit* unit, Unit* target);
int __stdcall WaitIfNotInBuildStance(Unit* unit, Order* order, int flags);
Vec3 __stdcall GetPiecePosition(Unit* unit, int piece);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, short a, Point16 cell, int b);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short type, Vec3 pos,
                           int a, int b, int c);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* builder, char piece, char p4);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner, Unit* id, void* pos,
                        int param_6, int param_7);
int __stdcall AddRepairProgress(Unit* builder, Unit* unit, float amount);
int __stdcall AddBuildProgress(Unit* unit, Unit* target, float amount);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitNanoParticles(Vec3* from, Box* to, int count);
void __stdcall MarkSelectionOrdersDirty(Unit* unit);
void __stdcall ApplyUnfinishedBuildDecay(Unit* unit, int param);
void __stdcall EnsurePatrolReturnOrder(Unit* unit, Order* order);
Unit* __stdcall FindBestTargetIfFireAtWill(Unit* unit);
int __stdcall IssueAttackOrder(Unit* unit, Order* order, int param);

// Handler of the "Stopping" entry in the order table at 0x4fc490: stops the
// unit and, for a VTOL that is flying, queues a VTOL_LANDIFCAN order.
// FUNCTION: 0x401c20
int __stdcall StopOrder(Unit* unit, Class_00438880* order, int unused)
{
    order->AnnounceStatusIfFlagged(0);
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
        ((Class_00439e80*)order)->SetDeadlineTicks(delay);
        return 2;
    }
    unsigned state=0;
    state=order->state;
    switch(state) {
    case 0: ((Class_00439e80*)order)->SetDeadlineTicks(order->wait); return 1;
    case 1: return 5;
    default: return 7;
    }
}


// 0x401e00 stays in its own file; the declaration keeps the symbol ids the handlers after it match at.
struct PadE_00401e00_0 { int field; };

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
                    ((Class_00439e80*)order)->SetDeadlineTicks(30);
                    order->flags|=2; return 1;
                }
                if(count==0) {
                    ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(15));
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
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
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
        ((Class_00439e80*)order)->SetDeadlineTicks(0x1e);
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
        ((Class_00439e80*)order)->SetDeadlineTicks(0x1e);
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
        if (order->target.owner->progress == 0.0f && (unit->flags10e & 1)) {
            unit->ClaimWeapons(3);
            return 1;
        }
        return 8;
    case 1:
        if ((unsigned int)unit->health >= unit->def->maxHealth)
            return 1;
        unit->workTime = g_game->ticks + 0x96;
        if (AddRepairProgress(order->target.owner, unit, (float)(order->target.owner->def->workerTime / 30))) {
            Vec3 nano;
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
        ((Class_00439e80*)order)->SetDeadlineTicks(1);
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
// FUNCTION: 0x402640
int __stdcall BuildingBuildOrder(Unit* unit, Order* order, int flags)
{
    if (flags & 2) {
        if (order->target.owner != 0) {
            float refund = (unsigned int)((1.0f - order->target.owner->progress) * order->target.owner->def->metal);
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
        UpdateBuildMenuIfFocusUnit(unit);
        return 5;
    }
    if (flags & 8) {
        QueueUnitSpeech(unit, 7, "Construction stopped");
        order->count--;
        UpdateBuildMenuIfFocusUnit(unit);
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
        return WaitIfNotInBuildStance(unit, order, 2);
    case 2: {
        int piece = -1;
        unit->script->QueryScript("QueryBuildInfo", &piece, 0, 0, 0);
        order->pos = GetPiecePosition(unit, piece);
        UnitDef* ut = &g_game->unitTypes[order->unitType];
        Point16 cell = GridCell(order->pos, ut->footprint);
        if (!CanPlaceUnitFootprint(ut, 0, cell, unit->flags & 3)) {
            ((Class_00439e80*)order)->SetDeadlineTicks(15);
            order->flags |= 2;
            return 2;
        }
        order->target.SetUnit(CreateUnit(unit->playerIndex, order->unitType, order->pos, 0, 1, 0));
        if (order->target.owner == 0) {
            QueueUnitSpeech(unit, 7, "Unable to create any more units");
            ((Class_00439e80*)order)->SetDeadlineTicks(300);
            order->flags |= 2;
            return 2;
        }
        QueueUnitSpeech(unit, 9, "Starting construction");
        AttachUnitToPiece(order->target.owner, unit, piece, 1);
        order->target.owner->bits.bits18 = unit->bits.bits18;
        order->target.owner->bits.bits20 = unit->bits.bits20;
        AddOrder("getbuilt", 1, order->target.owner, unit, 0, 0, 0);
        unit->SetStateBits(8, 1);
        UpdateBuildMenuIfFocusUnit(unit);
        return 1;
    }
    case 3:
        if (order->target.owner != 0) {
            if (AddBuildProgress(unit, order->target.owner, (float)(unit->def->workerTime / 30))) {
                Vec3 nano;
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
            if (order->target.owner->progress != 0.0f) {
                ((Class_00439e80*)order)->SetDeadlineTicks(1);
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
        UpdateBuildMenuIfFocusUnit(unit);
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
    WeaponDef* t = unit->weapons[order->weapon].type;
    switch (order->state) {
    case 0:
        if (order->count <= 0)
            return 5;
        if (unit->weapons[order->weapon].stockpile >= 200) {
            ((Class_00439e80*)order)->SetDeadlineTicks(300);
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
            ((Class_00439e80*)order)->SetDeadlineTicks(5);
            return 2;
        }
        ((Class_00439e80*)order)->SetDeadlineTicks(10);
        return 2;
    }
    case 2:
        unit->weapons[order->weapon].stockpile++;
        order->count--;
        UpdateBuildMenuIfFocusUnit(unit);
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
    ((Class_004388d0*)order)->SetAttachedFx(0);
    ((Class_00439e80*)order)->SetDeadlineTicks(order->ticks);
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
    if (unit->progress == 0.0f) {
        MarkSelectionOrdersDirty(unit);
        if (unit->active) {
            int queued = 0;
            if (order->target.owner) {
                Class_00438760 move("QMove");
                Class_00438760 patrol("QPatrol");
                for (Order* node = order->target.owner->order; node; node = node->next) {
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
        ((Class_00439e80*)order)->SetDeadlineTicks(300);
        order->Wait();
        return 1;
    case 1:
        ((Class_00439e80*)order)->SetDeadlineTicks(30);
        order->Wait();
        return 1;
    case 2:
        if (flags & 0x8000) {
            ((Class_00439e80*)order)->SetDeadlineTicks(30);
        } else if (flags & 1) {
            ((Class_00439e80*)order)->SetDeadlineTicks(11);
            ApplyUnfinishedBuildDecay(unit, 11);
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
    if (unit->transport == 0) {
        return 5;
    }
    switch (order->state) {
    case 0:
        unit->ClaimWeapons(3);
        return 1;
    case 1:
        ((Class_00439e80*)order)->SetDeadlineTicks(10);
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
    ((Class_00439e80*)param_2)->SetDeadlineTicks(0x3c);
    return 6;
}

// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.
// FUNCTION: 0x403180
void RegisterUnitOrders()
{
    // The name (the definition below spells it too) is what tools/progress.py
    // learns the table's address from.
    extern char g_unitOrders[];
    RegisterOrderTypes(g_unitOrders, 0x17);
}

// Order handler: asks GetOrderType for the next order kind (returned as a
// Class_00438760 by value), passes it on by value (the 0x406240 call site
// builds the same argument in place) and returns state 2.
// FUNCTION: 0x403190
int __stdcall AttackSpecialOrder(Unit* unit, Class_00438b90* order, int unused)
{
    order->MergeFlagsFromTable(GetOrderType(3, unit, order->target, 0));
    order->state = 2;
    return 2;
}

// Order handler: state 0 (only when the unit's +0x86 is clear) resets the
// order and calls AttachApproachRadiusGoal with its position; state 1 posts message 6
// when bit 0x20 of the third argument is set.
// FUNCTION: 0x4031d0
int __stdcall MoveGroundOrder(Unit* unit, Order* order, int flags)
{
    switch (order->state) {
    case 0:
        if (unit->transport != 0)
            return 7;
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos, order->field_36 + 4);
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
        if(unit->transport) return 7;
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos,unit->def->radius<16?16:unit->def->radius);
        ((Class_00439e80*)order)->SetDeadlineTicks(60);
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
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        EnsurePatrolReturnOrder(unit,order);
        ((Class_00439e80*)order)->SetDeadlineTicks(1); return 1;
    case 1:
        unit->ReleaseWeapons(3);
        ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos,0);
        ((Class_00439e80*)order)->SetDeadlineTicks(15);
        order->flags|=0xe0; return 1;
    case 2:
        if(flags&0xe0) { order->state=1; return 6; }
        {
            Order* next=(Order*)FindBestTargetIfFireAtWill(unit);
            if(next && IssueAttackOrder(unit,next,0)) { order->flags=0; order->state=1; return 3; }
        }
        ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(30)+30);
        order->state=1; return 4;
    default: return 7;
    }
}

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
struct Pad_00400000_0 { int field; };
struct Pad_00400000_1 { int field; };
struct Pad_00400000_2 { int field; };
struct Pad_00400000_3 { int field; };
struct Pad_00400000_4 { int field; };
struct Pad_00400000_5 { int field; };
struct Pad_00400000_6 { int field; };
struct Pad_00400000_7 { int field; };
struct Pad_00400000_8 { int field; };
struct Pad_00400000_9 { int field; };
struct Pad_00400000_10 { int field; };
struct Pad_00400000_11 { int field; };
struct Pad_00400000_12 { int field; };
struct Pad_00400000_13 { int field; };
struct Pad_00400000_14 { int field; };
struct Pad_00400000_15 { int field; };
struct Pad_00400000_16 { int field; };
struct Pad_00400000_17 { int field; };
struct Pad_00400000_18 { int field; };
struct Pad_00400000_19 { int field; };
struct Pad_00400000_20 { int field; };
struct Pad_00400000_21 { int field; };
struct Pad_00400000_22 { int field; };
struct Pad_00400000_23 { int field; };
struct Pad_00400000_24 { int field; };
struct Pad_00400000_25 { int field; };
struct Pad_00400000_26 { int field; };
struct Pad_00400000_27 { int field; };
struct Pad_00400000_28 { int field; };
struct Pad_00400000_29 { int field; };
struct Pad_00400000_30 { int field; };
struct Pad_00400000_31 { int field; };
struct Pad_00400000_32 { int field; };
struct Pad_00400000_33 { int field; };
struct Pad_00400000_34 { int field; };

extern int pad_00400000_0;
extern int pad_00400000_1;
extern int pad_00400000_2;

char* __stdcall lstrcpynA(char* dest, const char* src, int count);
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

class Class_00438a00 {
public:
    void AttachRingApproachGoal(Vec3* pos, int param, int param_3);
};

class Class_00438ad0 {
public:
    void AttachBuildFootprintMarker(Point16 cell, Point16 size);
};

class Class_004897e0 {
public:
    unsigned char ChooseWeapon();
};

struct Feature {
    char name[0x94];                   // +0x0
    Point16 footprint;                 // +0x94
    char unknown_98[0xec - 0x98];
    float metal;                       // +0xec
    float energy;                      // +0xf0
    char unknown_f4[0xfa - 0xf4];
    unsigned char height;              // +0xfa
    char unknown_fb[0xfe - 0xfb];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct FeatureSpot {
    char unknown_0[0x20];
    Rot16 rot;                         // +0x20
    char unknown_26[0x30 - 0x26];
};

struct Cell {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    char unknown_c;
};

class Mission {
public:
    int GetGameType();
};

struct Packet {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};

class Class_00405d90 {
public:
    Player* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_00405d90(Player* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void CollectDamagedAlly(Unit*);
};

// The original keeps part 2's 0.0f at 0x4fc920, after part 1's at 0x4fc6d0;
// the named constant keeps the two apart (a shared literal would sit at one).
static const float Zero_004fc920 = 0.0f;

// Unused here: the symbol ids this declaration takes keeps the allocation (docs/c2-regalloc.md).
extern char DAT_004fc6e8[];
extern const float DAT_004fc930, DAT_004fc934, DAT_004fc938, DAT_004fc93c;

unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size);
int __stdcall GetGroundHeight(Vec3* pos);
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
void __stdcall StartBuildingScript(Unit* unit, Order* order, short turn);
void __stdcall StopBuildingScript(Unit* unit, Order* order);
int __stdcall WaitIfCobBusy(Unit* unit, Order* order, int flags);
int __stdcall ComputeReclaimDamagePulse(Unit* unit, Unit* target, int flags);
int __stdcall IssueRepairOrder(Unit* unit, Unit* target, int param);
void __stdcall EmitNanoParticles(Vec3* from, Vec3* to, int count);
void __stdcall EmitReverseNanoParticles(Box* from, Vec3* to, int count);
void __stdcall EmitReverseNanoParticles(Vec3* from, Vec3* to, int count);
void __stdcall SnapWorldPosToFootprint(UnitDef* type, Vec3* pos);
void __stdcall GiveUnitToPlayer(Unit* unit, void* player, int param);
void __stdcall SetWeaponTargetPos(Unit* unit, Vec3* pos, int weapon);
void __stdcall ReclaimFeature(Unit* unit, Vec3* pos);
unsigned short __stdcall FindUnitTypeId(char* name);
Cell* __stdcall GetOriginCellAtPosition(Vec3* pos);
unsigned short __stdcall GetCellFeature(Cell* cell);
Cell* __stdcall GetMapCellAtPosition(Vec3* pos);
void __stdcall RemoveFeature(void* target, int flag);
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall VisitObjectsInRange(Vec3* pos, int range, const Class_00405d90& visitor);
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3* pos, int range, Vec3** energy, float* energyAmount,
                           Vec3** metal, float* metalAmount);
int __stdcall GetWeaponRange(Unit* unit, int weapon);
int __stdcall GetWeaponRange(Unit* unit, unsigned char weapon);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);
void __stdcall EmitTeleportParticles(Vec3* from, Vec3* to, int count, int param);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int param);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
void __stdcall AlignUnitToGround(Unit*);
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall DetonateUnitWeapon(Unit*, int);


// Order handler: attack-move style state machine. The inner step switch
// advances order->step after cases 6 and 7 in one `order->step++` after the
// switch; cases 0 and 5 go through the inline Advance(), so MSVC merges only
// those two (at the pushed distance) and keeps case 6 separate.
// FUNCTION: 0x4034a0
int __stdcall AttackChaseOrder(Unit* unit, Order* order, unsigned int flags)
{
    int weapon = order->weapon;
    if ((flags & 0x800) || !order->target.owner || (flags & 0x10008)) return 5;
    if (order->range && (int)_hypot(unit->pos.xh - order->start.x,
                                    unit->pos.zh - order->start.z) >= order->range)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (!unit->active || unit->def->flying || !(unit->flags & 0x80000000)) break;
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        order->pos = unit->pos;
        order->step = 0;
        if (!weapon) order->weapon = ((Class_004897e0*)unit)->ChooseWeapon();
        return 1;
    case 1:
        ((Class_004388d0*)order)->SetAttachedFx(0);
        if (flags & 0x3000) return 1;
        if (!WeaponCanReachUnit(unit, order->target.owner, weapon)) return 1;
        unit->ClaimWeapons(0);
        unit->ClaimWeapons(2);
        SetWeaponTargetUnit(unit, order->target.owner, weapon);
        order->flags = 0x13808;
        return 2;
    case 2:
        weapon = GetWeaponRange(unit, weapon);
        switch (order->step) {
        case 0:
            return order->Advance(weapon);
        case 1: case 2: case 3: case 4:
            if (abs(unit->pos.y - order->target.owner->pos.y) > 0x80000) {
                ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->target.owner->pos, weapon / 2);
                order->step = 6;
                return 1;
            } else {
                int angle = GetHeadingBetween(&order->target.owner->pos, &unit->pos);
                angle += RandomInt(0x8000) - 0x4000;
                int distance = weapon << 16;
                int dx = -FUN_004b70ef(angle, distance);
                int dz = -FUN_004b7123(angle, distance);
                Vec3* target = &order->target.owner->pos;
                Vec3 pos = *target + Vec3(dx, 0, dz);
                ((Class_00438930*)order)->AttachApproachRadiusGoal(&pos, weapon / 4);
                return 1;
            }
        case 5:
            return order->Advance(weapon / 2);
        case 6:
            ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->target.owner->pos, 0);
            break;
        case 7:
            ((Class_00438a00*)order)->AttachRingApproachGoal(&order->target.owner->pos, weapon, weapon / 2);
            break;
        case 8:
            ((Class_00438a00*)order)->AttachRingApproachGoal(&order->target.owner->pos, weapon * 2, weapon);
            order->step = 0;
            return 1;
        default: return 7;
        }
        order->step++;
        return 1;
    case 3:
        if (flags & 0x40e0) { order->state = 1; return 4; }
        if (WeaponCanReachUnit(unit, order->target.owner, weapon)) {
            unit->ClaimWeapons(0);
            unit->ClaimWeapons(2);
            SetWeaponTargetUnit(unit, order->target.owner, weapon);
            order->flags = 0x148e8;
            ((Class_00439e80*)order)->SetDeadlineTicks(30);
            return 2;
        }
        unit->ReleaseWeapons(3);
        order->flags = 0x100e8;
        ((Class_00439e80*)order)->SetDeadlineTicks(30);
        return 2;
    }
    return 7;
}

// FUNCTION: 0x4038a0
int __stdcall SuppressOrder(Unit* unit,Order* order,unsigned flags)
{
    if(flags&0x800) return 5;
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(unit->def->flying) return 8;
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        order->radius=GetWeaponRange(unit,(unsigned char)order->weapon); return 1;
    case 1:
        if(order->weapon==2) {
            unit->ClaimWeapons(3);
            SetWeaponTargetPos(unit,&order->pos,2);
            order->flags=0x1c00; return 1;
        }
        unit->ClaimWeapons(0);
        unit->ClaimWeapons(1);
        SetWeaponTargetPos(unit,&order->pos,0);
        SetWeaponTargetPos(unit,&order->pos,1);
        order->flags=0x1c00; return 1;
    case 2:
        unit->ReleaseWeapons(3);
        if(flags&0x400) { order->state=1; return 6; }
        if(unit->active) {
            if(order->radius<=0) return 9;
            ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos,order->radius);
            order->flags=0xe0;
            order->radius-=RandomInt(GetWeaponRange(unit,(unsigned char)order->weapon)/3);
            order->state=1; return 4;
        }
        return 9;
    default: return 7;
    }
}

// Unused here (0x403a20 stays in its own file): the symbol ids these helpers
// take keep the allocation (docs/c2-regalloc.md).
static inline UnitDef* Definitions() { return g_game->unitTypes; }

static inline Point16 WorldToCell(Vec3 v, Point16 footprint)
{
    Point16 c;
    c.x = (v.x - (footprint.x << 19) + 0x80000) >> 20;
    c.z = (v.z - (footprint.z << 19) + 0x80000) >> 20;
    return c;
}

static inline void CellToWorld(Point16 footprint, Point16 c, Vec3* v)
{
    v->x = (footprint.x + c.x * 2) << 19;
    v->z = (footprint.z + c.z * 2) << 19;
}




// The float constants of the handlers that stay in their own files must come
// first in this object's constant pool, as in the original translation unit
// (the copies of 0x403a20 and 0x403f70 in 0x404270's original file).
int __stdcall PadConstants_403a20(double x)
{
    if (x == 8.0) return 1;
    if (x == -8.0) return 2;
    if (x == 16.0) return 3;
    if (x == 0.0) return 4;
    return 5;
}

// Capture order handler, a state machine on order->state.
// FUNCTION: 0x404270
int __stdcall CaptureOrder(Unit* unit, Order* order, unsigned int flags)
{
    Unit* target = order->target.Get();
    if (!target || (flags & 0x10008)) {
        QueueUnitSpeech(unit, 7, "Capture failed");
        return 8;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0: {
        if (!unit->active) return 7;
        if (!(unit->def->flags245u & 0x1000)) return 7;
        if (target->def->capture) {
            QueueUnitSpeech(unit, 7, "That unit cannot be captured");
            return 8;
        }
        if (target->progress != Zero_004fc920) {
            QueueUnitSpeech(unit, 7, "That unit is a cloud of vapor and cannot be captured");
            return 8;
        }
        ((Class_00438880*)order)->AnnounceStatusIfFlagged("Capturing");
        order->duration = (int)(order->target.Get()->def->EnergyCost() / 2000 + order->target.Get()->def->MetalCost() / 140 + 150);
        order->duration = order->duration < 1800 ? order->duration : 1800;
        order->duration = (order->target.Get()->health + order->target.Get()->def->MaxHealth()) * order->duration / (order->target.Get()->def->maxHealth * 2);
        int experience = 0;
        experience = order->target.Get()->experience;
        order->duration = ((experience / 5 + 10) * order->duration * 10) / 100;
        unit->ClaimWeapons(3);
        ((Class_00438ad0*)order)->AttachBuildFootprintMarker(order->target.Get()->cell, order->target.Get()->footprint);
        order->flags = 0x100e8;
        return 1;
    }
    case 1: {
        if (flags & 0x40) return 8;
        Vec3* position = &unit->pos;
        Fixed distance;
        distance.value = (int)_hypot(unit->pos.x - target->pos.x, unit->pos.z - target->pos.z);
        int gap = distance.whole - (int)(_hypot(unit->footprint.x, unit->footprint.z) * 8.0);
        gap += (int)(_hypot(order->target.Get()->footprint.x, order->target.Get()->footprint.z) * -8.0);
        unsigned int range = 0;
        range = unit->def->buildRange;
        if (gap > (int)range) return 0;
        StartBuildingScript(unit, order, GetHeadingBetween(position, &order->target.Get()->pos) - unit->rot.y);
        return 1;
    }
    case 2:
        return WaitIfNotInBuildStance(unit, order, 0x10008);
    case 3:
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    case 4: {
        if (target->active && (target->flags & 0xc)) {
            StopBuildingScript(unit, order);
            ((Class_00439e80*)order)->SetDeadlineTicks(30);
            return 0;
        }
        if (order->elapsed >= order->duration) return 1;
        Vec3 start;
        GetNanoPiecePosition(order->source, &start);
        Vec3 bounds[2];
        bounds[1] = order->target.Get()->pos;
        bounds[0] = order->target.Get()->pos;
        bounds[0].x += order->target.Get()->def->bounds.lo.x;
        bounds[0].z += order->target.Get()->def->bounds.lo.z;
        bounds[1].x += order->target.Get()->def->bounds.hi.x;
        bounds[1].z += order->target.Get()->def->bounds.hi.z;
        bounds[1].y += order->target.Get()->def->bounds.hi.y;
        EmitReverseNanoParticles(bounds, &start, 6);
        unit->workTime = g_game->ticks + 900;
        order->elapsed += 2;
        ((Class_00439e80*)order)->SetDeadlineTicks(2);
        return 2;
    }
    case 5:
        GiveUnitToPlayer(target, unit->owner, 0);
        QueueUnitSpeech(unit, 16, 0);
        return 5;
    }
    return 7;
}

static inline int SquaredDistance(int dx, int dz)
{
    __int64 x = dx;
    __int64 z = dz;
    return (int)((x * x) >> 32) + (int)((z * z) >> 32);
}


// The same for 0x404ad0's -0.5f, which follows 0x404270's constants.
float PadConstant_404ad0(float x)
{
    return x * -0.5f;
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
        if (unit->active && (unit->def->flags245u & 0x400)) {
            if (!unit->CanReclaim(target)) {
                QueueUnitSpeech(unit, 7, "That unit cannot be reclaimed");
                QueueUnitSpeech(unit, 7, "Reclamation failed");
                return 8;
            }
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Reclaiming");
            unit->ClaimWeapons(3);
            return 1;
        }
        QueueUnitSpeech(unit, 7, "Reclamation failed");
        return 7;
    case 1:
        if (flags & 0x20) return 1;
        ((Class_00438ad0*)order)->AttachBuildFootprintMarker(target->cell, target->footprint);
        order->flags |= 0x100e8;
        ((Class_00439e80*)order)->SetDeadlineTicks(15);
        order->elapsed = ComputeReclaimDamagePulse(unit, order->target.Get(), 15);
        order->duration = 0;
        return 2;
    case 2:
        if (flags & 0x40) return 9;
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &target->pos) - unit->rot.y);
        return 1;
    case 3:
        return WaitIfNotInBuildStance(unit, order, 0x10008);
    case 4:
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    case 5: {
        Vec3 delta = unit->pos - target->pos;
        int range = 0;
        range = unit->def->buildRange;
        range += target->def->radius184;
        int square = delta.Square();
        if (square <= range * range && unit->CanReclaim(order->target.Get())) {
            if (order->duration >= 15) {
                DamageUnit(unit, order->target.Get(), order->elapsed, 5, 0);
                order->duration = 0;
            }
            unit->workTime = g_game->ticks + 900;
            Vec3 start;
            GetNanoPiecePosition(order->source, &start);
            Vec3 bounds[2];
            bounds[1] = order->target.Get()->pos;
            bounds[0] = order->target.Get()->pos;
            bounds[0].x += order->target.Get()->def->bounds.lo.x;
            bounds[0].z += order->target.Get()->def->bounds.lo.z;
            bounds[1].x += order->target.Get()->def->bounds.hi.x;
            bounds[1].z += order->target.Get()->def->bounds.hi.z;
            bounds[1].y += order->target.Get()->def->bounds.hi.y;
            EmitReverseNanoParticles(bounds, &start, 6);
            ((Class_00439e80*)order)->SetDeadlineTicks(2);
            order->duration += 2;
            return 2;
        }
        ((Class_00439e80*)order)->SetDeadlineTicks(15);
        StopBuildingScript(unit, order);
        return 0;
    }
    }
    return 7;
}



// Order handler "Repair" of a builder: walks up to the target unit, then
// spends worker time on it until its health is full.
// FUNCTION: 0x405300
int __stdcall RepairUnitOrder(Unit* unit, Order* order, int flags)
{
    if (!order->target.owner) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    if (order->range && (int)_hypot(unit->pos.xh - order->start.x, unit->pos.zh - order->start.z) >= order->range)
        return 5;
    if (order->target.owner->bits.mode != 1) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    switch (order->state) {
    case 0:
        if (unit->active && (unit->def->flags241 & 0x40) && order->target.owner->progress == Zero_004fc920) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Repairing");
            return 1;
        }
        break;
    case 1: {
        if (flags & 0x40)
            return 8;
        Fixed distance;
        distance.value = (int)_hypot(unit->pos.x - order->target.owner->pos.x, unit->pos.z - order->target.owner->pos.z);
        int gap = distance.whole - (int)(_hypot(unit->footprint.x, unit->footprint.z) * 8.0);
        gap += (int)(_hypot(order->target.owner->footprint.x, order->target.owner->footprint.z) * -8.0);
        unsigned int range = 0;
        range = unit->def->buildRange;
        if (gap > (int)range) {
            ((Class_00438ad0*)order)->AttachBuildFootprintMarker(order->target.owner->cell, order->target.owner->footprint);
            ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(30) + 30);
            order->flags |= 0xe8;
            return 2;
        }
        unit->ClaimWeapons(3);
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &order->target.owner->pos) - unit->rot.y);
        return 1;
    }
    case 2:
        return WaitIfNotInBuildStance(unit, order, 8);
    case 3:
        if ((unsigned int)order->target.owner->health >= order->target.owner->def->maxHealth)
            return 1;
        if (order->target.owner->flags & 0xc) {
            StopBuildingScript(unit, order);
            ((Class_00439e80*)order)->SetDeadlineTicks(15);
            return 0;
        }
        unit->workTime = g_game->ticks + 150;
        if (AddRepairProgress(unit, order->target.owner, (float)(unit->def->workerTime / 30))) {
            Vec3 nano;
            GetNanoPiecePosition(order->source, &nano);
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
        ((Class_00439e80*)order)->SetDeadlineTicks(1);
        order->flags |= 8;
        return 2;
    case 4:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}

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
        if (!(unit->def->flags241 & 0x40)) return 7;
        if (target->progress == Zero_004fc920 && (unit->flags10e & 1)) {
            ((Unit*)unit)->ClaimWeapons(3);
            return 1;
        }
        return 8;
    case 1: {
        if ((unsigned int)target->health >= target->def->maxHealth) return 1;
        if (target->flags & 0xc) return 1;
        unit->workTime = g_game->ticks + 150;
        int rate = 0;
        rate = unit->def->workerTime;
        if (AddRepairProgress(unit, order->target.Get(), (float)(rate / 30))) {
            Vec3 start;
            GetNanoPiecePosition(order->source, &start);
            Vec3 bounds[2];
            bounds[1] = order->target.Get()->pos;
            bounds[0] = order->target.Get()->pos;
            bounds[0].x += order->target.Get()->def->bounds.lo.x;
            bounds[0].z += order->target.Get()->def->bounds.lo.z;
            bounds[1].x += order->target.Get()->def->bounds.hi.x;
            bounds[1].z += order->target.Get()->def->bounds.hi.z;
            bounds[1].y += order->target.Get()->def->bounds.hi.y;
            EmitNanoParticles(&start, bounds, 6);
        }
        ((Class_00439e80*)order)->SetDeadlineTicks(1);
        order->flags |= 8;
        return 2;
    }
    case 2:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}


// FUNCTION: 0x405d90
void Class_00405d90::CollectDamagedAlly(Unit* unit)
{
    if (unit == self) return;
    unsigned int index = 0;
    index = unit->owner->index;
    if (!owner->allied[index]) return;
    unsigned int kind = unit->flags & 3;
    if ((unsigned char)kind != 1) return;
    if ((unsigned int)unit->health >= unit->def->maxHealth && unit->progress == Zero_004fc920) return;
    if (unit->orderPlayer == owner->index && unit->orderKind == 5) return;
    units->push_back(unit);
}

// FUNCTION: 0x405fe0
int __stdcall StandbyOrder(Unit* unit,Order* order,int flags)
{
    unsigned state=0; state=order->state;
    switch(state) {
    case 0:
        if(!unit->active) return 7;
        unit->ReleaseWeapons(3);
        order->flags|=0x10000;
        ((Class_00439e80*)order)->SetDeadlineTicks(1); return 1;
    case 1:
        {
            Unit* next=FindBestTargetIfFireAtWill(unit);
            if(next && IssueAttackOrder(unit,(Order*)next,0)) return 5;
        }
        order->flags|=0x10000;
        ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(30)+30); return 2;
    default: return 7;
    }
}

// FUNCTION: 0x406090
int __stdcall StandbyMineOrder(Unit* unit, Order* order, int unused)
{
    switch (order->state) {
    case 0:
        if (!(unit->flags & 0x20000000)) return 7;
        unit->ReleaseWeapons(3);
        order->flags |= 0x10000;
        ((Class_00439e80*)order)->SetDeadlineTicks(1);
        return 1;
    case 1:
        {
            Unit* other = FindBestTargetIfFireAtWill(unit);
            if (other && other->bits.mode == 1 && (unit->flags & 0x300000)) {
                AppendOrder(unit, new Class_0043a1f0("SELFDESTRUCT", 0, 0, 1, 0, 0));
                return 5;
            }
            order->flags |= 0x10000;
            ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(30) + 30);
            return 2;
        }
    default: return 7;
    }
}

static inline Point16 MakePoint(int x,int y) { Point16 p; p.x=x; p.z=y; return p; }
static inline Point16 SubPoint(Point16 a,Point16 b) { Point16 p; p.x=a.x-b.x; p.z=a.z-b.z; return p; }
static inline Point16 Convert(Vec3 v) { Point16 p; p.x=(unsigned)v.x>>20; p.z=(unsigned)v.z>>20; return p; }

// FUNCTION: 0x4061a0
int __stdcall ParkOrder(Unit* unit,Order* order,int flags)
{
    switch(order->state) {
    case 0:
        if(!unit->active) return 7;
        if(unit->def->flying) {
            order->pos=unit->pos;
            ((Class_00438b90*)order)->MergeFlagsFromTable("VTOL_MOVE");
            return 0;
        }
        {
            int size=unit->footprint.x;
            if(unit->def->height>=0) size+=3;
            Point16 start=SubPoint(Convert(unit->pos),MakePoint(size*4,size*3));
            Point16 extent=MakePoint(size*8,size*6);
            ((Class_00438ad0*)order)->AttachBuildFootprintMarker(start,extent);
            order->flags=0xe0;
            return 1;
        }
    case 1:
        if(flags&0x20) return 5;
        if(order->next) return 5;
        ((Class_00439e80*)order)->SetDeadlineTicks(30); return 0;
    default: return 7;
    }
}

// Unused here (0x406300 stays in its own file): the symbol ids these helpers
// take keep the allocation (docs/c2-regalloc.md).
static inline int Contains(unsigned int* bits, unsigned short index) { return bits[index >> 5] & (1 << (index & 31)); }

static inline Vec3 Offset(short angle, int distance) { Vec3 v; v.x=-FUN_004b70ef(angle,distance); v.y=0; v.z=-FUN_004b7123(angle,distance); return v; }


// Keep cases 1 and 3 separate: MSVC merges their identical bodies.
// FUNCTION: 0x406780
int __stdcall GroundPickupOrder(Unit* unit, Order* order, unsigned char flags)
{
    Unit* target = order->target.owner;
    if (target && !(flags & 8)) {
        switch(order->state) {
        case 0:
            if (!unit->active) break;
            if (!(unit->def->flags245u & 0x100)) break;
            if (target->footprint.x > (short)unit->def->capacity) {
                QueueUnitSpeech(unit, 7, "Unit is too large to transport"); return 8;
            }
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Loading unit"); return 1;
        case 1: return WaitIfCobBusy(unit, order, 8);
        case 2:
            {
            int id = target->id;
            unit->script->StartScriptWithArgs("TransportPickup", 0, 1, 1, id, 0, 0, 0);
            QueueUnitSpeech(unit, 12, 0);
            ++order->attempts;
            ((Class_00439e80*)order)->SetDeadlineTicks(15); return 1;
            }
        case 3: return WaitIfCobBusy(unit, order, 8);
        case 4:
            if (target->transport) return 5;
            if (order->attempts >= 3) return 9;
            ((Class_00438930*)order)->AttachApproachRadiusGoal(&target->pos, 0);
            order->flags = 0xe8; return 1;
        case 5: ((Class_004388d0*)order)->SetAttachedFx(0); return 0;
        default: break;
        }
        return 7;
    }
    QueueUnitSpeech(unit, 7, "Transport mission failed"); return 8;
}

// FUNCTION: 0x406900
int __stdcall GroundUnloadOrder(Unit* unit, Order* order, int flags)
{
    if (flags&8) {
        QueueUnitSpeech(unit,7,"Unloading process is proceeding non-optimally");
        return 8;
    }
    switch(order->state) {
    case 0:
        if (!unit->active) break;
        if (!(unit->def->flags245u&0x100)) break;
        order->target.SetUnit(unit->cargo);
        if (!order->target.owner) return 5;
        ((Class_00438880*)order)->AnnounceStatusIfFlagged("Unloading");
        unit->script->StartScriptWithArgs("TransportDrop",0,1,1,order->target.owner->id,
            (order->pos.x&0xffff0000)+(order->pos.z>>16),0,0);
        ++order->attempts;
        ((Class_00439e80*)order)->SetDeadlineTicks(15);
        return 1;
    case 1: return WaitIfCobBusy(unit,order,8);
    case 2:
        if (order->target.owner->transport!=unit) { QueueUnitSpeech(unit,13,0); return 5; }
        if (order->attempts>=3) return 9;
        if ((unsigned char)(unit->def->flags>>12)&1)
            ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos,(int)(unit->def->height180*1.5));
        else ((Class_00438930*)order)->AttachApproachRadiusGoal(&order->pos,0);
        order->flags=0xe8;
        return 1;
    case 3: return 0;
    }
    return 7;
}

// Unused here (0x406aa0 stays in its own file): the symbol ids these helpers
// take keep the allocation (docs/c2-regalloc.md).
static inline Vec3 Add(const Vec3& a,const Vec3& b) { Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
static inline Vec3 Sub(const Vec3& a,const Vec3& b) { Vec3 r; r.x=a.x-b.x; r.y=a.y-b.y; r.z=a.z-b.z; return r; }


// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.
// FUNCTION: 0x406bf0
void RegisterGroundOrders()
{
    extern char g_groundOrders[];
    RegisterOrderTypes(g_groundOrders, 0x16);
}

// std::vector<Unit*>::_Destroy(first, last) from MSVC 5's <vector>: empty,
// since the element type is trivial.
// Its callers (0x405d90, 0x40b530, 0x480250 and others) inline
// vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and 0x406c00
// (_Destroy) with ecx set to the vector. 0x40ad80 calls _Ucopy, _Ufill,
// size (0x40c560) and insert (0x408f30) on one vector of units, and 0x48d220
// calls _Destroy and erase (0x40c9f0), so all of these are members of the
// same std::vector<Unit*> (see 0x40c510.cpp).
typedef std::vector<Unit*> Vec_00406c00;
typedef void (Vec_00406c00::*DestroyFn_00406c00)(Vec_00406c00::iterator, Vec_00406c00::iterator);

// _Destroy is protected: the derived class takes its address to emit it out of line.
struct Access_00406c00 : Vec_00406c00 {
    static DestroyFn_00406c00 fn;
};

// FUNCTION: 0x406c00 ?_Destroy@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@IAEXPAPAUUnit@@0@Z
DestroyFn_00406c00 Access_00406c00::fn = &Access_00406c00::_Destroy;

// 0x406c10 (_Ucopy), 0x406c40 (_Ufill) and 0x406c70 (_Construct) stay in their
// own files: the inline _Construct above is what makes the repair patrol's
// vector insert (0x405d90) call 0x406c70, and with it in this file _Ucopy and
// _Ufill call it too instead of inlining the placement new, and 0x406c70
// itself is inlined into insert, so all three change bytes here.

struct Unit;
struct Order;
struct View;
struct Pos;

#pragma pack(push, 1)
struct UnitOrderType {
    const char* status;                                         // +0x00 shown while the order runs
    int (__stdcall* run)(Unit* unit, Order* order, int flags);  // +0x04 (0x43bad0 calls it)
    // +0x08 marks the order's target on screen.
    void (__stdcall* draw)(void* surface, View* view, Order* order, Pos* out, int flag);
    int target;                                                 // +0x0c
    unsigned int flags;                                         // +0x10
    unsigned char field_14;                                     // +0x14
    const char* name;                                           // +0x15 the key the table is sorted by
};
#pragma pack(pop)

int __stdcall StopOrder(Unit* unit, Order* order, int flags);
int __stdcall MakeSelectableOrder(Unit* unit, Order* order, int flags);
int __stdcall WaitOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackUTypeOrder(Unit* unit, Order* order, int flags);
int __stdcall WaitForAttackOrder(Unit* unit, Order* order, int flags);
int __stdcall SelfDestructOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall GuardNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall SelfRepairOrder(Unit* unit, Order* order, int flags);
int __stdcall BuildingBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall BuildWeaponOrder(Unit* unit, Order* order, int flags);
int __stdcall ParalyzeOrder(Unit* unit, Order* order, int flags);
int __stdcall GetBuiltOrder(Unit* unit, Order* order, int flags);
int __stdcall BeCarriedOrder(Unit* unit, Order* order, int flags);
int __stdcall ActivateOrder(Unit* unit, Order* order, int flags);
int __stdcall DeactivateOrder(Unit* unit, Order* order, int flags);
int __stdcall CloakOnOrder(Unit* unit, Order* order, int flags);
int __stdcall CloakOffOrder(Unit* unit, Order* order, int flags);
int __stdcall StandingMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall StandingFireOrder(Unit* unit, Order* order, int flags);
int __stdcall QMoveQPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackSpecialOrder(Unit* unit, Order* order, int flags);
int __stdcall MoveGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackKamikazeOrder(Unit* unit, Order* order, int flags);
int __stdcall PatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall AttackChaseOrder(Unit* unit, Order* order, int flags);
int __stdcall SuppressOrder(Unit* unit, Order* order, int flags);
int __stdcall MobileBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall HelpBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall CaptureOrder(Unit* unit, Order* order, int flags);
int __stdcall ReclaimUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall ReclaimOrder(Unit* unit, Order* order, int flags);
int __stdcall ResurrectOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairUnitNoMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall RepairPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall StandbyOrder(Unit* unit, Order* order, int flags);
int __stdcall StandbyMineOrder(Unit* unit, Order* order, int flags);
int __stdcall ParkOrder(Unit* unit, Order* order, int flags);
int __stdcall FollowGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall GroundPickupOrder(Unit* unit, Order* order, int flags);
int __stdcall GroundUnloadOrder(Unit* unit, Order* order, int flags);
int __stdcall TeleportOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolLandIfCanOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolStandbyOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolMoveOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolFollowOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolSeekAttackOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolSeekGuardOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolPickupOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolUnloadOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolLandingOrder(Unit* unit, Order* order, int flags);
int __stdcall AirStrikeOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToGroundOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToAirOrder(Unit* unit, Order* order, int flags);
int __stdcall AirToGroundHoverOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolEvadeOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolMobileBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolHelpBuildOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolReclaimOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolReclaimUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolRepairUnitOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolGetRepairedOrder(Unit* unit, Order* order, int flags);
int __stdcall VtolRepairPatrolOrder(Unit* unit, Order* order, int flags);
int __stdcall ReadyOrder(Unit* unit, Order* order, int flags);
void __stdcall DrawBuildFootprint(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall DrawPathAnim(void* surface, View* view, Order* order, Pos* out, int flag);
void __stdcall DrawWeaponCoverage(void* surface, View* view, Order* order, Pos* out, int flag);

// every unit's orders, registered by 0x403180.
// GLOBAL: 0x4fc490
extern const UnitOrderType g_unitOrders[23] = {
    {"Stopping", StopOrder, 0, 0, 0x13, 0, "Stop"},
    {"Attacking", AttackNoMoveOrder, DrawWeaponCoverage, 8, 0x28001, 0, "Attack_NoMove"},
    {"Activate", ActivateOrder, 0, 0, 0x1006013, 0, "Activate"},
    {"Deactivate", DeactivateOrder, 0, 0, 0x1006013, 0, "Deactivate"},
    {"Cloaking", CloakOnOrder, 0, 0, 0x1006013, 0, "Cloak_On"},
    {"Decloaking", CloakOffOrder, 0, 0, 0x1006013, 0, "Cloak_Off"},
    {"Acknowledged", StandingMoveOrder, 0, 0, 0x1006013, 0, "Standing_MoveOrder"},
    {"Acknowledged", StandingFireOrder, 0, 0, 0x1006013, 0, "Standing_FireOrder"},
    {"Nanolathing", BuildingBuildOrder, 0, 0, 0x10010c13, 0, "BuildingBuild"},
    {"Nanolathing", BuildWeaponOrder, 0, 0, 0xc014013, 0, "BuildWeapon"},
    {"SELF DESTRUCT ENGAGED", SelfDestructOrder, 0, 0, 0x4004013, 0, "SelfDestruct"},
    {"SELF DESTRUCT ENGAGED", SelfDestructOrder, 0, 0, 0x13, 0, "SelfDestructFG"},
    {"Paralyzed", ParalyzeOrder, 0, 0, 0x2413, 0, "Paralyze"},
    {"Under construction", GetBuiltOrder, 0, 0, 0x22413, 0, "GetBuilt"},
    {"Being transported", BeCarriedOrder, 0, 0, 0x2413, 0, "BeCarried"},
    {"Unit is available", MakeSelectableOrder, 0, 0, 0x413, 0, "MakeSelectable"},
    {"Waiting", WaitOrder, 0, 0, 0x413, 0, "Wait"},
    {"Waiting for attack", WaitForAttackOrder, 0, 0, 0x20413, 0, "WaitForAttack"},
    {"Attacking", AttackUTypeOrder, 0, 0, 0x413, 0, "AttackUType"},
    {"Ready", GuardNoMoveOrder, 0, 0, 0x2013, 0, "Guard_NoMove"},
    {"Repairing", SelfRepairOrder, 0, 0, 0x20413, 1, "SelfRepair"},
    {"Ready with orders", QMoveQPatrolOrder, DrawPathAnim, 2, 0x4000e, 0, "QMove"},
    {"Ready with orders", QMoveQPatrolOrder, DrawPathAnim, 2, 0x40007, 0, "QPatrol"},
};

// the orders of units that move on the ground, registered by 0x406bf0.
// GLOBAL: 0x4fc6e8
extern const UnitOrderType g_groundOrders[22] = {
    {"Standby", StandbyOrder, 0, 0x10, 0x200000f, 0, "Standby"},
    {"Standby", StandbyMineOrder, 0, 0x10, 0x200000f, 1, "Standby_Mine"},
    {"Moving", MoveGroundOrder, DrawPathAnim, 0x12, 0x4020e, 0, "Move_Ground"},
    {"Guarding", FollowGroundOrder, DrawPathAnim, 0x12, 0x20005, 0, "Follow_Ground"},
    {"Suppressing fire", SuppressOrder, DrawWeaponCoverage, 8, 0x41001, 0, "Suppress"},
    {"Attacking", AttackChaseOrder, DrawWeaponCoverage, 8, 0x28001, 0, "Attack_Chase"},
    {"Attacking", AttackKamikazeOrder, DrawWeaponCoverage, 8, 0x60001, 0, "Attack_Kamikaze"},
    {"Annihilating", AttackSpecialOrder, DrawWeaponCoverage, 8, 0x68001, 0, "AttackSpecial"},
    {"Parking", ParkOrder, 0, 0, 0xe, 0, "Park"},
    {"Patrolling", PatrolOrder, DrawPathAnim, 0x12, 0x41207, 0, "Patrol"},
    {"Loading", GroundPickupOrder, DrawWeaponCoverage, 8, 0x2000c, 0, "Ground_Pickup"},
    {"Unloading", GroundUnloadOrder, DrawWeaponCoverage, 8, 0x4000d, 0, "Ground_Unload"},
    {"Teleporting", TeleportOrder, DrawWeaponCoverage, 8, 0x60009, 0, "Teleport"},
    {"Nanolathing", MobileBuildOrder, DrawBuildFootprint, 0x13, 0x10050800, 0, "MobileBuild"},
    {"Nanolathing", HelpBuildOrder, DrawWeaponCoverage, 0x18, 0x10020806, 0, "HelpBuild"},
    {"Repair patrol", RepairPatrolOrder, DrawPathAnim, 0x12, 0x41207, 0, "RepairPatrol"},
    {"Repairing", RepairUnitOrder, DrawPathAnim, 0x12, 0x10020006, 0, "RepairUnit"},
    {"Capturing", CaptureOrder, DrawWeaponCoverage, 8, 0x20004, 0, "Capture"},
    {"Resurrecting", ResurrectOrder, DrawPathAnim, 0x12, 0x2000b, 0, "Resurrect"},
    {"Reclaiming", ReclaimOrder, DrawPathAnim, 0x12, 0x1008000b, 0, "Reclaim"},
    {"Reclaiming", ReclaimUnitOrder, DrawPathAnim, 0x12, 0x1002000b, 0, "ReclaimUnit"},
    {"Repairing", RepairUnitNoMoveOrder, DrawWeaponCoverage, 0x18, 0x20006, 0, "RepairUnitNoMove"},
};

// the orders of aircraft, registered by 0x415b20.
// GLOBAL: 0x4fca18
extern const UnitOrderType g_vtolOrders[22] = {
    {"Standby", VtolStandbyOrder, 0, 0, 0x200000f, 0, "VTOL_Standby"},
    {"Moving", VtolMoveOrder, DrawPathAnim, 2, 0x4020e, 0, "VTOL_Move"},
    {"Landing", VtolLandingOrder, DrawWeaponCoverage, 8, 0x6000e, 0, "VTOL_Landing"},
    {"Loading", VtolPickupOrder, DrawWeaponCoverage, 8, 0x20008, 0, "VTOL_Pickup"},
    {"Unloading", VtolUnloadOrder, DrawWeaponCoverage, 8, 0x40009, 0, "VTOL_Unload"},
    {"Guarding", VtolFollowOrder, DrawPathAnim, 2, 0x20005, 0, "VTOL_Follow"},
    {"Patrolling", VtolPatrolOrder, DrawPathAnim, 2, 0x41207, 0, "VTOL_Patrol"},
    {"Airstrike", AirStrikeOrder, DrawWeaponCoverage, 8, 0x60002, 0, "AirStrike"},
    {"Engaging target", AirToAirOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToAir"},
    {"Engaging target", AirToGroundOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToGround"},
    {"Engaging target", AirToGroundHoverOrder, DrawWeaponCoverage, 8, 0x20001, 0, "AirToGroundHover"},
    {"Nanolathing", VtolMobileBuildOrder, DrawBuildFootprint, 3, 0x10050800, 0, "VTOL_MobileBuild"},
    {"Nanolathing", VtolHelpBuildOrder, DrawWeaponCoverage, 8, 0x10020806, 0, "VTOL_HelpBuild"},
    {"Repair patrol", VtolRepairPatrolOrder, DrawPathAnim, 2, 0x41207, 0, "VTOL_RepairPatrol"},
    {"Repairing", VtolRepairUnitOrder, DrawPathAnim, 2, 0x10020006, 0, "VTOL_RepairUnit"},
    {"Reclaiming", VtolReclaimOrder, DrawPathAnim, 2, 0x1008000b, 0, "VTOL_Reclaim"},
    {"Reclaiming", VtolReclaimUnitOrder, DrawPathAnim, 2, 0x1002000b, 0, "VTOL_ReclaimUnit"},
    {"Evading", VtolEvadeOrder, 0, 0, 0x13, 0, "VTOL_Evade"},
    {"Seeking to attack", VtolSeekAttackOrder, 0, 0, 0x60013, 0, "VTOL_SeekAttack"},
    {"Seeking to guard", VtolSeekGuardOrder, 0, 0, 0x60013, 0, "VTOL_SeekGuard"},
    {"Under repair", VtolGetRepairedOrder, 0, 0, 0x20013, 0, "VTOL_GetRepaired"},
    {"Seeking to land", VtolLandIfCanOrder, 0, 0, 0x40013, 0, "VTOL_LandIfCan"},
};

// One order, registered on its own by 0x43bc90.
// GLOBAL: 0x4fd288
extern const UnitOrderType g_readyOrder[1] = {
    {"Ready", ReadyOrder, 0, 0, 0xf, 0, ""},
};



extern const int PadAlign_004fc6d0 = 0;
