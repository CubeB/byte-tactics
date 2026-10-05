// Decompiled by Haiku. Names are provisional.
// std::vector<short>::size() from MSVC 5's <vector>, out of line.
// Taking the member's address makes the compiler emit it. 0x409160 calls it
// with ecx set to its vector<short> at +0x7d, from the inlined
// resize() around the out-of-line insert (0x40d020) and erase (0x40d240).
// The element is signed: every read of it sign-extends (0x4099fc in
// 0x409730, 0x40bd41 in 0x40bb00, 0x40c21b in 0x40c200), and 0x40aa40
// fills it as a vector<short>. Renamed in #335 from vector<unsigned short>,
// which is the byte-identical set at 0x4251f0-0x425470 (0x424c00's table).
#include <vector>

typedef std::vector<short> Vec_0040d000;
typedef Vec_0040d000::size_type (Vec_0040d000::*SizeFn_0040d000)() const;

// FUNCTION: 0x40d000 ?size@?$vector@FV?$allocator@F@std@@@std@@QBEIXZ
SizeFn_0040d000 g_size_0040d000 = &Vec_0040d000::size;
