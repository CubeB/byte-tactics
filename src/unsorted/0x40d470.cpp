// Decompiled by Opus. Names are provisional.
// std::vector<unsigned char>::erase(iterator first, iterator last), called
// from the inlined resize() in 0x409160 (next to the unsigned short vector's
// erase at 0x40d240). Taking the member's address makes the compiler emit
// the template instantiation out of line.
#include <vector>

typedef std::vector<unsigned char> Vec_0040d470;
typedef Vec_0040d470::iterator (Vec_0040d470::*EraseFn_0040d470)(
    Vec_0040d470::iterator, Vec_0040d470::iterator);

// FUNCTION: 0x40d470 ?erase@?$vector@EV?$allocator@E@std@@@std@@QAEPAEPAE0@Z
EraseFn_0040d470 g_erase_0040d470 = &Vec_0040d470::erase;
