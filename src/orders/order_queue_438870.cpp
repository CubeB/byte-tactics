// Decompiled by Haiku, Opus, deepseek-v4.1, space-bunny-free, deepseek-v4.1-flash and Claude Opus 5.5. Names are provisional.
// A build-queue object: a kind from the order-type table at DAT_00512344, the
// unit it belongs to, a link into its owner's list and an attached object at
// +0x52. Its base (vtable 0x4fd2cc, one slot, the empty 0x43a1e0) has no
// destructor of its own, so the destructor stores only the derived vtable
// 0x4fd2c8.
#include <stdio.h>
#include <string.h>

class Class_0043a1f0;

#pragma pack(push, 1)
struct Vec3_0043a1f0 {
    int x, y, z;
};

struct Owner_0043a1f0;
class CobScript;

struct Unit {                          // 0x118 bytes
    Owner_0043a1f0* owner;             // +0x0
    char unknown_4[0x9a - 0x4];
    CobScript* names;                  // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short typeId;             // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];

    void ReleaseWeapons(int param_1);
};

struct Entry_0043a1f0 {                // 0x19-byte entries at DAT_00512344
    char unknown_0[4];
    void (__stdcall* notify)(Unit* unit, Class_0043a1f0* obj, int code);   // +0x4
    char unknown_8[0x14 - 0x8];
    unsigned char flag14;              // +0x14
    char* name;                        // +0x15
};

struct UnitType_0043a1f0 {             // 0x249 bytes
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
    UnitType_0043a1f0* unitTypes;      // +0x1439b
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

class CobScript {
public:
    int FindScript(char* name);
    int StartScriptWithArgsByIndex(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

class UnitRef {
public:
    void FUN_00489650();
};

// The parsed text file the writer is handed (the same object as HapiBank).
class File_0043a970 {
public:
    char unknown_0[1];
};

// The object at +0x52, deleted through its virtual destructor.
class Attached_0043a1f0 {
public:
    virtual ~Attached_0043a1f0();
    virtual int Slot1(Class_0043a1f0* obj, File_0043a970* file, char* name);
    virtual int Slot2();
};

class Slot_0043a1f0 {
public:
    virtual void Slot0();
    virtual void Slot1(int param_1);
    Attached_0043a1f0* current;        // +0x4
};

struct Owner_0043a1f0 {
    Slot_0043a1f0* slot;               // +0x0
};

class HapiBank {
public:
    int SetStringItem(const char* name, char* value);
    char* GetStringItem(char* name, char* def);
    int HasItem(const char* name);
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* dst, int len);
    int WriteBox(void* src, int len);
};

int __stdcall OrderTypeNameLess(int param_1, char* param_2);
Entry_0043a1f0* __stdcall FUN_0043c6b0(Entry_0043a1f0* first, Entry_0043a1f0* last,
                                       char* const& value, int(__stdcall* pred)(int, char*),
                                       int* unused);
Unit* __stdcall LoadUnit(unsigned short id, void* file);
unsigned short __stdcall FindUnitTypeId(const char* name);
int __stdcall SendScriptCallNoArgs(Unit* obj, short index);

extern Game* g_game;
extern Entry_0043a1f0* DAT_00512344;
extern Entry_0043a1f0* DAT_00512348;

// The 0x10-byte link at +0x12: its constructor puts the object in its
// owner's list; 0x489650 is its destructor.
class Class_004895c0 {
public:
    void* vptr;                        // +0x0
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    void* value;                       // +0xc, the object the link belongs to

    void SetValue(void* v) { value = v; }

    Class_004895c0(Unit* o, int v);
    void SetUnit(Unit* o);
};

// The attachments the file constructor makes, by the kind of attachment.
#pragma pack(push, 1)
class Class_0044de80 : public Attached_0043a1f0 {
public:
    char pad[0x32];
    Class_0044de80(int owner, HapiBank* file, char* name);
};

class Class_0044e740 : public Attached_0043a1f0 {
public:
    char pad[0x28];
    Class_0044e740(int owner, HapiBank* file, char* name);
};

class Class_0044d010 : public Attached_0043a1f0 {
public:
    char pad[0x10];
    Class_0044d010(int owner, HapiBank* file, char* name);
};

class Class_0044d470 : public Attached_0043a1f0 {
public:
    char pad[0x18];
    Class_0044d470(int owner, HapiBank* file, char* name);
};

class Class_0044d930 : public Attached_0043a1f0 {
public:
    char pad[0x14];
    Class_0044d930(int owner, HapiBank* file, char* name);
};
#pragma pack(pop)

// The base class: one virtual slot, its vtable at 0x4fd2cc.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int);  // slot 0: 0x43a1e0, empty
};

#pragma pack(push, 1)
// The fields between the vtable pointer and the link, as a second base whose
// inline constructor clears the kind (see the notes at 0x43a420).
struct Head_0043a1f0 {
    unsigned char kind;                // +0x4
    unsigned char flag5;               // +0x5
    unsigned int flags6;               // +0x6
    int last_id;                       // +0xa
    Unit* unit;                        // +0xe

    Head_0043a1f0() : kind(0) {}
};

class Class_0043a1f0 : public Class_0043a1e0, public Head_0043a1f0 {
public:
    // Slot 0 of vtable 0x4fd2c8, overriding the base's.
    virtual void FUN_0043a1e0(unsigned int);

    Class_004895c0 link;               // +0x12
    Vec3_0043a1f0 pos;                 // +0x22
    int field_2e;                      // +0x2e
    int field_32;                      // +0x32
    int field_36;                      // +0x36
    int field_3a;                      // +0x3a
    int field_3e;                      // +0x3e
    unsigned int flags;                // +0x42
    unsigned int created;              // +0x46
    int field_4a;                      // +0x4a
    int field_4e;                      // +0x4e
    Attached_0043a1f0* attached;       // +0x52

    // In order_list_43a0c0.cpp: it needs `kind(k)` as a plain member
    // initialiser, which this class's second base rules out (98.9%).
    Class_0043a1f0(int k, Unit* o, Vec3_0043a1f0* p, int a, int b, int c);
    ~Class_0043a1f0();
    Class_0043a1f0(Unit* punit, HapiBank* file, char* name);
    int FUN_0043a970(Unit* punit, File_0043a970* file, char* name);
};
#pragma pack(pop)

// FUNCTION: 0x438870
void Class_0043a1f0::FUN_0043a1e0(unsigned int param_1)
{
    field_4e |= param_1;
}

// Destructor (callers do `if (p) { p->~X(); operator delete(p); }`): notifies
// the owner through a callback table, stops the unit's build animation (the
// same code as StopBuildingScript), releases the attached object at +0x52 and
// unlinks the list node at +0x12 (0x489650 is the link's destructor).
// FUNCTION: 0x43a1f0
Class_0043a1f0::~Class_0043a1f0()
{
    if (flags6 & 2) {
        DAT_00512344[kind].notify(unit, this, 2);
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
                slot->Slot1(0);
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
    ((UnitRef*)&link)->FUN_00489650();
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
    Entry_0043a1f0* e = FUN_0043c6b0(DAT_00512344, DAT_00512348, s, OrderTypeNameLess, 0);
    if (e == DAT_00512348 || _strcmpi(e->name, s) != 0)
        return 0;
    return (unsigned char)(e - DAT_00512344);
}

static unsigned char KindByIndex_0043a420(unsigned char want)
{
    int idx = 0;
    int k = 0;
    for (Entry_0043a1f0* p = DAT_00512344; p <= DAT_00512348; p++, idx++) {
        if (!(p->flag14 & 1)) {
            if (k == want)
                break;
            k++;
        }
    }
    return (unsigned char)idx;
}

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
// ResolveUnitTypeKey is defined above and inlined, as in the original file.
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
// The case-3 attachment constructor is Class_0044e740's second constructor,
// 0x44e7d0, which shares its name with 0x44e740; data/aliases.csv has a row
// for it.
// FUNCTION: 0x43a420
Class_0043a1f0::Class_0043a1f0(Unit* punit, HapiBank* file, char* name)
    : link(0, 0)
{
    link.SetValue(this);
    link.SetUnit(0);
    field_4a = 0;
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

    char* sname = DAT_00512344[desc.kind].name;
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

// Serialises one build-queue object (Class_0043a1f0) into the parsed text file
// under the key `name`: a raw 0x3a-byte snapshot, a "<name>_name" string value
// pointing at the kind's name, a "UTYPENAME<id>" value when the object is a
// build kind, and finally "<name>g" handed to the attached object.
// FUNCTION: 0x43a970
int Class_0043a1f0::FUN_0043a970(Unit* punit, File_0043a970* file, char* name)
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
    desc.field_4 = attached ? attached->Slot2() : 0;
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
    ((HapiBank*)file)->SetStringItem(buf1, DAT_00512344[kind].name);

    char* s = DAT_00512344[kind].name;
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
        attached->Slot1(this, file, buf3);
    }
    return 1;
}
