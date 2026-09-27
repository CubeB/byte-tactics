// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<Elem_00434360>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>. The element's implicit copy constructor
// (0x434470) and its held vector's operator= (0x4345e0) and destructor
// (0x433a30) stay out of line. Its callers, 0x433380 and 0x4335f0, resize a
// vector<Elem_00434360> with it. Taking the member's address makes the
// compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_00433db0;
typedef void (Vec_00433db0::*InsertFn_00433db0)(
    Vec_00433db0::iterator, Vec_00433db0::size_type, const Elem_00434360&);

// FUNCTION: 0x433db0 ?insert@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEXPAUElem_00434360@@IABU3@@Z
InsertFn_00433db0 g_insert_00433db0 = &Vec_00433db0::insert;
