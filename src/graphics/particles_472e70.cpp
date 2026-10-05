// Decompiled by Opus. Names are provisional.
// Returns whether a std::vector of 52-byte elements is empty; the bool from
// the inlined vector::empty() is widened to the int return value.
#include <vector>

struct Elem_00472e70 {
    char unknown_0[0x34];
};

struct TeleportParticles {
    char unknown_0[0xc];
    std::vector<Elem_00472e70> items;   // +0xc (_First +0x10, _Last +0x14)

    int FUN_00472e70();
};

// FUNCTION: 0x472e70
int TeleportParticles::FUN_00472e70()
{
    return items.empty();
}
