// Decompiled by Opus. Names are provisional.
// std::vector<Unit*>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// Its callers (0x405d90, 0x40ad80, 0x40b530, 0x480250 and others) inline
// vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and 0x406c00
// (_Destroy) with ecx set to the vector. 0x40ad80 calls it on the same
// vector of units as size (0x40c560) and insert (0x408f30), so the element
// type is Unit* (see 0x406c00.cpp).
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00406c40;
typedef void (Vec_00406c40::*UfillFn_00406c40)(
    Vec_00406c40::iterator, Vec_00406c40::size_type, Unit* const&);

struct Access_00406c40 : Vec_00406c40 {
    static UfillFn_00406c40 fn;
};

// FUNCTION: 0x406c40 ?_Ufill@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@IAEXPAPAUUnit@@IABQAU3@@Z
UfillFn_00406c40 Access_00406c40::fn = &Access_00406c40::_Ufill;
