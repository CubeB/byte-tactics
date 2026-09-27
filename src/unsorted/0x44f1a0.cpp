// Decompiled by Space Bunny Free. Names are provisional.
// Slot 2 of the Class_0044f010 family (see 0x44f080.cpp, vtable 0x4fd458):
// tells the object at +0x4 about the owner, then, when the unit has reached
// its second path point (within 5 units of the owner), drops that point with
// an overlapping std::copy and refreshes the flags. std::copy, not memmove:
// memmove stays a library call, std::copy is the loop the original has.
// The class declarations are copied from 0x44f080.cpp, with the base slots
// 5 to 11 this function calls added to Base_00490a10 (vtable 0x4fd2f8; slot 8
// holds _purecall, slot 11 the stub FUN_0044cef0, which returns 0).
#include <algorithm>

struct Target_0044f1a0 {
    char unknown_0[0x2e];
    unsigned char field_2e;
};

struct Struct_004907e0 {                // the owner (see 0x4907e0.cpp)
    Target_0044f1a0* target;           // +0x0
    char unknown_4[0x6c - 0x4];
    short field_6c;                    // +0x6c
    char unknown_6e[0x74 - 0x6e];
    short field_74;                    // +0x74
};

class Base_00490a10 {
public:
    virtual ~Base_00490a10();                               // slot 0
    virtual void FUN_0044ce80();                            // slot 1
    virtual void FUN_0044ce40();                            // slot 2
    virtual void FUN_0044cf30();                            // slot 3
    virtual int FUN_0044cf00(Struct_004907e0* owner);       // slot 4
    virtual void FUN_0044cf20();                            // slot 5
    virtual void FUN_0044ce90();                            // slot 6
    virtual void FUN_0044cec0();                            // slot 7
    virtual void FUN_004e6110();                            // slot 8
    virtual void FUN_0044cf40();                            // slot 9
    virtual void FUN_0044cf50();                            // slot 10
    virtual int FUN_0044cef0();                             // slot 11
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
    virtual void FUN_0044efd0();                    // slot 9
    virtual void FUN_0044ef50();                    // slot 10
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

    virtual void FUN_0044ef90(void* param);         // slot 1, 0x44f2a0
    virtual void FUN_0044efb0();                    // slot 2, 0x44f1a0
};

// FUNCTION: 0x44f1a0
void Class_0044f010::FUN_0044efb0()
{
    if (field_4) {
        if (field_4->FUN_0044cf00(owner)) {
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x20);
            if (!field_4->FUN_0044cef0())
                FUN_0044ef90(0);
        }
    }
    if (count >= 2) {
        int dx = owner->field_74 - points[1].y;
        int dy = owner->field_6c - points[1].x;
        if (dy * dy + dx * dx <= 25) {
            std::copy(points + 1, points + count, points);
            int n = --count;
            if (n < 2)
                active = 0;
            flag_3 = 1;
        }
    }
    if (field_4 && (owner->target->field_2e & 4 || count < 2))
        flag_1 = 1;
}
