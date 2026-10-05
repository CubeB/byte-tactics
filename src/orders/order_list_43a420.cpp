// Decompiled by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// The constructor of Class_0043a1f0 that loads the object from the parsed
// text file (the other one is 0x43a0c0; 0x43a970 is the matching writer).
// data/symbols.csv still names this address Class_0043a420::Class_0043a420
// (the name its caller 0x487080 uses), but the vtables it stores are
// Class_0043a1f0's, so it is that class's second constructor.
//
// The kind is resolved by two inlined helpers, one per branch, each
// returning an unsigned char: by name through the sorted kind table
// (lower_bound), or, for files without a "<name>_name" key, by counting the
// entries without flag bit 0. Written inline (as earlier attempts did) the
// fallback scan put the compared counter in ECX and lost the `mov dl, cl`
// that copies the helper's result into place; as a helper with `int idx`
// declared before `int k` the scan is byte-identical. The real resolver
// FUN_0043a360 is defined above and inlined, as in the original file.
//
// The prologue (Claude Opus 5.5, #5496; 96.6% to MATCH): the original stores
// the base vtable 0x4fd2cc between the two `push edi` arguments of the link
// constructor. That happens when the kind is cleared by the inline
// constructor of a second base class (Head_0043a420, the fields from +0x4 to
// +0x11). Its `this` is the object plus 4, so MSVC 5 cannot tell that the kind
// store and the vtable store do not overlap, keeps them in order, and
// schedules the vtable store first. With `kind(0)` as a member initialiser
// (every earlier attempt) the stores are independent and both sink below the
// pushes. A base holding only the kind byte gives the same bytes; a member of
// class type with its own constructor does not (96.4%), and moving the link
// into the base breaks the rest (66.9%).
// Caveat for whoever unifies the class: the matched sibling constructor
// 0x43a0c0 needs `kind(k)` as a plain member initialiser. Written with this
// second base (`Head(k)`), its vtable store moves between its pushes too
// (98.9%), so the two files still describe the class differently.
// The case-3 attachment constructor is Class_0044e740's second constructor,
// 0x44e7d0, which shares its name with 0x44e740; data/aliases.csv has a row
// for it.
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

struct Unit {                          // 0x118 bytes
    char unknown_0[0x118];
};

struct UnitType_0043a420 {             // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x241 - 0x40];
    unsigned char flags;               // +0x241
    char unknown_242[0x249 - 0x242];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
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
Unit* __stdcall FUN_00487080(unsigned short id, void* file);
unsigned short __stdcall FUN_00488b10(const char* name);

extern Game* g_game;
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
// The fields between the vtable pointer and the link, as a second base whose
// inline constructor clears the kind (see the notes at the top).
struct Head_0043a420 {
    unsigned char kind;                // +0x4
    unsigned char flag5;               // +0x5
    unsigned int flags6;               // +0x6
    int last_id;                       // +0xa
    Unit* unit;                        // +0xe

    Head_0043a420() : kind(0) {}
};

class Class_0043a1f0 : public Class_0043a1e0, public Head_0043a420 {
public:
    virtual void FUN_0043a1e0(unsigned int);

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

    Class_0043a1f0(Unit* punit, Class_004b4ba0* file, char* name);
};
#pragma pack(pop)

// The real resolver at 0x43a360 (matched in its own file), defined here without
// its annotation as in the original file, which /Ob2 inlines into the constructor.
short __stdcall FUN_0043a360(Class_004b48f0* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (file->FUN_004b48f0(key))
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


static unsigned char KindByName_0043a420(char* s)
{
    Entry_0043a420* e = FUN_0043c6b0(DAT_00512344, DAT_00512348, s, FUN_0043a940, 0);
    if (e == DAT_00512348 || _strcmpi(e->name, s) != 0)
        return 0;
    return (unsigned char)(e - DAT_00512344);
}

static unsigned char KindByIndex_0043a420(unsigned char want)
{
    int idx = 0;
    int k = 0;
    for (Entry_0043a420* p = DAT_00512344; p <= DAT_00512348; p++, idx++) {
        if (!(p->flag14 & 1)) {
            if (k == want)
                break;
            k++;
        }
    }
    return (unsigned char)idx;
}

// FUNCTION: 0x43a420
Class_0043a1f0::Class_0043a1f0(Unit* punit, Class_004b4ba0* file, char* name)
    : link(0, 0)
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
        char* s = ((Class_004b48a0*)file)->FUN_004b48a0(buf1, 0);
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
        field_36 = (unsigned short)FUN_0043a360((Class_004b48f0*)file, (unsigned short)field_36);
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
