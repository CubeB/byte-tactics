// Decompiled by Opus, Sonnet and Haiku. Names are provisional.
// Class_004907e0 (vtable 0x4fd9b0), derived from Class_00490630 and
// Class_0044ef20 (see order_targets_44ef20.cpp for the family): the moving
// object that also sends its state, with a dirty bit and the owner's mode.

class BitWriter {
public:
    void WriteBits(int value, int bits);
};
class BitReader;

struct Vec3_004907e0 {
    int x, y, z;
    Vec3_004907e0() {}
    Vec3_004907e0(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct Target_00490880 {
    char unknown_0[0x2e];
    union {
        unsigned char field_2e;        // +0x2e
        struct {
            unsigned char mode : 2;    // +0x2e bits 0-1
        };
    };
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner
    Target_00490880* target;           // +0x0
    char unknown_4[0x66 - 0x4];
    short field_66;                    // +0x66
    short field_68;
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

// The object at +0x4 (see victory_490940.cpp).
class Base_00490a10 {
public:
    virtual ~Base_00490a10();
    virtual void vf1();
    virtual int GetType();                          // +0x08
    virtual void vf3();
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();
    virtual void vf7();
    virtual void vf8();
    virtual void vf9();
    virtual void Write(BitWriter* stream);          // +0x28
};

class Class_0044f010;                  // slot 6's result

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
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
    union {
        struct {
            unsigned char dirty : 1;   // +0x27 bit 0
            unsigned char mode : 2;    // +0x27 bits 1-2
        };
        struct {
            unsigned char state : 3;   // +0x27, both
        };
        unsigned char field_27;
    };

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

// Vtable 0x4fd9b0, constructor 0x4907e0, ??_G 0x490840.
class Class_004907e0 : public Class_00490630 {
public:
    Class_004907e0(Struct_004907e0* p);
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x490860
    virtual void FUN_0044efb0();                    // slot 2, 0x490880
    virtual int FUN_0044efe0();                     // slot 7, 0x4908b0
    virtual void FUN_0044efc0(BitWriter*);          // slot 8, 0x4908c0
};

// The constructor: the base constructor (0x44ef20) is out of line, the middle
// class's (out-of-line copy at 0x4905e0) is inlined. The middle class assigns
// its members in the body. Its vtable reference makes the
// compiler emit the scalar deleting destructor here too; both derived
// destructors are trivial, so only the inlined base destructor's store of
// 0x4fd428 is left in it.
// FUNCTION: 0x4907e0
// FUNCTION: 0x490840 ??_GClass_004907e0@@UAEPAXI@Z
Class_004907e0::Class_004907e0(Struct_004907e0* p)
    : Class_00490630(p)
{
    dirty = 1;
    mode = 0;
}

// Slot 1: the base's (0x44ef90) and then the dirty bit.
// FUNCTION: 0x490860
void Class_004907e0::FUN_0044ef90(void* param)
{
    Class_0044ef20::FUN_0044ef90(param);
    dirty = 1;
}

// Slot 2: sets the dirty bit when the mode differs from the owner's, then runs
// the middle class's own slot 2 (0x490690).
// FUNCTION: 0x490880
void Class_004907e0::FUN_0044efb0()
{
    if ((owner->target->field_2e & 3) != mode)
        dirty = 1;
    Class_00490630::FUN_0044efb0();
}

// Slot 7: the dirty bit.
// FUNCTION: 0x4908b0
int Class_004907e0::FUN_0044efe0()
{
    return field_27 & 1;
}

// Slot 8: writes the object at +0x4 (a 2-bit kind, then its own data) and the
// owner's mode to the stream, then takes that mode as its own and clears the
// dirty bit. The matching reader looks like 0x490a10 (Class_00490880's slot 9).
// FUNCTION: 0x4908c0
void Class_004907e0::FUN_0044efc0(BitWriter* stream)
{
    if (field_4 == 0) {
        stream->WriteBits(0, 2);
    } else if (field_4->GetType() == 2) {
        stream->WriteBits(1, 2);
        field_4->Write(stream);
    } else if (field_4->GetType() == 3) {
        stream->WriteBits(2, 2);
        field_4->Write(stream);
    }
    stream->WriteBits(owner->target->mode, 2);
    state = owner->target->mode << 1;     // clears the dirty bit too
}
