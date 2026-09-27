// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00433d50>::size() from MSVC 5's <vector>, out of line.
// Taking the member's address makes the compiler emit it.
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
#include <vector>

struct Elem_00433d50 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00433d50> Vec_00433a60;
typedef Vec_00433a60::size_type (Vec_00433a60::*SizeFn_00433a60)() const;

// FUNCTION: 0x433a60 ?size@?$vector@UElem_00433d50@@V?$allocator@UElem_00433d50@@@std@@@std@@QBEIXZ
SizeFn_00433a60 g_size_00433a60 = &Vec_00433a60::size;
