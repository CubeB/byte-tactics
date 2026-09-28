// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free. Names are provisional.
// Serialises one build-queue object (Class_0043a1f0) into the parsed text file
// under the key `name`: a raw 0x3a-byte snapshot, a "<name>_name" string value
// pointing at the kind's name, a "UTYPENAME<id>" value when the object is a
// build kind, and finally "<name>g" handed to the attached object.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Vec3_0043a970 {
    int x, y, z;
};

struct Unit_0043a970 {
    void* owner;                       // +0x0
    char unknown_4[0x9a - 0x4];
    void* names;                       // +0x9a
    char unknown_9e[0xa8 - 0x9e];
    unsigned short typeId;             // +0xa8
    char unknown_aa[0x110 - 0xaa];
    unsigned int flags;                // +0x110
};

struct Link_0043a970 {                 // the 0x10-byte link member at +0x12
    void* vptr;                        // +0x0
    Unit_0043a970* owner;              // +0x4
    Link_0043a970* next;               // +0x8
    void* value;                       // +0xc
};

struct Entry_0043a970 {                // 0x19-byte entries at DAT_00512344
    char unknown_0[0x15];
    char* name;                        // +0x15
};

struct UnitType_0043a970 {             // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x249 - 0x40];
};

struct Game_0043a970 {
    char unknown_0[0x1438f];
    int unitTypeCount;                 // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043a970* unitTypes;      // +0x1439b
};

// The parsed text file (each method is a slot of the same object, and every
// one of them is named after its own address, so one class apiece).
class File_0043a970 {
public:
    char unknown_0[1];
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* src, int len);
};

class Class_004b4750 {
public:
    int FUN_004b4750(char* name, char* value);
};

class Class_004b48f0 {
public:
    int FUN_004b48f0(char* name);
};

class Class_0043a1f0;

class Attached_0043a970 {
public:
    virtual void Slot0();
    virtual int Slot1(Class_0043a1f0* obj, File_0043a970* file, char* name);
    virtual int Slot2();
};

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;                // +0x4
    unsigned char flag5;               // +0x5
    unsigned int flags6;               // +0x6
    int last_id;                       // +0xa
    Unit_0043a970* unit;               // +0xe
    Link_0043a970 link;                // +0x12
    Vec3_0043a970 pos;                 // +0x22
    int field_2e;                      // +0x2e
    int field_32;                      // +0x32
    int field_36;                      // +0x36
    int field_3a;                      // +0x3a
    int field_3e;                      // +0x3e
    unsigned int flags;                // +0x42
    int field_46;                      // +0x46
    int field_4a;                      // +0x4a
    int field_4e;                      // +0x4e
    Attached_0043a970* attached;       // +0x52

    int FUN_0043a970(Unit_0043a970* punit, File_0043a970* file, char* name);
};

struct SaveDesc_0043a970 {             // the 0x3a-byte snapshot written raw
    unsigned short unitType;           // +0x00
    unsigned short ownerType;          // +0x02
    int field_4;                       // +0x04
    unsigned char kind;                // +0x08
    unsigned char flag5;               // +0x09
    unsigned int flags6;               // +0x0a
    int last_id;                       // +0x0e
    Vec3_0043a970 pos;                 // +0x12
    int field_1e;                      // +0x1e
    int field_22;                      // +0x22
    int field_26;                      // +0x26
    int field_2a;                      // +0x2a
    int field_2e;                      // +0x2e
    unsigned int flags;                // +0x32
    int field_36;                      // +0x36
};
#pragma pack(pop)

extern Entry_0043a970* DAT_00512344;
extern Game_0043a970* g_game;

// FUNCTION: 0x43a970
int Class_0043a1f0::FUN_0043a970(Unit_0043a970* punit, File_0043a970* file, char* name)
{
    if (unit->typeId != punit->typeId || !file || !name)
        return 0;
    if (strlen(name) > 0x1f)
        return 0;

    SaveDesc_0043a970 desc;
    // A local copy of the pointer and the test written as `== 0` is what the
    // original wants here: the copy stops if-conversion, and the polarity
    // gives the `jne` over the zero store instead of the `je` after it.
    Unit_0043a970* u = unit;
    if (u == 0)
        desc.unitType = 0;
    else
        desc.unitType = u->typeId;
    // The owner is tested for null twice on purpose: mixing the copy with a
    // fresh read of link.owner keeps the compiler from dropping the third test.
    Unit_0043a970* o = link.owner;
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

    ((Class_004b4ba0*)file)->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(&desc, 0x3a);

    char buf1[0x80];
    sprintf(buf1, "%s%s", name, "_name");
    ((Class_004b4750*)file)->FUN_004b4750(buf1, DAT_00512344[kind].name);

    char* s = DAT_00512344[kind].name;
    if (strcmp(s, "MobileBuild") == 0 || strcmp(s, "VTOL_MobileBuild") == 0 ||
        strcmp(s, "BuildingBuild") == 0) {
        unsigned short id = field_36;
        char buf2[0x80];
        sprintf(buf2, "UTYPENAME%4d", id);
        if (!((Class_004b48f0*)file)->FUN_004b48f0(buf2) && id >= 1 && id < g_game->unitTypeCount)
            ((Class_004b4750*)file)->FUN_004b4750(buf2, g_game->unitTypes[id].name);
    }

    if (desc.field_4 != 0) {
        char buf3[0x20];
        sprintf(buf3, "%s%s", name, "g");
        attached->Slot1(this, file, buf3);
    }
    return 1;
}
