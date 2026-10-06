// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Virtual save method (vtable 0x4fd3b8, slot 1) for the class built by
// 0x44e330 / 0x44e250. Builds a 0x36-byte record on the stack, writing the
// referenced unit's id, the embedded link's owner id, five flag shorts and
// the stored position, then writes it to the file via
// HapiBank::WriteBox. The read counterpart is 0x44de80.
//
// The record's first 8 bytes are never assigned and are still written out:
// 0x36 bytes of stack, 8 of them uninitialised, go to the save file. The read
// counterpart (0x44de80) reads all 0x36 bytes but never looks at 0..7, so the
// bytes are only leaked, never used. Kept as the original does it.

#include "../util/hapi_bank.h"

struct Unit_0044dfb0 {
    char unknown_0[0xa8];
    short id;                       // +0xa8
};

struct Vec3_0044dfb0 {
    int x;
    int y;
    int z;
};

class Class_004895c0 {
public:
    Class_004895c0(int unit = 0, int value = 0);
};

class UnitRef {
public:
    void FUN_00489650();
};

#pragma pack(push, 2)
struct Rec_0044dfb0 {
    int unknown_0;                  // +0x0
    int unknown_4;                  // +0x4
    short id1;                      // +0x8
    void* ref_vt;                   // +0xa (Class_004895c0)
    void* ref_owner;                // +0xe
    void* ref_next;                 // +0x12
    int ref_value;                  // +0x16
    short id2;                      // +0x1a
    short f1;                       // +0x1c
    short f2;                       // +0x1e
    short f3;                       // +0x20
    short f4;                       // +0x22
    short f5;                       // +0x24
    Vec3_0044dfb0 pos;              // +0x26
    int i4;                         // +0x32
};

class Class_0044dfb0 {
public:
    void* vtable;                   // +0x0
    void* field_4;                  // +0x4
    short f8;                       // +0x8
    short fa;                       // +0xa
    short fc;                       // +0xc
    short fe;                       // +0xe
    short f10;                      // +0x10
    Unit_0044dfb0* unit1;           // +0x12
    void* ref_vt;                   // +0x16 (Class_004895c0)
    Unit_0044dfb0* owner;           // +0x1a
    void* ref_next;                 // +0x1e
    int ref_value;                  // +0x22
    Vec3_0044dfb0 pos;              // +0x26
    int i4;                         // +0x32

    int FUN_0044dfb0(int unused, HapiBank* file, char* name);
};
#pragma pack(pop)

// FUNCTION: 0x44dfb0
int Class_0044dfb0::FUN_0044dfb0(int unused, HapiBank* file, char* name)
{
    Rec_0044dfb0 rec;
    Class_004895c0* ref = (Class_004895c0*)&rec.ref_vt;
    ref->Class_004895c0::Class_004895c0(0, 0);
    if (unit1 == 0)
        rec.id1 = 0;
    else
        rec.id1 = unit1->id;
    if (owner == 0)
        rec.id2 = 0;
    else
        rec.id2 = owner->id;
    rec.f1 = f8;
    rec.f2 = fa;
    rec.f3 = fc;
    rec.f4 = fe;
    rec.f5 = f10;
    rec.pos = pos;
    rec.i4 = i4;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&rec, 0x36);
    ((UnitRef*)&rec.ref_vt)->FUN_00489650();
    return 1;
}
