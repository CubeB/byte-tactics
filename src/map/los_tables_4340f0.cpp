// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<std::vector<Elem_00434360> >::insert(iterator, size_type,
// const T&) from MSVC 5's <vector>, where Elem_00434360 is a struct holding
// one std::vector<Elem_00434020>: the insert of n copies behind the global at
// 0x51e6a0. The inner vector's copy constructor (0x4344e0), operator=
// (0x434770) and destructor (0x433a80) stay out of line. Taking the member's
// address makes the compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Inner_004340f0;
typedef std::vector<Inner_004340f0> Outer_004340f0;
typedef void (Outer_004340f0::*InsertFn_004340f0)(
    Outer_004340f0::iterator, Outer_004340f0::size_type, const Inner_004340f0&);

// FUNCTION: 0x4340f0 ?insert@?$vector@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@V?$allocator@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@@2@@std@@QAEXPAV?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@2@IABV32@@Z
InsertFn_004340f0 g_insert_004340f0 = &Outer_004340f0::insert;
