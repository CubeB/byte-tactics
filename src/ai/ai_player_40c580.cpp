// Decompiled by Sonnet. Names are provisional.
// std::vector<Elem_0040cc40>::~vector() from MSVC 5's <vector>, out of line
// (the same code as 0x40c530 for another element type). 0x40b390 (which
// deletes the player AI object built by 0x409160) calls it on the vector at
// +0x4d, whose _Ucopy (0x40cc40), _Ufill (0x40d5b0), _Destroy (0x40cc30) and
// size (0x40c5b0) 0x40a7b0 calls on the same member.
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

typedef std::vector<Elem_0040cc40> Inner_0040c580;
typedef std::vector<Inner_0040c580> Outer_0040c580;
typedef Outer_0040c580& (Outer_0040c580::*AssignFn_0040c580)(const Outer_0040c580&);

// Taking the address of the outer vector's operator= makes the compiler emit
// the inner destructor out of line.
AssignFn_0040c580 g_assign_0040c580 = &Outer_0040c580::operator=;

// FUNCTION: 0x40c580 ??1?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QAE@XZ
