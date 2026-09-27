// Decompiled by Opus. Names are provisional.
// Constructor of a class with two values, two zeroed fields and a
// std::vector (allocator byte +0x10, _First +0x14, _Last +0x18, _End +0x1c).
// The vector's default allocator argument is a temporary copied into the
// allocator byte; MSVC keeps it in the first parameter's stack slot.
#include <vector>

class Class_00480160 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<int> items;            // +0x10

    Class_00480160(int a, int b);
};

// FUNCTION: 0x480160
Class_00480160::Class_00480160(int a, int b)
    : field_0(a), field_4(b), field_8(0), field_c(0)
{
}
