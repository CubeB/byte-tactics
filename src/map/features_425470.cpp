// Decompiled by Opus. Names are provisional.
// std::vector<unsigned short>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its caller (0x424c00) shrinks its
// feature type remap table twice, once through the out-of-line erase
// 0x425430 and once through an inlined erase that calls std::copy
// (0x4256a0) and then this with ecx set to the vector. Renamed in #335 from
// vector<Elem_00425430>; the byte-identical 0x40d280 is
// vector<short>::_Destroy. _Destroy is protected, so a derived class takes
// its address to make the compiler emit it out of line.
#include <vector>

typedef std::vector<unsigned short> Vec_00425470;
typedef void (Vec_00425470::*DestroyFn_00425470)(Vec_00425470::iterator, Vec_00425470::iterator);

struct Access_00425470 : Vec_00425470 {
    static DestroyFn_00425470 fn;
};

// FUNCTION: 0x425470 ?_Destroy@?$vector@GV?$allocator@G@std@@@std@@IAEXPAG0@Z
DestroyFn_00425470 Access_00425470::fn = &Access_00425470::_Destroy;
