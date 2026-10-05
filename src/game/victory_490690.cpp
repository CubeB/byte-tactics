// Decompiled by Space Bunny Free. Names are provisional.
// Class_00490630's override of slot 2 (vtable 0x4fd980, inherited by
// Class_00490880 and called directly by Class_004907e0's own override,
// 0x490880; the class family is listed in 0x44ef60.cpp): one update step of
// a moving object. The object at +0x4 moves this object's position (its slot
// 8) and the difference goes into vel. When the object has drifted further than
// 0xa00000 from its owner, its height is snapped to the ground under it, the
// sea level for a unit whose def has the flag at +0x241 bit 22 set, the
// owner's field_82 byte 1 otherwise, plus the def's field_21c, and past
// 0x1400000 (or 0x100000 with slot 9 refusing) it turns to face the owner.
// Then the object at +0x4 gets the last word: slot 4 saying it is done, plus
// slot 11, means slot 1 with 0.
//
// Match notes: `vel = pos - old` needs an inline `operator-` returning the
// struct by value, so that all three differences are computed before the
// first store. The flag at def+0x241 is a 1-bit bitfield at bit 22
// (`shr ecx, 0x16; test cl, 1`). In the else branch field_21c has to be
// reached through `owner->def->` and be the *first* operand of the add:
// MSVC orders the operands of a commutative add by the size of the two
// expression trees, and only the longer path makes the add asymmetric
// enough for the two branches not to be tail-merged into one.
#include <math.h>

struct Vec3_004907e0 {
    int x, y, z;
};

static inline Vec3_004907e0 operator-(const Vec3_004907e0& p, const Vec3_004907e0& q)
{
    Vec3_004907e0 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

#pragma pack(push, 1)
struct UnitDef {
    char pad0[0x21c];
    short field_21c;                   // +0x21c
    char pad21e[0x241 - 0x21e];
    unsigned int unknown_241_0 : 22;
    unsigned int seaUnit : 1;          // +0x241 bit 22
    unsigned int unknown_241_1 : 9;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Struct_004907e0 {               // the object at +0x8
    char pad0[0x66];
    short field_66;                    // +0x66
    short field_68;                    // +0x68
    Vec3_004907e0 pos;                 // +0x6a
    char pad76[0x82 - 0x76];
    unsigned char* field_82;           // +0x82
    char pad86[0x92 - 0x86];
    UnitDef* def;                      // +0x92
};
#pragma pack(pop)

struct Game {
    char pad0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};

extern Game* g_game;

// The object at +0x4. Only the offsets of the four virtuals it is asked for
// (slots 4, 8, 9 and 11) matter, so twelve of them are declared here.
class Class_0044ced0 {
public:
    virtual ~Class_0044ced0();
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual int FUN_0044ef40(int, int, int);        // slot 3
    virtual int FUN_0044f000(Struct_004907e0* param);   // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual int FUN_0044eff0();                     // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(Vec3_004907e0* param);  // slot 8
    virtual int FUN_0044efd0(short* param);         // slot 9
    virtual void FUN_0044ef50(int);                 // slot 10
    virtual int FUN_0044ef50_11();                  // slot 11
    void FUN_0044ced0(int param);
};

class Class_0044f010;                  // slot 6's result (see 0x44f450.cpp)
class BitWriter;                       // the bit writer slot 8 takes
class BitReader;                       // the bit reader slot 9 takes

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Class_0044ced0* field_4;            // +0x4
    Struct_004907e0* owner;             // +0x8

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
    Vec3_004907e0 pos;                  // +0xc
    Vec3_004907e0 vel;                  // +0x18
    short field_24;                     // +0x24
    char field_26;                      // +0x26
    unsigned char dirty : 1;            // +0x27 bit 0
    unsigned char mode : 2;             // +0x27 bits 1-2

    Class_00490630(Struct_004907e0* p);
    virtual void FUN_0044efb0();                    // slot 2, 0x490690
};

unsigned short __stdcall FUN_0048a980(Vec3_004907e0* from, Vec3_004907e0* to);

// FUNCTION: 0x490690
void Class_00490630::FUN_0044efb0()
{
    if (!field_4)
        return;
    Vec3_004907e0 old = pos;
    field_4->FUN_0044efc0(&pos);
    vel = pos - old;
    int dist = (int)_hypot(owner->pos.x - pos.x, owner->pos.z - pos.z);
    if (dist > 0xa00000) {
        if (owner->def->seaUnit)
            pos.y = (g_game->seaLevel + owner->def->field_21c) << 16;
        else
            pos.y = (owner->def->field_21c + owner->field_82[1]) << 16;
    }
    if (dist > 0x1400000 || (!field_4->FUN_0044efd0(&field_24) && dist > 0x100000))
        field_24 = (short)FUN_0048a980(&owner->pos, &pos);
    if (field_4->FUN_0044f000(owner)) {
        field_4->FUN_0044ced0(0x20);
        if (!field_4->FUN_0044ef50_11())
            FUN_0044ef90(0);
    }
}
