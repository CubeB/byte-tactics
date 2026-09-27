// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::~vector() from MSVC 5's <vector>, out of line:
// the destroy loop of the trivial elements leaves only the dead store of
// _First (the push ecx slot), then _First is freed and the three pointers
// zeroed. The original calls it from the element destroy loop of
// vector<Elem_00434360>::operator= (0x434770, at 0x4348f5), where the
// element's implicit destructor is inlined and the inliner stops at the held
// vector's; taking that operator='s address makes the compiler emit this
// destructor out of line the same way.
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

AssignFn_00433a30 g_assign_00433a30 = &Outer_00433a30::operator=;

// FUNCTION: 0x433a30 ??1?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@XZ
