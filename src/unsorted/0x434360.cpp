// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_00434360> >::erase(iterator first, iterator last)
// from MSVC 5's <vector>: copy the tail down with the inner vector's
// operator= (0x434770), then destroy the leftover inner vectors. Destroying
// an element calls the out-of-line FUN_00434440 (which frees a vector held in
// the element), reproduced here with a std::_Destroy overload for the element
// type. Same shape as 0x434020.cpp. Taking the member's address makes the
// compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434360 {
    int unknown_0;                     // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

void __stdcall FUN_00434440(Elem_00434360* p);

namespace std {
inline void _Destroy(Elem_00434360* p)
{
    FUN_00434440(p);
}
}

typedef std::vector<Elem_00434360> Inner_00434360;
typedef std::vector<Inner_00434360> Outer_00434360;
typedef Outer_00434360::iterator (Outer_00434360::*EraseFn_00434360)(
    Outer_00434360::iterator, Outer_00434360::iterator);

// FUNCTION: 0x434360 ?erase@?$vector@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@V?$allocator@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@@2@@std@@QAEPAV?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@2@PAV32@0@Z
EraseFn_00434360 g_erase_00434360 = &Outer_00434360::erase;
