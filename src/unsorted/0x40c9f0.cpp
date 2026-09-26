// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040c9f0*>::erase(iterator first, iterator last) for a
// 4-byte element type. Taking the member's address makes the compiler emit
// the template instantiation out of line. The element type is a guess: any
// 4-byte type with a trivial destructor compiles to the same code.
#include <vector>

struct Elem_0040c9f0 {
    int unknown_0;
};

typedef std::vector<Elem_0040c9f0*> Vec_0040c9f0;
typedef Vec_0040c9f0::iterator (Vec_0040c9f0::*EraseFn_0040c9f0)(
    Vec_0040c9f0::iterator, Vec_0040c9f0::iterator);

// FUNCTION: 0x40c9f0 ?erase@?$vector@PAUElem_0040c9f0@@V?$allocator@PAUElem_0040c9f0@@@std@@@std@@QAEPAPAUElem_0040c9f0@@PAPAU3@0@Z
EraseFn_0040c9f0 g_erase_0040c9f0 = &Vec_0040c9f0::erase;
