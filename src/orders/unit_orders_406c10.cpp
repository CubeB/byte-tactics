// Decompiled by Opus. Names are provisional.
// std::vector<Unit*>::_Ucopy(first, last, dest) from MSVC 5's <vector>:
// copies [first, last) into raw storage at dest and returns the end of the
// copies.
// Its callers (0x405d90, 0x40ad80, 0x40b530, 0x480250 and others) inline
// vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and 0x406c00
// (_Destroy) with ecx set to the vector. 0x40ad80 calls it on the same
// vector of units as size (0x40c560) and insert (0x408f30), so the element
// type is Unit* (see 0x406c00.cpp).
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00406c10;
typedef Vec_00406c10::iterator (Vec_00406c10::*UcopyFn_00406c10)(
    Vec_00406c10::const_iterator, Vec_00406c10::const_iterator, Vec_00406c10::iterator);

// _Ucopy is protected: the derived class takes its address to emit it out of line.
struct Access_00406c10 : Vec_00406c10 {
    static UcopyFn_00406c10 fn;
};

// FUNCTION: 0x406c10 ?_Ucopy@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@IAEPAPAUUnit@@PBQAU3@0PAPAU3@@Z
UcopyFn_00406c10 Access_00406c10::fn = &Access_00406c10::_Ucopy;
