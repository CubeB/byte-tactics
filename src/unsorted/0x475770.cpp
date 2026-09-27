// Decompiled by space-bunny-free. Names are provisional.
// std::vector<Elem_00475880>::reserve from MSVC 5's <vector>: the 48-byte
// element type is the same one as in 0x475880 (_Ucopy), 0x475870 (_Destroy)
// and 0x476710 (_Ufill), all siblings in this container's family. The
// pointer differences in the header (`_End - _First`, `_Last - _First`) are
// what compile to the signed division by 0x30, and `allocator::allocate` is
// MSVC 5's `_Allocate` with its `if (_N < 0) _N = 0;` clamp, so nothing here
// is hand-written. The store of _First into the dead parameter slot is
// `allocator<Elem>::deallocate`'s inlined first parameter; see 0x475110.cpp
// for the same store from the inlined ~vector.
// Its caller 0x473d50 inlines vector::insert and calls this reserve (and
// vector::size, out of line at 0x475840) with ecx set to the vector.
#include <vector>

struct Elem_00475880 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Elem_00475880> Vec_00475770;
typedef void (Vec_00475770::*ReserveFn_00475770)(Vec_00475770::size_type);

struct Access_00475770 : Vec_00475770 {
    static ReserveFn_00475770 fn;
};

// FUNCTION: 0x475770 ?reserve@?$vector@UElem_00475880@@V?$allocator@UElem_00475880@@@std@@@std@@QAEXI@Z
ReserveFn_00475770 Access_00475770::fn = &Access_00475770::reserve;
