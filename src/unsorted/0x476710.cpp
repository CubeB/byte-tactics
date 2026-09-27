// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00475880>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage at first. _Ufill is
// protected, so a derived class takes its address to make the compiler
// emit it out of line.
// A 48-byte element type (its layout is a guess). Its caller 0x473d50
// inlines vector::insert and calls 0x476710 (_Ufill), 0x475880 (_Ucopy)
// and 0x475870 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00475880 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Elem_00475880> Vec_00476710;
typedef void (Vec_00476710::*UfillFn_00476710)(
    Vec_00476710::iterator, Vec_00476710::size_type, const Elem_00475880&);

struct Access_00476710 : Vec_00476710 {
    static UfillFn_00476710 fn;
};

// FUNCTION: 0x476710 ?_Ufill@?$vector@UElem_00475880@@V?$allocator@UElem_00475880@@@std@@@std@@IAEXPAUElem_00475880@@IABU3@@Z
UfillFn_00476710 Access_00476710::fn = &Access_00476710::_Ufill;
