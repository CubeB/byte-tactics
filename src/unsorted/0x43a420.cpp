// Decompiled by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// Started by deepseek-v4.1-flash, continued by GPT-6, finished by deepseek-v4.1,
// extended by space-bunny-free. Names are provisional.
// Partial: 93.8% (real check.py runs: 3 in total for this session, every other
// experiment scored free with check.py --sym). Exact 1300-byte size and 0x164-byte
// frame. Two differences are left, and the first one is the whole ballgame:
//
// 0. THE FALLBACK SCAN AT 0x43a556: the original keeps the raw scan index in
//    ECX and the filtered (compared) counter in EDX, and copies the result
//    with `mov dl,cl` at the join; ours keeps the compared counter in ECX and
//    the raw index in EDX, so the store needs no copy. The plain
//    `if (k == desc.kind) break;` form (not the `continue` form used below)
//    is otherwise byte-identical in this region: it already gives
//    ESI = end pointer, EDI = desc.kind, EAX = p, and it is exactly 2 bytes
//    short, which is only the missing `mov dl,cl`. That 2 bytes shift every
//    later branch target and the switch jump table, so the plain form scores
//    86.2% and the `continue` form (which fakes the 2 bytes with an extra
//    `inc` + `jmp`) scores 93.8%. Fix the register choice and the `continue`
//    hack becomes unnecessary.
//    What is known about the cause: MSVC 5 weights register priority by loop
//    nesting, and the variable used in the loop's COMPARE wins ECX in every
//    spelling tried (declaration order, int/unsigned short/unsigned char/long
//    /register on either counter, hoisting the end pointer into a named
//    local, hoisting desc.kind into a local, for / while(1) / do-while,
//    ++i vs i = i + 1, and an extra use after the loop: all still give ECX to
//    the compared counter, 83.7% to 86.2%). Declaration order does move the
//    two `xor`s, but not the roles. The weighting theory is confirmed
//    directly: adding ONE in-scope use of the raw index inside the loop (a
//    byte local assigned from idx each iteration, scratch g1) does flip it,
//    raw index into ECX with the `mov dl,cl` appearing as in the original,
//    but it pushes the compared counter out to the stack. So the missing
//    construct is one extra in-loop USE of the raw index that emits no
//    instructions. A dead in-loop store of desc.kind does not count: MSVC kills
//    it before the allocator runs (and the whole function then moves `this`
//    into esi, 53.9%). A store of desc.kind inside the break arm is also
//    killed early and changes nothing (86.2%).
//    Also worth knowing: naming the end pointer (`Entry* last = ...`) is not
//    neutral, it moves the end pointer to ECX and the compared counter to ESI.
//    Still ECX-for-the-compared-counter in every one of these, all scored free
//    with check.py --sym (86.2%, i.e. the same as the plain form):
//    `int idx` before `int k` and the other way round, `unsigned int` counters,
//    `k = k + 1` / `idx = idx + 1`, `p++` before `idx++`, a `for` whose latch is
//    `p++, idx++` (both orders), a `for` that initialises all three in its
//    condition, `while (1) { if (p > end) break; ... }`, a third dead
//    counter incremented in the latch, a `unsigned char f = p->flag14` read
//    into a local, a local copy `int t = k` as the compared operand, and a
//    two-armed `if (flag) idx++; else { ... }` written so the latch is shared
//    by tail merging. Three forms are much WORSE and are known dead ends:
//    `unsigned int want = desc.kind;` hoisted before the loop (69.5%: the hoisted
//    copy takes over the register the original keeps for the raw index and the
//    whole region is reallocated), `if (k++ == desc.kind) break;` (64.5%:
//    the post-increment sinks k out of a register into EBP), and
//    `p += 0x19` (95.1% but WRONG, see below).
//    TRAP, do not chase it: writing `p += 0x19` instead of `p++` scores 95.1%
//    with 45 differing lines, the best number this function has ever shown, and
//    it is a BUG: `p += 0x19` advances 0x19 ELEMENTS of a 0x19-byte struct, so
//    MSVC emits `add eax, 0x271` (25 * 25 bytes per step) and the scan walks
//    off the end of the table. The plain `p++` is the only correct spelling.
// 1. PROLOGUE SCHEDULING: the original stores the base-class vtable between
//    the two `push edi` argument pushes of the link member's constructor
//    (push edi / mov [ebp],0x4fd2cc / push edi / mov ecx,esi / mov byte
//    [ebp+4],0 / call 0x4895c0); ours emits both pushes and mov ecx,esi
//    first, then the store. Writing link(0,0) before kind(0) in the init list,
//    giving Class_0043a1e0 an explicit empty constructor (with and without
//    __inline), listing the base explicitly, calling the link constructor
//    from the body, `kind = 0` as a body statement, naming link's first
//    argument, and a base constructor taking an unused int (the argument push
//    is dropped, so the order is unchanged) all leave it alone. The scheduler
//    is what moves that store, so the fix is something that changes the node
//    order of the prologue, not the order of the init list.
//    Note the two remaining diffs look independent but may be one allocator
//    state, as they often are: the vtable store is a node in the same block
//    that seeds EBX for `file`, and `file` is the range that has to be
//    reloaded twice inside the scan loop.
// Preserve the inclusive fallback-table scan: 0x43a58d uses JBE even though
// the named lookup passes the same end pointer to exclusive lower_bound.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Vec3_0043a420 {
    int x, y, z;
};

struct Entry_0043a420 {                // 0x19-byte entries at DAT_00512344
    char unknown_0[0x14];
    unsigned char flag14;              // +0x14
    char* name;                        // +0x15
};

struct Unit_0043a420 {                 // 0x118 bytes
    char unknown_0[0x118];
};

struct UnitType_0043a420 {             // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x241 - 0x40];
    unsigned char flags;               // +0x241
    char unknown_242[0x249 - 0x242];
};

struct Game_0043a420 {
    char unknown_0[0x14357];
    Unit_0043a420* units;              // +0x14357
    char unknown_1435b[0x1438f - 0x1435b];
    int unitTypeCount;                 // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043a420* unitTypes;      // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SaveDesc_0043a420 {             // the 0x3a-byte snapshot, read raw
    unsigned short unitType;           // +0x00
    unsigned short ownerType;          // +0x02
    int field_4;                       // +0x04
    unsigned char kind;                // +0x08
    unsigned char flag5;               // +0x09
    unsigned int flags6;               // +0x0a
    int last_id;                       // +0x0e
    Vec3_0043a420 pos;                 // +0x12
    int field_1e;                      // +0x1e
    int field_22;                      // +0x22
    int field_26;                      // +0x26
    int field_2a;                      // +0x2a
    int field_2e;                      // +0x2e
    unsigned int flags;                // +0x32
    int field_36;                      // +0x36
};
#pragma pack(pop)

// Every file method is a slot of the same parsed-text object, but each is
// named after its own address, so one class apiece.
class Class_004b4ba0 {
public:
    int FUN_004b4ba0(const char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int len);
};

class Class_004b48a0 {
public:
    char* FUN_004b48a0(const char* name, char* def);
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* key);
};

int __stdcall FUN_0043a940(int param_1, char* param_2);
Entry_0043a420* __stdcall FUN_0043c6b0(Entry_0043a420* first, Entry_0043a420* last,
                                       char* const& value, int(__stdcall* pred)(int, char*),
                                       int* unused);
Unit_0043a420* __stdcall FUN_00487080(unsigned short id, void* file);
unsigned short __stdcall FUN_00488b10(const char* name);

extern Game_0043a420* g_game;
extern Entry_0043a420* DAT_00512344;
extern Entry_0043a420* DAT_00512348;

class Class_004895c0 {
public:
    void* vptr;                        // +0x0
    void* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    void* value;                       // +0xc

    void SetValue(void* v) { value = v; }

    Class_004895c0(void* o, int v);
    void FUN_00489690(void* v);
};

class Class_0044de80 {
public:
    char pad[0x36];
    Class_0044de80(int owner, Class_004b4ba0* file, char* name);
};

class Class_0044e740 {
public:
    char pad[0x2c];
    Class_0044e740(int owner, Class_004b4ba0* file, char* name);
};

class Class_0044d010 {
public:
    char pad[0x14];
    Class_0044d010(int owner, Class_004b4ba0* file, char* name);
};

class Class_0044d470 {
public:
    char pad[0x1c];
    Class_0044d470(int owner, Class_004b4ba0* file, char* name);
};

class Class_0044d930 {
public:
    char pad[0x18];
    Class_0044d930(int owner, Class_004b4ba0* file, char* name);
};

class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);
};

#pragma pack(push, 1)
class Class_0043a1f0 : public Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);

    unsigned char kind;                // +0x4
    unsigned char flag5;               // +0x5
    unsigned int flags6;               // +0x6
    int last_id;                       // +0xa
    Unit_0043a420* unit;               // +0xe
    Class_004895c0 link;               // +0x12
    Vec3_0043a420 pos;                 // +0x22
    int field_2e;                      // +0x2e
    int field_32;                      // +0x32
    int field_36;                      // +0x36
    int field_3a;                      // +0x3a
    int field_3e;                      // +0x3e
    unsigned int flags;                // +0x42
    unsigned int created;              // +0x46
    int field_4a;                      // +0x4a
    int field_4e;                      // +0x4e
    void* attached;                    // +0x52

    Class_0043a1f0(Unit_0043a420* punit, Class_004b4ba0* file, char* name);
};
#pragma pack(pop)

// The resolver FUN_0043a360 inlined here: the "UTYPENAME<id>" key names the
// type, otherwise the id'th unit type without flag 0x20 gives its index.
static unsigned short ResolveType_0043a420(Class_004b4ba0* file, unsigned short id, char* key)
{
    sprintf(key, "UTYPENAME%4d", id);
    if (((Class_004b48f0*)file)->FUN_004b48f0(key))
        return FUN_00488b10(((Class_004b48a0*)file)->FUN_004b48a0(key, 0));
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

// FUNCTION: 0x43a420
Class_0043a1f0::Class_0043a1f0(Unit_0043a420* punit, Class_004b4ba0* file, char* name)
    : kind(0), link(0, 0)
{
    link.SetValue(this);
    link.FUN_00489690(0);
    field_4a = 0;
    attached = 0;
    created = g_game->ticks;
    if (file == 0)
        return;
    if (name == 0)
        return;
    if (strlen(name) > 0x1f)
        return;

    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    SaveDesc_0043a420 desc;
    if (((Class_004b4c80*)file)->FUN_004b4c80(&desc, 0x3a) != 0x3a)
        return;

    char buf1[0x80];
    sprintf(buf1, "%s%s", name, "_name");
    {
        char* result = ((Class_004b48a0*)file)->FUN_004b48a0(buf1, 0);
        if (result != 0) {
            char* s = result;
            Entry_0043a420* e = FUN_0043c6b0(DAT_00512344, DAT_00512348, s, FUN_0043a940, 0);
            if (e == DAT_00512348 || _strcmpi(e->name, s) != 0)
                desc.kind = 0;
            else
                desc.kind = (unsigned char)(e - DAT_00512344);
        } else {
            int k = 0;
            int idx = 0;
            Entry_0043a420* p = DAT_00512344;
            // The `k != desc.kind -> k++; continue;` form (rather than the plain
            // `if (k == desc.kind) break;`) is what keeps this region 2 bytes
            // longer than the plain form, which aligns every later branch
            // target and the switch jump table: 86.2% -> 93.8%.
            if (p <= DAT_00512348) {
                do {
                    if (!(p->flag14 & 1)) {
                        if (k != desc.kind) {
                            k++;
                            continue;
                        }
                        break;
                    }
                    idx++;
                    p++;
                } while (p <= DAT_00512348);
            }
            desc.kind = (unsigned char)idx;
        }
    }

    Unit_0043a420* u;
    if (desc.unitType == 0)
        u = 0;
    else
        u = g_game->units + desc.unitType;
    if (punit != u)
        return;
    if (desc.kind == 0)
        return;

    unit = punit;
    link.FUN_00489690(FUN_00487080(desc.ownerType, file));

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

    char* sname = DAT_00512344[desc.kind].name;
    if (strcmp(sname, "MobileBuild") == 0 || strcmp(sname, "VTOL_MobileBuild") == 0 ||
        strcmp(sname, "BuildingBuild") == 0) {
        char key[0x80];
        field_36 = ResolveType_0043a420(file, (unsigned short)field_36, key);
    }

    char buf3[0x20];
    sprintf(buf3, "%s%s", name, "g");
    switch (desc.field_4) {
    case 2:
        attached = new Class_0044de80((int)this, file, buf3);
        return;
    case 3:
        attached = new Class_0044e740((int)this, file, buf3);
        return;
    case 4:
        attached = new Class_0044d010((int)this, file, buf3);
        return;
    case 5:
        attached = new Class_0044d470((int)this, file, buf3);
        return;
    case 6:
        attached = new Class_0044d930((int)this, file, buf3);
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
