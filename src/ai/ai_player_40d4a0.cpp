// Decompiled by Opus. Names are provisional.
// std::vector<unsigned char>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its caller (0x40b390) runs the inlined
// destructors of the vectors at +0x8d and +0x9d with ecx set to each;
// 0x409160 resizes the one at +0x9d with vector<unsigned char>::insert
// (0x40d290) and erase (0x40d470). _Destroy is protected, so a derived
// class takes its address to make the compiler emit it out of line.
#include <vector>

typedef std::vector<unsigned char> Vec_0040d4a0;
typedef void (Vec_0040d4a0::*DestroyFn_0040d4a0)(Vec_0040d4a0::iterator, Vec_0040d4a0::iterator);

struct Access_0040d4a0 : Vec_0040d4a0 {
    static DestroyFn_0040d4a0 fn;
};

// FUNCTION: 0x40d4a0 ?_Destroy@?$vector@EV?$allocator@E@std@@@std@@IAEXPAE0@Z
DestroyFn_0040d4a0 Access_0040d4a0::fn = &Access_0040d4a0::_Destroy;
