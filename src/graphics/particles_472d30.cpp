// Decompiled by Haiku; renamed to the real template member by the orchestrator (#418). Names are provisional.
// std::vector<Elem_00473500>::size() from MSVC 5's <vector>, out of line
// (`sar eax, 2`: a 4-byte element). It belongs to the same vector as _Ucopy
// (0x473500), _Ufill (0x473530) and _Destroy (0x4732d0); 0x471820 and 0x471a50
// call it where their inlined insert needs the size. Taking the member's
// address makes the compiler emit it.
#include <vector>

struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_00472d30;
typedef Vec_00472d30::size_type (Vec_00472d30::*SizeFn_00472d30)() const;

// FUNCTION: 0x472d30 ?size@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@QBEIXZ
SizeFn_00472d30 g_size_00472d30 = &Vec_00472d30::size;
