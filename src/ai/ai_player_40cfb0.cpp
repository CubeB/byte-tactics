// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040cfb0>::erase(iterator first, iterator last) for a
// 3-byte element type. Taking the member's address makes the compiler emit
// the template instantiation out of line.
#include <vector>

struct Elem_0040cfb0 {
    char a;                            // +0x0
    char b;                            // +0x1
    char c;                            // +0x2
};

typedef std::vector<Elem_0040cfb0> Vec_0040cfb0;
typedef Vec_0040cfb0::iterator (Vec_0040cfb0::*EraseFn_0040cfb0)(
    Vec_0040cfb0::iterator, Vec_0040cfb0::iterator);

// FUNCTION: 0x40cfb0 ?erase@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEPAUElem_0040cfb0@@PAU3@0@Z
EraseFn_0040cfb0 g_erase_0040cfb0 = &Vec_0040cfb0::erase;
