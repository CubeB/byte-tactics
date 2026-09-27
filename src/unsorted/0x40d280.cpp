// Decompiled by Opus. Names are provisional.
// std::vector<short>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its caller (0x40b390) runs the inlined
// destructor of the vector at +0x7d with ecx set to it; 0x409160 resizes
// that same vector with vector<short>::erase (0x40d240).
// _Destroy is protected, so a derived class takes its address to make the
// compiler emit it out of line.
#include <vector>

typedef std::vector<short> Vec_0040d280;
typedef void (Vec_0040d280::*DestroyFn_0040d280)(Vec_0040d280::iterator, Vec_0040d280::iterator);

struct Access_0040d280 : Vec_0040d280 {
    static DestroyFn_0040d280 fn;
};

// FUNCTION: 0x40d280 ?_Destroy@?$vector@FV?$allocator@F@std@@@std@@IAEXPAF0@Z
DestroyFn_0040d280 Access_0040d280::fn = &Access_0040d280::_Destroy;
