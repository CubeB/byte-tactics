// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<Elem_00434360>::operator=(const vector&) from MSVC 5's
// <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: the three-way size/capacity test with copy,
// _Ucopy and the allocate branch. The element's implicit operator= inlines to
// the held vector's operator= (0x4345e0), while its implicit destructor
// (0x4349f0, called as ??_G) and copy constructor (0x434470) stay out of
// line. Its callers are the outer vector<vector<Elem_00434360>>'s insert
// (0x4340f0) and erase (0x434360). Taking the member's address makes the
// compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_00434770;
typedef Vec_00434770& (Vec_00434770::*AssignFn_00434770)(const Vec_00434770&);

// FUNCTION: 0x434770 ??4?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_00434770 g_assign_00434770 = &Vec_00434770::operator=;
