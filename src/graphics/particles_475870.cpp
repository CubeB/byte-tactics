// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00475880>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// A 48-byte element type (its layout is a guess). Its caller 0x473d50
// inlines vector::insert and calls 0x476710 (_Ufill), 0x475880 (_Ucopy)
// and 0x475870 (_Destroy) with ecx set to the vector.
#include <vector>

struct Elem_00475880 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Elem_00475880> Vec_00475870;
typedef void (Vec_00475870::*DestroyFn_00475870)(Vec_00475870::iterator, Vec_00475870::iterator);

struct Access_00475870 : Vec_00475870 {
    static DestroyFn_00475870 fn;
};

// FUNCTION: 0x475870 ?_Destroy@?$vector@UElem_00475880@@V?$allocator@UElem_00475880@@@std@@@std@@IAEXPAUElem_00475880@@0@Z
DestroyFn_00475870 Access_00475870::fn = &Access_00475870::_Destroy;
