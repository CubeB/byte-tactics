// Decompiled by space-bunny-free. Names are provisional.
// The copy constructor of Class_0046ded0, the 0x5c-byte class whose
// out-of-line destructor is 0x46ded0 and whose scalar deleting destructor is
// 0x470300. Its one caller copies a 0x5c-byte array of these.
// A std::vector in this build is 16 bytes: the empty allocator member is its
// first dword and _First, _Last and _End follow it, so the single byte copy
// in front of each vector is the allocator's own initializer, and the vector
// at +0x04 has its _First at +0x08, the one at +0x14 at +0x18, the one at
// +0x3c at +0x40 and the one at +0x4c at +0x50. The same four members and the
// same offsets are declared in 0x46ded0.cpp and 0x470300.cpp.
// Each vector member is copied by the copy constructor of MSVC 5's <vector>:
// size(), allocator.allocate() (??2@YAPAXI@Z, the array new) and _Ucopy.
// /Ob2 inlines the first three _Ucopy loops, but the fourth one is a call to
// 0x46faf0 (UElem_0046faf0::?$vector::_Ucopy) with ecx set to the vector.
// The inline budget is reached because the two 14-byte-element vectors are
// members of one sub-struct with a copy constructor of its own: declaring
// them as two plain members inlines all four loops and gives 493 bytes.
#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {                 // 14 bytes
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

struct Pair_0046faf0 {                 // 32 bytes
    std::vector<Elem_0046faf0> list_c;
    std::vector<Elem_0046faf0> list_d;

    Pair_0046faf0(const Pair_0046faf0& other)
        : list_c(other.list_c), list_d(other.list_d)
    {
    }
};

class Class_0046ded0 {
public:
    int field_0;                               // +0x00
    std::vector<int> list_a;                   // +0x04
    std::vector<int> list_b;                   // +0x14
    int field_24;                              // +0x24
    int field_28;                              // +0x28
    int field_2c;                              // +0x2c
    int field_30;                              // +0x30
    int field_34;                              // +0x34
    int field_38;                              // +0x38
    Pair_0046faf0 pair;                        // +0x3c

    Class_0046ded0(const Class_0046ded0& other);
};

// FUNCTION: 0x470390
Class_0046ded0::Class_0046ded0(const Class_0046ded0& other)
    : field_0(other.field_0), list_a(other.list_a), list_b(other.list_b),
      field_24(other.field_24), field_28(other.field_28), field_2c(other.field_2c),
      field_30(other.field_30), field_34(other.field_34), field_38(other.field_38),
      pair(other.pair)
{
}
