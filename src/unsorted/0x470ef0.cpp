// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00470f00>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The element type is a guess: any 4-byte trivially copyable type compiles
// to the same code. Its caller 0x470c10 inlines vector::insert and calls
// 0x470f30 (_Ufill), 0x470f00 (_Ucopy) and 0x470ef0 (_Destroy) with ecx
// set to the vector.
#include <vector>

struct Elem_00470f00 {
    int unknown_0;
};

typedef std::vector<Elem_00470f00> Vec_00470ef0;
typedef void (Vec_00470ef0::*DestroyFn_00470ef0)(Vec_00470ef0::iterator, Vec_00470ef0::iterator);

struct Access_00470ef0 : Vec_00470ef0 {
    static DestroyFn_00470ef0 fn;
};

// FUNCTION: 0x470ef0 ?_Destroy@?$vector@UElem_00470f00@@V?$allocator@UElem_00470f00@@@std@@@std@@IAEXPAUElem_00470f00@@0@Z
DestroyFn_00470ef0 Access_00470ef0::fn = &Access_00470ef0::_Destroy;
