// Decompiled by Haiku. Names are provisional.
// std::vector<Elem_0040cc40>::size() from MSVC 5's <vector>, out of line.
// 0x40a7b0 calls it with ecx set to the player AI object's vector at +0x4d,
// next to that vector's _Ucopy (0x40cc40), _Ufill (0x40d5b0) and _Destroy
// (0x40cc30); 0x40a260 and 0x40ca50 call it from the same inlined insert code.
#include <vector>

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

typedef std::vector<Elem_0040cc40> Vec_0040c5b0;
// Taking the member's address makes the compiler emit it.
typedef Vec_0040c5b0::size_type (Vec_0040c5b0::*SizeFn_0040c5b0)() const;

// FUNCTION: 0x40c5b0 ?size@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QBEIXZ
SizeFn_0040c5b0 g_size_0040c5b0 = &Vec_0040c5b0::size;
