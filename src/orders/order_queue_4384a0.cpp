// Decompiled by Opus, Haiku, deepseek-v4.1, DeepSeek V4.1 Flash, deepseek-v4.1-flash, space-bunny-free, Space Bunny Free and Claude Opus 5.5. Names are provisional.
// The build-queue object Order and the code around it: the unit's
// list of orders at +0x5c, the order-type table at g_missionOrderTableBegin, the
// StartBuilding and StopBuilding script calls and the objects attached to an
// order. A build-queue object holds a kind from the order-type table, the unit
// it belongs to, a link into its owner's list and an attached object at +0x52.
// Its base (vtable 0x4fd2cc, one slot, the empty 0x43a1e0) has no destructor
// of its own, so the destructor stores only the derived vtable 0x4fd2c8.
#include <stdio.h>
#include <string.h>

class Order;
class HapiBank;

#pragma pack(push, 1)
struct Vec3_0043a1f0 {
    int x, y, z;
};

class CobScript;
class Slot_0043a1f0;

struct Owner_0043a1f0 {
    Slot_0043a1f0* slot;               // +0x0
};

struct UnitType {                      // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x18a - 0x40];
    float metalCost;                   // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int maxHealth;            // +0x1fa
    unsigned short field_1fe;          // +0x1fe
    char unknown_200[0x241 - 0x200];
    union {
        unsigned char flags;           // +0x241
        unsigned int flags32;          // +0x241
    };
    char unknown_245[0x249 - 0x245];
};

struct Unit {                          // 0x118 bytes
    Owner_0043a1f0* owner;             // +0x0
    char unknown_4[0x5c - 0x4];
    Order* first;                      // +0x5c
    Order* firstTop;                   // +0x60, for children with flag 0x40000
    char unknown_64[0x86 - 0x64];
    int carrier;                      // +0x86
    char unknown_8a[0x92 - 0x8a];
    UnitType* type;                    // +0x92
    char unknown_96[0x9a - 0x96];
    CobScript* names;                  // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short typeId;             // +0xa8
    char unknown_aa[0xb8 - 0xaa];
    unsigned short killCount;           // +0xb8
    char unknown_ba[0x110 - 0xba];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];

    void ReleaseWeapons(int param_1);
};

struct Entry_0043a1f0 {                // 0x19-byte entries at g_missionOrderTableBegin
    char unknown_0[4];
    void (__stdcall* notify)(Unit* unit, Order* obj, int code);            // +0x4
    char unknown_8[0x11 - 0x8];
    union {
        unsigned int flags;            // +0x11, default flags of the kind
        struct {
            char unknown_11[3];
            unsigned char flag14;      // +0x14
        };
    };
    char* name;                        // +0x15
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x1438f - 0x1435b];
    int unitTypeCount;                 // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType* unitTypes;               // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;                // +0x38a47
};

struct SaveDesc_0043a1f0 {             // the 0x3a-byte snapshot, read and written raw
    unsigned short unitType;           // +0x00
    unsigned short ownerType;          // +0x02
    int field_4;                       // +0x04
    unsigned char kind;                // +0x08
    unsigned char flag5;               // +0x09
    unsigned int flags6;               // +0x0a
    int last_id;                       // +0x0e
    Vec3_0043a1f0 pos;                 // +0x12
    int field_1e;                      // +0x1e
    int field_22;                      // +0x22
    int field_26;                      // +0x26
    int field_2a;                      // +0x2a
    int field_2e;                      // +0x2e
    unsigned int flags;                // +0x32
    int field_36;                      // +0x36
};
#pragma pack(pop)

#include "../units/cob_script.h"

#include "../units/unit_ref.h"

// The parsed text file the writer is handed (the same object as HapiBank).
class File_0043a970 {
public:
    char unknown_0[1];
};

#include "../util/vec3.h"
#include "air_maneuver_order.h"

// Unused here: forward declarations of real functions; their symbol ids keep
// ComputeReclaimDamagePulse (0x438650) matching (docs/c2-regalloc.md).
void BeginMouseScroll();
void EndMouseScroll();
void UpdateMouseScroll();
void UpdateEdgeScroll();
void CenterCameraOnRadarClick();
void CenterCameraOnStartPosition();
void RegisterDataArchives();
void OpenEndMissionScreen();
int GetCdPathMismatch();
void CreateGameObject();
void InitMissionStatus();
void SetUpEndMissionScreen();
void StartScreenFade();
void StepScreenFade();
void ScheduleFadeTick();
int IsFadeDone();

class Slot_0043a1f0 {
public:
    virtual void Slot0();
    virtual void Attach(void* obj);
    OrderFx* current;                  // +0x4
};

int __stdcall OrderTypeNameLess(int param_1, char* param_2);
Entry_0043a1f0* __stdcall LowerBoundOrderTypes(Entry_0043a1f0* first, Entry_0043a1f0* last,
                                       char* const& value, int(__stdcall* pred)(int, char*),
                                       int* unused);
Unit* __stdcall LoadUnit(unsigned short id, void* file);
unsigned short __stdcall FindUnitTypeId(const char* name);
int __stdcall SendScriptCallNoArgs(Unit* obj, short index);
void __stdcall SendScriptCall(Unit* obj, int index, int param_3, int param_4, int param_5, int param_6, int param_7);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);

extern Game* g_game;
extern Entry_0043a1f0* g_missionOrderTableBegin;
extern Entry_0043a1f0* g_missionOrderTableEnd;

// The 0x10-byte link at +0x12: its constructor puts the object in its
// owner's list; 0x489650 is its destructor.
class PathOrderAttach {
public:
    void* vptr;                        // +0x0
    Unit* owner;                       // +0x4
    PathOrderAttach* next;             // +0x8
    void* value;                       // +0xc, the object the link belongs to

    void SetValue(void* v) { value = v; }

    PathOrderAttach(Unit* o, int v);
    void SetUnit(Unit* o);
};

// An order type held as its index in the sorted order-type table. Callers
// build it from a name as a by-value temporary (0x403260, 0x4118e0, ...), so
// this is its constructor.
class Class_00438760 {
public:
    unsigned char index;
    char unknown_1[3];
    Class_00438760(const char* name);
};

// The attachments the file constructor makes, by the kind of attachment.
#pragma pack(push, 1)
class Class_0044de80 : public OrderFx {
public:
    char pad[0x2e];
    Class_0044de80(int owner, HapiBank* file, char* name);
};

// The attachments the order functions make from a position.
struct Source_0044cf60;
struct Point_00438ad0 {
    short x;
    short y;
};

class ApproachRadius : public OrderFx {
public:
    char pad[0xc];
    ApproachRadius(int owner, HapiBank* file, char* name);
    ApproachRadius(Source_0044cf60* source, int x, int y, int r);
    // Unused here: the type's members the other views declare keep the symbol
    // ids of the functions after the merged classes (docs/c2-regalloc.md).
    void* Destroy(unsigned char flag);
    int ContainsCell(int px, int py);
    int FillWorldPos(int* out);
    int ApproxDistExcess(int px, int py);
    int Serialize(int unused, HapiBank* file, char* name);
};

class RingApproach : public OrderFx {
public:
    char pad[0x14];
    RingApproach(int owner, HapiBank* file, char* name);
    RingApproach(void* source, int x, int y, int r1, int r2);
    // Unused here: the type's members the other views declare keep the symbol
    // ids of the functions after the merged classes (docs/c2-regalloc.md).
    void* Destroy(unsigned char flag);
    int ContainsCell(int param_1, int param_2);
    int ContainsUnit(Unit* unit);
};

class PointMarker : public OrderFx {
public:
    char pad[0x10];
    PointMarker(int owner, HapiBank* file, char* name);
    PointMarker(void* owner, Point_00438ad0 pos, Point_00438ad0 size);
    // Unused here: the type's members the other views declare keep the symbol
    // ids of the functions after the merged classes (docs/c2-regalloc.md).
    void* Destroy(unsigned char should_delete);
    int ContainsCell(int x, int y);
    int ApproxDist(int x, int y);
};
#pragma pack(pop)

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void OrStatusFlags(unsigned int);  // slot 0: 0x43a1e0, empty
};

#pragma pack(push, 1)
// The fields between the vtable pointer and the link, as a second base whose
// inline constructor clears the kind.
struct Head_0043a1f0 {
    unsigned char kind;                // +0x4
    unsigned char flag5;               // +0x5
    unsigned int flags6;               // +0x6
    int last_id;                       // +0xa
    Unit* unit;                        // +0xe

    // The second base clears the kind: as a member initialiser the stores reorder.
    Head_0043a1f0() : kind(0) {}
};

class Order : public Class_0043a1e0, public Head_0043a1f0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's.
    virtual void OrStatusFlags(unsigned int);

    PathOrderAttach link;              // +0x12
    Vec3_0043a1f0 pos;                 // +0x22
    int field_2e;                      // +0x2e
    int field_32;                      // +0x32
    int field_36;                      // +0x36
    int field_3a;                      // +0x3a
    int field_3e;                      // +0x3e
    unsigned int flags;                // +0x42
    unsigned int created;              // +0x46
    Order* next;                       // +0x4a
    int field_4e;                      // +0x4e
    OrderFx* attached;                 // +0x52

    // The real constructor is 0x43a0c0, in order_list.cpp: it needs
    // `kind(k)` as a plain member initialiser, which this class's second base
    // rules out (98.9%).
    Order(Class_00438760, int, void*, int, int, int);
    ~Order();
    Order(Unit* punit, HapiBank* file, char* name);
    int SerializeToSave(Unit* punit, File_0043a970* file, char* name);
    void AnnounceStatusIfFlagged(char* text);
    void ReattachFxToUnit();
    void SetAttachedFx(OrderFx* obj);
    void AttachApproachRadiusGoal(int* p, int n);
    void AttachRingApproachGoal(Vec3_0043a1f0* pos, int radius1, int radius2);
    void AttachBuildFootprintMarker(Point_00438ad0 cell, Point_00438ad0 size);
    void MergeFlagsFromTable(int k);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    Unit* Target();
};
#pragma pack(pop)

// Links `child` into the owner's list in front of `before`.
static inline void InsertBefore(Unit* p, Order* child, Order* before)
{
    Order** link = (child->flags & 0x40000) ? &p->firstTop : &p->first;
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

// If the owner has a count at +0x86, drop every object without flag 4 from its
// list at +0x5c (marking all but the head 0x10000 first, as DeleteOrders does),
// then build a "BECARRIED" object and insert it at the head of the list its
// own flag 0x40000 selects.
// FUNCTION: 0x4384a0
void __stdcall AddBeCarriedOrder(Unit* p)
{
    if (p->carrier) {
        Order* first = p->first;
        Order** pp = &p->first;
        Order* node;
        while ((node = *pp) != 0) {
            if (node->flags & 4) {
                pp = &node->next;
            } else {
                *pp = node->next;
                if (node != first) {
                    node->flags |= 0x10000;
                }
                delete node;
            }
        }
        Order* child =
            new Order("BECARRIED", p->carrier, 0, 0, 0, 0);
        InsertBefore(p, child, (child->flags & 0x40000) ? p->firstTop : p->first);
    }
}

// FUNCTION: 0x438590
void __stdcall StartBuildingScript(Unit* obj, Order* target, unsigned short param_3)
{
    int index = obj->names->FindScript("StartBuilding");
    ((CobScript*)obj->names)->StartScriptWithArgsByIndex(index, 0, 0, 1, param_3, 0, 0, 0);
    SendScriptCall(obj, index, 1, param_3, 0, 0, 0);
    target->flags |= 0x400000;
}

// FUNCTION: 0x4385f0
void __stdcall StopBuildingScript(Unit* obj, Order* target)
{
    if (target->flags & 0x400000) {
        int index = obj->names->FindScript("StopBuilding");
        ((CobScript*)obj->names)->StartScriptWithArgsByIndex(index, 0, 0, 0, 0, 0, 0, 0);
        SendScriptCallNoArgs(obj, index);
        target->flags &= ~0x400000;
    }
}

// FUNCTION: 0x438650
int __stdcall ComputeReclaimDamagePulse(Unit* a, Unit* b, int n)
{
    UnitType* bt = b->type;
    float v = bt->metalCost > 10.0f ? bt->metalCost : 10.0f;
    // The 64-bit numerator is built by hand (signed 32-bit chain in lo, zero hi):
    // keeps the multiply order and the unsigned fild qword.
    union {
        __int64 q;
        struct {
            int lo;
            int hi;
        } w;
    } p;
    p.w.lo = a->type->field_1fe * ((a->killCount + 5) / 5) * (int)bt->maxHealth * n;
    p.w.hi = 0;
    int r = (int)((double)p.q / (v * 300.0f));
    if (r <= 1) {
        r = 1;
    }
    return r;
}

// FUNCTION: 0x438700
unsigned int __stdcall WaitIfNotInBuildStance(void* param_1, void* param_2, unsigned int param_3)
{
    if ((*(unsigned char*)((char*)param_1 + 0x10f) & 1) == 0) {
        *(unsigned int*)((char*)param_2 + 6) = param_3 | 4;
        return 2;
    }
    return 1;
}

// FUNCTION: 0x438730
int __stdcall WaitIfCobBusy(char* param_1, char* param_2, unsigned int param_3)
{
    if (*(unsigned char*)(param_1 + 0x10f) & 2) {
        *(unsigned int*)(param_2 + 6) = param_3 | 4;
        return 2;
    }
    return 1;
}

extern int __cdecl _strcmpi(const char*, const char*);

// FUNCTION: 0x438760
Class_00438760::Class_00438760(const char* name)
{
    Entry_0043a1f0* first = g_missionOrderTableBegin;
    int n = g_missionOrderTableEnd - g_missionOrderTableBegin;
    for (; 0 < n; ) {
        int n2 = n / 2;
        Entry_0043a1f0* m = first;
        m += n2;
        int less = _strcmpi(m->name, name) < 0;
        if (less)
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    if (first != g_missionOrderTableEnd && _strcmpi(first->name, name) == 0) {
        index = (unsigned char)(first - g_missionOrderTableBegin);
        return;
    }
    index = 0;
}

struct OrderType;

struct OrderType
{
public:
    int GetTableEntry();
};

// FUNCTION: 0x438830
int OrderType::GetTableEntry()
{
    unsigned int result = 0;
    result = *(unsigned char*)this;
    int* base = (int*)&g_missionOrderTableBegin;
    int fives = result * 5;
    return *base + fives * 4 + fives;
}

// FUNCTION: 0x438850
int __fastcall GetTableEntryByTypeByte(unsigned char* param_1)
{
    unsigned int eax = 0;
    eax = *param_1;
    eax = eax + eax * 4;
    int edx = (int)g_missionOrderTableBegin + eax * 4;
    eax = eax + edx;
    return eax;
}

// FUNCTION: 0x438870
void Order::OrStatusFlags(unsigned int param_1)
{
    field_4e |= param_1;
}

// Clears flag 0x2000 of an order and, if it was set, posts message kind 5
// with the given text for the order's unit.
// FUNCTION: 0x438880
void Order::AnnounceStatusIfFlagged(char* text)
{
    if (flags & 0x2000) {
        flags &= ~0x2000;
        QueueUnitSpeech(unit, 5, text);
    }
}

// FUNCTION: 0x4388b0
void Order::ReattachFxToUnit()
{
    if (attached != 0) {
        void* p1 = *(void**)unit;
        void* p2 = *(void**)p1;
        void* p3 = *(void**)p2;
        void* fn_ptr = *(void**)((char*)p3 + 4);
        ((void (__stdcall*)(int))fn_ptr)((int)attached);
    }
}

// FUNCTION: 0x4388d0
void Order::SetAttachedFx(OrderFx* obj)
{
    if (unit->owner) {
        if (attached) {
            unit->owner->slot->Attach(0);
            delete attached;
            attached = 0;
        }
        if (obj) {
            field_4e &= ~0x3e0;
            unit->owner->slot->Attach(obj);
            attached = obj;
        }
    }
}

// Creates an ApproachRadius from an order position when the unit's definition
// does not have flag 0x800 set, detaches the current attachment and attaches
// the new one. Same class as 0x4388d0; the flag test comes first here.
// FUNCTION: 0x438930
void Order::AttachApproachRadiusGoal(int* p, int n)
{
    if ((unit->type->flags32 & 0x800) == 0) {
        ApproachRadius* obj = new ApproachRadius((Source_0044cf60*)this, p[0], p[2], n);
        if (unit->owner) {
            if (attached) {
                unit->owner->slot->Attach(0);
                delete attached;
                attached = 0;
            }
            if (obj) {
                field_4e &= ~0x3e0;
                unit->owner->slot->Attach(obj);
                attached = obj;
            }
        }
    } else {
        if (unit->owner && attached) {
            unit->owner->slot->Attach(0);
            delete attached;
            attached = 0;
        }
    }
}

// FUNCTION: 0x438a00
void Order::AttachRingApproachGoal(Vec3_0043a1f0* pos, int radius1, int radius2)
{
    if (!(unit->type->flags32 & 0x800)) {
        RingApproach* obj = new RingApproach(this, pos->x, pos->z, radius1, radius2);
        if (unit->owner) {
            if (attached) {
                unit->owner->slot->Attach(0);
                delete attached;
                attached = 0;
            }
            if (obj) {
                field_4e &= ~0x3e0;
                unit->owner->slot->Attach(obj);
                attached = obj;
            }
        }
    } else {
        if (unit->owner) {
            if (attached) {
                unit->owner->slot->Attach(0);
                delete attached;
                attached = 0;
            }
        }
    }
}

// FUNCTION: 0x438ad0
void Order::AttachBuildFootprintMarker(Point_00438ad0 cell, Point_00438ad0 size)
{
    if (!(unit->type->flags32 & 0x800)) {
        PointMarker* obj = new PointMarker(this, cell, size);
        if (unit->owner) {
            if (attached) {
                unit->owner->slot->Attach(0);
                delete attached;
                attached = 0;
            }
            if (obj) {
                field_4e &= ~0x3e0;
                unit->owner->slot->Attach(obj);
                attached = obj;
            }
        }
    } else {
        if (unit->owner) {
            if (attached) {
                unit->owner->slot->Attach(0);
                delete attached;
                attached = 0;
            }
        }
    }
}

// Changes the object's kind: stores the new kind and reloads its flags from
// the kind table (g_missionOrderTableBegin, 0x19-byte entries, default flags at +0x11),
// keeping the object's own bits 9 and 10 (0x600; the constructor 0x43a0c0
// clears them individually). The layout (kind at +0x4, flags at +0x42) is
// Order's.
// FUNCTION: 0x438b90
void Order::MergeFlagsFromTable(int k)
{
    kind = k;
    // The two table index expressions must differ, or the entry load is shared.
    flags = ((g_missionOrderTableBegin[k & 0xff].flags ^ flags) & 0x600) ^ g_missionOrderTableBegin[kind].flags;
}

// FUNCTION: 0x438be0
int __stdcall GetOrderFlags(void* param)
{
    if (!param) return 0;
    void* ptr1 = *(void**)((char*)param + 0x5c);
    if (!ptr1) return 0;
    return *(int*)((char*)ptr1 + 0x42);
}

#include "../util/hapi_bank.h"

// Destructor (callers do `if (p) { p->~X(); operator delete(p); }`): notifies
// the owner through a callback table, stops the unit's build animation (the
// same code as StopBuildingScript), releases the attached object at +0x52 and
// unlinks the list node at +0x12 (0x489650 is the link's destructor).
// FUNCTION: 0x43a1f0
Order::~Order()
{
    if (flags6 & 2) {
        g_missionOrderTableBegin[kind].notify(unit, this, 2);
    }
    if (flags & 0x400000) {
        Unit* obj = unit;
        int index = obj->names->FindScript("StopBuilding");
        obj->names->StartScriptWithArgsByIndex(index, 0, 0, 0, 0, 0, 0, 0);
        SendScriptCallNoArgs(obj, index);
        flags &= ~0x400000;
    }
    if (unit->owner != 0) {
        Slot_0043a1f0* slot = unit->owner->slot;
        if (slot->current != 0 && slot->current == attached) {
            if (attached != 0) {
                slot->Attach(0);
                delete attached;
                attached = 0;
            }
        } else {
            delete attached;
            attached = 0;
        }
    }
    if (!(flags & 0x10000)) {
        unit->ReleaseWeapons(3);
    }
    ((UnitRef*)&link)->Unlink();
}

// The real resolver at 0x43a360 (matched in its own file), defined here without
// its annotation as in the original file, which /Ob2 inlines into the constructor.
short __stdcall ResolveUnitTypeKey(HapiBank* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (file->HasItem(key))
        return FindUnitTypeId(file->GetStringItem(key, 0));
    int i, n = 0;
    unsigned short k = 0;
    for (i = 1; i < g_game->unitTypeCount; i++, n++) {
        if (!(g_game->unitTypes[(unsigned short)i].flags & 0x20)) {
            if (k == id)
                return n;
            k++;
        }
    }
    return 0;
}

static unsigned char KindByName_0043a420(char* s)
{
    Entry_0043a1f0* e = LowerBoundOrderTypes(g_missionOrderTableBegin, g_missionOrderTableEnd, s, OrderTypeNameLess, 0);
    if (e == g_missionOrderTableEnd || _strcmpi(e->name, s) != 0)
        return 0;
    return (unsigned char)(e - g_missionOrderTableBegin);
}

static unsigned char KindByIndex_0043a420(unsigned char want)
{
    // Stays a helper with idx declared before k: inline, the scan keeps its counter in ECX.
    int idx = 0;
    int k = 0;
    for (Entry_0043a1f0* p = g_missionOrderTableBegin; p <= g_missionOrderTableEnd; p++, idx++) {
        if (!(p->flag14 & 1)) {
            if (k == want)
                break;
            k++;
        }
    }
    return (unsigned char)idx;
}

// The constructor of Order that loads the object from the parsed
// text file (the other one is 0x43a0c0; 0x43a970 is the matching writer).
// data/symbols.csv still names this address Order::Order
// (the name its caller 0x487080 uses), but the vtables it stores are
// Order's, so it is that class's second constructor.
//
// The kind is resolved by two helpers, one per branch, each
// returning an unsigned char: by name through the sorted kind table
// (lower_bound), or, for files without a "<name>_name" key, by counting the
// entries without flag bit 0. The real resolver
// ResolveUnitTypeKey is defined above.
// The case-3 attachment constructor is AirManeuverOrder's second constructor,
// 0x44e7d0, which shares its name with 0x44e740; data/aliases.csv has a row
// for it.
// FUNCTION: 0x43a420
Order::Order(Unit* punit, HapiBank* file, char* name)
    : link(0, 0)
{
    link.SetValue(this);
    link.SetUnit(0);
    next = 0;
    attached = 0;
    created = g_game->ticks;
    if (file == 0)
        return;
    if (name == 0)
        return;
    if (strlen(name) > 0x1f)
        return;

    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    SaveDesc_0043a1f0 desc;
    if (((HapiBank*)file)->ReadBox(&desc, 0x3a) != 0x3a)
        return;

    char buf1[0x80];
    sprintf(buf1, "%s%s", name, "_name");
    {
        char* s = file->GetStringItem(buf1, 0);
        if (s != 0)
            desc.kind = KindByName_0043a420(s);
        else
            desc.kind = KindByIndex_0043a420(desc.kind);
    }

    Unit* u;
    if (desc.unitType == 0)
        u = 0;
    else
        u = g_game->units + desc.unitType;
    if (punit != u)
        return;
    if (desc.kind == 0)
        return;

    unit = punit;
    link.SetUnit(LoadUnit(desc.ownerType, file));

    kind = desc.kind;
    flag5 = desc.flag5;
    flags6 = desc.flags6;
    last_id = desc.last_id;
    pos = desc.pos;
    field_2e = desc.field_1e;
    field_32 = desc.field_22;
    field_36 = desc.field_26;
    field_3a = desc.field_2a;
    field_3e = desc.field_2e;
    flags = desc.flags;
    field_4e = desc.field_36;

    char* sname = g_missionOrderTableBegin[desc.kind].name;
    if (strcmp(sname, "MobileBuild") == 0 || strcmp(sname, "VTOL_MobileBuild") == 0 ||
        strcmp(sname, "BuildingBuild") == 0) {
        field_36 = (unsigned short)ResolveUnitTypeKey((HapiBank*)file, (unsigned short)field_36);
    }

    char buf3[0x20];
    sprintf(buf3, "%s%s", name, "g");
    switch (desc.field_4) {
    case 2:
        attached = new Class_0044de80((int)this, file, buf3);
        return;
    case 3:
        attached = new AirManeuverOrder((int)this, file, buf3);
        return;
    case 4:
        attached = new ApproachRadius((int)this, file, buf3);
        return;
    case 5:
        attached = new RingApproach((int)this, file, buf3);
        return;
    case 6:
        attached = new PointMarker((int)this, file, buf3);
        return;
    case 0:
        attached = 0;
        return;
    case 1:
        attached = 0;
        return;
    default:
        attached = 0;
        return;
    }
}

// Serialises one build-queue object (Order) into the parsed text file
// under the key `name`: a raw 0x3a-byte snapshot, a "<name>_name" string value
// pointing at the kind's name, a "UTYPENAME<id>" value when the object is a
// build kind, and finally "<name>g" handed to the attached object.
// FUNCTION: 0x43a970
int Order::SerializeToSave(Unit* punit, File_0043a970* file, char* name)
{
    if (unit->typeId != punit->typeId || !file || !name)
        return 0;
    if (strlen(name) > 0x1f)
        return 0;

    SaveDesc_0043a1f0 desc;
    // A local copy of the pointer and the test written as `== 0` is what the
    // original wants here: the copy stops if-conversion, and the polarity
    // gives the `jne` over the zero store instead of the `je` after it.
    Unit* u = unit;
    if (u == 0)
        desc.unitType = 0;
    else
        desc.unitType = u->typeId;
    // The owner is tested for null twice on purpose: mixing the copy with a
    // fresh read of link.owner keeps the compiler from dropping the third test.
    Unit* o = link.owner;
    desc.ownerType = (o != 0 && (link.owner->flags & 0x10000000) != 0 && link.owner != 0) ? o->typeId : 0;
    desc.field_4 = attached ? attached->GetType() : 0;
    desc.kind = kind;
    desc.flag5 = flag5;
    desc.flags6 = flags6;
    desc.last_id = last_id;
    desc.pos = pos;
    desc.field_1e = field_2e;
    desc.field_22 = field_32;
    desc.field_26 = field_36;
    desc.field_2a = field_3a;
    desc.field_2e = field_3e;
    desc.flags = flags;
    desc.field_36 = field_4e;

    ((HapiBank*)file)->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&desc, 0x3a);

    char buf1[0x80];
    sprintf(buf1, "%s%s", name, "_name");
    ((HapiBank*)file)->SetStringItem(buf1, g_missionOrderTableBegin[kind].name);

    char* s = g_missionOrderTableBegin[kind].name;
    if (strcmp(s, "MobileBuild") == 0 || strcmp(s, "VTOL_MobileBuild") == 0 ||
        strcmp(s, "BuildingBuild") == 0) {
        unsigned short id = field_36;
        char buf2[0x80];
        sprintf(buf2, "UTYPENAME%4d", id);
        if (!((HapiBank*)file)->HasItem(buf2) && id >= 1 && id < g_game->unitTypeCount)
            ((HapiBank*)file)->SetStringItem(buf2, g_game->unitTypes[id].name);
    }

    if (desc.field_4 != 0) {
        char buf3[0x20];
        sprintf(buf3, "%s%s", name, "g");
        attached->SerializeToSave(this, (HapiBank*)file, buf3);
    }
    return 1;
}
