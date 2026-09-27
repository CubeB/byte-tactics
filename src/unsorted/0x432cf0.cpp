// Decompiled by Haiku. Names are provisional.
// std::allocator<Entry_00432cf0>::construct: placement-new copy of a string
// handle plus an int. Taking the member's address makes the compiler emit the
// template instantiation out of line.
#include <memory>

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

struct Entry_00432cf0 {
    Class_004c91a0 name;     // +0x0
    int value;               // +0x4
};

typedef std::allocator<Entry_00432cf0> Alloc_00432cf0;
typedef void (Alloc_00432cf0::*ConstructFn_00432cf0)(Entry_00432cf0*, const Entry_00432cf0&);

// FUNCTION: 0x432cf0 ?construct@?$allocator@UEntry_00432cf0@@@std@@QAEXPAUEntry_00432cf0@@ABU3@@Z
ConstructFn_00432cf0 g_construct_00432cf0 = &Alloc_00432cf0::construct;
