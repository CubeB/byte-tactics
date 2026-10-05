// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434360>::size() from MSVC 5's <vector>, out of line
// (16-byte elements, each a struct holding a std::vector<Elem_00434020>).
// Its caller 0x433380 resizes the same vector with 0x433db0 (its insert),
// which copies elements with the vector<Elem_00434020> copy constructor
// (0x434470) and operator= (0x4345e0). Taking the member's address makes the
// compiler emit it.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_00433b00;
typedef Vec_00433b00::size_type (Vec_00433b00::*SizeFn_00433b00)() const;

// FUNCTION: 0x433b00 ?size@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QBEIXZ
SizeFn_00433b00 g_size_00433b00 = &Vec_00433b00::size;
