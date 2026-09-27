// Decompiled by Opus. Names are provisional.
// std::vector<Entry_00432cf0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copy-constructs {string handle, int} pairs into raw storage
// (the placement-new construct of 0x432cf0, inlined). Called with ecx set to
// the vector. _Ucopy is protected, so a derived class takes its address to
// make the compiler emit it out of line.
#include <vector>

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

struct Entry_00432cf0 {
    Class_004c91a0 name;     // +0x0
    int value;               // +0x4
};

typedef std::vector<Entry_00432cf0> Vec_00432c40;
typedef Vec_00432c40::iterator (Vec_00432c40::*UcopyFn_00432c40)(
    Vec_00432c40::const_iterator, Vec_00432c40::const_iterator, Vec_00432c40::iterator);

struct Access_00432c40 : Vec_00432c40 {
    static UcopyFn_00432c40 fn;
};

// FUNCTION: 0x432c40 ?_Ucopy@?$vector@UEntry_00432cf0@@V?$allocator@UEntry_00432cf0@@@std@@@std@@IAEPAUEntry_00432cf0@@PBU3@0PAU3@@Z
UcopyFn_00432c40 Access_00432c40::fn = &Access_00432c40::_Ucopy;
