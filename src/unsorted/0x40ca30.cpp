// Decompiled by Haiku. Names are provisional.
// std::vector<Elem_0040cc40>::capacity() from MSVC 5's <vector>, out of
// line. Taking the member's address makes the compiler emit it. 0x40a260
// calls it from its inlined vector::reserve, where /Ob2's budget ran out
// (its neighbours there are size(), 0x40c5b0, and Elem_0040cc40's copy
// constructor, 0x40a5b0).
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

typedef std::vector<Elem_0040cc40> Vec_0040ca30;
typedef Vec_0040ca30::size_type (Vec_0040ca30::*CapacityFn_0040ca30)() const;

// FUNCTION: 0x40ca30 ?capacity@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QBEIXZ
CapacityFn_0040ca30 g_capacity_0040ca30 = &Vec_0040ca30::capacity;
