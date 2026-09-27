// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00406c10>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x405d90, 0x40b530, 0x480250 and others)
// inline vector::insert and call 0x406c40 (_Ufill), 0x406c10 (_Ucopy) and
// 0x406c00 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00406c10 {
    int unknown_0;
};

typedef std::vector<Elem_00406c10> Vec_00406c40;
typedef void (Vec_00406c40::*UfillFn_00406c40)(
    Vec_00406c40::iterator, Vec_00406c40::size_type, const Elem_00406c10&);

struct Access_00406c40 : Vec_00406c40 {
    static UfillFn_00406c40 fn;
};

// FUNCTION: 0x406c40 ?_Ufill@?$vector@UElem_00406c10@@V?$allocator@UElem_00406c10@@@std@@@std@@IAEXPAUElem_00406c10@@IABU3@@Z
UfillFn_00406c40 Access_00406c40::fn = &Access_00406c40::_Ufill;
