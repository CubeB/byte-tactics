// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00473500>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its callers (0x471160, 0x471820, 0x471a50) inline
// vector::insert and call 0x473530 (_Ufill), 0x473500 (_Ucopy) and
// 0x4732d0 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00473500 {
    int unknown_0;
};

typedef std::vector<Elem_00473500> Vec_00473530;
typedef void (Vec_00473530::*UfillFn_00473530)(
    Vec_00473530::iterator, Vec_00473530::size_type, const Elem_00473500&);

struct Access_00473530 : Vec_00473530 {
    static UfillFn_00473530 fn;
};

// FUNCTION: 0x473530 ?_Ufill@?$vector@UElem_00473500@@V?$allocator@UElem_00473500@@@std@@@std@@IAEXPAUElem_00473500@@IABU3@@Z
UfillFn_00473530 Access_00473530::fn = &Access_00473530::_Ufill;
