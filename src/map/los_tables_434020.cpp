// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434360>::erase(iterator first, iterator last) from MSVC
// 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: copy the tail down with the held vector's
// operator= (0x4345e0), then destroy the leftover elements. Destroying an
// inner element calls the empty out-of-line FUN_00434430, reproduced here
// with a std::_Destroy overload for the element type; the overload for
// Elem_00434360 runs the held vector's destructor directly, one inline level
// shallower than the implicit destructor, so that FUN_00434430 is still
// reached inline. Taking the member's address makes the compiler emit the
// template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

void __stdcall FUN_00434430(int);

namespace std {
inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434020> Inner_00434020;

namespace std {
inline void _Destroy(Elem_00434360* p)
{
    p->v.~Inner_00434020();
}
}

typedef std::vector<Elem_00434360> Outer_00434020;
typedef Outer_00434020::iterator (Outer_00434020::*EraseFn_00434020)(
    Outer_00434020::iterator, Outer_00434020::iterator);

// FUNCTION: 0x434020 ?erase@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEPAUElem_00434360@@PAU3@0@Z
EraseFn_00434020 g_erase_00434020 = &Outer_00434020::erase;
