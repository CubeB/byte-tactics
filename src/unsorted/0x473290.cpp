// Decompiled by Opus. Names are provisional.
// Returns whether a std::vector of 68-byte elements is empty; the bool from
// the inlined vector::empty() is widened to the int return value.
#include <vector>

struct Elem_00473290 {
    char unknown_0[0x44];
};

struct Class_00473290 {
    char unknown_0[0xc];
    std::vector<Elem_00473290> items;   // +0xc (_First +0x10, _Last +0x14)

    int FUN_00473290();
};

// FUNCTION: 0x473290
int Class_00473290::FUN_00473290()
{
    return items.empty();
}
