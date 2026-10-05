// Decompiled by Opus. Names are provisional.
// std::vector<Point_0044eec0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage. Its callers (around
// 0x44d190) set ecx to the vector; its _Ucopy is 0x44ee90 and its copy is
// 0x44eef0 (4-byte elements, two shorts). _Ufill is protected, so a derived
// class takes its address to make the compiler emit it out of line.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044eec0;
typedef void (Vec_0044eec0::*UfillFn_0044eec0)(
    Vec_0044eec0::iterator, Vec_0044eec0::size_type, const Point_0044eec0&);

struct Access_0044eec0 : Vec_0044eec0 {
    static UfillFn_0044eec0 fn;
};

// FUNCTION: 0x44eec0 ?_Ufill@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@IABU3@@Z
UfillFn_0044eec0 Access_0044eec0::fn = &Access_0044eec0::_Ufill;
