// Decompiled by Opus. Names are provisional.
// std::vector<Unit*>::_Destroy(first, last) from MSVC 5's <vector>: empty,
// since the element type is trivial. _Destroy is protected, so a derived
// class takes its address to make the compiler emit it out of line.
// Its callers (0x405d90, 0x40b530, 0x480250 and others) inline
// vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and 0x406c00
// (_Destroy) with ecx set to the vector. 0x40ad80 calls _Ucopy, _Ufill,
// size (0x40c560) and insert (0x408f30) on one vector of units, and 0x48d220
// calls _Destroy and erase (0x40c9f0), so all of these are members of the
// same std::vector<Unit*> (see 0x40c510.cpp).
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00406c00;
typedef void (Vec_00406c00::*DestroyFn_00406c00)(Vec_00406c00::iterator, Vec_00406c00::iterator);

struct Access_00406c00 : Vec_00406c00 {
    static DestroyFn_00406c00 fn;
};

// FUNCTION: 0x406c00 ?_Destroy@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@IAEXPAPAUUnit@@0@Z
DestroyFn_00406c00 Access_00406c00::fn = &Access_00406c00::_Destroy;
