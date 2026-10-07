// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::~vector(): frees _First and zeroes the three
// pointers. The original calls it from the element destroy loop of
// vector<Elem_00434360>::operator= (0x434770, at 0x4348f5).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Outer_00433a30;
typedef Outer_00433a30& (Outer_00433a30::*AssignFn_00433a30)(const Outer_00433a30&);

// Taking this operator='s address makes the destructor get emitted out of line.
AssignFn_00433a30 g_assign_00433a30 = &Outer_00433a30::operator=;

// FUNCTION: 0x433a30 ??1?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@XZ
