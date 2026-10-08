// Decompiled by Claude Opus 5.5, GPT-6, GPT-6 Astra, GPT-6.1-sol, deepseek-v4.1, space-bunny-free, Claude Sonnet 5.5, deepseek-v4.1-flash, Opus and Haiku. Names are provisional.
// The second half of the ground order handlers, from the attack-chase handler
// (0x4034a0) to the std::vector<Unit*> destroy member at 0x406c00: the mobile
// builders, capture, reclaim, resurrect, repair, standby, park, follow,
// transport and teleport handlers, with the order tables' registrations.
// The handlers 0x403a20, 0x403f70, 0x404ad0, 0x404db0, 0x405980, 0x406300 and
// 0x406aa0 stay in files of their own: their register allocation follows
// symbol ids this file's context cannot give them. The vector members
// 0x406c10, 0x406c40 and 0x406c70 stay in theirs: the inline _Construct below
// makes the repair patrol call 0x406c70, which would then also change their
// own bytes, and 0x406c70 itself would be inlined into the patrol.
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
struct Pad_00400000_35 { int field; };
struct Pad_00400000_36 { int field; };
struct Pad_00400000_37 { int field; };
struct Pad_00400000_38 { int field; };
struct Pad_00400000_39 { int field; };
struct Pad_00400000_40 { int field; };
struct Pad_00400000_41 { int field; };
struct Pad_00400000_42 { int field; };
struct Pad_00400000_43 { int field; };
struct Pad_00400000_44 { int field; };
struct Pad_00400000_45 { int field; };
struct Pad_00400000_46 { int field; };
struct Pad_00400000_47 { int field; };
struct Pad_00400000_48 { int field; };
struct Pad_00400000_49 { int field; };
struct Pad_00400000_50 { int field; };
struct Pad_00400000_51 { int field; };
struct Pad_00400000_52 { int field; };
struct Pad_00400000_53 { int field; };
struct Pad_00400000_54 { int field; };
struct Pad_00400000_55 { int field; };
struct Pad_00400000_56 { int field; };
struct Pad_00400000_57 { int field; };
struct Pad_00400000_58 { int field; };
struct Pad_00400000_59 { int field; };
struct Pad_00400000_60 { int field; };
struct Pad_00400000_61 { int field; };
struct Pad_00400000_62 { int field; };
struct Pad_00400000_63 { int field; };
struct Pad_00400000_64 { int field; };
struct Pad_00400000_65 { int field; };
struct Pad_00400000_66 { int field; };
struct Pad_00400000_67 { int field; };
struct Pad_00400000_68 { int field; };
struct Pad_00400000_69 { int field; };
struct Pad_00400000_70 { int field; };
struct Pad_00400000_71 { int field; };
struct Pad_00400000_72 { int field; };
struct Pad_00400000_73 { int field; };
struct Pad_00400000_74 { int field; };
struct Pad_00400000_75 { int field; };
struct Pad_00400000_76 { int field; };
struct Pad_00400000_77 { int field; };
struct Pad_00400000_78 { int field; };
struct Pad_00400000_79 { int field; };
struct Pad_00400000_80 { int field; };
struct Pad_00400000_81 { int field; };
struct Pad_00400000_82 { int field; };
struct Pad_00400000_83 { int field; };
struct Pad_00400000_84 { int field; };
struct Pad_00400000_85 { int field; };
struct Pad_00400000_86 { int field; };
struct Pad_00400000_87 { int field; };
struct Pad_00400000_88 { int field; };
struct Pad_00400000_89 { int field; };
struct Pad_00400000_90 { int field; };
struct Pad_00400000_91 { int field; };
extern int pad_00400000_0;
extern int pad_00400000_1;
extern int pad_00400000_2;
struct Unit;
struct Order;
void __stdcall FUN_00406c70(Unit** dest, Unit* const* src);
// The repair patrol's push_back inlines vector::insert, whose _Construct this
// overload replaces; it must be declared before <vector>.
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { FUN_00406c70(dest, &src); }
}

char* __stdcall lstrcpynA(char* dest, const char* src, int count);
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

struct Rot16 {
    short x, y, z;
};

// A 16.16 fixed-point integer seen as its fraction and whole halves.
union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int a, int b, int c, int d, int e, int f);
};

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() {}
    int operator==(const Class_00438760& v) const { return index == v.index; }
};

class Class_00438880 {
public:
    void AnnounceStatusIfFlagged(const char* text);
};

class Class_004388d0 {
public:
    void SetAttachedFx(int param);
};

class Class_00438930 {
public:
    void AttachApproachRadiusGoal(Vec3* pos, int param);
};

class Class_00438a00 {
public:
    void AttachRingApproachGoal(Vec3* pos, int param, int param_3);
};

class Class_00439e80 {
public:
    void SetDeadlineTicks(int ticks);
};

class Class_00438ad0 {
public:
    void AttachBuildFootprintMarker(Point16 cell, Point16 size);
};

class Class_00438b90 {
public:
    void MergeFlagsFromTable(Class_00438760 kind);
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

class Class_004897e0 {
public:
    unsigned char ChooseWeapon();
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec3* pos, int c, int d, int e);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct WeaponDef {
    char unknown_0[0x111];
    unsigned int flags;                // +0x111
};

struct Weapon {
    char unknown_0[8];
    WeaponDef* def;                    // +0x8, at +0x10 for weapon[0]
    char unknown_c[0x17 - 0xc];
    unsigned char flags;               // +0x17, at +0x1f for weapon[0]
    char unknown_18[0x1c - 0x18];
};

struct UnitDef {
    char unknown_0[0x14a];
    Point16 footprint;                 // +0x14a
    char unknown_14e[0x15e - 0x14e];
    Box bounds;                        // +0x15e
    char unknown_176[0x180 - 0x176];
    short height180;                   // +0x180
    char unknown_182[0x184 - 0x182];
    short radius;                      // +0x184
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
    char unknown_214[0x22a - 0x214];
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
        unsigned int flags245;         // +0x245
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

struct Owner {
    char unknown_0[0x8c];
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

struct Game {
    char unknown_0[0x1420b];
    FeatureSpot* spots;                // +0x1420b
    char unknown_1420f[0x14233 - 0x1420f];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell* cells;                       // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x1439b - 0x1435f];
    UnitDef* unitTypes;                // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

struct Unit {
    int active;                        // +0x0
    char unknown_4[0x8 - 0x4];
    Weapon weapons[3];                 // +0x8
    Order* order;                      // +0x5c
    char unknown_60[0x64 - 0x60];
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
    Owner* owner;                      // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short category;           // +0xa6
    unsigned short id;                 // +0xa8
    char unknown_aa[0xb0 - 0xaa];
    int workTime;                      // +0xb0
    char unknown_b4[0xb8 - 0xb4];
    unsigned short experience;         // +0xb8
    char unknown_ba[0xf0 - 0xba];
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
    char unknown_10f;
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 30;
        } bits;
    };
    char unknown_114[0x118 - 0x114];
    void ReleaseWeapons(int param);
    void ClaimWeapons(int param);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void SetStateBits(int which, int on);
    int CanReclaim(Unit* unit);
};

struct Order {
    char unknown_0[4];
    Class_00438760 kind;               // +0x4
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    Class_004895c0 target;             // +0x12, its owner is at +0x16
    Vec3 pos;                          // +0x22
    Point16 start;                     // +0x2e
    char unknown_32[0x36 - 0x32];
    union {
        int weapon;                    // +0x36
        int type;
        int elapsed;
        int unitType;
        int attempts;
        int time;
        int guardRange;
    };
    union {
        int step;                      // +0x3a
        int radius;
        int unused;
        int duration;
        int countdown;
    };
    union {
        int range;                     // +0x3e
        int retries;
        int capabilities;
    };
    char unknown_42[0x4a - 0x42];
    Order* next;                       // +0x4a

    Unit* Target() { return target.owner; }
    Vec3* Position() { return &pos; }
    int Advance(int distance) {
        ((Class_00438930*)this)->AttachApproachRadiusGoal(&target.owner->pos, distance);
        step++;
        return 1;
    }
};
#pragma pack(pop)

class Class_00405d90 {
public:
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
    Class_00405d90(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    virtual void FUN_00405d90(Unit*);
};

extern Game* g_game;
extern char DAT_004fc490[];
extern char DAT_004fc6e8[];
extern const float DAT_004fc930, DAT_004fc934, DAT_004fc938, DAT_004fc93c;

unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, const char* text);
int __stdcall RandomInt(int range);
int __stdcall GetGroundHeight(Vec3* pos);
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
void __stdcall StartBuildingScript(Unit* unit, Order* order, short turn);
void __stdcall StopBuildingScript(Unit* unit, Order* order);
int __stdcall WaitIfNotInBuildStance(Unit* unit, Order* order, int flags);
int __stdcall WaitIfCobBusy(Unit* unit, Order* order, int flags);
int __stdcall ComputeReclaimDamagePulse(Unit* unit, Unit* target, int flags);
void __stdcall EnsurePatrolReturnOrder(Unit* unit, Order* order);
int __stdcall IssueAttackOrder(Unit* unit, Unit* target, int param);
int __stdcall IssueAttackOrder(Unit* unit, Order* order, int param);
Unit* __stdcall FindBestTargetIfFireAtWill(Unit* unit);
int __stdcall IssueRepairOrder(Unit* unit, Unit* target, int param);
void __stdcall MarkSelectionOrdersDirty(Unit* unit);
int __stdcall AddRepairProgress(Unit* builder, Unit* unit, float amount);
int __stdcall AddBuildProgress(Unit* unit, Unit* target, float amount);
Unit* __stdcall GetWeaponTargetUnit(Unit* unit, int index);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitNanoParticles(Vec3* from, Box* to, int count);
void __stdcall EmitNanoParticles(Vec3* from, Vec3* to, int count);
void __stdcall EmitReverseNanoParticles(Box* from, Vec3* to, int count);
void __stdcall EmitReverseNanoParticles(Vec3* from, Vec3* to, int count);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, int a, Point16 cell, int b);
void __stdcall SnapWorldPosToFootprint(UnitDef* type, Vec3* pos);
Unit* __stdcall CreateUnit(unsigned char player, short type, Vec3 pos, int a, int b, int c);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner, Unit* id, Vec3* pos,
                        int param_6, int param_7);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit, Unit* target, int flags);
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);
void __stdcall DamageUnit(Unit* unit, Unit* target, int n, int kind, int flag);
void __stdcall GiveUnitToPlayer(Unit* unit, void* player, int param);
int __stdcall WeaponCanReachUnit(Unit* unit, Unit* target, int param_3);
void __stdcall SetWeaponTargetUnit(Unit* unit, Unit* target, int weapon);
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
int __stdcall FUN_0049adf0(Unit* unit, int weapon);
int __stdcall FUN_0049adf0(Unit* unit, unsigned char weapon);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);
void __stdcall EmitTeleportParticles(Vec3* from, Vec3* to, int count, int param);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int param);
void __stdcall RegisterOrderTypes(void* table, int id);
// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
void __stdcall AlignUnitToGround(Unit*);
int __stdcall FindLandingPad(Unit*, int);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall ClearWeaponTarget(Unit*, int);
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
        if (!unit->active || (unit->def->flags & 0x800) || !(unit->flags & 0x80000000)) break;
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
        weapon = FUN_0049adf0(unit, weapon);
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
        order->radius=FUN_0049adf0(unit,(unsigned char)order->weapon); return 1;
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
            order->radius-=RandomInt(FUN_0049adf0(unit,(unsigned char)order->weapon)/3);
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
        if (!(unit->def->flags245 & 0x1000)) return 7;
        if (target->def->capture) {
            QueueUnitSpeech(unit, 7, "That unit cannot be captured");
            return 8;
        }
        if (target->progress != 0.0f) {
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
        if (unit->active && (unit->def->flags245 & 0x400)) {
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
        range += target->def->radius;
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
        if (unit->active && (unit->def->flags241 & 0x40) && order->target.owner->progress == 0.0f) {
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
        if (target->progress == 0.0f && (unit->flags10e & 1)) {
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
void Class_00405d90::FUN_00405d90(Unit* unit)
{
    if (unit == self) return;
    unsigned int index = 0;
    index = unit->owner->index;
    if (!owner->allied[index]) return;
    unsigned int kind = unit->flags & 3;
    if ((unsigned char)kind != 1) return;
    if ((unsigned int)unit->health >= unit->def->maxHealth && unit->progress == 0.0f) return;
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
            if (!(unit->def->flags245 & 0x100)) break;
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
        if (!(unit->def->flags245&0x100)) break;
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
    RegisterOrderTypes(DAT_004fc6e8, 0x16);
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
