// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434360>::_Destroy(first, last) from MSVC 5's <vector>:
// runs each element's inlined destructor, which is ~vector<Elem_00434020>
// (free _First and zero the three pointers; the empty destroy loop of the
// trivial element type leaves the dead store). The caller (0x433130) calls it
// with ecx set to a local vector. _Destroy is protected, so a derived class
// takes its address to make the compiler emit it out of line.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_004340b0;
typedef void (Vec_004340b0::*DestroyFn_004340b0)(Vec_004340b0::iterator, Vec_004340b0::iterator);

struct Access_004340b0 : Vec_004340b0 {
    static DestroyFn_004340b0 fn;
};

// FUNCTION: 0x4340b0 ?_Destroy@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@IAEXPAUElem_00434360@@0@Z
DestroyFn_004340b0 Access_004340b0::fn = &Access_004340b0::_Destroy;
