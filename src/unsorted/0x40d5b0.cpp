// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040cc40>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// The element type is a guess: any 8-byte trivially copyable type compiles
// to the same code. Its callers (0x40a7b0, 0x40ca50) inline vector::insert
// and call 0x40d5b0 (_Ufill), 0x40cc40 (_Ucopy) and 0x40cc30 (_Destroy)
// with ecx set to the vector.
#include <vector>

struct Elem_0040cc40 {
    int a;                             // +0x0
    int b;                             // +0x4
};

typedef std::vector<Elem_0040cc40> Vec_0040d5b0;
typedef void (Vec_0040d5b0::*UfillFn_0040d5b0)(
    Vec_0040d5b0::iterator, Vec_0040d5b0::size_type, const Elem_0040cc40&);

struct Access_0040d5b0 : Vec_0040d5b0 {
    static UfillFn_0040d5b0 fn;
};

// FUNCTION: 0x40d5b0 ?_Ufill@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEXPAUElem_0040cc40@@IABU3@@Z
UfillFn_0040d5b0 Access_0040d5b0::fn = &Access_0040d5b0::_Ufill;
