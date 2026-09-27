// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00475880>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies. _Ucopy is protected, so a derived class takes its
// address to make the compiler emit it out of line.
// A 48-byte element type (its layout is a guess). Its caller 0x473d50
// inlines vector::insert and calls 0x476710 (_Ufill), 0x475880 (_Ucopy)
// and 0x475870 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00475880 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Elem_00475880> Vec_00475880;
typedef Vec_00475880::iterator (Vec_00475880::*UcopyFn_00475880)(
    Vec_00475880::const_iterator, Vec_00475880::const_iterator, Vec_00475880::iterator);

struct Access_00475880 : Vec_00475880 {
    static UcopyFn_00475880 fn;
};

// FUNCTION: 0x475880 ?_Ucopy@?$vector@UElem_00475880@@V?$allocator@UElem_00475880@@@std@@@std@@IAEPAUElem_00475880@@PBU3@0PAU3@@Z
UcopyFn_00475880 Access_00475880::fn = &Access_00475880::_Ucopy;
