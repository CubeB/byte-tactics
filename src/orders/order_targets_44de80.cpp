// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loading counterpart of the virtual save method 0x44dfb0 (vtable
// DAT_004fd3b8, slot 1): reads a 0x36-byte record from a named entry of an
// open file and copies its fields into this object. The record it reads is
// built by 0x44dfb0 and by 0x44e330 / 0x44e250.

struct Vec3_0044de80 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0xa8];
    short id;                          // +0xa8
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int size);
};

Unit* __stdcall LoadUnit(unsigned short index, void* file);

class Class_004895c0 {
public:
    Class_004895c0(void* o = 0, int v = 0);
    void SetUnit(void* o);
    virtual ~Class_004895c0();

    void* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc
};

class UnitRef {
public:
    void FUN_00489650();
};

extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];

#pragma pack(push, 2)
struct Rec_0044de80 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    short id1;                         // +0x8
    void* ref_vt;                      // +0xa
    void* ref_owner;                   // +0xe
    void* ref_next;                    // +0x12
    int ref_value;                     // +0x16
    short id2;                         // +0x1a
    short f1;                          // +0x1c
    short f2;                          // +0x1e
    short f3;                          // +0x20
    short f4;                          // +0x22
    short f5;                          // +0x24
    Vec3_0044de80 pos;                 // +0x26
    int i4;                            // +0x32
};

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4

    Class_0044ce20(int param_1)
    {
        vtable = DAT_004fd2f8;
        field_4 = param_1;
    }
};

class Class_0044de80 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044de80 pos;                 // +0x26
    int field_32;                      // +0x32

    Class_0044de80(int owner, Class_004b4ba0* file, char* name);
};
#pragma pack(pop)

// FUNCTION: 0x44de80
Class_0044de80::Class_0044de80(int owner, Class_004b4ba0* file, char* name)
    : Class_0044ce20(owner), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    Rec_0044de80 rec;
    ((Class_004895c0*)&rec.ref_vt)->Class_004895c0::Class_004895c0(0, 0);
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, 0x36) == 0x36) {
        field_12 = LoadUnit(rec.id1, file);
        ref.SetUnit(LoadUnit(rec.id2, file));
        field_8 = rec.f1;
        field_a = rec.f2;
        field_c = rec.f3;
        field_e = rec.f4;
        field_10 = rec.f5;
        pos = rec.pos;
        field_32 = rec.i4;
    }
    ((UnitRef*)&rec.ref_vt)->FUN_00489650();
}
