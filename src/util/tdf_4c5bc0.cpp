// Decompiled by Opus. Names are provisional.
// std::vector<TdfField>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copy-constructs [first, last) into raw storage at dest and
// returns the end of the copies. The element holds two reference-counted
// handles whose copy constructor is 0x4c91a0. _Ucopy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
#include <vector>

class Class_004c91a0 {
public:
    char* p;
    Class_004c91a0();
    Class_004c91a0(const Class_004c91a0& other);
};

struct TdfField {
    Class_004c91a0 a;                  // +0x0
    Class_004c91a0 b;                  // +0x4
};

typedef std::vector<TdfField> Vec_004c5bc0;
typedef Vec_004c5bc0::iterator (Vec_004c5bc0::*UcopyFn_004c5bc0)(
    Vec_004c5bc0::const_iterator, Vec_004c5bc0::const_iterator, Vec_004c5bc0::iterator);

struct Access_004c5bc0 : Vec_004c5bc0 {
    static UcopyFn_004c5bc0 fn;
};

// FUNCTION: 0x4c5bc0 ?_Ucopy@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEPAUTdfField@@PBU3@0PAU3@@Z
UcopyFn_004c5bc0 Access_004c5bc0::fn = &Access_004c5bc0::_Ucopy;
