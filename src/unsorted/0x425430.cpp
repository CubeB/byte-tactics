// Decompiled by Opus. Names are provisional.
// std::vector<unsigned short>::erase(iterator first, iterator last), called
// from the first inlined resize() of 0x424c00's feature type remap table
// (with size() 0x4251f0 and insert 0x425210). The element is a scalar: the
// resize value temporary in 0x424c00 is stored as a dword, which a 2-byte
// struct would not give. Renamed in #335 from vector<Elem_00425430>; the
// byte-identical 0x40d240 is vector<short>::erase. Taking the member's
// address makes the compiler emit the template instantiation out of line.
#include <vector>

typedef std::vector<unsigned short> Vec_00425430;
typedef Vec_00425430::iterator (Vec_00425430::*EraseFn_00425430)(
    Vec_00425430::iterator, Vec_00425430::iterator);

// FUNCTION: 0x425430 ?erase@?$vector@GV?$allocator@G@std@@@std@@QAEPAGPAG0@Z
EraseFn_00425430 g_erase_00425430 = &Vec_00425430::erase;
