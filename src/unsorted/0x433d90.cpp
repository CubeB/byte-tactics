// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Vec_00433d90;
typedef void (Vec_00433d90::*DestroyFn_00433d90)(Vec_00433d90::iterator, Vec_00433d90::iterator);

struct Access_00433d90 : Vec_00433d90 {
    static DestroyFn_00433d90 fn;
};

// FUNCTION: 0x433d90 ?_Destroy@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@IAEXPAUElem_00434020@@0@Z
DestroyFn_00433d90 Access_00433d90::fn = &Access_00433d90::_Destroy;
