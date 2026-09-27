// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_00434020> >::size() from MSVC 5's <vector>,
// out of line (16-byte elements). Its caller 0x433380 resizes the same
// vector with 0x433db0 (its insert), which copies elements with the
// vector<Elem_00434020> copy constructor (0x434470) and operator=
// (0x4345e0). Taking the member's address makes the compiler emit it.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00433b00;

typedef std::vector<Inner_00433b00> Vec_00433b00;
typedef Vec_00433b00::size_type (Vec_00433b00::*SizeFn_00433b00)() const;

// FUNCTION: 0x433b00 ?size@?$vector@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@V?$allocator@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@@2@@std@@QBEIXZ
SizeFn_00433b00 g_size_00433b00 = &Vec_00433b00::size;
