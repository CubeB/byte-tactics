// Decompiled by Opus. Names are provisional.
// std::vector<Unit*>::erase(iterator first, iterator last). 0x40aa40 calls it
// (through clear()) on the unit lists at +0x05, +0x15 and +0x25 of the player
// AI object, whose destructor is 0x40c530, and 0x48d220 calls it together
// with _Destroy (0x406c00), so it is a member of the same std::vector<Unit*>
// (see 0x406c00.cpp).
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_0040c9f0;
typedef Vec_0040c9f0::iterator (Vec_0040c9f0::*EraseFn_0040c9f0)(
    Vec_0040c9f0::iterator, Vec_0040c9f0::iterator);

// FUNCTION: 0x40c9f0 ?erase@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEPAPAUUnit@@PAPAU3@0@Z
EraseFn_0040c9f0 g_erase_0040c9f0 = &Vec_0040c9f0::erase;
