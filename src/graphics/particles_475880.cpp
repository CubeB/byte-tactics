// Decompiled by Opus. Names are provisional.
// std::vector<Class_004739b0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copies [first, last) into raw storage at dest and returns the
// end of the copies.
// A 48-byte element type (its layout is a guess). Its caller 0x473d50
// inlines vector::insert and calls 0x476710 (_Ufill), 0x475880 (_Ucopy)
// and 0x475870 (_Destroy) with ecx set to the vector.
#include <vector>

struct Class_004739b0 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Class_004739b0> Vec_00475880;
typedef Vec_00475880::iterator (Vec_00475880::*UcopyFn_00475880)(
    Vec_00475880::const_iterator, Vec_00475880::const_iterator, Vec_00475880::iterator);

struct Access_00475880 : Vec_00475880 {
    static UcopyFn_00475880 fn;
};

// FUNCTION: 0x475880 ?_Ucopy@?$vector@UClass_004739b0@@V?$allocator@UClass_004739b0@@@std@@@std@@IAEPAUClass_004739b0@@PBU3@0PAU3@@Z
UcopyFn_00475880 Access_00475880::fn = &Access_00475880::_Ucopy;
