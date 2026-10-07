// Decompiled by space-bunny-free, Sonnet, Opus, Haiku, Claude Sonnet 5.5, Claude Opus 5.5 and DeepSeek V4.1 Flash. Names are provisional.
// The unit's order list: the nodes (Class_0043a1f0) linked through the unit's
// two lists at +0x5c and +0x60, their creation, insertion, deletion, and the
// dispatcher that walks the list and calls one helper per kind flag bit.
#include <stdio.h>
#include <string.h>

#include "../util/hapi_bank.h"

#pragma pack(push, 1)

struct Entry_0043a1f0 {              // 0x19-byte entries, table at DAT_00512344
    int value;                       // +0x0
    char unknown_4[0xc - 4];
    unsigned int field_c;            // +0xc, the flags 0x439b30 masks
    char unknown_10[0x11 - 0x10];
    unsigned int flags;              // +0x11, default flags of the kind
    char unknown_15[4];
};

struct Vec3_0043a1f0 {
    int x, y, z;
};

// The temporary the position is built through (the derived type is what
// makes the compiler materialise it).
struct Vec3Init_0043a1f0 : Vec3_0043a1f0 {
    Vec3Init_0043a1f0(int a, int b, int c) { x = a; y = b; z = c; }
};

// A 4-byte point of two shorts, and the temporary the constructor builds it
// through (the derived type is what makes the compiler keep the temporary).
struct Point_0043a1f0 {
    short x, y;
};

struct PointInit_0043a1f0 : Point_0043a1f0 {
    PointInit_0043a1f0(short a, short b) { x = a; y = b; }
};

// One of the unit's two order-target slots (+0x10); its target's +0xe4 is the
// divisor 0x439d20 uses.
struct Target_0043a1f0 {
    char unknown_0[0xe4];
    unsigned short field_e4;         // +0xe4
};

struct Slot_0043a1f0 {
    Target_0043a1f0* target;         // +0x0
    char unknown_4[0x18];
};

struct UnitType_0043a1f0 {           // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
    char unknown_40[0x241 - 0x40];
    unsigned char flags;             // +0x241
    char unknown_242[0x249 - 0x242];
};

struct Game {
    char unknown_0[0x1438f];
    int unitTypeCount;               // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043a1f0* unitTypes;    // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;              // +0x38a47
};

struct Unit;
class Class_0043a1f0;

// The 0x10-byte link at +0x12: its constructor puts the object in its
// owner's list; 0x489650 is its destructor.
class Class_004895c0 {
public:
    void* vptr;                      // +0x0
    Unit* owner;                     // +0x4
    Class_004895c0* next;            // +0x8
    void* value;                     // +0xc, the object the link belongs to

    void SetValue(void* v) { value = v; }

    Class_004895c0(Unit* o, int v);
    void SetUnit(Unit* o);
};

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);  // slot 0: 0x43a1e0, empty
};

class Class_0043a1f0 : public Class_0043a1e0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's: 0x438870 (defined
    // in order_queue_438870.cpp).
    virtual void FUN_0043a1e0(unsigned int);

    unsigned char kind;              // +0x4
    unsigned char flag5;             // +0x5
    unsigned int flags6;             // +0x6
    int last_id;                     // +0xa
    void* unit;                      // +0xe
    Class_004895c0 link;             // +0x12
    Vec3_0043a1f0 pos;               // +0x22
    Point_0043a1f0 field_2e;         // +0x2e
    Point_0043a1f0 field_32;         // +0x32
    int field_36;                    // +0x36
    int field_3a;                    // +0x3a
    int field_3e;                    // +0x3e
    unsigned int flags;              // +0x42
    unsigned int created;            // +0x46
    Class_0043a1f0* next;            // +0x4a
    int field_4e;                    // +0x4e
    void* attached;                  // +0x52

    Class_0043a1f0(unsigned char k, Unit* o, Vec3_0043a1f0* p, int a, int b, int c);
    ~Class_0043a1f0();
};

struct Unit {
    char unknown_0[0x10];
    Slot_0043a1f0 entries[2];        // +0x10
    char unknown_48[0x5c - 0x48];
    Class_0043a1f0* first;           // +0x5c
    Class_0043a1f0* firstTop;        // +0x60
    char unknown_64[0x6a - 0x64];
    Vec3_0043a1f0 pos;               // +0x6a
};
#pragma pack(pop)

// A partial view of Class_0043a1f0 (flags6 at +0x6, last_id at +0xa), named
// after its method's address: its callers spell this class.
class Class_00439e80
{
public:
    void FUN_00439e80(int param);
};

extern Game* g_game;
extern Entry_0043a1f0* DAT_00512344;

extern int __cdecl _strcmpi(const char*, const char*);

short __stdcall FindUnitTypeId(char* name);

void __stdcall DrawBuildFootprint(void* a, void* b, Class_0043a1f0* e, Vec3_0043a1f0* p, int c);
void __stdcall DrawUnitRangeRings(void* a, void* b, Class_0043a1f0* e, Vec3_0043a1f0* p, int c);
void __stdcall DrawPathAnim(void* a, void* b, Class_0043a1f0* e, Vec3_0043a1f0* p, int c);
void __stdcall DrawWeaponCoverage(void* a, void* b, Class_0043a1f0* e, Vec3_0043a1f0* p, int c);
void __stdcall DrawOrderRangeRing(void* a, void* b, Class_0043a1f0* e, Vec3_0043a1f0* p, int c);

// The list-walking dispatcher of the unit: for every object linked into the unit's
// list at +0x5c (link field at +0x4a, kind byte at +0x4) it looks up the kind's
// default flags in the table at DAT_00512344 (0x19-byte entries, the flags dword
// at +0xc, indexed by the kind byte) and, masked with the `mask` parameter, calls
// one of five helpers per bit: bit 0 -> 0x438c00, bit 1 -> 0x4394e0, bit 2 ->
// 0x4399f0, bit 3 -> 0x439740, bit 4 -> 0x4390a0. Bit 4 is only acted on for the
// first object of the list (`done`). Every helper is __stdcall with five dword
// arguments and takes the position as a pointer to a 12-byte object.
// FUNCTION: 0x439b30
void __stdcall FUN_00439b30(Unit* unit, unsigned int mask, void* obj,
                            void* sel, int flag)
{
    // Two copies, base first: each call but bit 4's is preceded by pos = base,
    // and the loop ends with base = pos.
    Vec3_0043a1f0 base = unit->pos;
    Vec3_0043a1f0 pos = unit->pos;
    bool done = false;
    for (Class_0043a1f0* e = unit->first; e != 0; e = e->next) {
        if (DAT_00512344[e->kind].field_c & mask & 1) {
            pos = base;
            DrawBuildFootprint(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].field_c & mask & 2) {
            pos = base;
            DrawPathAnim(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].field_c & mask & 4) {
            pos = base;
            DrawOrderRangeRing(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].field_c & mask & 8) {
            pos = base;
            DrawWeaponCoverage(obj, sel, e, &pos, flag);
        }
        if (DAT_00512344[e->kind].field_c & mask & 0x10) {
            if (!done) {
                DrawUnitRangeRings(obj, sel, e, &pos, flag);
                done = true;
            }
        }
        base = pos;
    }
}

// FUNCTION: 0x439cf0
int __stdcall FUN_00439cf0(void* param_1)
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
int __stdcall FUN_00439d20(Unit* owner)
{
    Class_0043a1f0* p;
    for (p = owner->firstTop; p; p = p->next) {
        if (p->flags & 0x80000)
            return p->field_3e * 100 / owner->entries[p->field_36].target->field_e4;
    }
    return 0;
}

// Sums the amounts of the nodes flagged 0x100 with the given index over both
// of the owner's node lists (+0x5c and +0x60).
// FUNCTION: 0x439d80
int __stdcall FUN_00439d80(Unit* owner, int index)
{
    int total = 0;
    Class_0043a1f0* p;
    for (p = owner->first; p; p = p->next) {
        if ((p->flags & 0x100) && p->field_36 == index)
            total += p->field_3a;
    }
    for (p = owner->firstTop; p; p = p->next) {
        if ((p->flags & 0x100) && p->field_36 == index)
            total += p->field_3a;
    }
    return total;
}

// FUNCTION: 0x439dd0
int __stdcall FUN_00439dd0(int param_1)
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
int __stdcall FUN_00439df0(Unit* obj)
{
    if (obj && obj->first) {
        return DAT_00512344[obj->first->kind].value;
    }
    return DAT_00512344[0].value;
}

// Finds the object's list node of the given kind; the kind table's flag
// 0x40000 selects which of the object's two lists (+0x60 or +0x5c) to search.
// FUNCTION: 0x439e30
Class_0043a1f0* __stdcall FUN_00439e30(Unit* obj, unsigned char kind)
{
    Class_0043a1f0* n;
    if (DAT_00512344[kind].flags & 0x40000)
        n = obj->firstTop;
    else
        n = obj->first;
    while (n) {
        if (n->kind == kind)
            return n;
        n = n->next;
    }
    return 0;
}

// FUNCTION: 0x439e80
void Class_00439e80::FUN_00439e80(int param)
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
    // Read through the owner, loop through a separate link pointer: one
    // shared local merges the two loads.
    Class_0043a1f0* first = owner->first;
    Class_0043a1f0** pp = &owner->first;
    Class_0043a1f0* node;
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
        Class_0043a1f0** head2 = &owner->firstTop;
        while ((node = *head2) != 0) {
            Class_0043a1f0* first2 = owner->first;
            Class_0043a1f0** link = (node->flags & 0x40000) ? head2 : &owner->first;
            for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
                if (n == node) {
                    *link = node->next;
                    if (node != first2) {
                        node->flags |= 0x10000;
                    }
                    delete node;
                    break;
                }
                link = &n->next;
            }
        }
    }
}

// Unlinks `node` from the owner's list at +0x5c (or +0x60 when the node has
// flag 0x40000) and deletes it; every node except the head of the +0x5c list
// is marked 0x10000 first. 0x439eb0 inlines the same code.
// FUNCTION: 0x439f80
void __stdcall DeleteOrder(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0* first = owner->first;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->firstTop : &owner->first;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
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

// FUNCTION: 0x439fe0
void __stdcall FUN_00439fe0(char* param_1, char* param_2)
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

// Links `child` into the parent's list in front of `before` (see 0x43b730.cpp).
static inline void InsertBefore(Unit* p, Class_0043a1f0* child, Class_0043a1f0* before)
{
    Class_0043a1f0** link = (child->flags & 0x40000) ? &p->firstTop : &p->first;
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
void __stdcall FUN_0043a020(Unit* p, Class_0043a1f0* item)
{
    int add = 1;
    for (Class_0043a1f0* c = p->first; c != 0; c = c->next) {
        if (c->flags & 0x8000) {
            add = 0;
            break;
        }
    }
    if (add) {
        Class_0043a1f0* child = new Class_0043a1f0(item->kind, 0, &p->pos, 0, 0, 0);
        InsertBefore(p, child, 0);
    }
    item->flags |= 0x8000;
}

// The constructor of Class_0043a1f0 (destructor 0x43a1f0, vtable 0x4fd2c8).
// The rest of the class is in order_queue_438870.cpp.
// It first stores 0x4fd2cc, the vtable of the inline constructor of the base
// class Class_0043a1e0, then runs the member initialisers, then stores its
// own vtable. Each vtable has one slot: the base's is the empty 0x43a1e0 and
// the derived class overrides it with 0x438870 (which ORs bits into +0x4e).
// The other constructor, 0x43a420 (which loads the object from a file),
// stores the same two vtables; the destructor stores only 0x4fd2c8, so the
// base has no destructor of its own.
// The link member's constructor puts the object in its owner's list and the
// body then points the link's value at the object; the kind's default flag
// bits come from the kind table at DAT_00512344, with bits 9 and 10 cleared
// when the list owner or the position pointer is absent.
// This view keeps `kind(k)` a plain member initialiser; the other file's view
// adds a second base that rules it out.
// FUNCTION: 0x43a0c0
Class_0043a1f0::Class_0043a1f0(unsigned char k, Unit* o, Vec3_0043a1f0* p, int a, int b, int c)
    : kind(k), link(o, 0), field_2e(PointInit_0043a1f0(0, 0)), field_32(PointInit_0043a1f0(0, 0)),
      field_36(a), field_3a(b), field_3e(c), created(g_game->ticks)
{
    // Through the inline method: a plain `link.value = this` reorders the
    // position pointer's compare before the vtable store.
    link.SetValue(this);
    flag5 = 0;
    flags6 = 0;
    field_4e = 0;
    last_id = -1;
    pos = p ? *p : Vec3Init_0043a1f0(0, 0, 0);
    flags = DAT_00512344[k & 0xff].flags;
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
// Class_0043a1e0 is the base of Class_0043a1f0 (vtable 0x4fd2c8), which
// overrides this slot with 0x438870 (see order_queue_438870.cpp).
// FUNCTION: 0x43a1e0
void Class_0043a1e0::FUN_0043a1e0(unsigned int)
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
void __stdcall FUN_0043ac60(Unit* owner, Class_0043a1f0* node,
                            Class_0043a1f0* before)
{
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->firstTop
                                                   : &owner->first;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->unit = owner;
    node->next = before;
    if (before != 0)
        node->flags |= before->flags & 0x4000;
}

// FUNCTION: 0x43acb0
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node)
{
    unsigned int which = node->flags & 0x40000;
    Class_0043a1f0* before = which ? owner->firstTop : owner->first;
    Class_0043a1f0** link = which ? &owner->firstTop : &owner->first;
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
void __stdcall FUN_0043ad10(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->firstTop : &owner->first;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next)
        link = &n->next;
    *link = node;
    node->unit = owner;
    node->next = 0;
}

// Marks a node with flag 0x1000 and links it into the owner's list at +0x5c
// just after the node currently carrying that mark (moving the mark to the
// new node), or at the end of the list when no node has it; compare 0x43ad10.
// FUNCTION: 0x43ad50
void __stdcall FUN_0043ad50(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0** link = &owner->first;
    node->unit = owner;
    node->flags |= 0x1000;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
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
static inline void UnlinkNode(Class_0043a1f0** link, Class_0043a1f0* node, Class_0043a1f0* first)
{
    *link = node->next;
    if (node != first)
        node->flags |= 0x10000;
    delete node;
}

// Unlinks `node` from the list starting at `list` and deletes it.
static inline void RemoveFromList(Class_0043a1f0** list, Class_0043a1f0* node, Class_0043a1f0* first)
{
    Class_0043a1f0** link = list;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            UnlinkNode(link, node, first);
            return;
        }
        link = &n->next;
    }
}

// Drops every node of the owner's +0x5c list that has no flag 4.
static inline void PruneLoose(Class_0043a1f0** link, Class_0043a1f0* first)
{
    Class_0043a1f0* n = *link;
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
    Class_0043a1f0** base = &owner->first;
    for (Class_0043a1f0* n = owner->first; n != 0; n = owner->first) {
        if (!(n->flags & 0x4000))
            break;
        // Keep the `node = n` copy and the fresh owner->list read at the call.
        Class_0043a1f0* node = n;
        RemoveFromList((node->flags & 0x40000) ? &owner->firstTop : base, node, owner->first);
    }
}

// Puts `node` in front of the list head its flag 0x40000 selects (0x43acb0).
static inline void AddFront(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0* before = (node->flags & 0x40000) ? owner->firstTop : owner->first;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->firstTop : &owner->first;
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
static inline void AddMarked(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0** link = &owner->first;
    node->unit = owner;
    node->flags |= 0x1000;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
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

// Creates a new Class_0043a1f0 node of the given kind and links it into the
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
                            Vec3_0043a1f0* pos, int param_6, int param_7)
{
    // The constructor's owner argument is the fourth parameter, not `owner`,
    // which stays in its stack slot.
    Class_0043a1f0* obj = new Class_0043a1f0(kind, (Unit*)id, pos, param_6, param_7, 0);

    if (remove == 0 && !(obj->flags & 0x40))
        // Link and head passed separately so the two loads of owner->list stay.
        PruneLoose(&owner->first, owner->first);

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
