// Decompiled by Sonnet. Names are provisional.
// std::vector<Elem_0040cfb0>::~vector() from MSVC 5's <vector>, out of line
// (the same code as 0x40c530 for another element type). 0x40b390 (which
// deletes the player AI object built by 0x409160) calls it on the vector of
// 3-byte elements at +0x65, whose size (0x40cc80) and erase (0x40cfb0)
// 0x409160 calls on the same member. A destructor's address can't be taken;
// taking the address of the outer vector's operator= makes the compiler
// emit it out of line from its element destroy loop (as in 0x433a30).
#include <vector>

struct Elem_0040cfb0 {
    char a;                            // +0x0
    char b;                            // +0x1
    char c;                            // +0x2
};

typedef std::vector<Elem_0040cfb0> Inner_0040c5d0;
typedef std::vector<Inner_0040c5d0> Outer_0040c5d0;
typedef Outer_0040c5d0& (Outer_0040c5d0::*AssignFn_0040c5d0)(const Outer_0040c5d0&);

AssignFn_0040c5d0 g_assign_0040c5d0 = &Outer_0040c5d0::operator=;

// FUNCTION: 0x40c5d0 ??1?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAE@XZ
