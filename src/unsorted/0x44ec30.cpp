// Decompiled by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Point_0044eec0>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on the 4-byte point (two shorts) that the only caller,
// 0x44da00, builds and appends with insert(end(), 1, p). Taking the member's
// address makes the compiler emit it out of line.
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector members the TU instantiates: with a
// reserve use (as below) or a copy constructor this MATCHes
// (docs/field-notes.md Part 7). Without /Gi the best file, a hand copy of the
// <vector> class, reached 99.6%: one SIB byte at 0x44ecf8 (`lea eax,[ecx+ebx]`
// against the original's `lea eax,[ebx+ecx]`), which Part 6 took for a
// different compiler build. The earlier passes are in git history.
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
InsertFn_0044ec30 g_insert_0044ec30 = &Vec_0044ec30::insert;
