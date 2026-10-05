// Decompiled by Haiku. Names are provisional.
// std::vector<Unit*>::vector(const allocator&) from MSVC 5's <vector>, out of
// line: copies the empty allocator byte and zeroes _First, _Last and _End.
// It is also the default constructor (the allocator is a default argument,
// so each caller pushes the address of a temporary allocator). 0x409160
// calls it for its unit list at +0x25 once its inline budget has run out.
// 0x4152f0 builds a local with it, then calls 0x40c560 (size) and 0x40c530
// (the destructor) on that same local; 0x410850 also builds a local with it.
// A constructor's address can't be taken and no vector member calls this
// one, so an explicit instantiation of the whole class emits it.
#include <vector>

struct Unit {
    int unknown_0;
};

template class std::vector<Unit*>;

// FUNCTION: 0x40c510 ??0?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAE@ABV?$allocator@PAUUnit@@@1@@Z
