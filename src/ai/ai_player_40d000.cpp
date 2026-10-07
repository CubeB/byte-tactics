// Decompiled by Haiku. Names are provisional.
// std::vector<short>::size(). 0x409160 calls it with ecx set to its
// vector<short> at +0x7d, from the inlined resize() around the insert
// (0x40d020) and erase (0x40d240).
// The element is signed: every read of it sign-extends (0x4099fc in
// 0x409730, 0x40bd41 in 0x40bb00, 0x40c21b in 0x40c200), and 0x40aa40
// fills it as a vector<short>.
#include <vector>

typedef std::vector<short> Vec_0040d000;
typedef Vec_0040d000::size_type (Vec_0040d000::*SizeFn_0040d000)() const;

// FUNCTION: 0x40d000 ?size@?$vector@FV?$allocator@F@std@@@std@@QBEIXZ
SizeFn_0040d000 g_size_0040d000 = &Vec_0040d000::size;
