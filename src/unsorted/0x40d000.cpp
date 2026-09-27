// Decompiled by Haiku. Names are provisional.
// std::vector<unsigned short>::size() from MSVC 5's <vector>, out of line.
// Taking the member's address makes the compiler emit it. 0x409160 calls it
// with ecx set to its vector<unsigned short> at +0x7d, from the inlined
// resize() around the out-of-line insert (0x40d020) and erase (0x40d240).
#include <vector>

typedef std::vector<unsigned short> Vec_0040d000;
typedef Vec_0040d000::size_type (Vec_0040d000::*SizeFn_0040d000)() const;

// FUNCTION: 0x40d000 ?size@?$vector@GV?$allocator@G@std@@@std@@QBEIXZ
SizeFn_0040d000 g_size_0040d000 = &Vec_0040d000::size;
