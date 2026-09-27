// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::~vector() from MSVC 5's <vector>, out of line:
// the destroy loop of the trivial elements leaves only the dead store of
// _First (the push ecx slot), then _First is freed and the three pointers
// zeroed. The original calls it from the element destroy loop of the outer
// vector's operator= (0x434770, at 0x4348f5), where the inliner stops;
// taking that operator='s address makes the compiler emit this destructor out
// of line the same way.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00433a30;
typedef std::vector<Inner_00433a30> Outer_00433a30;
typedef Outer_00433a30& (Outer_00433a30::*AssignFn_00433a30)(const Outer_00433a30&);

AssignFn_00433a30 g_assign_00433a30 = &Outer_00433a30::operator=;

// FUNCTION: 0x433a30 ??1?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@XZ
