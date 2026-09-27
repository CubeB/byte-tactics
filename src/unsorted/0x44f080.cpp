// Decompiled by Opus. Names are provisional.
// A method of Class_0044f010 (see 0x44ef60.cpp for the class family): sets
// the path points (at most 20) and marks it active, or with no points asks
// the object at +0x4 about the owner and flags it (0x40) when that fails.
// The class declarations are copied from 0x44f450.cpp, with the fields this
// function (and 0x44f010/0x44f150) uses added to Class_0044f010.
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

class Class_0044ced0 {
public:
    void FUN_0044ced0(int param_1);
};

struct Point_0044f080 {
    short x;
    short y;
};

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(int, int, int);       // slot 3
    virtual void FUN_0044f000(int, int, int);       // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual int FUN_0044eff0();                     // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(int);                 // slot 8
    virtual void FUN_0044efd0(int);                 // slot 9
    virtual void FUN_0044ef50(int);                 // slot 10
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
    virtual void FUN_0044ef40(int, int, int);       // slot 3, 0x44f150
    virtual int FUN_0044ef80();                     // slot 5, 0x44f290
    virtual int FUN_0044eff0();                     // slot 6, 0x44f260
    virtual int FUN_0044efe0();                     // slot 7, 0x44f480
    virtual void FUN_0044efc0(int);                 // slot 8, 0x44f4a0
    virtual void FUN_0044ef50(int);                 // slot 10, 0x417e00

    void FUN_0044f080(Point_0044f080* src, int n);
};

// FUNCTION: 0x44f080
void Class_0044f010::FUN_0044f080(Point_0044f080* src, int n)
{
    if (n == 0) {
        if (field_4 && field_4->FUN_0044cf00(owner) == 0)
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x40);
        active = 0;
    } else {
        if (n >= 20)
            n = 20;
        count = n;
        std::copy(src, src + n, points);
        active = 1;
    }
    flag_1 = 0;
    flag_3 = 1;
}
