// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_004340b0> >::_Destroy(first, last) from MSVC
// 5's <vector>: runs each inner vector's inlined destructor (free _First and
// zero the three pointers; the empty destroy loop of the trivial element
// type leaves the dead store). The caller (0x433130) calls it with ecx set to
// a local vector. _Destroy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
#include <vector>

struct Elem_004340b0 {
    int value;
};

typedef std::vector<Elem_004340b0> Inner_004340b0;
typedef std::vector<Inner_004340b0> Outer_004340b0;
typedef void (Outer_004340b0::*DestroyFn_004340b0)(Outer_004340b0::iterator, Outer_004340b0::iterator);

struct Access_004340b0 : Outer_004340b0 {
    static DestroyFn_004340b0 fn;
};

// FUNCTION: 0x4340b0 ?_Destroy@?$vector@V?$vector@UElem_004340b0@@V?$allocator@UElem_004340b0@@@std@@@std@@V?$allocator@V?$vector@UElem_004340b0@@V?$allocator@UElem_004340b0@@@std@@@std@@@2@@std@@IAEXPAV?$vector@UElem_004340b0@@V?$allocator@UElem_004340b0@@@std@@@2@0@Z
DestroyFn_004340b0 Access_004340b0::fn = &Access_004340b0::_Destroy;
