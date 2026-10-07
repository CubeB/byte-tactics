// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_00434360> >::erase(iterator first, iterator last)
// from MSVC 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: copy the tail down with the inner vector's
// operator= (0x434770), then destroy the leftover inner vectors. Destroying
// an element calls the out-of-line FUN_00434440 (std::_Destroy for the
// element, which frees the vector it holds). Same shape as 0x434020.cpp.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

void __stdcall FUN_00434440(Elem_00434360* p);

namespace std {
// Overload for the element type calls FUN_00434440.
inline void _Destroy(Elem_00434360* p)
{
    FUN_00434440(p);
}
}

typedef std::vector<Elem_00434360> Inner_00434360;
typedef std::vector<Inner_00434360> Outer_00434360;
typedef Outer_00434360::iterator (Outer_00434360::*EraseFn_00434360)(
    Outer_00434360::iterator, Outer_00434360::iterator);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x434360 ?erase@?$vector@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@V?$allocator@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@@2@@std@@QAEPAV?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@2@PAV32@0@Z
EraseFn_00434360 g_erase_00434360 = &Outer_00434360::erase;
