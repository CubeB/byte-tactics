// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::vector<Elem_0046faf0>::operator= from MSVC 5's <vector>. The element
// type is 14 bytes (three ints and a short, packed to 2), the same vector as
// _Ucopy (0x46faf0), _Destroy (0x46e870), _Ufill (0x46fb40) and size (0x470770).
// Its one caller is PacketSequencer::operator= (0x470560), where the first
// vector's operator= is inlined by /Ob2 but this second one is emitted
// out of line. operator= is defined in the class (implicitly inline), so its
// body is emitted once its address is taken by a pointer to member.
#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

typedef std::vector<Elem_0046faf0> Vec_0046faf0;
typedef Vec_0046faf0& (Vec_0046faf0::*AssignFn_0046faf0)(const Vec_0046faf0&);

// FUNCTION: 0x4707a0 ??4?$vector@UElem_0046faf0@@V?$allocator@UElem_0046faf0@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_0046faf0 g_assign_0046faf0 = &Vec_0046faf0::operator=;
