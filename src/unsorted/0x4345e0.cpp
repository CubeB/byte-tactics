// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<Elem_00434020>::operator=(const vector&) from MSVC 5's
// <vector>: the three-way size/capacity test with copy, _Ucopy and the
// allocate branch. Taking the member's address makes the compiler emit the
// template instantiation out of line, the way the original file did.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Inner_004345e0;
typedef Inner_004345e0& (Inner_004345e0::*AssignFn_004345e0)(const Inner_004345e0&);

// FUNCTION: 0x4345e0 ??4?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_004345e0 g_assign_004345e0 = &Inner_004345e0::operator=;
