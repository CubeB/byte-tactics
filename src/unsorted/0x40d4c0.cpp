// Decompiled by Haiku. Names are provisional.
// std::vector<Elem_0040d550>::size() from MSVC 5's <vector>, out of line
// (`>> 2`: a 4-byte element). Taking the member's address makes the compiler
// emit it. Its caller 0x40c7f0 is the same vector's resize(), and 0x40d4e0,
// 0x40d550 and 0x40d580 are more of its members.
#include <vector>

struct Elem_0040d550 {
    int value;                         // +0x0
};

typedef std::vector<Elem_0040d550> Vec_0040d4c0;
typedef Vec_0040d4c0::size_type (Vec_0040d4c0::*SizeFn_0040d4c0)() const;

// FUNCTION: 0x40d4c0 ?size@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@QBEIXZ
SizeFn_0040d4c0 g_size_0040d4c0 = &Vec_0040d4c0::size;
