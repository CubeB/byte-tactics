// Decompiled by Opus and Sonnet. Names are provisional.
// Class_00490880 (vtable 0x4fd9e0), derived from Class_00490630 and
// Class_0044ef20 (see order_targets_44ef20.cpp for the family): the moving
// object that reads its state from the stream.

class BitReader {
public:
    int ReadBits(int bits);
};
class BitWriter;

struct Vec3_004907e0 {
    int x, y, z;
    Vec3_004907e0() {}
    Vec3_004907e0(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct Struct_004907e0;

class UnitMotion {
public:
    void SetFlightMode(Struct_004907e0* owner, int state);
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner
    UnitMotion* obj;                   // +0x0
    char unknown_4[0x66 - 0x4];
    short field_66;                    // +0x66
    short field_68;
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

// The object at +0x4, one of the two classes the stream names.
class Base_00490a10 {
public:
    virtual ~Base_00490a10();
};

#pragma pack(push, 2)
class Class_0044e080 : public Base_00490a10 {
public:
    char unknown_4[0x36 - 0x4];
    Class_0044e080(Struct_004907e0* owner, BitReader* reader);
};

class Class_0044e9c0 : public Base_00490a10 {
public:
    char unknown_4[0x2c - 0x4];
    Class_0044e9c0(Struct_004907e0* owner, BitReader* reader);
};
#pragma pack(pop)

class Class_0044f010;                  // slot 6's result

class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual Class_0044f010* FUN_0044eff0();         // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(BitWriter*);          // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// Vtable 0x4fd980, constructor 0x4905e0, ??_G 0x490630.
class Class_00490630 : public Class_0044ef20 {
public:
    Vec3_004907e0 pos;                 // +0xc
    Vec3_004907e0 vel;                 // +0x18
    short field_24;                    // +0x24
    char field_26;                     // +0x26
    unsigned char dirty : 1;           // +0x27 bit 0
    unsigned char mode : 2;            // +0x27 bits 1-2

    Class_00490630(Struct_004907e0* p)
        : Class_0044ef20(p)
    {
        pos = p->pos;
        vel = Vec3_004907e0(0, 0, 0);
        field_24 = p->field_66;
    }
    virtual void FUN_0044efb0();                    // slot 2, 0x490690
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4, 0x490650
};

// Vtable 0x4fd9e0, constructor 0x490940, destructor 0x4909e0, ??_G 0x4909a0.
// Slots 2 and 4 are inherited from Class_00490630.
class Class_00490880 : public Class_00490630 {
public:
    Class_00490880(Struct_004907e0* p);
    virtual ~Class_00490880();                      // slot 0
    virtual void FUN_0044efd0(BitReader*);          // slot 9, 0x490a10
};

// The constructor: the base constructor (0x44ef20) is out of line, the middle
// class's constructor is inlined, as in victory_4907e0.cpp.
// FUNCTION: 0x490940
Class_00490880::Class_00490880(Struct_004907e0* p)
    : Class_00490630(p)
{
}

// The destructor frees the object at +0x4 through its virtual destructor. Its
// scalar deleting destructor inlines the same body; in both, the middle
// class's vtable store is dead, and the empty inline base destructor leaves
// only the base vtable store of 0x4fd428.
// FUNCTION: 0x4909a0 ??_GClass_00490880@@UAEPAXI@Z
// FUNCTION: 0x4909e0
Class_00490880::~Class_00490880()
{
    delete field_4;
    field_4 = 0;
}

// Slot 9: rebuilds the object at +0x4 from the bit stream (a 2-bit kind: 1 and
// 2 pick its class, anything else leaves none), then passes a 2-bit state read
// after it to the owner.
// FUNCTION: 0x490a10
void Class_00490880::FUN_0044efd0(BitReader* reader)
{
    if (field_4) {
        delete field_4;
        field_4 = 0;
    }
    int kind = reader->ReadBits(2);
    if (kind == 1)
        field_4 = new Class_0044e080(owner, reader);
    else if (kind == 2)
        field_4 = new Class_0044e9c0(owner, reader);
    int state = reader->ReadBits(2);
    owner->obj->SetFlightMode(owner, state);
}
