// Decompiled by space-bunny-free. Names are provisional.
// Class_004716a0's slot 1 override, FUN_00472d50 (vtable 0x4fd5d8, see
// 0x4716a0.cpp and 0x471cc0.cpp for the family). It walks the std::vector of 0x3c-byte
// elements at +0xc, advances each one with FUN_00474130, and erases every
// element FUN_004742a0 reports as expired, then, when virtual slot 5
// (0x4730c0) is true, calls virtual slot 4 (0x4743a0).
// The inlined vector::erase is what leaves the 0x3c-byte per-element
// `rep movsd` shift loop, the dead reload of the p + 1 local after it, and
// the reload of _Last in the loop test. The two callees have placeholder
// names from two different classes, so the second is reached by a cast.

#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game_00473010 {
    char unknown_0[0x38a47];
    int f_38a47;                     // +0x38a47
};
#pragma pack(pop)

extern Game_00473010* g_game;

class Class_00474130 {                   // vector element
public:
    char unknown_0[0x3c];

    void FUN_00474130();
};

class Class_004742a0 {                   // the same element, second method
public:
    char unknown_0[0x3c];

    int FUN_004742a0(int param_1);
};

// Vtable 0x4fd5a8, the base of the pool-allocated family (see 0x471cc0.cpp).
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
};

// Vtable 0x4fd5d8, this function is its slot 1 override.
class Class_004716a0 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00474130> items;                  // +0xc (_First +0x10)

    virtual void FUN_00472d50();                        // slot 1, 0x473010
    virtual void FUN_00472e30(int);                     // slot 2, 0x4730f0
    virtual int FUN_00472e70();                         // slot 3, 0x473130
    virtual void FUN_004743a0();                        // slot 4, 0x4743a0
    virtual int FUN_004730c0();                         // slot 5, 0x4730c0
};

// FUNCTION: 0x473010
void Class_004716a0::FUN_00472d50()
{
    for (std::vector<Class_00474130>::iterator it = items.begin(); it != items.end(); ) {
        it->FUN_00474130();
        if (((Class_004742a0*)it)->FUN_004742a0(g_game->f_38a47))
            items.erase(it);
        else
            ++it;
    }
    if (FUN_004730c0())
        FUN_004743a0();
}
