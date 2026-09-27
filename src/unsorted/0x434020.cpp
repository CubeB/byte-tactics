// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_00434020> >::erase(iterator first, iterator last)
// from MSVC 5's <vector>: copy the tail down with the inner vector's
// operator= (0x4345e0), then destroy the leftover inner vectors. Destroying
// an inner element calls the empty out-of-line FUN_00434430, reproduced here
// with a std::_Destroy overload for the element type. Taking the member's
// address makes the compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

void __stdcall FUN_00434430(int);

namespace std {
inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

typedef std::vector<Elem_00434020> Inner_00434020;
typedef std::vector<Inner_00434020> Outer_00434020;
typedef Outer_00434020::iterator (Outer_00434020::*EraseFn_00434020)(
    Outer_00434020::iterator, Outer_00434020::iterator);

// FUNCTION: 0x434020 ?erase@?$vector@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@V?$allocator@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@@2@@std@@QAEPAV?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@2@PAV32@0@Z
EraseFn_00434020 g_erase_00434020 = &Outer_00434020::erase;
