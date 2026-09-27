// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00433d50>::erase(iterator first, iterator last) for a
// 4-byte element (its caller at 0x4336f0 shrinks the vector with it, as an
// inlined resize). Taking the member's address makes the compiler emit the
// template instantiation out of line. The element layout (two unsigned
// shorts) follows 0x4339e0, which reads entries of a vector at +0x4.
#include <vector>

struct Elem_00433d50 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00433d50> Vec_00433d50;
typedef Vec_00433d50::iterator (Vec_00433d50::*EraseFn_00433d50)(
    Vec_00433d50::iterator, Vec_00433d50::iterator);

// FUNCTION: 0x433d50 ?erase@?$vector@UElem_00433d50@@V?$allocator@UElem_00433d50@@@std@@@std@@QAEPAUElem_00433d50@@PAU3@0@Z
EraseFn_00433d50 g_erase_00433d50 = &Vec_00433d50::erase;
