// Decompiled by Haiku. Names are provisional.
// std::vector<unsigned short>::size(), out of line.
// 0x424c00 calls it from the first inlined resize() of its feature type remap
// table, around the out-of-line insert (0x425210) and erase (0x425430); the
// table maps saved feature type numbers to loaded ones (0xffff is "none"),
// hence unsigned.
#include <vector>

typedef std::vector<unsigned short> Vec_004251f0;
// Taking the member's address is what emits it out of line.
typedef Vec_004251f0::size_type (Vec_004251f0::*SizeFn_004251f0)() const;

// FUNCTION: 0x4251f0 ?size@?$vector@GV?$allocator@G@std@@@std@@QBEIXZ
SizeFn_004251f0 g_size_004251f0 = &Vec_004251f0::size;
