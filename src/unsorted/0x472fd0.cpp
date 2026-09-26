// Decompiled by Opus. Names are provisional.
// Returns whether a std::vector of 48-byte elements is empty; the bool from
// the inlined vector::empty() is widened to the int return value.
#include <vector>

struct Elem_00472fd0 {
    char unknown_0[0x30];
};

struct Class_00472fd0 {
    char unknown_0[0xc];
    std::vector<Elem_00472fd0> items;   // +0xc (_First +0x10, _Last +0x14)

    int FUN_00472fd0();
};

// FUNCTION: 0x472fd0
int Class_00472fd0::FUN_00472fd0()
{
    return items.empty();
}
