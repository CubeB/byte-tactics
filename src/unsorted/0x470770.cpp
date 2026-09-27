// Decompiled by Sonnet; renamed to the real template member by the orchestrator (#417). Names are provisional.
// std::vector<Elem_0046faf0>::size() for the 14-byte element (three ints and
// a short, packed to 2 bytes), the same vector as _Ucopy (0x46faf0) and the
// operator= at 0x4707a0. Its callers are in 0x470560.
#include <vector>
#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;
    int b;
    int c;
    short d;
};
#pragma pack(pop)
typedef std::vector<Elem_0046faf0> Vec_00470770;
typedef Vec_00470770::size_type (Vec_00470770::*SizeFn_00470770)() const;
// FUNCTION: 0x470770 ?size@?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QBEIXZ
SizeFn_00470770 g_size_00470770 = &Vec_00470770::size;
