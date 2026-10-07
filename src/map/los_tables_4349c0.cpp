// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::_Ucopy(first, last, dest) from MSVC 5's
// <vector> for a 4-byte element type.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Vec_004349c0;
typedef Vec_004349c0::iterator (Vec_004349c0::*UcopyFn_004349c0)(
    Vec_004349c0::const_iterator, Vec_004349c0::const_iterator, Vec_004349c0::iterator);

// _Ucopy is protected: a derived class takes its address to emit it out of line.
struct Access_004349c0 : Vec_004349c0 {
    static UcopyFn_004349c0 fn;
};

// FUNCTION: 0x4349c0 ?_Ucopy@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@IAEPAUElem_00434020@@PBU3@0PAU3@@Z
UcopyFn_004349c0 Access_004349c0::fn = &Access_004349c0::_Ucopy;
