// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00470f00>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its caller 0x470c10 inlines vector::insert and calls
// 0x470f30 (_Ufill), 0x470f00 (_Ucopy) and 0x470ef0 (_Destroy) with ecx
// set to the vector.
#include <vector>

struct Elem_00470f00 {
    int unknown_0;
};

typedef std::vector<Elem_00470f00> Vec_00470f30;
typedef void (Vec_00470f30::*UfillFn_00470f30)(
    Vec_00470f30::iterator, Vec_00470f30::size_type, const Elem_00470f00&);

struct Access_00470f30 : Vec_00470f30 {
    static UfillFn_00470f30 fn;
};

// FUNCTION: 0x470f30 ?_Ufill@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEXPAUElem_00470f00@@IABU3@@Z
UfillFn_00470f30 Access_00470f30::fn = &Access_00470f30::_Ufill;
