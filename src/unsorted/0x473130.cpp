// Decompiled by Opus. Names are provisional.
// Returns whether a std::vector of 60-byte elements is empty; the bool from
// the inlined vector::empty() is widened to the int return value.
#include <vector>

struct Elem_00473130 {
    char unknown_0[0x3c];
};

struct Class_00473130 {
    char unknown_0[0xc];
    std::vector<Elem_00473130> items;   // +0xc (_First +0x10, _Last +0x14)

    int FUN_00473130();
};

// FUNCTION: 0x473130
int Class_00473130::FUN_00473130()
{
    return items.empty();
}
