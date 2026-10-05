// Decompiled by Opus. Names are provisional.
// std::vector<Point_0044eec0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// The same vector as 0x44ee90 (_Ucopy) and 0x44eec0 (_Ufill): 0x44d0e0,
// 0x44d560 and 0x44da00 call 0x44ee90 and then this with ecx set to it.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044ee60;
typedef void (Vec_0044ee60::*DestroyFn_0044ee60)(Vec_0044ee60::iterator, Vec_0044ee60::iterator);

struct Access_0044ee60 : Vec_0044ee60 {
    static DestroyFn_0044ee60 fn;
};

// FUNCTION: 0x44ee60 ?_Destroy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@0@Z
DestroyFn_0044ee60 Access_0044ee60::fn = &Access_0044ee60::_Destroy;
