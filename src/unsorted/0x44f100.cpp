// Decompiled by Opus. Names are provisional.
// A method of Class_0044f010 (see 0x44ef60.cpp for the class family): drops
// the first n path points, clears the active flag when fewer than two are
// left and sets flag 3. The class declarations are copied from 0x44f080.cpp.
#include <algorithm>

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

// The object at +0x4 (see 0x490a10.cpp); base vtable 0x4fd2f8.
class Base_00490a10 {
public:
    virtual ~Base_00490a10();                               // slot 0
    virtual void FUN_0044ce80();                            // slot 1
    virtual void FUN_0044ce40();                            // slot 2
    virtual void FUN_0044cf30();                            // slot 3
    virtual int FUN_0044cf00(Struct_004907e0* owner);       // slot 4
};

struct Point_0044f080 {
    short x;
    short y;
};

struct Vec3_004907e0;                  // a position (see 0x4907e0.cpp)
class Class_0044f010;                  // slot 6's result (see 0x44f450.cpp)
class Class_00415c10;                  // the bit writer slot 8 takes
class Class_00415dc0;                  // the bit reader slot 9 takes

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
    virtual void FUN_0044efc0(Class_00415c10*);     // slot 8
    virtual void FUN_0044efd0(Class_00415dc0*);     // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
// Slots 4 and 9 are inherited.
class Class_0044f010 : public Class_0044ef20 {
public:
    Point_0044f080 points[20];         // +0xc
    int count;                         // +0x5c
    int field_60;                      // +0x60
    unsigned char active : 1;          // +0x64 bit 0
    unsigned char flag_1 : 1;          // bit 1
    unsigned char flag_2 : 1;          // bit 2
    unsigned char flag_3 : 1;          // bit 3

    Class_0044f010(Struct_004907e0* p);
    virtual ~Class_0044f010();                      // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x44f2a0
    virtual void FUN_0044efb0();                    // slot 2, 0x44f1a0
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f150
    virtual int FUN_0044ef80();                     // slot 5, 0x44f290
    virtual Class_0044f010* FUN_0044eff0();         // slot 6, 0x44f260
    virtual int FUN_0044efe0();                     // slot 7, 0x44f480
    virtual void FUN_0044efc0(Class_00415c10*);     // slot 8, 0x44f4a0
    virtual void FUN_0044ef50(void*);               // slot 10, 0x417e00

    void FUN_0044f080(Point_0044f080* src, int n);
    void FUN_0044f100(int n);
};

// FUNCTION: 0x44f100
void Class_0044f010::FUN_0044f100(int n)
{
    if (n != 0) {
        std::copy(points + n, points + count, points);
        count -= n;
        if (count < 2)
            active = 0;
        flag_3 = 1;
    }
}
