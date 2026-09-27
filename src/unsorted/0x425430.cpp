// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00425430>::erase(iterator first, iterator last) for a
// 2-byte element type (the dead store of the old _Last into the argument
// slot comes from the inlined _Destroy). Taking the member's address makes
// the compiler emit the template instantiation out of line. The element
// type is a guess: any 2-byte trivially copyable type compiles the same.
#include <vector>

struct Elem_00425430 {
    short value;
};

typedef std::vector<Elem_00425430> Vec_00425430;
typedef Vec_00425430::iterator (Vec_00425430::*EraseFn_00425430)(
    Vec_00425430::iterator, Vec_00425430::iterator);

// FUNCTION: 0x425430 ?erase@?$vector@UElem_00425430@@V?$allocator@UElem_00425430@@@std@@@std@@QAEPAUElem_00425430@@PAU3@0@Z
EraseFn_00425430 g_erase_00425430 = &Vec_00425430::erase;
