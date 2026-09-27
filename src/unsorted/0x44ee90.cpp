// Decompiled by Opus. Names are provisional.
// std::vector<Point_0044eec0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copy-constructs [first, last) into raw storage at dest and
// returns the end of the copies. Its callers (around 0x44d190) set ecx to the
// vector whose _Ufill is 0x44eec0. _Ucopy is protected, so a derived class
// takes its address to make the compiler emit it out of line.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044ee90;
typedef Vec_0044ee90::iterator (Vec_0044ee90::*UcopyFn_0044ee90)(
    Vec_0044ee90::const_iterator, Vec_0044ee90::const_iterator, Vec_0044ee90::iterator);

struct Access_0044ee90 : Vec_0044ee90 {
    static UcopyFn_0044ee90 fn;
};

// FUNCTION: 0x44ee90 ?_Ucopy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEPAUPoint_0044eec0@@PBU3@0PAU3@@Z
UcopyFn_0044ee90 Access_0044ee90::fn = &Access_0044ee90::_Ucopy;
