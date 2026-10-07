// Decompiled by deepseek-v4.1-flash, finished by Claude Opus 5.5, verified by GPT-6. Names are provisional.
// FLAGS: /Gi
// std::vector<Point_0044eec0>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on the 4-byte point (two shorts) that the only caller,
// 0x44da00, builds and appends with insert(end(), 1, p).
#include <vector>

// The element as 0x44da00.cpp declares it, so the insert has the name that
// caller already uses.
struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044ec30;
typedef void (Vec_0044ec30::*InsertFn_0044ec30)(
    Vec_0044ec30::iterator, Vec_0044ec30::size_type, const Point_0044eec0&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or the copy
// constructor).
void __stdcall Grow_0044ec30(Vec_0044ec30* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x44ec30 ?insert@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@QAEXPAUPoint_0044eec0@@IABU3@@Z
// Taking the member's address makes the compiler emit it out of line.
InsertFn_0044ec30 g_insert_0044ec30 = &Vec_0044ec30::insert;
