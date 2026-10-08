// Decompiled by Opus, Claude Opus 5.5, GPT-5.6 Astra, deepseek-v4.1-flash, GPT-6, deepseek-v4.1, GPT-6.1-sol, xiaomi/mimo-v2.6-pro, Space Bunny Free, GPT-6 Astra, Sonnet, space-bunny-free and GPT-6.1-Sol. Names are provisional.
// The VTOL order handlers that share this file's symbol context, in address
// order: landing (0x40f2a0), standby (0x40f7d0), move (0x40fa20), unload
// (0x411560), landing on a pad (0x411840), evade (0x413bc0), mobile build
// (0x413d80), repair (0x414e70, 0x415250) and the visitor 0x4158d0, with the
// helpers they share: PrepVtolClimb, DirectionFromAngle, CellToWorldPos, the
// LandingPadList constructor and the externs below.
// The module's other handlers (0x40f790, 0x40fbe0, 0x4103e0, 0x410850,
// 0x410c70, 0x410e70, 0x4111b0, 0x4118e0, 0x411f50, 0x412710, 0x412d40,
// 0x413470, 0x414380, 0x414770, 0x414a80 and 0x4152f0) each match only in
// their own file's symbol context, so they stay in files of their own.
// The helpers below that no handler here calls still count symbols: removing
// them moves the register allocation of the handlers that do match.
#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <dplay.h>
#include <stdio.h>
struct Unit;
void __stdcall CopyDwordIfNonNull(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { CopyDwordIfNonNull(dest, &src); }
}
#include <vector>
#include <list>
#include <map>
#include <shlobj.h>
#include <imagehlp.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <float.h>
namespace ta {
#include <ta_types.h>
}
struct Unit;
struct Order;

// The push_back of the two visitor vectors copies through this helper
// instead of the generic std::_Construct: it must be declared before
// <vector> so its inlining is the original's.
void __stdcall CopyDwordIfNonNull(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { CopyDwordIfNonNull(dest, &src); }
}
#include <vector>
namespace ta {
#include <ta_types.h>
}

// A 16.16 position or velocity, and the fixed-point halves the VTOL code
// reads from it (the fraction and whole parts of x, y and z).
struct Vec3 {
    union { int x; struct { unsigned short xf; short xw; }; };
    union { int y; struct { unsigned short yf; short yw; }; };
    union { int z; struct { unsigned short zf; short zw; }; };

    void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
    void operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
    Vec3 operator+(const Vec3& v) const { Vec3 r; r.x = x + v.x; r.y = y + v.y; r.z = z + v.z; return r; }
    Vec3 operator-(const Vec3& v) const { Vec3 r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }

    // The horizontal distance squared, as two 32-bit products.
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
};

struct Point { short x, y; };
struct Point16 { short x, z; };
struct Pair16 { short lo, hi; };

struct Box {
    Vec3 lo;
    Vec3 hi;
};

// A 16.16 fixed-point number, or its two halves.
union Fixed {
    int value;
    struct { unsigned short frac; short whole; } parts;
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* p2, int p3, int p4, int p5, int p6, int p7, int p8);
    void StartScript(const char* name, int p2, int p3);
    int QueryScript(char* name, int* p2, int* p3, int* p4, int* p5);
};

// The command kind, one byte wide, but not a POD type.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760() {}
    Class_00438760(const char* name);
    int operator==(const Class_00438760& v) const { return index == v.index; }
};

// The mover object (UnitMotion) at the unit's +0x0: its +0x8 is a velocity,
// its +0x22 a turn rate and its +0x2e the low bits the VTOL handlers test.
class UnitMotion {
public:
    char unknown_0[8];
    Vec3 v;                            // +0x8
    char unknown_14[0x22 - 0x14];
    short field_22;                    // +0x22
    char unknown_24[0x2e - 0x24];
    unsigned char flags;               // +0x2e
    void SetFlightMode(Unit* unit, int state);
};

#pragma pack(push, 1)
struct WeaponDef {
    char unknown_0[0xdc];
    int range;                         // +0xdc
    char unknown_e0[0x111 - 0xe0];
    unsigned int flags;                // +0x111
};

struct Weapon {
    char unknown_0[8];
    WeaponDef* def;                    // +0x8
    char unknown_c[11];
    unsigned char flags;               // +0x17
    char unknown_18[4];
};

// Unused here: the symbol ids this declaration takes keep 0x415250's allocation.
struct Mover {
    char unknown_0[0xdc];
    int speed;                         // +0xdc
};

// The player object at the unit's +0x96.
struct Owner {
    char unknown_0[0x8c];
    float energy;                      // +0x8c
    float GetEnergy() { return energy; }
    char unknown_90[8];
    float metal;                       // +0x98
    float GetMetal() { return metal; }
    char unknown_9c[8];
    float energyCapacity;              // +0xa4
    float metalCapacity;               // +0xa8
    char unknown_ac[0x108 - 0xac];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct UnitDef {
    char unknown_0[0x14a];
    Point origin;                      // +0x14a
    char unknown_14e[0x156 - 0x14e];
    int canBuild;                      // +0x156
    char unknown_15a[0x15e - 0x15a];
    Vec3 min;                          // +0x15e
    union {
        Vec3 max;                      // +0x16a
        struct {
            char unknown_16a[4];
            union {
                int modelMaxY;         // +0x16e, max.y
                struct {
                    short unknown_16e;
                    short field_170;   // +0x170, the top half of max.y
                };
            };
            short unknown_172;
            short unknown_174;
        };
    };
    char unknown_176[0x192 - 0x176];
    int maxvelocity;                   // +0x192
    char unknown_196[0x1fa - 0x196];
    unsigned int maxHealth;            // +0x1fa
    union {
        unsigned short workerTime;     // +0x1fe
        unsigned short buildRate;
    };
    char unknown_200[0x202 - 0x200];
    short range;                       // +0x202
    char unknown_204[0x212 - 0x204];
    unsigned short buildRange;         // +0x212
    char unknown_214[0x216 - 0x214];
    unsigned short attackrunlength;    // +0x216
    char unknown_218[0x21c - 0x218];
    short altitude;                    // +0x21c
    char unknown_21e[0x22a - 0x21e];
    unsigned char capacity;            // +0x22a
    char unknown_22b[0x231 - 0x22b];
    unsigned int* weaponCategories[3]; // +0x231
    unsigned int* categories;          // +0x23d
    unsigned int flags;                // +0x241
    unsigned int flags2;               // +0x245
};

struct Feature {
    char unknown_0[0x94];
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

struct Struct_Game391e9 {
    char unknown_0[0xd3c];
    int field_d3c;                     // +0xd3c
};

struct Game {
    char unknown_0[0x1422b];
    int width;                         // +0x1422b
    int height;                        // +0x1422f
    char unknown_14233[0x1426f - 0x14233];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int overflowBucket;                // +0x142b7
    char unknown_142bb[0x1439b - 0x142bb];
    UnitDef* defs;                     // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    union {
        int ticks;                     // +0x38a47
        unsigned int tick;
    };
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Struct_Game391e9* mapInfo;         // +0x391e9
};

// The link node embedded in an order at +0x12: its +0x4 is the target unit.
class PathOrderAttach {
public:
    Unit* owner;                       // +0x4
    PathOrderAttach* next;             // +0x8
    int value;                         // +0xc
    virtual ~PathOrderAttach();
    void SetUnit(Unit* o);
};

// The link node at an order's +0x12 seen without its virtual destructor, so
// it can share a union with the plain target pointer the handlers use.
struct Link_004895c0 {
    void* vptr;                        // +0x0
    Unit* owner;                       // +0x4
    Link_004895c0* next;               // +0x8
    int value;                         // +0xc
};

struct Order {
    char unknown_0[4];
    Class_00438760 kind;               // +0x4
    unsigned char state;               // +0x5
    union {
        unsigned int flags;            // +0x6
        struct {
            unsigned int flag_0 : 1;
            unsigned int flag_1 : 1;
            unsigned int flag_2 : 1;
            unsigned int started : 1;
            unsigned int flag_rest : 28;
        };
    };
    char unknown_a[0xe - 0xa];
    Unit* source;                      // +0xe
    union {
        Link_004895c0 target;          // +0x12
        struct {
            char unknown_12[4];
            Unit* targetUnit;          // +0x16
        };
    };
    Vec3 pos;                          // +0x22
    union {
        struct { short x; short z; };  // +0x2e
        Point16 start;
    };
    char unknown_32[0x36 - 0x32];
    union {
        struct { int angle; int parity; };    // +0x36
        struct { int side; int misses; };
        struct { int time; };
        struct { int elapsed; int duration; };
        struct { int piece; };
        struct { int type; int unused; };
    };
    union {
        int range;                         // +0x3e
        int retries;
    };
    unsigned int field_42;             // +0x42
    char unknown_46[4];
    int field_4a;                      // +0x4a
    unsigned int field_4e;             // +0x4e
};

struct Unit {
    UnitMotion* type;                  // +0x0
    char unknown_4[4];
    union {
        Weapon weapons[3];             // +0x8
        struct {
            char unknown_8[8];
            WeaponDef* weapon;         // +0x10, weapons[0].def
        };
    };
    Order* order;                      // +0x5c
    char unknown_60[0x66 - 0x60];
    short heading;                     // +0x66
    char unknown_68[2];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x7e - 0x76];
    Point footprint;                   // +0x7e
    int spatialBucket;                      // +0x82
    int carrier;                      // +0x86
    Unit* cargo;                       // +0x8a
    char unknown_8e[4];
    UnitDef* def;                      // +0x92
    Owner* owner;                      // +0x96
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa6 - 0x9e];
    unsigned short category;           // +0xa6
    char unknown_a8[0xb0 - 0xa8];
    int workTime;                      // +0xb0
    char unknown_b4[0xf0 - 0xb4];
    Unit* attacker;                    // +0xf0
    unsigned char orderPlayer;         // +0xf4
    unsigned char orderKind;           // +0xf5
    char unknown_f6[0xff - 0xf6];
    unsigned char player;              // +0xff
    char unknown_100[0x104 - 0x100];
    float progress;                    // +0x104
    short health;                      // +0x108
    char unknown_10a[0x110 - 0x10a];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;
            unsigned int unknown_2 : 30;
        } bits;
    };
    void ClaimWeapons(int);
    void ReleaseWeapons(int);
    void SetStateBits(int, int);
    int CanRepair(Unit*);
    int CanReclaim(Unit*);
};
#pragma pack(pop)

class Class_0044e2d0 {
public:
    char unknown_0[0x36];
    Class_0044e2d0(Order* order, const Vec3& pos);
};

class Class_0044e190 {
public:
    char unknown_0[0x36];
    Class_0044e190(Order* order, Unit* target);
};

class Class_0044e250 {
public:
    char unknown_0[0x36];
    Class_0044e250(Order* order, Unit* target, int value);
};

class Class_0044e330 {
public:
    char unknown_0[0x36];
    Class_0044e330(Order* order, Unit* unit, const Vec3& p);
};

class AirManeuverOrder {
public:
    char unknown_0[0x2c];
    AirManeuverOrder(Order* order, const Vec3& a, const Vec3& b);
    void SetAltitude(int);
};

class Class_004388d0 { public: void SetAttachedFx(int); };
class Class_00438880 { public: void AnnounceStatusIfFlagged(const char*); };
class Class_00438930 { public: void AttachApproachRadiusGoal(Vec3*, int); };
class Class_00439e80 { public: void SetDeadlineTicks(int); };
class Class_0044e6c0 { public: void SetAltitude(int); };
class Class_0044e730 { public: void SetApproachRadius(int); };
class Class_0044e720 { public: void SetHeading(int); };
class Class_00438ad0 { public: void AttachBuildFootprintMarker(Point, Point); };

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, Vec3* pos, int c, int d, int e);
};
#pragma pack(pop)

// The visitor passed to VisitObjectsInRange by VtolSeekGuardOrder (vtable
// 0x4fcc60); its constructor is the inline one 0x410850 builds.
class GroundAllyVisitor {
public:
    virtual void CollectGroundAlly(Unit*);
    GroundAllyVisitor(Owner* o, std::vector<Unit*>* u, Unit* s) : owner(o), units(u), self(s) {}
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
};

// The visitor passed to VisitObjectsInRange by VtolRepairPatrolOrder (vtable
// 0x4fcc64).
class RepairableUnitVisitor {
public:
    virtual void CollectRepairableUnit(Unit*);
    RepairableUnitVisitor(Owner* o, std::vector<Unit*>* v, Unit* s) : owner(o), units(v), self(s) {}
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
};

// The landing-pad list: a vector<Unit*> whose constructor is 0x410830. It
// stays out of line (the original calls it) even though it is defined here.
class LandingPadList : public std::vector<Unit*> {
public:
    LandingPadList();
};

extern Game* g_game;

// External functions the handlers call.
int __stdcall RandomInt(int);
int __cdecl FUN_004b70ef(short, int);
int __cdecl FUN_004b7123(short, int);
short __cdecl FUN_004b715a(int, int);
int __stdcall GetHeadingBetween(Vec3*, Vec3*);
int __stdcall GetGroundHeight(Vec3*);
int __stdcall CanPlaceFootprintAt(Unit*, Vec3*);
void __stdcall AttachUnitToPiece(Unit*, Unit*, int, int);
void __stdcall QueueUnitSpeech(Unit*, int, const char*);
void __stdcall GetFactoriesInRadius(int, Vec3*, int, std::vector<Unit*>*);
Unit* __stdcall FindBestTargetIfFireAtWill(Unit*);
int __stdcall IssueAttackOrder(Unit*, Unit*, int);
int __stdcall IssueRepairOrder(Unit*, Unit*, int);
void __stdcall AppendOrder(Unit*, Class_0043a1f0*);
void __stdcall AppendOrderToTail(Unit*, Class_0043a1f0*);
void __stdcall EnsurePatrolReturnOrder(Unit*, Order*);
Class_00438760 __stdcall GetOrderType(unsigned char, Unit*, Unit*, int);
Unit* __stdcall GetWeaponTargetUnit(Unit*, int);
int __stdcall WeaponCanReachUnit(Unit*, Unit*, unsigned char);
void __stdcall SetWeaponTargetUnit(Unit*, Unit*, int);
void __stdcall SetWeaponTargetPos(Unit*, Vec3*, int);
void __stdcall ClearWeaponTarget(Unit*, int);
int __stdcall IsPadSlotFree(Unit*, int);
int __stdcall CanPlaceUnitFootprint(UnitDef*, int, Point, int);
void __stdcall SnapWorldPosToFootprint(UnitDef*, Vec3*);
Unit* __stdcall CreateUnit(unsigned char, short, Vec3, int, int, int);
void __stdcall AddOrder(Class_00438760, int, Unit*, Unit*, Vec3*, int, int);
void __stdcall StartBuildingScript(Unit*, Order*, short);
int __stdcall WaitIfNotInBuildStance(Unit*, Order*, int);
int __stdcall AddBuildProgress(Unit*, Unit*, float);
void __stdcall GetNanoPiecePosition(Unit*, Vec3*);
void __stdcall EmitNanoParticles(Vec3*, Box*, int);
void __stdcall EmitReverseNanoParticles(Box*, Vec3*, int);
void __stdcall ReclaimFeature(Unit*, Vec3*);
unsigned short __stdcall FindFeatureAtPos(Vec3*, Point16*, Point16*);
int __stdcall ComputeReclaimDamagePulse(Unit*, Unit*, int);
void __stdcall DamageUnit(Unit*, Unit*, int, int, int);
void __stdcall MarkSelectionOrdersDirty(Unit*);
int __stdcall AddRepairProgress(Unit*, Unit*, float);
int __stdcall PickRandomReclaimableResourcesInRadius(Vec3*, Fixed, Vec3**, float*, Vec3**, float*);
void __stdcall VisitObjectsInRange(Vec3*, int, const GroundAllyVisitor&);
void __stdcall VisitObjectsInRange(Vec3*, int, const RepairableUnitVisitor&);
int __stdcall SendScriptCallByName(Unit*, char*, char, int, int, int, int);
Vec3 __stdcall GetPieceOffset(Unit*, int);
// Unused here: the symbol ids these declarations take keep the allocation
// (docs/c2-regalloc.md).
int __stdcall IsUnitCommander(Unit*);
void __stdcall AlignUnitToGround(Unit*);
unsigned short __stdcall ChooseBuildOption(unsigned int, Unit*);
void __stdcall DetonateUnitWeapon(Unit*, int);
void __stdcall DrawUnit(void*, Unit*);
void __stdcall KillUnit(Unit*, int);

// The functions of this file that the handlers below call before their
// definitions.
Vec3 __stdcall AddVec3(const Vec3& a, const Vec3& b);
Vec3 __stdcall DirectionFromAngle(short angle, Fixed scale);
int __stdcall FindLandingPad(Unit* unit, int pad);
void __stdcall CellToWorldPos(Point a, Vec3* out, Point b);

// The fixed-point direction of a heading and a distance.
static inline Vec3 Direction(short angle, int range)
{
    Fixed distance;
    distance.value = range;
    return DirectionFromAngle(angle, distance);
}

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

// The body 0x410e70 inlines where the original did (AddVec3 is out of
// line everywhere else).
static inline Vec3 AddVectors(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

static inline Vec3 Add(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}


// Adds the components one at a time, unlike operator+, which copies the
// whole vector first. Used at the one site whose original schedule consumes
// off.x before it loads target->pos.y.
static inline Vec3 AddComponentwise(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x;
    r.x += b.x;
    r.y = a.y;
    r.y += b.y;
    r.z = a.z;
    r.z += b.z;
    return r;
}

static inline Point WorldToCell(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - (origin.x << 19) + 0x80000) >> 20;
    c.y = (v.z - (origin.y << 19) + 0x80000) >> 20;
    return c;
}

// The multiply, not a shift by 19: the shift changes the register use.
static inline Point WorldToCellMul(Vec3 v, Point origin)
{
    Point c;
    c.x = (v.x - origin.x * 0x80000 + 0x80000) >> 20;
    c.y = (v.z - origin.y * 0x80000 + 0x80000) >> 20;
    return c;
}

static inline void CellToWorld(Point origin, Point c, Vec3* v)
{
    v->x = (origin.x + c.x * 2) << 19;
    v->z = (origin.y + c.y * 2) << 19;
}

static inline void Snap(Vec3* v, Point size)
{
    Point c = WorldToCellMul(*v, size);
    CellToWorld(size, c, v);
}

static inline int Contains(unsigned int* bits, unsigned short index)
{
    return bits[index >> 5] & (1 << (index & 31));
}

// Empty inline call sites: they use up the inline budget so the vector
// destructor sites call _Destroy out of line or inline it, as the original.
static inline void Dummy(void) {}

static inline int IsDamaged(Unit* u)
{
    return (unsigned int)u->health < (u->def->maxHealth >> 2) * 3;
}

static inline int GetSpeed(Unit* unit)
{
    return unit->weapons[0].def->range;
}

// Used on the target argument of the order-steal calls: the plain chain
// changes codegen.
static inline Unit* OrderTarget(Order* order)
{
    return order->target.owner;
}

// Stops a unit (ClaimWeapons(3), AttachUnitToPiece when +0x86 is set, then
// SetStateBits(1, 1)); for units whose type has mode 1 in the low bits of
// +0x2e, also attaches a new Class_0044e2d0 at the unit's position to the
// order and sets flags on it.
// FUNCTION: 0x40f200
void __stdcall PrepVtolClimb(Unit* unit, Order* order, unsigned int flags)
{
    unit->ClaimWeapons(3);
    if (unit->carrier)
        AttachUnitToPiece(unit, 0, -1, 2);
    unit->SetStateBits(1, 1);
    if ((unit->type->flags & 3) == 1) {
        unit->type->SetFlightMode(unit, 2);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude / 2);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags |= flags | 0xe0;
    }
}

// Order handler that ends a transport (the unit script's "EndTransport").
// When unit->spatialBucket equals g_game->overflowBucket the unit heads towards the
// map centre instead. State 0 prepares the order, state 1 tries the unit's
// own spot, then twelve random nearby cells snapped to the map grid, then a
// point circling the order's position; state 2 finishes.
// FUNCTION: 0x40f2a0
int __stdcall VtolLandIfCanOrder(Unit* unit, Order* order, int flags)
{
    if (order->field_4a)
        return 5;
    if (flags & 0x40)
        return 5;
    if (unit->spatialBucket == g_game->overflowBucket) {
        Vec3 centre;
        centre.x = g_game->width / 2 << 16;
        centre.z = g_game->height / 2 << 16;
        short angle = GetHeadingBetween(&unit->pos, &centre);
        Vec3 dest = AddVec3(unit->pos, Offset(angle, 0x3200000));
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->SetApproachRadius(0x80);
        order->flags |= 0xe0;
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        return 2;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            if (order->pos.x == 0 && order->pos.z == 0 && order->pos.y == 0)
                order->pos = unit->pos;
            int a = RandomInt(0x10000);
            order->angle = a;
            order->parity = a & 1;
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        if (CanPlaceFootprintAt(unit, &unit->pos)) {
            unit->script->StartScript("EndTransport", 0, 1);
            Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
            int h = max(GetGroundHeight(&unit->pos), g_game->seaLevel);
            ((Class_0044e6c0*)obj)->SetAltitude(h <= g_game->seaLevel ? GetGroundHeight(&unit->pos) - g_game->seaLevel : 0);
            ((Class_004388d0*)order)->SetAttachedFx((int)obj);
            order->flags = 0xe0;
            unit->SetStateBits(1, 0);
            return 1;
        }
        for (int r = 0x40; r < 0x100; r += 0x10) {
            Vec3 p = unit->pos;
            p.x += (RandomInt(r * 2 + 1) - r) << 16;
            p.z += (RandomInt(r * 2 + 1) - r) << 16;
            Point fp = unit->footprint;
            Point cell = WorldToCellMul(p, fp);
            CellToWorld(fp, cell, &p);
            if (CanPlaceFootprintAt(unit, &p)) {
                ((Class_004388d0*)order)->SetAttachedFx((int)new Class_0044e2d0(order, p));
                order->flags = 0xe0;
                return 2;
            }
        }
        // The (unsigned char) cast selects the original's flags test.
        if ((unsigned char)flags & 0xe0)
            order->angle -= 0x5555;
        Vec3 off = Offset(order->angle, 0xa00000);
        Vec3 sum;
        sum.x = order->pos.x + off.x;
        sum.y = order->pos.y + off.y;
        sum.z = order->pos.z + off.z;
        // Summed into a temporary then copied: keeps all three sums live at once.
        Vec3 dest = sum;
        Class_0044e2d0* obj = new Class_0044e2d0(order, dest);
        ((Class_0044e730*)obj)->SetApproachRadius(0x40);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags |= 0xe0;
        return 2;
    }
    case 2:
        if (flags & 0x20) {
            unit->type->SetFlightMode(unit, 1);
            return 5;
        }
        return 8;
    }
    return 7;
}

// Adds two integer 3-vectors and returns the sum by value.
#pragma auto_inline(off)
#pragma auto_inline(on)

// Order handler for an aircraft waiting in place: state 0 stops the unit and
// records the order unit's position in whole map units, state 1 ends the wait
// when FindBestTargetIfFireAtWill finds a unit that IssueAttackOrder accepts, and state 2 either
// queues VTOL_LANDIFCAN or moves to a random point near the recorded spot
// before waiting again.
// FUNCTION: 0x40f7d0
int __stdcall VtolStandbyOrder(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            unit->ClaimWeapons(3);
            order->flags |= 0x10000;
            ((Class_00439e80*)order)->SetDeadlineTicks(1);
            order->x = order->source->pos.xw;
            order->z = order->source->pos.zw;
            return 1;
        }
        break;
    case 1: {
        Unit* other = FindBestTargetIfFireAtWill(unit);
        if (other && IssueAttackOrder(unit, other, 0)) {
            order->flags = 0;
            order->state = 0;
            return 3;
        }
        return 1;
    }
    case 2:
        if ((unit->def->flags & 0x800) && (unit->flags & 3) == 2) {
            if (unit->cargo) {
                short angle = RandomInt(0x10000);
                Vec3 p;
                p.x = order->x << 16;
                p.y = 0;
                p.z = order->z << 16;
                int distance = (RandomInt(0x20) + 8) << 16;
                p += Offset(angle, distance);
                Class_0044e2d0* obj = new Class_0044e2d0(order, p);
                ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude);
                ((Class_004388d0*)order)->SetAttachedFx((int)obj);
                ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(0xf) + 0x1e);
                order->state = 1;
                return 2;
            }
            AppendOrder(unit, new Class_0043a1f0(Class_00438760("VTOL_LANDIFCAN"), 0, &order->pos, 0, 0, 0));
            return 5;
        }
        order->flags |= 0x10000;
        ((Class_00439e80*)order)->SetDeadlineTicks(RandomInt(0x1e) + 0x1e);
        order->state = 1;
        return 2;
    }
    return 7;
}

// Order handler: state 0 prepares the order (PrepVtolClimb), state 1 snaps
// the order's position to the map grid for the unit's footprint and heads
// there, state 2 finishes.
// FUNCTION: 0x40fa20
int __stdcall VtolMoveOrder(Unit* unit, Order* order, int flags)
{
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        break;
    case 1:
        ((Class_00438880*)order)->AnnounceStatusIfFlagged(0);
        unit->ReleaseWeapons(3);
        Snap(&order->pos, unit->footprint);
        ((Class_004388d0*)order)->SetAttachedFx((int)new Class_0044e2d0(order, order->pos));
        order->flags = 0xe0;
        return 1;
    case 2:
        if (!order->field_4a)
            QueueUnitSpeech(unit, 6, 0);
        return 5;
    }
    return 7;
}

// VTOL guard order handler ("Guarding"): the aircraft circles the guarded
// unit, fights back against its attacker, repairs it, and takes over the
// guarded unit's own build or reclaim order (as the VTOL_ variant of that
// order). The ground twin is 0x406300.
// Builds the horizontal vector (-f1(angle, scale), 0, -f2(angle, scale))
// from the fixed-point trig helpers. It stays out of line: every site calls it.
#pragma auto_inline(off)
// FUNCTION: 0x4103a0
Vec3 __stdcall DirectionFromAngle(short angle, Fixed scale)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, scale.value);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale.value);
    return v;
}
#pragma auto_inline(on)

// The `push ecx` reserves a 4-byte local (guide: "push ecx as the first
// instruction usually just reserves stack space"); the compiler never
// initialises it before its one byte is copied into the vector's allocator
// byte. It stays out of line: the original calls it from 0x4103e0 and
// 0x410850.
#pragma auto_inline(off)
// FUNCTION: 0x410830
LandingPadList::LandingPadList()
{
    unsigned char local;
    ((unsigned char*)this)[0] = local;
    _First = 0;
    _Last = 0;
    _End = 0;
}
#pragma auto_inline(on)

// The include set (the system headers, then ta_types.h in namespace ta) fixes
// unit's front-end id, which decides the register order of the health test.
// VtolSeekGuardOrder and its Patrol helper; the vector calls inline or not
// according to the original, so IsDamaged, FindPads and SearchRange stay
// small helpers.
class UnitList : public std::vector<Unit*> {};

static inline void FindPads(Unit* u, std::vector<Unit*>* pads)
{
    GetFactoriesInRadius(u->owner->index,&u->pos,0xf00,pads);
}

static inline int SearchRange(Unit* u)
{
    return u->def->range<<16;
}

static inline int Patrol(Unit* unit, Order* order, int flags)
{
    if (IsDamaged(unit)) {
        LandingPadList pads;
        FindPads(unit,&pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->SetAttachedFx(0);
            Unit* pad=pads[RandomInt(pads.size())];
            AppendOrder(unit,new Class_0043a1f0("VTOL_LANDING",pad,0,0,0,0));
            order->flags=0;
            return 0;
        }
    }
    UnitList units;
    int range=SearchRange(unit);
    GroundAllyVisitor visitor(unit->owner,&units,unit);
    VisitObjectsInRange(&unit->pos,range,visitor);
    if (!units.empty()) {
        ((Class_004388d0*)order)->SetAttachedFx(0);
        Class_00438760 kind=GetOrderType(7,unit,units[0],0);
        AppendOrder(unit,new Class_0043a1f0(kind,units[0],0,0,0,0));
        order->flags=0;
        return 3;
    }
    if (flags&0xe0) order->angle+=-RandomInt(0x2000)-0x4000;
    Vec3 pos=Add(order->pos,Offset((short)order->angle,(unit->weapons[0].def->range+160)<<16));
    Class_0044e2d0* move=new Class_0044e2d0(order,pos);
    ((Class_0044e730*)move)->SetApproachRadius(128);
    ((Class_004388d0*)order)->SetAttachedFx((int)move);
    ((Class_00439e80*)order)->SetDeadlineTicks(30);
    order->flags|=0xf8;
    return 2;
}

// Unit visitor (vtable 0x4fcc60, built on the stack by 0x410850 and passed to
// VisitObjectsInRange): collects every unit whose owner is allied with this owner,
// whose def lacks flag 0x800, and that is not the visitor's own unit. The
// same shape as DamagedAllyCollector (0x405d90), with the vector::push_back inlined.
// VTOL patrol order handler ("Patrolling"). State 0 prepares the order
// (PrepVtolClimb), state 1 clears the order's 0xe0 bits, state 2 flies to a
// point 0x140 units away along the heading to the order's position, lands on a
// free pad when damaged (VTOL_LANDING, as in 0x412710), or takes the next
// queued order.
// Kept: the flags/health block's register allocation depends on this include.
// VTOL transport (air lift) order handler. Fails when there is no target,
// with flags 0x10048, or when the target sits below sea level. State 0
// prepares the order ("Loading"; PrepVtolClimb), state 1 flies to the target,
// state 2 asks the script for the attach piece (QueryTransport), state 3
// starts BeginTransport and hovers down to the piece's height, state 4 ends
// the pickup (EndTransport when cancelled) and state 5 finishes.
// The target's top (its def's max.y) is at or below the sea. An inline
// helper that re-reads order->target: written inline, MSVC keeps target->def
// in ecx for state 3 and every register choice changes.
static inline int BelowSeaLevel(Order* order)
{
    Unit* target = order->target.owner;
    return target->def->modelMaxY + target->pos.y <= g_game->seaLevel << 16;
}

// Order handler that unloads the unit a transport carries ("Unloading").
// State 0 targets the cargo and heads for the order's position, state 1
// checks the cargo fits there and moves on, state 2 checks again, runs the
// script's "EndTransport" and drops the cargo, state 3 reports and ends.
// FUNCTION: 0x411560
int __stdcall VtolUnloadOrder(Unit* unit, Order* order, int flags)
{
    if (!unit->cargo)
        return 5;
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Unloading");
            ((PathOrderAttach*)&order->target)->SetUnit(unit->cargo);
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude);
            ((Class_0044e730*)obj)->SetApproachRadius(0x140);
            ((Class_004388d0*)order)->SetAttachedFx((int)obj);
            order->flags = 0xe8;
            return 1;
        }
        break;
    case 1: {
        Unit* cargo = order->target.owner;
        if (CanPlaceUnitFootprint(cargo->def, 0, WorldToCell(order->pos, cargo->footprint), 1)) {
            Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
            // Through an int local: passing the expression straight sign-extends differently.
            int h = unit->cargo->def->field_170;
            ((Class_0044e6c0*)obj)->SetAltitude(h);
            ((Class_004388d0*)order)->SetAttachedFx((int)obj);
            order->flags = 0xe8;
            return 1;
        }
        QueueUnitSpeech(unit, 7, "Unable to unload unit");
        return 9;
    }
    case 2: {
        if (flags & 0x40)
            return 9;
        Unit* cargo = order->target.owner;
        if (!CanPlaceUnitFootprint(cargo->def, 0, WorldToCell(order->pos, cargo->footprint), 1)) {
            QueueUnitSpeech(unit, 7, "Unable to unload unit");
            return 9;
        }
        unit->script->StartScript("EndTransport", 0, 0);
        AttachUnitToPiece(unit->cargo, 0, -1, 1);
        Class_0044e2d0* obj = new Class_0044e2d0(order, unit->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0xe0;
        return 1;
    }
    case 3:
        QueueUnitSpeech(unit, 0xd, 0);
        return 5;
    }
    return 7;
}

// Returns `pad` if it is usable, otherwise the first usable pad the unit's
// script reports from QueryLandingPad, or -1.
// FUNCTION: 0x411840
int __stdcall FindLandingPad(Unit* unit, int pad)
{
    if (pad != -1 && IsPadSlotFree(unit, pad)) {
        return pad;
    }
    int pads[4];
    pads[0] = -1;
    pads[1] = -1;
    pads[2] = -1;
    pads[3] = -1;
    unit->script->QueryScript("QueryLandingPad", &pads[0], &pads[1], &pads[2], &pads[3]);
    for (int i = 0; i < 4; i++) {
        if (pads[i] != -1 && IsPadSlotFree(unit, pads[i])) {
            return pads[i];
        }
    }
    return -1;
}

// "Landing" order handler of air units landing on a pad unit (the order's
// target). State 0 prepares the order (PrepVtolClimb) and picks a random
// angle, state 1 looks for a free pad (FindLandingPad) and circles the pad
// unit until one is free, states 2 to 5 approach the pad and land, and state
// 6 hands the unit (or its cargo) to the pad and starts a "SELFREPAIR" order
// when the pad repairs and the unit is damaged.
// Kept: some header set is needed here or the state 6 health compare changes.
// "Attacking" order handler of aircraft (VTOL). Interrupts hand over to a
// "VTOL_SEEKATTACK" order; the order follows its target unit and gives up
// outside its range. State 0 prepares the order (PrepVtolClimb), states 1 and
// 2 make attack runs past the target, state 4 turns around after a pause,
// state 5 aims at the target again and state 6 flies on and, when the unit is
// below three quarters of its health, sends it to a random repair pad
// ("VTOL_LANDING").
// VTOL attack order handler ("Attacking"). With flags 0x1000a, or with no
// target and order flag 0x200, it queues VTOL_SEEKATTACK instead; when out of
// the order's range it gives up. State 0 prepares the order (PrepVtolClimb),
// state 1 flies to a random point halfway to the target, state 2 attacks,
// state 3 pulls away from the target, state 4 lands on a free pad when damaged
// (VTOL_LANDING) or circles.
// Kept: the health test's registers follow the file's symbol count, which the
// IsDamaged helper and this include set fix.
// Stays an inline helper: its locals then share stack slots with the later ones.
static inline int IsAhead(Unit* unit, Order* order)
{
    Vec3 toward = Offset(GetHeadingBetween(&unit->pos, &order->target.owner->pos), 0x140000);
    Fixed dist;
    dist.value = 0x140000;
    Vec3 facing = DirectionFromAngle(unit->heading, dist);
    return (short)(toward.xw * facing.xw + toward.zw * facing.zw) > 0;
}

// VTOL strafing attack order handler. With flags 0x10008, or with no target
// and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge player it
// heads for the map centre. State 0 prepares the order ("Attacking";
// PrepVtolClimb). State 1 attacks:
// when the target lies ahead (IsAhead) it makes a strafing run, otherwise it
// circles, leading the target by its velocity, and after 90 ticks of that
// it queues VTOL_EVADE.
// VTOL attack order handler for a unit target. With flags 0x10008, or with
// no target and order flag 0x200, it queues VTOL_SEEKATTACK; on the map-edge
// player it heads for the map centre. State 0 prepares the order
// ("Attacking"; PrepVtolClimb), state 1 flies to a random point halfway to
// the target, state 2 attacks, state 3 circles the target, alternating
// sides, and lands on a free pad when damaged (VTOL_LANDING).
//
// Suspected original bug: that same branch builds a Class_0044e2d0 waypoint
// and sets its speed, but never passes it to the order (no SetAttachedFx
// call, unlike every other branch), so the object leaks.
// FUNCTION: 0x413bc0
int __stdcall VtolEvadeOrder(Unit* unit,Order* order,int flags)
{
    int range=unit->weapons[0].def->range;
    if (order->targetUnit && !(flags&0x10008)) {
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0:
        if (unit->type && (unit->def->flags&0x800)) {
            order->side=RandomInt(2);
            Vec3 pos;
        if (order->side) pos=unit->pos+Offset(unit->heading-0x4000,range<<16);
        else pos=unit->pos+Offset(unit->heading+0x4000,range<<16);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e730*)move)->SetApproachRadius(128);
            ((Class_004388d0*)order)->SetAttachedFx((int)move);
            order->flags=0x100e8;
            return 1;
        }
        goto invalid;
    case 1: {
        Vec3 pos;
        if (order->side) pos=unit->pos+Offset(unit->heading-0x4000,range<<17);
        else pos=unit->pos+Offset(unit->heading+0x4000,range<<17);
        Class_0044e2d0* move=new Class_0044e2d0(order,pos);
        ((Class_0044e730*)move)->SetApproachRadius(128);
        ((Class_004388d0*)order)->SetAttachedFx((int)move);
        order->flags=0x100e8;
        return 1;
    }
    case 2: break;
    default: invalid: return 7;
    }
    }
    return 5;
}

// FUNCTION: 0x413d80
int __stdcall VtolMobileBuildOrder(Unit* unit,Order* order,int flags)
{
    if (flags&2) { MarkSelectionOrdersDirty(unit); return 5; }
    if (flags&8) { QueueUnitSpeech(unit,7,"Construction terminated"); MarkSelectionOrdersDirty(unit); return 8; }
    unsigned int state=0; state=order->state;
    switch(state) {
    case 0:
        if (unit->type && (unit->def->flags&0x800)) {
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Building");
            unit->ClaimWeapons(3);
            if (unit->carrier) AttachUnitToPiece(unit,0,-1,2);
            unit->SetStateBits(1,1);
            if ((unit->type->flags&3)==1) {
                unit->type->SetFlightMode(unit,2);
                Class_0044e2d0* move=new Class_0044e2d0(order,unit->pos);
                ((Class_0044e6c0*)move)->SetAltitude(unit->def->altitude/2);
                ((Class_004388d0*)order)->SetAttachedFx((int)move);
                order->flags|=0xe0;
            }
            return 1;
        }
        break;
    case 1: {
        UnitDef* def=&g_game->defs[order->type];
        order->retries=0;
        Point origin=def->origin;
        Point cell=WorldToCell(order->pos,origin);
        CellToWorldPos(cell,&order->pos,origin);
        Class_0044e2d0* move=new Class_0044e2d0(order,order->pos);
        ((Class_0044e730*)move)->SetApproachRadius(unit->def->buildRange);
        ((Class_004388d0*)order)->SetAttachedFx((int)move);
        order->flags=0xe0;
        return 1;
    }
    case 2: {
        if (flags&0x40) return 8;
        UnitDef* def=&g_game->defs[order->type];
        if (!CanPlaceUnitFootprint(def,0,WorldToCell(order->pos,g_game->defs[order->type].origin),1)) {
            if (!order->retries) QueueUnitSpeech(unit,7,"Waiting for target area to clear");
            else if (order->retries>10) { QueueUnitSpeech(unit,7,"Target area was blocked"); return 8; }
            ++order->retries;
            ((Class_00439e80*)order)->SetDeadlineTicks(30);
            return 2;
        }
        SnapWorldPosToFootprint(def,&order->pos);
        ((PathOrderAttach*)&order->target)->SetUnit(CreateUnit(unit->player,(short)order->type,order->pos,0,1,0));
        if (!order->targetUnit) { QueueUnitSpeech(unit,7,"Unable to create any more units"); return 8; }
        QueueUnitSpeech(unit,9,"Starting construction");
        AddOrder("GETBUILT",1,order->targetUnit,unit,0,0,0);
        StartBuildingScript(unit,order,FUN_004b715a(unit->pos.x-order->targetUnit->pos.x,unit->pos.z-order->targetUnit->pos.z)-unit->heading);
        MarkSelectionOrdersDirty(unit);
        return 1;
    }
    case 3:
        WaitIfNotInBuildStance(unit,order,10);
    case 4: {
        if (g_game->tick%150==0) {
            int angle=GetHeadingBetween(&unit->pos,&order->targetUnit->pos);
            int range=unit->def->buildRange<<16;
            angle+=0xdb6e;
            Vec3 pos=order->targetUnit->pos-Offset(angle,range);
            Class_0044e2d0* move=new Class_0044e2d0(order,pos);
            ((Class_0044e720*)move)->SetHeading((unsigned short)angle);
            ((Class_004388d0*)order)->SetAttachedFx((int)move);
        }
        int rate=0; rate=unit->def->buildRate;
        if (AddBuildProgress(unit,order->targetUnit,(float)(rate/30))) {
            Vec3 start;
            GetNanoPiecePosition(unit,&start);
            Vec3 bounds[2];
            bounds[0]=order->targetUnit->pos+order->targetUnit->def->min;
            bounds[1]=order->targetUnit->pos+order->targetUnit->def->max;
            EmitNanoParticles(&start,(Box*)bounds,6);
        }
        if (order->targetUnit->progress!=0.0f) {
            ((Class_00439e80*)order)->SetDeadlineTicks(1);
            order->flags|=0xa;
            return 2;
        }
        return 1;
    }
    case 5:
        QueueUnitSpeech(unit,8,"Building complete");
        return 5;
    }
    return 7;
}

// Writes the cell corner (b + 2*a) back as a world position. It stays out
// of line: every site calls it.
#pragma auto_inline(off)
// FUNCTION: 0x414350
void __stdcall CellToWorldPos(Point a, Vec3* out, Point b)
{
    out->x = (b.x + a.x * 2) << 19;
    out->z = (b.y + a.y * 2) << 19;
}
#pragma auto_inline(on)

// Kept: the operand order of the six bounds adds follows symbol ids, so this
// header set matters.
// <stdio.h> (or another header, see tools/headers.py) is needed only for
// compiler state: without it the energy + metal sum loads metal first.
// Order handler "Reclaiming" (second variant, driven by a Class_0044e2d0
// move object rather than the turn/approach states of 0x404ad0).
// Order handler "Reclaiming" for a unit target (the hovering variant of
// 0x404730): state 0 checks the builder can reclaim, state 1 snaps the order
// position to the target's cell and moves over it, state 2 drains the target
// while it stays in build range.
// Order handler "Repairing" (the hovering variant of 0x405300): the order
// position follows the target, state 1 moves over it, state 2 spends worker
// time on it until its health is full.
// FUNCTION: 0x414e70
int __stdcall VtolRepairUnitOrder(Unit* unit, Order* order, int flags)
{
    if (!order->targetUnit) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 8;
    }
    if (order->range && (int)_hypot(unit->pos.xw - order->x, unit->pos.zw - order->z) >= order->range)
        return 5;
    if (order->targetUnit->bits.mode != 1) {
        QueueUnitSpeech(unit, 7, "Repairs unsuccessful.");
        return 5;
    }
    // Copied through a pointer local, not `order->pos = ...`: changes what stays in ebx.
    Vec3* dst = &order->pos;
    *dst = order->targetUnit->pos;
    switch (order->state) {
    case 0:
        if (unit->type && (unit->def->flags & 0x800)) {
            if (!unit->CanRepair(order->targetUnit)) {
                QueueUnitSpeech(unit, 7, "Repair mission failed");
                return 8;
            }
            ((Class_00438880*)order)->AnnounceStatusIfFlagged("Repairing");
            PrepVtolClimb(unit, order, 0);
            return 1;
        }
        break;
    case 1: {
        Class_0044e2d0* obj = new Class_0044e2d0(order, order->pos);
        ((Class_0044e6c0*)obj)->SetAltitude(unit->def->altitude);
        ((Class_004388d0*)order)->SetAttachedFx((int)obj);
        order->flags = 0xe8;
        return 1;
    }
    case 2:
        if (flags & 0x40)
            return 8;
        if (order->targetUnit->bits.mode != 1)
            return 8;
        if (order->targetUnit->flags & 0xc) {
            ((Class_00439e80*)order)->SetDeadlineTicks(15);
            return 0;
        }
        if ((unsigned int)order->targetUnit->health >= order->targetUnit->def->maxHealth)
            return 1;
        {
            AddRepairProgress(unit, order->targetUnit, (float)(unit->def->workerTime / 30));
            Vec3 nano;
            GetNanoPiecePosition(order->source, &nano);
            Box box;
            box.hi = order->targetUnit->pos;
            box.lo = order->targetUnit->pos;
            box.lo.x += order->targetUnit->def->min.x;
            box.lo.z += order->targetUnit->def->min.z;
            box.hi.x += order->targetUnit->def->max.x;
            box.hi.z += order->targetUnit->def->max.z;
            box.hi.y += order->targetUnit->def->max.y;
            EmitNanoParticles(&nano, &box, 6);
        }
        ((Class_00439e80*)order)->SetDeadlineTicks(1);
        order->flags |= 8;
        return 2;
    case 3:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    }
    return 7;
}

// Repair order step: report an aborted or finished repair, or keep repairing
// while the unit's health is below its type's maximum.
// FUNCTION: 0x415250
int __stdcall VtolGetRepairedOrder(Unit* unit, Order* order, int unused)
{
    if (order->targetUnit == 0) {
        QueueUnitSpeech(unit, 7, "Repair aborted.");
        return 8;
    }
    switch (order->state) {
    case 0:
        if (unit->health >= unit->def->maxHealth)
            return 1;
        ((Class_00439e80*)order)->SetDeadlineTicks(0x1e);
        order->started = 1;
        return 2;
    case 1:
        QueueUnitSpeech(unit, 10, "Unit repaired");
        return 5;
    default:
        return 7;
    }
}

static inline int Land(Unit* unit, Order* order)
{
    if ((unsigned int)unit->health < (unit->def->maxHealth >> 2) * 3) {
        std::vector<Unit*> pads;
        GetFactoriesInRadius(unit->owner->index, &unit->pos, 0xf00, &pads);
        if (!pads.empty()) {
            ((Class_004388d0*)order)->SetAttachedFx(0);
            Unit* pad = pads[RandomInt(pads.size())];
            AppendOrder(unit, new Class_0043a1f0("VTOL_LANDING", pad, 0, 0, 0, 0));
            order->flags = 0;
            return 1;
        }
    }
    // Empty statement after the health test folds the landed test away.
    do {} while (0);
    return 0;
}

static inline float Total(float base, float amount)
{
    float value = base;
    value += amount;
    return value;
}

// VTOL patrol order handler for construction aircraft. State 0 starts
// patrolling ("Patrolling"; PrepVtolClimb). State 1 sets the next waypoint,
// then lands on a free pad when damaged (VTOL_LANDING), helps build or guards
// a unit it can see (VTOL_HELPBUILD), or reclaims metal or energy
// (VTOL_RECLAIM).
// Visitor used by VtolRepairPatrolOrder (vtable 0x4fcc64): collects allied units
// whose flags & 3 == 1 that are damaged or still being built and are not
// already running this player's order kind 5. A near copy of
// DamagedAllyCollector::CollectDamagedAlly.
// Some header must be included here: without one the def and health loads swap.
// FUNCTION: 0x4158d0
void RepairableUnitVisitor::CollectRepairableUnit(Unit* unit)
{
    if (unit == self) return;
    unsigned int index = 0;
    index = unit->owner->index;
    if (!owner->allied[index]) return;
    unsigned int kind = unit->flags & 3;
    if ((unsigned char)kind != 1) return;
    if (unit->health >= unit->def->maxHealth && unit->progress == 0.0f) return;
    if (unit->orderPlayer == owner->index && unit->orderKind == 5) return;
    units->push_back(unit);
}
