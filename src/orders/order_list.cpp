// Decompiled by space-bunny-free, Space Bunny Free, Sonnet, Opus, Haiku, Claude Sonnet 5.5, Claude Opus 5.5, DeepSeek V4.1 Flash, GPT-6.1-sol, GPT-6, deepseek-v4.1, deepseek-v4.1-flash and mimo-v2.6-pro. Names are provisional.
// The unit's order list: the nodes (Order) linked through the unit's
// two lists at +0x5c and +0x60, their creation, insertion, deletion, and the
// dispatcher that walks the list and calls one helper per kind flag bit; then
// adding, removing and driving the command nodes in the two lists, the global
// order-type table at 0x512340 and its sort, and the unit's mover (UnitMotion).
// The sort's five out-of-line instantiations stay in files of their own:
// 0x43c720 (order_list_43c720.cpp), 0x43c940, 0x43c990, 0x43ca70 and
// 0x43cb20, because the two registration functions (0x43bc90 and 0x43c050)
// need the inlined sort here, and its inline chain defines the same functions.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include "../util/hapi_bank.h"

#pragma pack(push, 1)

struct FP_0043d6d0 {
    unsigned int frac : 16;
    int whole : 16;
};

union Fixed_0043d6d0 {
    int value;
    FP_0043d6d0 parts;
};

static inline Fixed_0043d6d0 MakeFixed_0043d6d0(int i)
{
    Fixed_0043d6d0 f;
    f.parts.frac = 0;
    f.parts.whole = i;
    return f;
}

inline int operator>(const Fixed_0043d6d0& a, const Fixed_0043d6d0& b) { return a.value > b.value; }

#define MAXM_0043d6d0(a, b) ((a) > (b) ? (a) : (b))

static inline void MulFixed(int& v, int f)
{
    v = (int)(((__int64)v * f) >> 16);
}

// A 16.16 position or velocity.
struct Vec3 {
    int x;
    union {
        int y;
        Fixed_0043d6d0 fy;
        struct {
            unsigned short yFraction;
            short yWhole;
        };
    };
    int z;

    Vec3() {}
    Vec3(int a, int b, int c) : x(a), y(b), z(c) {}
    int Length() const {
        double a = x, b = y, c = z;
        return (int)sqrt(a * a + b * b + c * c);
    }
    Vec3 operator-(const Vec3& o) const {
        Vec3 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Vec3& operator+=(const Vec3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    void Scale(int s) {
        MulFixed(x, s);
        MulFixed(y, s);
        MulFixed(z, s);
    }
    void Add(const Vec3* o)
    {
        x += o->x;
        y += o->y;
        z += o->z;
    }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

// The temporary the position is built through (the derived type is what
// makes the compiler materialise it).
struct Vec3Init_0043a1f0 : Vec3 {
    Vec3Init_0043a1f0(int a, int b, int c) { x = a; y = b; z = c; }
};

struct Point {
    short x, y;
    Point() {}
    Point(int a, int b) : x(a), y(b) {}
};

// A 4-byte point of two shorts, and the temporary the constructor builds it
// through (the derived type is what makes the compiler keep the temporary).
struct PointInit_0043a1f0 : Point {
    PointInit_0043a1f0(short a, short b) { x = a; y = b; }
};

struct Short3 {
    short x, y, z;
};

struct Player_0043b7c0 {
    int active;                    // +0x0
    char unknown_4[0x73 - 0x4];
    char state;                    // +0x73
};

// The unit's target: the object at unit+0x96, whose +0xe4 is the divisor
// 0x439d20 uses.
struct Target_0043d6d0 {
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0xe4 - 0x74];
    unsigned short field_e4;           // +0xe4
};

struct TargetData_0043d6d0 {
    char unknown_0[8];
    Vec3 velocity;                     // +0x8
    char unknown_14[0x20 - 0x14];
    int speed;                         // +0x20
};

struct Path_0043d6d0 {
    TargetData_0043d6d0* motion;       // +0x0
};

struct UnitType_0043cd20 {           // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
    char unknown_40[0x192 - 0x40];
    int maxvelocity;                   // +0x192, the range
    char unknown_196[0x19a - 0x196];
    int field_19a;                     // +0x19a, the rate (top speed)
    int field_19e;                     // +0x19e, the long-step distance (acceleration)
    int field_1a2;                     // +0x1a2
    int field_1a6;                     // +0x1a6
    char unknown_1aa[0x1ae - 0x1aa];
    int field_1ae;                     // +0x1ae
    int field_1b2;                     // +0x1b2
    int field_1b6;                     // +0x1b6
    unsigned short max_turn;           // +0x1ba
    char unknown_1bc[0x202 - 0x1bc];
    short range;                       // +0x202
    char unknown_204[0x214 - 0x204];
    unsigned short field_214;          // +0x214
    char unknown_216[0x22c - 0x216];
    unsigned char draft;               // +0x22c
    char unknown_22d[0x230 - 0x22d];
    unsigned char field_230;           // +0x230
    char unknown_231[0x241 - 0x231];
    union {
        int flags1;                    // +0x241
        unsigned char flags;           // +0x241
        struct {
            unsigned int mode_bits : 11;
            unsigned int flag_800 : 1; // bit 11
        };
        struct {
            unsigned int low : 19;
            unsigned int b19 : 1;      // bit 19
            unsigned int rest : 12;
        };
    };
    char unknown_245[0x249 - 0x245];
};

#include "../units/cob_script.h"

// Unused here: these forward declarations take the symbol ids that keep RegisterOrderTypes (0x43bc90), RegisterAllOrderTypes (0x43c050) and UpdatePosition (0x43d6d0) matching
// (docs/c2-regalloc.md).
struct BmpFileHeader;
struct BmpInfo;
struct BmpInfoHeader;
struct CalcedExplosion;
struct FrameTable;

// The command kind, one byte wide, but not a POD type.
class Class_00438760 {
public:
    unsigned char index;

    Class_00438760() {}
};

struct Unit;
class Order;

// The 0x10-byte link at +0x12: its constructor puts the object in its
// owner's list; 0x489650 is its destructor.
class PathOrderAttach {
public:
    void* vptr;                      // +0x0
    Unit* owner;                     // +0x4
    PathOrderAttach* next;           // +0x8
    void* value;                     // +0xc, the object the link belongs to

    void SetValue(void* v) { value = v; }

    PathOrderAttach(Unit* o, int v);
    void SetUnit(Unit* o);
};

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void OrStatusFlags(unsigned int);  // slot 0: 0x43a1e0, empty
};

class Order : public Class_0043a1e0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's: 0x438870 (defined
    // in order_queue_4384a0.cpp).
    virtual void OrStatusFlags(unsigned int);

    unsigned char kind;              // +0x4, index into g_missionOrderTableBegin
    unsigned char count;             // +0x5
    unsigned int flags6;             // +0x6, bit 0 set while the node waits
    unsigned int wakeFrame;          // +0xa
    void* unit;                      // +0xe
    PathOrderAttach link;            // +0x12
    Vec3 pos;                        // +0x22
    Point field_2e;                  // +0x2e
    Point field_32;                  // +0x32
    int id;                          // +0x36
    int amount;                      // +0x3a
    int field_3e;                    // +0x3e
    unsigned int flags;              // +0x42
    unsigned int created;            // +0x46
    Order* next;                     // +0x4a
    unsigned int field_4e;           // +0x4e
    void* attached;                  // +0x52

    Order(unsigned char k, Unit* o, Vec3* p, int a, int b, int c);
    Order(Class_00438760 kind, Unit* o, Vec3* p, int a, int b, int c);
    Order(unsigned char type, int a, int b, int c, int d, int e);
    ~Order();
    void SetDeadlineTicks(int param);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    void ReattachFxToUnit();
    void AttachApproachRadiusGoal(Vec3* pos, int radius);
    Unit* Target();
    void Wait();
    Vec3* Position();
};

// One of the unit's two order-target slots (+0x10); its target's +0xe4 is the
// divisor 0x439d20 uses.
struct Slot_0043a1f0 {
    Target_0043d6d0* target;         // +0x0
    char unknown_4[0x18];
};

struct Game {
    char unknown_0[0x14263];
    int count2;                        // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int overflowBucket;                // +0x142b7
    char unknown_142bb[0x1438f - 0x142bb];
    int unitTypeCount;                 // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043cd20* unitTypes;      // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    union {
        unsigned int frame;            // +0x38a47
        int field_38a47;               // +0x38a47
        unsigned int ticks;            // +0x38a47
    };
};

struct Unit {                          // 0x118 bytes
    char unknown_0[0x10];
    Slot_0043a1f0 entries[2];          // +0x10
    char unknown_48[0x5c - 0x48];
    Order* list;                       // +0x5c
    Order* list2;                      // +0x60, nodes with flag 0x40000
    union {
        Short3 f64;                    // +0x64
        struct {
            short bank;            // +0x64
            short heading;             // +0x66
            short pitch;            // +0x68, in 2048ths of a circle
        };
    };
    Vec3 pos;                          // +0x6a, 16.16
    Point cell;                        // +0x76
    char unknown_7a[0x7e - 0x7a];
    Point draft;                       // +0x7e
    int spatialBucket;                      // +0x82
    Path_0043d6d0* obj;                // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType_0043cd20* type;           // +0x92
    union {
        Target_0043d6d0* target;       // +0x96
        Player_0043b7c0* player;       // +0x96
    };
    CobScript* script;                 // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short id;                 // +0xa8
    char unknown_aa[0xba - 0xaa];
    unsigned short netDirtyFlags;           // +0xba, the unit's pending mask
    char unknown_bc[0xf9 - 0xbc];
    signed char index;                 // +0xf9
    char unknown_fa[0x110 - 0xfa];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int mode : 2;     // bits 0-1
            unsigned int flags_2 : 14;
            unsigned int moved : 1;    // bit 16
            unsigned int flags_17 : 3;
            unsigned int mode20 : 2;   // bits 20-21
            unsigned int flags_22 : 10;
        };
    };

    void SetStateBits(int param_1, int param_2);
};

// The whole (high) halves of the position's x and z components, which 43b1f0
// reads as its own short view of the position.
static inline short PosXWhole(Unit* unit) { return *(short*)((char*)&unit->pos + 2); }
static inline short PosZWhole(Unit* unit) { return *(short*)((char*)&unit->pos + 0xa); }

// One 25-byte record of the global order-type table at 0x512344 (a
// std::vector's _First): the status text at +0, a per-kind notify callback at
// +4, the draw mask at +0xc and its name at +0x15.
struct Elem_0043c390 {
    int value;                         // +0x0, the status string
    int (__stdcall* notify)(void* unit, Order* obj, unsigned int code); // +4
    char unknown_8[0xc - 8];
    int drawMask;                      // +0xc, the flags 0x439b30 masks
    char unknown_10[0x11 - 0x10];
    unsigned int flags;                // +0x11, default flags of the kind
    char* name;                        // +0x15

    // Only so the explicit instantiation of the rest of the vector compiles.
    int operator<(const Elem_0043c390&) const { return 0; }
    int operator==(const Elem_0043c390&) const { return 0; }
    int operator!=(const Elem_0043c390&) const { return 0; }
};

#pragma pack(pop)

extern Game* g_game;
extern Elem_0043c390* g_missionOrderTableBegin;

void __stdcall DeleteOrder(Unit* owner, Order* node);

extern int __cdecl _strcmpi(const char*, const char*);

short __stdcall FindUnitTypeId(char* name);

void __stdcall DrawBuildFootprint(void* a, void* b, Order* e, Vec3* p, int c);
void __stdcall DrawUnitRangeRings(void* a, void* b, Order* e, Vec3* p, int c);
void __stdcall DrawPathAnim(void* a, void* b, Order* e, Vec3* p, int c);
void __stdcall DrawWeaponCoverage(void* a, void* b, Order* e, Vec3* p, int c);
void __stdcall DrawOrderRangeRing(void* a, void* b, Order* e, Vec3* p, int c);

// The list-walking dispatcher of the unit: for every object linked into the unit's
// list at +0x5c (link field at +0x4a, kind byte at +0x4) it looks up the kind's
// default flags in the table at g_missionOrderTableBegin (0x19-byte entries, the flags dword
// at +0xc, indexed by the kind byte) and, masked with the `mask` parameter, calls
// one of five helpers per bit: bit 0 -> 0x438c00, bit 1 -> 0x4394e0, bit 2 ->
// 0x4399f0, bit 3 -> 0x439740, bit 4 -> 0x4390a0. Bit 4 is only acted on for the
// first object of the list (`done`). Every helper is __stdcall with five dword
// arguments and takes the position as a pointer to a 12-byte object.
// FUNCTION: 0x439b30
void __stdcall DrawOrderOverlays(Unit* unit, unsigned int mask, void* obj,
                            void* sel, int flag)
{
    // Two copies, base first: each call but bit 4's is preceded by pos = base,
    // and the loop ends with base = pos.
    Vec3 base = unit->pos;
    Vec3 pos = unit->pos;
    bool done = false;
    for (Order* e = unit->list; e != 0; e = e->next) {
        if (g_missionOrderTableBegin[e->kind].drawMask & mask & 1) {
            pos = base;
            DrawBuildFootprint(obj, sel, e, &pos, flag);
        }
        if (g_missionOrderTableBegin[e->kind].drawMask & mask & 2) {
            pos = base;
            DrawPathAnim(obj, sel, e, &pos, flag);
        }
        if (g_missionOrderTableBegin[e->kind].drawMask & mask & 4) {
            pos = base;
            DrawOrderRangeRing(obj, sel, e, &pos, flag);
        }
        if (g_missionOrderTableBegin[e->kind].drawMask & mask & 8) {
            pos = base;
            DrawWeaponCoverage(obj, sel, e, &pos, flag);
        }
        if (g_missionOrderTableBegin[e->kind].drawMask & mask & 0x10) {
            if (!done) {
                DrawUnitRangeRings(obj, sel, e, &pos, flag);
                done = true;
            }
        }
        base = pos;
    }
}

// FUNCTION: 0x439cf0
int __stdcall GetOrderTargetIfFlagged(void* param_1)
{
    int eax;
    if (param_1 == 0) {
        eax = 0;
    } else {
        int* field_5c = *(int**)((char*)param_1 + 0x5c);
        if (field_5c == 0)
            eax = 0;
        else
            eax = *(int*)((char*)field_5c + 0x42);
    }
    if ((eax & 8) != 0)
        return *(int*)((char*)*(int**)((char*)param_1 + 0x5c) + 0x16);
    return 0;
}

// FUNCTION: 0x439d20
int __stdcall GetBuildWeaponPercent(Unit* owner)
{
    Order* p;
    for (p = owner->list2; p; p = p->next) {
        if (p->flags & 0x80000)
            return p->field_3e * 100 / owner->entries[p->id].target->field_e4;
    }
    return 0;
}

// Sums the amounts of the nodes flagged 0x100 with the given index over both
// of the owner's node lists (+0x5c and +0x60).
// FUNCTION: 0x439d80
int __stdcall SumQueuedBuildCount(Unit* owner, int index)
{
    int total = 0;
    Order* p;
    for (p = owner->list; p; p = p->next) {
        if ((p->flags & 0x100) && p->id == index)
            total += p->amount;
    }
    for (p = owner->list2; p; p = p->next) {
        if ((p->flags & 0x100) && p->id == index)
            total += p->amount;
    }
    return total;
}

// FUNCTION: 0x439dd0
int __stdcall GetOrderTarget(int param_1)
{
    int eax = param_1;
    if (eax != 0) {
        eax = *(int*)(eax + 0x5c);
        if (eax != 0) {
            eax = *(int*)(eax + 0x16);
            return eax;
        }
    }
    return 0;
}

// FUNCTION: 0x439df0
int __stdcall GetOrderName(Unit* obj)
{
    if (obj && obj->list) {
        return g_missionOrderTableBegin[obj->list->kind].value;
    }
    return g_missionOrderTableBegin[0].value;
}

// Finds the object's list node of the given kind; the kind table's flag
// 0x40000 selects which of the object's two lists (+0x60 or +0x5c) to search.
// FUNCTION: 0x439e30
Order* __stdcall FindOrderByType(Unit* obj, unsigned char kind)
{
    Order* n;
    if (g_missionOrderTableBegin[kind].flags & 0x40000)
        n = obj->list2;
    else
        n = obj->list;
    while (n) {
        if (n->kind == kind)
            return n;
        n = n->next;
    }
    return 0;
}

// FUNCTION: 0x439e80
void Order::SetDeadlineTicks(int param)
{
    *(unsigned int*)((char*)this + 0x6) |= 1;
    int val = *(int*)((char*)g_game + 0x38a47);
    *(int*)((char*)this + 0xa) = val + param;
}

// FUNCTION: 0x439ea0
int __stdcall ReadyOrder(int arg1, int arg2, int arg3)
{
    return 5;
}

// Deletes the objects in the owner's list at +0x5c (all of them, or only
// those without flag 4 when `all` is 0). With `all` set, also deletes every
// object of the list at +0x60, unlinking each from the list its flag 0x40000
// selects. Every deleted object except the list's head is marked 0x10000
// first.
// FUNCTION: 0x439eb0
void __stdcall DeleteOrders(Unit* owner, int all)
{
    Order* first = owner->list;
    Order** pp = &owner->list;
    Order* node;
    while ((node = *pp) != 0) {
        if (!all && (node->flags & 4)) {
            pp = &node->next;
        } else {
            *pp = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
        }
    }
    if (all) {
        while ((node = owner->list2) != 0)
            DeleteOrder(owner, node);
    }
}

// Unlinks `node` from the owner's list at +0x5c (or +0x60 when the node has
// flag 0x40000) and deletes it; every node except the head of the +0x5c list
// is marked 0x10000 first. 0x439eb0 inlines the same code.
// FUNCTION: 0x439f80
void __stdcall DeleteOrder(Unit* owner, Order* node)
{
    Order* first = owner->list;
    Order** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    for (Order* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
            return;
        }
        link = &n->next;
    }
}

// FUNCTION: 0x439fe0 ?MoveOrderToTail@@YGXPAD0@Z
void __stdcall MoveOrderToTail(char* param_1, char* param_2)
{
    char** pp = (char**)(param_1 + 0x5c);
    char* node = *pp;
    for (; node != param_2; node = *(char**)(node + 0x4a)) {
        pp = (char**)(node + 0x4a);
    }
    node = *(char**)(param_2 + 0x4a);
    *pp = node;
    for (; node != 0; node = *(char**)(node + 0x4a)) {
        pp = (char**)(node + 0x4a);
    }
    *pp = param_2;
    *(char**)(param_2 + 0x4a) = 0;
}

// Links `child` into the parent's list in front of `before` (see EnqueueOrderType).
static inline void InsertBefore(Unit* p, Order* child, Order* before)
{
    Order** link = (child->flags & 0x40000) ? &p->list2 : &p->list;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = child;
    child->unit = p;
    child->next = before;
    if (before != 0) {
        child->flags |= before->flags & 0x4000;
    }
}

// FUNCTION: 0x43a020
void __stdcall EnsurePatrolReturnOrder(Unit* p, Order* item)
{
    int add = 1;
    for (Order* c = p->list; c != 0; c = c->next) {
        if (c->flags & 0x8000) {
            add = 0;
            break;
        }
    }
    if (add) {
        Order* child = new Order(item->kind, 0, &p->pos, 0, 0, 0);
        InsertBefore(p, child, 0);
    }
    item->flags |= 0x8000;
}

// The constructor of Order (destructor 0x43a1f0, vtable 0x4fd2c8).
// The rest of the class is in order_queue_4384a0.cpp.
// It first stores 0x4fd2cc, the vtable of the inline constructor of the base
// class Class_0043a1e0, then runs the member initialisers, then stores its
// own vtable. Each vtable has one slot: the base's is the empty 0x43a1e0 and
// the derived class overrides it with 0x438870 (which ORs bits into +0x4e).
// The other constructor, 0x43a420 (which loads the object from a file),
// stores the same two vtables; the destructor stores only 0x4fd2c8, so the
// base has no destructor of its own.
// The link member's constructor puts the object in its owner's list and the
// body then points the link's value at the object; the kind's default flag
// bits come from the kind table at g_missionOrderTableBegin, with bits 9 and 10 cleared
// when the list owner or the position pointer is absent.
// This view keeps `kind(k)` a plain member initialiser; the other file's view
// adds a second base that rules it out.
// FUNCTION: 0x43a0c0
Order::Order(unsigned char k, Unit* o, Vec3* p, int a, int b, int c)
    : kind(k), link(o, 0), field_2e(PointInit_0043a1f0(0, 0)), field_32(PointInit_0043a1f0(0, 0)),
      id(a), amount(b), field_3e(c), created(g_game->ticks)
{
    // Through the inline method: a plain `link.value = this` reorders the
    // position pointer's compare before the vtable store.
    link.SetValue(this);
    count = 0;
    flags6 = 0;
    field_4e = 0;
    wakeFrame = -1;
    pos = p ? *p : Vec3Init_0043a1f0(0, 0, 0);
    flags = g_missionOrderTableBegin[k & 0xff].flags;
    unit = 0;
    next = 0;
    attached = 0;
    if (o == 0)
        flags &= ~0x200;
    if (p == 0)
        flags &= ~0x400;
    if (!(flags & 0x200))
        link.SetUnit(0);
}

// Slot 0 of Class_0043a1e0's vtable (0x4fd2cc): an empty virtual method.
// Class_0043a1e0 is the base of Order (vtable 0x4fd2c8), which
// overrides this slot with 0x438870 (see order_queue_4384a0.cpp).
// FUNCTION: 0x43a1e0
void Class_0043a1e0::OrStatusFlags(unsigned int)
{
}

// Makes sure the text file has a "UTYPENAME<id>" key: if it is missing and
// `id` is a valid unit type, writes the type's name under that key.
// FUNCTION: 0x43a2d0
void __stdcall WriteUnitTypeNameKey(HapiBank* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (!file->HasItem(key) && id >= 1 && id < g_game->unitTypeCount)
        ((HapiBank*)file)->SetStringItem(key, g_game->unitTypes[id].name);
}

// Resolves a unit definition slot number to a unit type id. The parsed text
// file can name the type of each slot with the key "UTYPENAME<id>"; when that
// key is present the name it holds is looked up in the unit type table by
// FindUnitTypeId. Otherwise the id'th unit type whose +0x241 flags do not have
// bit 5 set is taken, and its table index minus one is returned.
// FUNCTION: 0x43a360
short __stdcall ResolveUnitTypeKey(HapiBank* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (file->HasItem(key))
        return FindUnitTypeId(((HapiBank*)file)->GetStringItem(key, 0));
    int i, n = 0;                     // n counts every table entry, k only the
    unsigned short k = 0;             // ones without flag bit 5
    for (i = 1; i < g_game->unitTypeCount; i++, n++) {
        if (!(g_game->unitTypes[(unsigned short)i].flags & 0x20)) {
            if (k == id)
                return n;
            k++;
        }
    }
    return 0;
}

// FUNCTION: 0x43a940
int __stdcall OrderTypeNameLess(int param_1, char* param_2)
{
    int result = _strcmpi(*(char**)(param_1 + 0x15), param_2);
    return result < 0;
}

// FUNCTION: 0x43ac60
void __stdcall InsertOrderBefore(Unit* owner, Order* node,
                            Order* before)
{
    Order** link = (node->flags & 0x40000) ? &owner->list2
                                                   : &owner->list;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->unit = owner;
    node->next = before;
    if (before != 0)
        node->flags |= before->flags & 0x4000;
}

// FUNCTION: 0x43acb0
void __stdcall AppendOrder(Unit* owner, Order* node)
{
    unsigned int which = node->flags & 0x40000;
    Order* before = which ? owner->list2 : owner->list;
    Order** link = which ? &owner->list2 : &owner->list;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = node;
    node->unit = owner;
    node->next = before;
    if (before != 0) {
        node->flags |= before->flags & 0x4000;
    }
}

// Appends a node to the end of the owner's list that its flag 0x40000
// selects (+0x60 when set, +0x5c otherwise); compare 0x43acb0.
// FUNCTION: 0x43ad10
void __stdcall AppendOrderToTail(Unit* owner, Order* node)
{
    Order** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    for (Order* n = *link; n != 0; n = n->next)
        link = &n->next;
    *link = node;
    node->unit = owner;
    node->next = 0;
}

// Marks a node with flag 0x1000 and links it into the owner's list at +0x5c
// just after the node currently carrying that mark (moving the mark to the
// new node), or at the end of the list when no node has it; compare 0x43ad10.
// FUNCTION: 0x43ad50
void __stdcall InsertOrderAfterMarked(Unit* owner, Order* node)
{
    Order** link = &owner->list;
    node->unit = owner;
    node->flags |= 0x1000;
    for (Order* n = *link; n != 0; n = n->next) {
        if (n->flags & 0x1000) {
            n->flags &= ~0x1000;
            node->unit = owner;
            node->next = n->next;
            n->next = node;
            return;
        }
        link = &n->next;
    }
    node->next = 0;
    *link = node;
}

// Unlinks the node `link` points at and deletes it, marking it 0x10000 (so its
// destructor does not unlink it again) unless it is the list head.
static inline void UnlinkNode(Order** link, Order* node, Order* first)
{
    *link = node->next;
    if (node != first)
        node->flags |= 0x10000;
    delete node;
}

// Unlinks `node` from the list starting at `list` and deletes it.
static inline void RemoveFromList(Order** list, Order* node, Order* first)
{
    Order** link = list;
    for (Order* n = *link; n != 0; n = n->next) {
        if (n == node) {
            UnlinkNode(link, node, first);
            return;
        }
        link = &n->next;
    }
}

// Drops every node of the owner's +0x5c list that has no flag 4.
static inline void PruneLoose(Order** link, Order* first)
{
    Order* n = *link;
    while (n != 0) {
        if (n->flags & 4)
            link = &n->next;
        else
            UnlinkNode(link, n, first);
        n = *link;
    }
}

// Drops the leading run of nodes carrying flag 0x4000, each from the list its
// own flag 0x40000 selects.
static inline void PruneUsed(Unit* owner)
{
    Order** base = &owner->list;
    for (Order* n = owner->list; n != 0; n = owner->list) {
        if (!(n->flags & 0x4000))
            break;
        // Keep the `node = n` copy and the fresh owner->list read at the call.
        Order* node = n;
        RemoveFromList((node->flags & 0x40000) ? &owner->list2 : base, node, owner->list);
    }
}

// Puts `node` in front of the list head its flag 0x40000 selects (0x43acb0).
static inline void AddFront(Unit* owner, Order* node)
{
    Order* before = (node->flags & 0x40000) ? owner->list2 : owner->list;
    Order** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->unit = owner;
    node->next = before;
    if (before != 0) {
        node->flags |= before->flags & 0x4000;
        return;
    }
}

// Puts `node` right after the node carrying flag 0x1000 (0x43ad50).
static inline void AddMarked(Unit* owner, Order* node)
{
    Order** link = &owner->list;
    node->unit = owner;
    node->flags |= 0x1000;
    for (Order* n = *link; n != 0; n = n->next) {
        if (n->flags & 0x1000) {
            n->flags &= ~0x1000;
            node->unit = owner;
            node->next = n->next;
            n->next = node;
            return;
        }
        link = &n->next;
    }
    node->next = 0;
    *link = node;
}

// Creates a new Order node of the given kind and links it into the
// owner's two lists (+0x60 when the node has flag 0x40000, otherwise +0x5c),
// after pruning them.  With `remove` clear and the new node lacking flag 0x40,
// every node of the +0x5c list that has no flag 4 is deleted, all but the
// first getting flag 0x10000 so that its destructor does not unlink it again.
// Then, while the new node has no flag 0x40000, the leading run of nodes
// carrying flag 0x4000 is deleted, each from the list its own flag 0x40000
// selects.  The new node is marked in use (flag 1, plus flag 0x2000 when
// `remove` is clear) and linked in: in front of the old list head, taking over
// its flag 0x4000, through the code of 0x43acb0 when the new node has flag
// 0x20 or 0x40000, otherwise right after the node that carries flag 0x1000, as
// 0x43ad50 does.
// FUNCTION: 0x43adc0
void __stdcall AddOrder(int kind, int remove, Unit* owner, void* id,
                            Vec3* pos, int param_6, int param_7)
{
    // The constructor's owner argument is the fourth parameter, not `owner`,
    // which stays in its stack slot.
    Order* obj = new Order(kind, (Unit*)id, pos, param_6, param_7, 0);

    if (remove == 0 && !(obj->flags & 0x40))
        // Link and head passed separately so the two loads of owner->list stay.
        PruneLoose(&owner->list, owner->list);

    if (!(obj->flags & 0x40000))
        PruneUsed(owner);

    obj->flags |= 1;
    if (remove == 0)
        obj->flags |= 0x2000;

    if (obj->flags & 0x40020)
        AddFront(owner, obj);
    else
        AddMarked(owner, obj);
}

// The second part's own views: the mover object and the sort's holders.
#pragma pack(push, 1)

class MissionOrderTable {
public:
    char unknown_0[0x4];
    int field_4;
    int field_8;

    int GetCount(void);
};

// Unused here: forward declarations of real functions; their symbol ids keep
// the allocation the merged UnitMotion view moved (docs/c2-regalloc.md).
void EnableAICommands();
void ResetAIPlayers();
void StepAllGafSequences();
void ResetNetStats();
void InitCommands();
int UpdatePlacementGhostValidity();
void RefreshSelectionOrders();

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef int(__stdcall* Pred_0043c390)(const Elem_0043c390&, const Elem_0043c390&);

// The movement state saved as "u%04xmob".
struct Record_0043dd70 {
    Vec3 velocity;                     // +0x00
    Vec3 p2;                           // +0x0c
    int field_20;                      // +0x18
    short field_24;                    // +0x1c
    int field_26;                      // +0x1e
    unsigned char mode : 2;            // +0x22 bits 0-1
    unsigned char flag : 1;            // +0x22 bit 2
};

// The behaviour object at +0x0: the path the mover follows.
class Iface_0043dd20 {
public:
    virtual void v0(int);
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4(Vec3* a, Vec3* b, short* heading);
    virtual int v5();
};

class PackedPosGoal { public: char unknown[0x27]; PackedPosGoal(Unit* unit); };
class LiteGoal { public: char unknown[0x28]; LiteGoal(Unit* unit); };
class PatrolGoal { public: char unknown[0x1c]; PatrolGoal(Unit* unit); };
class AiSearchGoal { public: char unknown[0x65]; AiSearchGoal(Unit* unit); };
class Class_0043db50 { public: void UpdateSfxOccupy(Unit* u); };

class UnitMotion {
public:
    Iface_0043dd20* obj;               // +0x0
    int movementClass;                 // +0x4
    Vec3 velocity;                     // +0x8
    Vec3 p2;                           // +0x14
    int speed;                         // +0x20
    short turn;                        // +0x24, turn this tick
    int pathLockStamp;                 // +0x26
    int lastMoveTick;                  // +0x2a
    union {
        unsigned char flags;           // +0x2e
        struct {
            unsigned char mode : 2;    // bits 0-1
            unsigned char flag : 1;    // bit 2
            unsigned char rest : 5;
        };
    };

    void UpdateVelocityFromHeading(Unit* unit, int amount);
    void SteerGroundUnit(Unit* unit);
    void ApplyBankAndPitch(Unit* owner, Vec3* v);
    void SetFlightMode(Unit* owner, int state);
    void SteerAircraft(Unit* unit);
    void UpdatePosition(Unit* unit);
    void UpdateMoveRate(Unit* unit);
    UnitMotion(Unit* unit);
    void DestroyObject();
    void UpdateMotion(Unit* u);
    void ApplyClampedTurnDelta(Unit* unit, short amount);
    void SaveMotion(Unit* info, HapiBank* file);
    void LoadMotion(Unit* unit, HapiBank* file);
};
#pragma pack(pop)

extern Game* g_game;
extern Elem_0043c390* g_missionOrderTableBegin;
extern int g_missionOrderTableEnd;
extern Elem_0043c390 g_readyOrder[];
extern signed char g_slopeSpeedFactor[];

void* __cdecl operator new(unsigned int size);

int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __cdecl FUN_004b7173(short angle, int* xz);
int __cdecl FUN_004b715a(int x, int z);
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
Vec3 __stdcall GetPiecePosition(Path_0043d6d0* obj, int index);
Short3 __stdcall GetPieceAngles(Path_0043d6d0* obj, int index);
void __stdcall SetUnitPosition(Unit* unit, Vec3 pos, int mode);
int __stdcall CanPlaceUnitFootprint(UnitType_0043cd20* type, short a8, Point cell, int mode);
void __stdcall RemoveUnitFromMap(Unit* unit);
void __stdcall AddUnitToMap(Unit* unit);
void __stdcall UpdateUnitLineOfSight(Unit* unit);
Class_00438760 __stdcall GetOrderType(unsigned char relation, Unit* unit, Unit* target, Vec3* pos);
void __stdcall AddOrder(unsigned char kind, int remove, Unit* owner, int id, int* pos, int param_6, int param_7);
int __stdcall FindWeaponTarget(Unit* unit, int a, int b);
void __stdcall ClearWeaponTarget(Unit* unit, int index);
int __stdcall RandomInt(int n);
int __stdcall CompareOrderTypeNames(const Elem_0043c390& a, const Elem_0043c390& b);
void RegisterGroundOrders();
void RegisterVtolOrders();
void RegisterAICommands();
void RegisterUnitOrders();
void __stdcall DeleteOrder(Unit* owner, Order* node);
void __stdcall DeleteOrders(Unit* owner, int all);
void __stdcall MoveOrderToTail(Unit* owner, Order* node);
void __stdcall InsertOrderBefore(Unit* owner, Order* node, Order* before);

// |d| <= 0x100000
static inline int InRange(int d)
{
    return (unsigned int)(d + 0x100000) <= 0x200000;
}

// Matches and RemoveNode stay helpers: written in place, the parameters
// get other registers and the blocks lay out differently.
static inline int Matches(Order* node, unsigned char kind, int id, int* pos)
{
    return kind == node->kind && (id == 0 || id == (int)node->link.owner) &&
           (!pos || (InRange(pos[0] - node->pos.x) && InRange(pos[2] - node->pos.z)));
}

// Unlinks `node` from the list its flag 0x40000 selects and deletes it.
static inline void RemoveNode(Unit* owner, Order* node, Order* first)
{
    Order** link = &owner->list;
    if (node->flags & 0x40000) {
        link = &owner->list2;
    }
    for (Order* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
            return;
        }
        link = &n->next;
    }
}

// With `remove` set, looks in the owner's list at +0x5c for an object of the
// given kind (and id, and within 0x100000 of `pos` in x and z, when those are
// given) and deletes it; otherwise, or when nothing matches, hands all the
// arguments on to AddOrder.
// FUNCTION: 0x43afc0
void __stdcall IssueOrCancelOrder(unsigned char kind, int remove, Unit* owner, int id, int* pos, int param_6, int param_7)
{
    if (remove) {
        Order* first = owner->list;
        for (Order* node = first; node != 0; node = node->next) {
            if (Matches(node, kind, id, pos)) {
                RemoveNode(owner, node, first);
                return;
            }
        }
    }
    AddOrder(kind, remove, owner, id, pos, param_6, param_7);
}

// Adds `amount` to the object's last list node of the given kind and id when the
// amount is positive (asking AddOrder to make a new node if there is none);
// when it is not positive it takes the amount off the matching node, deleting
// nodes (and asking again) until the amount is used up. The kind table's flag
// 0x40000 selects which of the object's two lists (+0x60 or +0x5c) is used.
// FUNCTION: 0x43b0b0
void __stdcall AdjustBuildCount(int kind, Unit* owner, int id, int amount)
{
    Order* node;
    if (amount > 0) {
        if (g_missionOrderTableBegin[kind & 0xff].flags & 0x40000)
            node = owner->list2;
        else
            node = owner->list;
        while (node != 0 && node->next != 0) {
            node = node->next;
        }
        if (node != 0 && node->kind == (unsigned char)kind && node->id == id) {
            node->amount += amount;
            return;
        }
        AddOrder((unsigned char)kind, 1, owner, 0, 0, id, amount);
        return;
    }
    for (;;) {
        Order* found = 0;
        if (g_missionOrderTableBegin[kind & 0xff].flags & 0x40000)
            node = owner->list2;
        else
            node = owner->list;
        for (; node != 0; node = node->next) {
            if (node->kind == (unsigned char)kind && node->id == id)
                found = node;
        }
        if (found == 0)
            break;
        if (found->amount > -amount) {
            found->amount += amount;
            break;
        }
        Order* first = owner->list;
        amount += found->amount;
        Order** link = &owner->list;
        if (found->flags & 0x40000)
            link = &owner->list2;
        for (node = *link; node != 0; node = node->next) {
            if (node == found) {
                *link = found->next;
                if (found != first)
                    found->flags |= 0x10000;
                delete found;
                break;
            }
            link = &node->next;
        }
    }
}

// Puts `cmd` in front of `before` in the list its own flag 0x40000 picks, and
// copies bit 0x4000 of the command it displaces.
static inline void Insert(Unit* unit, Order* cmd, Order* before)
{
    unsigned int which = cmd->flags & 0x40000;
    Order* last = which ? unit->list2 : unit->list;
    Order** link = which ? &unit->list2 : &unit->list;
    while (*link != before) {
        link = &(*link)->next;
    }
    *link = cmd;
    cmd->unit = unit;
    cmd->next = before;
    if (before != 0) {
        cmd->flags |= before->flags & 0x4000;
    }
}

// Appends a command object (Order, 0x56 bytes) to the unit's list
// when the unit is allowed to use one on the target: GetOrderType (with
// relation 3, then 2) picks the kind of the command. The unit's flags at
// +0x110 gate it (bits 18-19 and 20-21), except when the third argument is
// non-zero, and when bit 18 alone is set a second, positional command is made
// at the unit's own position before the one aimed at the target.
// FUNCTION: 0x43b1f0
int __stdcall IssueAttackOrder(Unit* unit, Unit* target, int param_3)
{
    if (unit == target)
        return 0;
    if (!(unit->flags & 0xc0000) && !param_3)
        return 0;
    if (!(unit->flags & 0x300000) && !param_3)
        return 0;
    Class_00438760 kind = GetOrderType(3, unit, target, 0);
    if (!kind.index)
        return 0;
    if ((unit->flags & 0xc0000) == 0x40000 && !param_3) {
        Class_00438760 kind2 = GetOrderType(2, unit, 0, &unit->pos);
        Order* cmd = new Order(kind2, 0, &unit->pos, 0, 0, 0);
        Insert(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : unit->list);
        Order* cmd2 = new Order(kind, target, 0, 0, 0, unit->type->field_214);
        cmd2->field_2e.x = PosXWhole(unit);
        cmd2->field_2e.y = PosZWhole(unit);
        Insert(unit, cmd2, (cmd2->flags & 0x40000) ? unit->list2 : unit->list);
    } else {
        Order* cmd3 = new Order(kind, target, 0, 0, 0, 0);
        Insert(unit, cmd3, (cmd3->flags & 0x40000) ? unit->list2 : unit->list);
    }
    return 1;
}

// Sibling of 0x43b1f0: appends command objects (Order, 0x56 bytes)
// to the unit's list. GetOrderType (relation 8, then 2) picks the kinds; the
// unit's flags at +0x110 gate the three cases (bits 18-19 equal to 0, 0x40000
// or 0x80000), and when bit 18 alone is set a second, positional command is
// made at the unit's own position before the one aimed at the target.
// The two-command cases differ only in the last constructor argument: the
// first reads a signed short at def+0x202, the second an unsigned short at
// def+0x214.
// FUNCTION: 0x43b400
int __stdcall IssueRepairOrder(Unit* unit, Unit* target, int param_3)
{
    Class_00438760 kind = GetOrderType(8, unit, target, 0);
    if (!kind.index)
        return 0;
    unsigned int f = unit->flags & 0xc0000;
    if (f == 0 && !param_3) {
        Class_00438760 kind2 = GetOrderType(2, unit, 0, &unit->pos);
        Order* cmd = new Order(kind2, 0, &unit->pos, 0, 0, 0);
        Insert(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : unit->list);
        Order* cmd2 = new Order(kind, target, 0, 0, 0, unit->type->range);
        cmd2->field_2e.x = PosXWhole(unit);
        cmd2->field_2e.y = PosZWhole(unit);
        Insert(unit, cmd2, (cmd2->flags & 0x40000) ? unit->list2 : unit->list);
        // Explicit `return 1;` in each case: a single trailing return splits the store blocks.
        return 1;
    } else if (f == 0x40000 && !param_3) {
        Class_00438760 kind2 = GetOrderType(2, unit, 0, &unit->pos);
        Order* cmd = new Order(kind2, 0, &unit->pos, 0, 0, 0);
        Insert(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : unit->list);
        Order* cmd2 = new Order(kind, target, 0, 0, 0, unit->type->field_214);
        cmd2->field_2e.x = PosXWhole(unit);
        cmd2->field_2e.y = PosZWhole(unit);
        Insert(unit, cmd2, (cmd2->flags & 0x40000) ? unit->list2 : unit->list);
        return 1;
    } else if (f == 0x80000 && !param_3) {
        Order* cmd3 = new Order(kind, target, 0, 0, 0, 0);
        Insert(unit, cmd3, (cmd3->flags & 0x40000) ? unit->list2 : unit->list);
        return 1;
    } else {
        return 0;
    }
}

// FUNCTION: 0x43b700
int __stdcall FindBestTargetIfFireAtWill(Unit* unit)
{
    if (unit->mode20 == 2)
        return FindWeaponTarget(unit, 0, 0);
    return 0;
}

// FUNCTION: 0x43b730
void __stdcall EnqueueOrderType(Unit* p, unsigned char type)
{
    Order* child = new Order(type, 0, 0, 0, 0, 0);
    child->flags |= 0x4000;
    InsertBefore(p, child, (child->flags & 0x40000) ? p->list2 : p->list);
}

// 0x439fe0: moves `node` to the end of the +0x5c list.
void __stdcall MoveOrderToTail(Unit* owner, Order* node)
{
    Order** pp = &owner->list;
    Order* n = *pp;
    for (; n != node; n = n->next) {
        pp = &n->next;
    }
    n = node->next;
    *pp = n;
    for (; n != 0; n = n->next) {
        pp = &n->next;
    }
    *pp = node;
    node->next = 0;
}

// 0x43b730 inlined into RunOrders: links through 0x43ac60 rather than
// inlining its body, so the call stays.
static void InsertCommand(Unit* p, unsigned char type)
{
    Order* child = new Order(type, 0, 0, 0, 0, 0);
    child->flags |= 0x4000;
    InsertOrderBefore(p, child, (child->flags & 0x40000) ? p->list2 : p->list);
}

// Puts the node to sleep: it is due again a random delay (RandomInt(n))
// plus 30 frames from now.
static void Wait_0043b7c0(Order* node, int n)
{
    unsigned int when = RandomInt(n) + 0x1e;
    node->flags6 |= 1;
    node->wakeFrame = g_game->frame + when;
}

// Clears the unit's three weapon targets.
static void ClearTargets_0043b7c0(Unit* unit)
{
    // Char counter: the loop countdown register depends on it.
    for (char i = 0; i < 3; i++)
        ClearWeaponTarget(unit, i);
}

// Per-frame driver of a unit's command list (+0x5c): every node that is due
// (or has a pending bit) is handed to its kind's callback in g_missionOrderTableBegin,
// and the answer decides what happens to it. The list head is re-read after
// every node, so a node the callback re-queues is seen again in the same
// pass. 0x43bad0 is the twin for the +0x60 list.
// FUNCTION: 0x43b7c0
void __stdcall RunOrders(Unit* unit)
{
    Order* node;
    // This loop shape puts the early returns on the default case's epilogue.
    for (;;) {
        node = unit->list;
        if (node == 0)
            break;
        if (g_game->frame >= node->wakeFrame) {
            node->wakeFrame = 0xffffffff;
            node->field_4e |= 1;
        }
        unsigned int pending = (node->field_4e | unit->netDirtyFlags) & node->flags6;
        if (node->flags6 != 0 && pending == 0)
            return;
        unit->netDirtyFlags &= ~pending;
        node->field_4e &= ~pending;
        node->flags6 = 0;
        if (pending & 0x10000)
            ClearTargets_0043b7c0(unit);
        switch (g_missionOrderTableBegin[node->kind].notify(node->unit, node, pending)) {
        case 3:
            Wait_0043b7c0(node, 0xf);
            break;
        case 1:
            node->count++;
            break;
        case 0:
            node->count = 0;
            break;
        case 2:
        case 4:
            break;
        case 5:
        case 8:
            DeleteOrder(unit, node);
            break;
        case 9:
            node->flags |= 0x800000;
            if (node->next != 0) {
                DeleteOrder(unit, node);
            } else {
                node->count = 0;
                Wait_0043b7c0(node, 0x1e);
            }
            break;
        case 6:
            MoveOrderToTail(unit, node);
            break;
        case 7:
            DeleteOrders(unit, 1);
            return;
        default:
            DeleteOrders(unit, 1);
            return;
        }
    }
    if (unit->player->active == 0)
        return;
    char state = unit->player->state;
    if (state != 1 && state != 2)
        return;
    if (unit->type->field_230 == 0)
        return;
    InsertCommand(unit, unit->type->field_230);
}

// Takes the node out of the parent's list and frees it. The search can fail,
// when the node is not in the list at all, and then nothing is freed. The head
// of the list is read before the unlink, so a node that was the head does not
// get the 0x10000 flag. The local `first` is what forces that early read.
static void RemoveAndDelete(Unit* p, Order* child)
{
    Order* first = p->list;
    Order** link = (child->flags & 0x40000) ? &p->list2 : &p->list;
    Order* node = *link;
    while (node != 0) {
        if (node == child) {
            *link = child->next;
            if (child != first)
                child->flags |= 0x10000;
            delete child;
            break;
        }
        link = &node->next;
        node = *link;
    }
}

// Drives a parent's list of attach/spot nodes (Order) once per pass:
// every node that is due (or not waiting) is offered to the callback table
// g_missionOrderTableBegin, and the answer decides what happens to it. The list is
// restarted from the head after every node, so nodes added by the callback
// are seen in the same pass.
// FUNCTION: 0x43bad0
void __stdcall RunSecondaryOrders(Unit* p)
{
    Order* child = p->list2;
    while (child != 0) {
        if (child->flags6 == 0 || g_game->frame >= child->wakeFrame) {
            child->flags6 = 0;
            // Case order (3, 1, 0, 2/4, 5/8/9, 6/7, default) sets the body layout.
            switch (g_missionOrderTableBegin[child->kind].notify(child->unit, child, 0)) {
            case 3: {
                // Ask again in a while. The temporary keeps the sum from being
                // folded into one lea, which is what the original does.
                unsigned int when = RandomInt(0xf) + 0x1e;
                child->flags6 |= 1;
                child->wakeFrame = g_game->frame + when;
                break;
            }
            case 1:
                child->count++;
                break;
            case 0:
                child->count = 0;
                break;
            case 2:
            case 4:
                break;
            case 5:
            case 8:
            case 9:
                RemoveAndDelete(p, child);
                break;
            case 6:
            case 7:
                RemoveAndDelete(p, child);
                return;
            default:
                RemoveAndDelete(p, child);
                break;
            }
            child = p->list2;
        } else {
            child = child->next;
            continue;
        }
    }
}

// File-static: internal linkage keeps the vector bounds in registers across the sort.
static Vec_0043c390 g_missionOrderTableVec;

// std::_Unguarded_insert
inline void __stdcall InsertShiftOrderTypes(Elem_0043c390* _L, Elem_0043c390 _V, Pred_0043c390 _P)
{
    for (Elem_0043c390* _M = _L; _P(_V, *--_M); _L = _M)
        *_L = *_M;
    *_L = _V;
}

// std::_Insertion_sort_1
inline void __stdcall InsertionSortOrderTypes(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                                   Elem_0043c390*)
{
    if (_F != _L)
        for (Elem_0043c390* _M = _F; ++_M != _L; ) {
            Elem_0043c390 _V = *_M;
            if (!_P(_V, *_F))
                InsertShiftOrderTypes(_M, _V, _P);
            else {
                std::copy_backward(_F, _M, _M + 1);
                *_F = _V;
            }
        }
}

// std::_Insertion_sort
inline void _Insertion_sort_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P)
{
    InsertionSortOrderTypes(_F, _L, _P, std::_Val_type(_F));
}

// std::_Median
inline Elem_0043c390 __stdcall MedianOrderTypes(Elem_0043c390 _X, Elem_0043c390 _Y, Elem_0043c390 _Z,
                                            Pred_0043c390 _P)
{
    if (_P(_X, _Y))
        return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
    else
        return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y);
}

// std::_Unguarded_partition
inline Elem_0043c390* __stdcall PartitionOrderTypes(Elem_0043c390* _F, Elem_0043c390* _L,
                                             Elem_0043c390 _Piv, Pred_0043c390 _P)
{
    for (; ; ++_F) {
        for (; _P(*_F, _Piv); ++_F)
            ;
        for (; _P(_Piv, *--_L); )
            ;
        if (_L <= _F)
            return (_F);
        std::iter_swap(_F, _L);
    }
}

// std::_Sort
inline void __stdcall SortOrderTypes(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                                   Elem_0043c390*)
{
    for (; std::_SORT_MAX < _L - _F; ) {
        Elem_0043c390* _M = PartitionOrderTypes(_F, _L, MedianOrderTypes(Elem_0043c390(*_F),
            Elem_0043c390(*(_F + (_L - _F) / 2)), Elem_0043c390(*(_L - 1)), _P), _P);
        if (_L - _M <= _M - _F)
            SortOrderTypes(_M, _L, _P, std::_Val_type(_F)), _L = _M;
        else
            SortOrderTypes(_F, _M, _P, std::_Val_type(_F)), _F = _M;
    }
}

// std::_Sort_0
inline void _Sort_0_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                             Elem_0043c390*)
{
    if (_L - _F <= std::_SORT_MAX)
        _Insertion_sort_0043bc90(_F, _L, _P);
    else {
        SortOrderTypes(_F, _L, _P, (Elem_0043c390*)0);
        _Insertion_sort_0043bc90(_F, _F + std::_SORT_MAX, _P);
        for (_F += std::_SORT_MAX; _F != _L; ++_F)
            InsertShiftOrderTypes(_F, Elem_0043c390(*_F), _P);
    }
}

// std::sort with a predicate
inline void sort_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P)
{
    _Sort_0_0043bc90(_F, _L, _P, std::_Val_type(_F));
}

// The vector's _End at +0xc, declared for the symbol count the sort needs
// (one declaration here keeps 0x43bc90's inlined sort on the original's
// registers).
extern int DAT_0051234c;

// Appends `count` 25-byte records to the global registration table (a
// std::vector at 0x512340) and sorts the whole table by name. The record has a
// per-kind notify callback at +4 (0x43bad0 calls it) and its name at +0x15.
// FUNCTION: 0x43bc90
void __stdcall RegisterOrderTypes(Elem_0043c390* from, int count)
{
    // Own statement: `size() + count` in one expression swaps the lea operands.
    int sz = g_missionOrderTableVec.size();
    g_missionOrderTableVec.reserve(sz + count);
    std::copy(from, from + count, std::back_inserter(g_missionOrderTableVec));
    sort_0043bc90(g_missionOrderTableVec.begin(), g_missionOrderTableVec.end(), CompareOrderTypeNames);
}

// FUNCTION: 0x43c020
int __stdcall CompareOrderTypeNames(const Elem_0043c390& a, const Elem_0043c390& b)
{
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

// Registers this module's own entry of the global registration table (the
// 25-byte record at 0x4fd288) and then calls four other modules' registration
// functions, each of which calls 0x43bc90 with its own records.
// Nothing may follow this function in the file: a later function flips the sort order tie.
// FUNCTION: 0x43c050
void RegisterAllOrderTypes()
{
    RegisterOrderTypes(g_readyOrder, 1);
    RegisterGroundOrders();
    RegisterVtolOrders();
    RegisterAICommands();
    RegisterUnitOrders();
}

// FUNCTION: 0x43c350
void ClearOrderTypeTable()
{
    g_missionOrderTableEnd = (int)g_missionOrderTableBegin;
}

// FUNCTION: 0x43c360
int MissionOrderTable::GetCount(void)
{
    return field_4 == 0 ? 0 : (field_8 - field_4) / 0x19;
}

// std::vector<Elem_0043c390>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its callers (0x43bc90, 0x43c050)
// inline vector::reserve on the global vector of 25-byte records at
// 0x512340 and call this with ecx set to it.
typedef void (Vec_0043c390::*DestroyFn_0043c390)(Vec_0043c390::iterator, Vec_0043c390::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_0043c390 : Vec_0043c390 {
    static DestroyFn_0043c390 fn;
};

// FUNCTION: 0x43c390 ?_Destroy@?$vector@UElem_0043c390@@V?$allocator@UElem_0043c390@@@std@@@std@@IAEXPAUElem_0043c390@@0@Z
DestroyFn_0043c390 Access_0043c390::fn = &Access_0043c390::_Destroy;

// std::vector<Elem_0043c390>::insert(iterator, size_type, const _Ty&) of
// MSVC 5's <vector>, the three-argument insert, emitted out of line: the two
// callers (0x43bc90 and 0x43c050) insert one 25-byte record at a time into the
// global table at 0x512340. It calls the global operator new (0x4b4f10) and
// delete (0x4b4f20). The element is the same 25-byte record as the one
// _Destroy (0x43c390) destroys.
// Explicit instantiation: the member is emitted itself, not inlined into a wrapper.
template class std::vector<Elem_0043c390>;

// FUNCTION: 0x43c3a0 ?insert@?$vector@UElem_0043c390@@V?$allocator@UElem_0043c390@@@std@@@std@@QAEXPAUElem_0043c390@@IABU3@@Z

// std::_Lower_bound(first, last, value, pred, (int*)0) over a sorted table of
// 25-byte entries (the order-type table at 0x512344..0x512348), instantiated
// in a file compiled with __stdcall as the default; the one caller (0x43a513)
// passes the case-insensitive name compare 0x43a940.
typedef int (__stdcall* Pred_0043c6b0)(const Elem_0043c390& entry, char* name);

// FUNCTION: 0x43c6b0
Elem_0043c390* __stdcall LowerBoundOrderTypes(Elem_0043c390* first, Elem_0043c390* last,
                                      char* const& value, Pred_0043c6b0 pred, int*)
{
    int n = 0;
    n += last - first;
    for (; 0 < n; ) {
        int n2 = n / 2;
        Elem_0043c390* m = first;
        m += n2;
        if (pred(*m, value))
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    return first;
}

// Clamps a turn amount to +-the unit type's limit, applies it to the unit's
// heading and flags the unit as moved.
// FUNCTION: 0x43cbb0
void UnitMotion::ApplyClampedTurnDelta(Unit* unit, short amount)
{
    if (amount != 0) {
        unsigned short max = unit->type->max_turn;
        if (amount >= max)
            turn = max;
        else if (amount <= -max)
            turn = -max;
        else
            turn = amount;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }
}

// The behaviour object at +0x0: the path the mover follows.
static inline void ClampToZero(int& value)
{
    if (value < 0)
        value = 0;
}

static inline void AddFixed(int& v, float f)
{
    v += (int)((double)f * 65536.0);
}

static inline Vec3 Offset(short angle, int distance)
{
    Vec3 v;
    v.x = -FUN_004b70ef(angle, distance);
    v.y = 0;
    v.z = -FUN_004b7123(angle, distance);
    return v;
}

static inline void ClampToCell(Vec3& pos, Point cell, Point draft)
{
    int cx = (draft.x + cell.x * 2) << 19;
    int cz = (draft.y + cell.y * 2) << 19;
    if (pos.x > cx + 0x7ffff)
        pos.x = cx + 0x7ffff;
    else if (pos.x < cx - 0x7ffff)
        pos.x = cx - 0x7ffff;
    if (pos.z > cz + 0x7ffff)
        pos.z = cz + 0x7ffff;
    else if (pos.z < cz - 0x7ffff)
        pos.z = cz - 0x7ffff;
}

// Adds `amount` to the object's distance accumulator (clamped at zero), limits
// it to a range taken from the table at g_slopeSpeedFactor, then writes the offset
// for the unit's heading at that distance into the velocity.
// FUNCTION: 0x43cc20
void UnitMotion::UpdateVelocityFromHeading(Unit* unit, int amount)
{
    speed = speed + amount;
    ClampToZero(speed);

    // Direction index, limited to the eleven entries of the table.
    int idx = unit->pitch >> 11;
    if (idx < -5)
        idx = -5;
    if (idx > 5)
        idx = 5;

    // 16.16 range from the table entry, halved below sea level.
    int range = (int)(((__int64)(g_slopeSpeedFactor[idx] << 16) * unit->type->maxvelocity) >> 16);
    range = (int)(((__int64)range << 16) / 0x640000);
    if (unit->pos.yWhole < g_game->seaLevel && !(unit->type->flags1 & 0x81000))
        range = (int)(((__int64)range * 0x8000) >> 16);
    if (speed > range)
        speed = range;

    int dist = speed;
    unsigned short angle = unit->heading;
    Vec3 v;
    v.x = -FUN_004b70ef(angle, dist);
    v.y = 0;
    v.z = -FUN_004b7123(angle, dist);
    velocity = v;
}

// FUNCTION: 0x43cd20
void UnitMotion::SteerGroundUnit(Unit* unit)
{
    if (obj->v5() == 0) {
        turn = 0;
        // Bound temporary: loads unit before the turn store and keeps the rate in eax.
        const int& amount = -unit->type->field_19a;
        UpdateVelocityFromHeading(unit, amount);
        return;
    }

    Vec3 p[3];
    obj->v3(p, 0, 3);

    Vec3* ppos = &unit->pos;
    Vec3 d = p[1] - *ppos;
    int gap1 = (int)_hypot(d.x, d.z);

    int dz;
    if (gap1 > 0x500000) {
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            int ndx = (int)(((__int64)dx << 16) / len);
            int ndz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)ndx * back) >> 16);
            p[1].z -= (int)(((__int64)ndz * back) >> 16);
        }
    }

    int az = p[1].z - unit->pos.z;
    int ax = p[1].x - unit->pos.x;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short ang = (short)GetHeadingBetween(ppos, &p[1]);
    short diff = ang - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bz = p[2].z - unit->pos.z;
    int bx = p[2].x - unit->pos.x;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            turn = max;
        else if (sdiff <= -max)
            turn = -max;
        else
            turn = diff;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }

    // speed is copied to spd below so its sign extension is not shared with this
    // multiply; otherwise it becomes _allmul instead of a one-operand imul.
    int turned = (int)((((__int64)(adiff & 0xffff) * (__int64)speed)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int spd = speed;
    int t = (int)(((__int64)spd * spd) >> 16);
    int q = (int)(((__int64)t << 16) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    // UpdateVelocityFromHeading stays defined above this function so the two calls cross-jump.
    if (d1 > lim && d2 > r)
        UpdateVelocityFromHeading(unit, unit->type->field_19e);
    else
        UpdateVelocityFromHeading(unit, -rate);
}

// Scales the object's second vector (+0x14) by 0.95 (16.16 fixed point), adds the
// offset `v`, rotates the (x, z) pair by the owner's heading, then derives two
// short offsets from the rotated components and the owner type's fields at
// +0x1a2/+0x1a6.
//
// Suspected original bug: the second FUN_004b715a call reads xz[0] (the rotated
// x component) instead of xz[1], so pitch (the z offset) is computed from
// the rotated x.
// The original calls this from SetFlightMode and SteerAircraft.
#pragma auto_inline(off)
// FUNCTION: 0x43d0d0
void UnitMotion::ApplyBankAndPitch(Unit* owner, Vec3* v)
{
    // Scale and Add stay inlined methods, not separate statements on p2:
    // each field is then stored immediately.
    p2.Scale(0xf333);
    p2.Add(v);
    int xz[2];
    xz[0] = p2.x;
    xz[1] = p2.z;
    FUN_004b7173(owner->heading, xz);
    int n = (int)(((__int64)g_game->count2 << 16) / 0xccd);
    owner->bank = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a2 * -xz[0]) >> 16), n);
    owner->pitch = (short)FUN_004b715a((int)(((__int64)owner->type->field_1a6 * -xz[0]) >> 16), n);
}
#pragma auto_inline(on)

// Changes the 2-bit mode at +0x2e; entering state 1 clears the velocity and
// speed and clears flag 1 on the owner, any other state sets it.
// FUNCTION: 0x43d210
void UnitMotion::SetFlightMode(Unit* owner, int newState)
{
    if (mode != newState) {
        if (newState == 1) {
            speed = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
            ApplyBankAndPitch(owner, &zero);
            owner->SetStateBits(1, 0);
        } else {
            owner->SetStateBits(1, 1);
        }
        mode = newState;
    }
}

// Steering update for the mover object (UnitMotion): in mode 2 it asks the
// path object for a target position `a`, a target velocity `b` and a heading,
// damps the velocity, clamps its horizontal speed (bleeding the excess off
// along the unit's heading), turns the unit towards the heading, then steers
// p1 towards the target: k * (pos - a) - (velocity - b), limited to the acceleration
// f18. Finally it stores the new speed and hands the velocity change on.
// FUNCTION: 0x43d290
void UnitMotion::SteerAircraft(Unit* unit) {
    if (mode != 2) {
        velocity = Vec3(0, 0, 0);
        speed = 0;
        turn = 0;
        return;
    }

    Vec3 old = velocity;
    Vec3 a;
    Vec3 b;
    short heading;
    obj->v4(&a, &b, &heading);

    UnitType_0043cd20* type = unit->type;
    const float eps = 1.52587890625e-05f;

    float f18 = (float)type->field_19e * eps;
    int q = (int)(((__int64)type->field_19e << 16) / type->maxvelocity);
    int scale = 0x10000 - q;
    velocity.Scale(scale);

    float dist = (float)_hypot(velocity.x, velocity.z) * eps;
    float maxd = (float)unit->type->field_19a * eps;
    if (dist > maxd) {
        int f = (int)((double)(maxd / dist) * 65536.0);
        // MulFixed and AddFixed take the component by reference; written
        // inline the clamp differs.
        MulFixed(velocity.x, f);
        MulFixed(velocity.z, f);
        int g = (int)((double)(dist - maxd) * 65536.0);
        velocity += Offset(unit->f64.y, g);
    }

    Vec3 da = unit->pos - a;
    Vec3 db = velocity - b;

    if (unit->spatialBucket != g_game->overflowBucket) {
        int lim;
        if ((speed & -4) < 0x40000)
            lim = 0x10000;
        else
            lim = speed >> 2;
        if (da.y <= -lim)
            velocity.y = lim;
        else if (da.y >= lim)
            velocity.y = -lim;
        else
            velocity.y = -da.y;
    }

    float hd = (float)_hypot(da.x, da.z) * eps;
    short d = (short)(heading - unit->f64.y);
    if (d != 0) {
        unsigned short max = unit->type->max_turn;
        if (d >= (int)max)
            turn = max;
        else if (d <= -(int)max)
            turn = (short)-max;
        else
            turn = d;
        unit->f64.y = (short)(unit->f64.y + turn);
        unit->moved = 1;
    } else {
        turn = 0;
    }

    if (hd < 8.0f)
        hd = 8.0f;

    float k = -sqrt((2.0f * f18) / hd);
    // The parentheses keep k * eps in st(2) instead of spilling it.
    float vx = ((float)da.x) * k * eps - (float)db.x * eps;
    float vz = (float)da.z * k * eps - (float)db.z * eps;
    float mag = (float)_hypot(vx, vz);
    if (mag > f18) {
        vx = vx * (f18 / mag);
        vz = vz * (f18 / mag);
    }

    AddFixed(velocity.x, vx);
    AddFixed(velocity.z, vz);
    speed = velocity.Length();

    // Own block so the address-taken delta shares a stack slot with db.
    {
        Vec3 delta = velocity - old;
        ApplyBankAndPitch(unit, &delta);
    }
}

// Moves the unit one step. With a path object it snaps to the path's next
// point (raised to the draft/sea-level floor for units with the type flag at
// +0x241 bit 19) and copies the path's velocity; without one it adds the
// velocity to the position, and if the unit moves into a new cell that the
// target says is blocked it clamps the position to the current cell and caps
// the speed at half the type's range.
// FUNCTION: 0x43d6d0
void UnitMotion::UpdatePosition(Unit* u)
{
    if (u->obj != 0) {
        // Copy through the returned pointer, not an initialiser.
        Vec3 v;
        v = GetPiecePosition(u->obj, u->index);
        if (u->type->b19) {
            // MAX stays a macro over the Fixed union with a prvalue second operand.
            v.fy = MAXM_0043d6d0(v.fy, MakeFixed_0043d6d0(u->type->draft * 0xffff + g_game->seaLevel));
        }
        SetUnitPosition(u, v, mode);
        Short3 o = GetPieceAngles(u->obj, u->index);
        u->f64 = o;
        if (u->obj->motion != 0) {
            speed = u->obj->motion->speed;
            velocity = u->obj->motion->velocity;
        } else {
            speed = 0;
            Vec3 zero(0, 0, 0);
            velocity = zero;
        }
        u->moved = 0;
        return;
    }

    // Assigned (not initialised) through the inline operator+: its result temporary
    // fixes the frame layout. The operands' order is the original's load order.
    Vec3 pos;
    pos = u->pos + velocity;
    int m = mode;
    if (pos.x == u->pos.x && pos.z == u->pos.z && pos.y == u->pos.y && m == u->mode)
        return;

    lastMoveTick = g_game->field_38a47;
    Point draft = u->draft;
    // Field by field, with draft.x * 0x80000 (a << 19 evaluates draft.x first).
    Point cell;
    cell.x = (pos.x - draft.x * 0x80000 + 0x80000) >> 20;
    cell.y = (pos.z - draft.y * 0x80000 + 0x80000) >> 20;
    if (cell.x == u->cell.x && cell.y == u->cell.y && m == u->mode) {
        u->pos = pos;
        u->moved = 1;
        return;
    }

    if (u->target->field_0 != 0) {
        if (u->target->type == 1 || u->target->type == 2)
            flag = CanPlaceUnitFootprint(u->type, u->id, cell, m) == 0;
    }

    if (flag) {
        // ClampToCell stays an inline helper taking both Points by value.
        ClampToCell(pos, u->cell, u->draft);

        if (speed > (u->type->maxvelocity / 2)) {
            int half = u->type->maxvelocity / 2;
            speed = half;
            unsigned short angle = u->f64.y;
            Vec3 vec;
            vec.x = -FUN_004b70ef(angle, half);
            vec.y = 0;
            // z goes through an int temporary.
            int z = -FUN_004b7123(angle, half);
            vec.z = z;
            velocity = vec;
        }
        u->pos = pos;
        u->moved = 1;
        return;
    }

    RemoveUnitFromMap(u);
    u->pos = pos;
    u->cell = cell;
    // The (short) cast gives the and/and/or store of the 2-bit field.
    u->mode = (short)m;
    AddUnitToMap(u);
    u->moved = 1;
    UpdateUnitLineOfSight(u);
}

// FUNCTION: 0x43da70
void UnitMotion::UpdateMoveRate(Unit* unit)
{
    int rate;
    if ((flags & 4) == 0 && unit->obj == 0
        && (speed != 0 || turn != 0)) {
        if (speed <= unit->type->field_1ae) {
            rate = 1;
        } else {
            rate = 2 + (speed > unit->type->field_1b2);
        }
    } else {
        rate = 0;
    }
    if (rate == (int)((unit->flags >> 2) & 3))
        return;
    if (rate == 0) {
        unit->script->StartScript("StopMoving", 0, 1);
    } else if ((unit->flags & 0xc) == 0) {
        unit->script->StartScript("StartMoving", 0, 1);
    }
    switch (rate) {
    case 1:
        unit->script->StartScript("MoveRate1", 0, 1);
        break;
    case 2:
        unit->script->StartScript("MoveRate2", 0, 1);
        break;
    case 3:
        unit->script->StartScript("MoveRate3", 0, 1);
        break;
    }
    unit->flags = (unit->flags & 0xfffffff3) | ((rate & 3) << 2);
}

// Constructor of the 0x2f-byte behaviour holder stored at unit+0. It zeroes two
// Vec3-shaped triples and a few scalars, mirrors a value from the unit type and
// then creates one of four behaviour objects, chosen by the unit's target type
// (byte at +0x73 == 3) and bit 11 of the unit type's flags at +0x241.
// FUNCTION: 0x43dc00
UnitMotion::UnitMotion(Unit* unit)
{
    velocity = Vec3(0, 0, 0);
    speed = 0;
    turn = 0;
    pathLockStamp = 0;
    p2 = Vec3(0, 0, 0);
    mode = 1;
    flag = 0;
    movementClass = unit->type->field_1b6;
    if (unit->target->field_0 != 0 && unit->target->type == 3) {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new PackedPosGoal(unit);
        else
            obj = (Iface_0043dd20*)new PatrolGoal(unit);
    } else {
        if (unit->type->flag_800)
            obj = (Iface_0043dd20*)new LiteGoal(unit);
        else
            obj = (Iface_0043dd20*)new AiSearchGoal(unit);
    }
}

// FUNCTION: 0x43dd10
void UnitMotion::DestroyObject()
{
    if (obj != 0) {
        obj->v0(1);
    }
}

// FUNCTION: 0x43dd20
void UnitMotion::UpdateMotion(Unit* u)
{
    obj->v2();
    if (u->type->flag_800)
        SteerAircraft(u);
    else
        SteerGroundUnit(u);
    UpdatePosition(u);
    UpdateMoveRate(u);
    ((Class_0043db50*)this)->UpdateSfxOccupy(u);
}

// FUNCTION: 0x43dd70
void UnitMotion::SaveMotion(Unit* info, HapiBank* file)
{
    char name[32];
    Record_0043dd70 hdr;
    hdr.velocity = velocity;
    hdr.p2 = p2;
    hdr.field_20 = speed;
    hdr.field_24 = turn;
    hdr.field_26 = pathLockStamp;
    hdr.mode = mode;
    hdr.flag = flag;
    sprintf(name, "u%04xmob", info->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 0x23);
}

// Load counterpart of 0x43dd70: reads this unit type's movement state back
// from the unit's "u%04xmob" entry.
// FUNCTION: 0x43de30
void UnitMotion::LoadMotion(Unit* unit, HapiBank* file)
{
    char name[32];
    Record_0043dd70 rec;
    sprintf(name, "u%04xmob", unit->id);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->ReadBox(&rec, 0x23);
    velocity = rec.velocity;
    p2 = rec.p2;
    speed = rec.field_20;
    turn = rec.field_24;
    pathLockStamp = rec.field_26;
    mode = rec.mode;
    flag = rec.flag;
}
