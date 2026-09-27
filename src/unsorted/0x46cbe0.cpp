// Decompiled by Opus. Names are provisional.
// Constructor of a local object (built in 0x46dad0) with two empty
// std::vectors at +0xc and +0x1c (the allocator bytes are copied from an
// uninitialised temporary) and three dwords at +0x0 zeroed in the body.
#include <vector>

struct Elem_0046cbe0 {
    int unknown_0;
};

class Class_0046cbe0 {
public:
    int field_0;                              // +0x0
    int field_4;                              // +0x4
    int field_8;                              // +0x8
    std::vector<Elem_0046cbe0> first;         // +0xc (_First +0x10)
    std::vector<Elem_0046cbe0> second;        // +0x1c (_First +0x20)

    Class_0046cbe0();
};

// FUNCTION: 0x46cbe0
Class_0046cbe0::Class_0046cbe0()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
}
