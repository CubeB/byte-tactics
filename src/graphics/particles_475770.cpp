// Decompiled by space-bunny-free. Names are provisional.
// std::vector<Class_004739b0>::reserve from MSVC 5's <vector>: the 48-byte
// element type is the same one as in 0x475880 (_Ucopy), 0x475870 (_Destroy)
// and 0x476710 (_Ufill), all siblings in this container's family.
// Its caller 0x473d50 inlines vector::insert and calls this reserve (and
// vector::size, out of line at 0x475840) with ecx set to the vector.
#include <vector>

struct Class_004739b0 {
    int dwords[12];                    // 0x30 bytes
};

typedef std::vector<Class_004739b0> Vec_00475770;
typedef void (Vec_00475770::*ReserveFn_00475770)(Vec_00475770::size_type);

struct Access_00475770 : Vec_00475770 {
    static ReserveFn_00475770 fn;
};

// FUNCTION: 0x475770 ?reserve@?$vector@UClass_004739b0@@V?$allocator@UClass_004739b0@@@std@@@std@@QAEXI@Z
ReserveFn_00475770 Access_00475770::fn = &Access_00475770::reserve;
