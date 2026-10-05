// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::vector<Elem_00434020>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, for the 4-byte element (two unsigned shorts) used by the
// vector at 0x433d50 (erase), 0x433d90 (_Destroy) and 0x433da0 (deallocate).
// Its only caller, 0x4336f0, is an inlined resize: it calls this with
// end(), n - size() and a temporary element. Taking the member's address
// makes the compiler emit the template instantiation out of line, as in
// 0x433db0 (the same insert for the 16-byte element).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Vec_00433b20;
typedef void (Vec_00433b20::*InsertFn_00433b20)(
    Vec_00433b20::iterator, Vec_00433b20::size_type, const Elem_00434020&);

// FUNCTION: 0x433b20 ?insert@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAEXPAUElem_00434020@@IABU3@@Z
InsertFn_00433b20 g_insert_00433b20 = &Vec_00433b20::insert;
