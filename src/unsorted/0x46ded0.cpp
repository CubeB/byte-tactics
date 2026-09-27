// Decompiled by Opus. Names are provisional.
// Destructor of a class holding four std::vector members; each inlined
// ~vector frees its buffer and zeroes _First/_Last/_End, in reverse order.
#include <vector>

class Class_0046ded0 {
public:
    int field_0;                    // +0x00
    std::vector<int> list_a;        // +0x04
    std::vector<int> list_b;        // +0x14
    char unknown_24[0x18];
    std::vector<int> list_c;        // +0x3c
    std::vector<int> list_d;        // +0x4c

    ~Class_0046ded0();
};

// FUNCTION: 0x46ded0
Class_0046ded0::~Class_0046ded0()
{
}
