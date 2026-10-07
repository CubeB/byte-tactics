// Decompiled by Haiku. Names are provisional.
// std::vector<Unit*>::size() from MSVC 5's <vector>, out of line.
// 0x4152f0 calls it on a local vector that 0x40c510 (vector<Unit*>'s
// constructor) builds and 0x40c530 (its destructor) frees; 0x4103e0 and
// 0x410850 call it on locals that 0x40c530 frees. 0x40ad80, 0x480250, 0x48ca20
// and 0x48ddc0 call it from inlined insert code for 4-byte elements.
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_0040c560;
// Taking the member's address makes the compiler emit it.
typedef Vec_0040c560::size_type (Vec_0040c560::*SizeFn_0040c560)() const;

// FUNCTION: 0x40c560 ?size@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QBEIXZ
SizeFn_0040c560 g_size_0040c560 = &Vec_0040c560::size;
