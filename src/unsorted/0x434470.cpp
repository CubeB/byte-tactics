// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434020>::vector(const vector&), the copy constructor
// from MSVC 5's <vector>: allocate size() elements and copy them with the
// placement-new construct. The original calls it out of line from the outer
// vector's insert (0x433db0), which does not inline it; taking insert's
// address makes the compiler emit both, as in the original file.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

void __stdcall FUN_00434430(int);

namespace std {
inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

typedef std::vector<Elem_00434020> Inner_00434470;
typedef std::vector<Inner_00434470> Outer_00434470;
typedef void (Outer_00434470::*InsertFn_00434470)(
    Outer_00434470::iterator, Outer_00434470::size_type, const Inner_00434470&);

// FUNCTION: 0x434470 ??0?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@ABV01@@Z
InsertFn_00434470 g_insert_00434470 = &Outer_00434470::insert;
