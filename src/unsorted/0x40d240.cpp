// Decompiled by Opus. Names are provisional.
// std::vector<short>::erase(iterator first, iterator last), called
// from the inlined resize() in 0x409160. Taking the member's address makes
// the compiler emit the template instantiation out of line.
#include <vector>

typedef std::vector<short> Vec_0040d240;
typedef Vec_0040d240::iterator (Vec_0040d240::*EraseFn_0040d240)(
    Vec_0040d240::iterator, Vec_0040d240::iterator);

// FUNCTION: 0x40d240 ?erase@?$vector@FV?$allocator@F@std@@@std@@QAEPAFPAF0@Z
EraseFn_0040d240 g_erase_0040d240 = &Vec_0040d240::erase;
