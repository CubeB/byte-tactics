// Decompiled by Opus. Names are provisional.
// std::allocator<Elem_00434020>::deallocate(p, n) from MSVC 5's <xmemory>:
// operator delete(p). Called with ecx set to a vector's allocator, with
// _First and _End - _First as arguments (an inlined ~vector). Taking the
// member's address makes the compiler emit it out of line.
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::allocator<Elem_00434020> Alloc_00433da0;
typedef void (Alloc_00433da0::*DeallocateFn_00433da0)(void*, Alloc_00433da0::size_type);

// FUNCTION: 0x433da0 ?deallocate@?$allocator@UElem_00434020@@@std@@QAEXPAXI@Z
DeallocateFn_00433da0 g_deallocate_00433da0 = &Alloc_00433da0::deallocate;
