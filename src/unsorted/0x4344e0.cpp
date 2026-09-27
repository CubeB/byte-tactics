// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<std::vector<Elem_00434020> >::vector(const vector&), the copy
// constructor from MSVC 5's <vector>: copy the empty allocator byte, allocate
// size() elements and copy them with the placement-new construct. The inner
// vector<Elem_00434020> copy constructor is inlined here, but its
// allocator::construct is not, so every element goes through the out-of-line
// allocator<Elem_00434020>::construct (0x4345c0). That call is forced by
// giving std::_Construct a non-trivial body that calls 0x4345c0, as 0x405d90
// does for its element, which also reproduces the original register spills.
// The only caller is the outer vector<vector<vector<Elem_00434020>>>::insert
// (0x4340f0), one level above 0x434470's caller; taking insert's address makes
// the compiler emit this copy constructor out of line, as in the original.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

void __stdcall FUN_00434430(int);
void __stdcall FUN_004345c0(int* p, const int* q);

namespace std {
inline void _Construct(Elem_00434020* p, const Elem_00434020& v)
{
    FUN_004345c0((int*)p, (const int*)&v);
}

inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

typedef std::vector<Elem_00434020> Inner_004344e0;
typedef std::vector<Inner_004344e0> Middle_004344e0;
typedef std::vector<Middle_004344e0> Outer_004344e0;
typedef void (Outer_004344e0::*InsertFn_004344e0)(
    Outer_004344e0::iterator, Outer_004344e0::size_type, const Middle_004344e0&);

// FUNCTION: 0x4344e0 ??0?$vector@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@V?$allocator@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@@2@@std@@QAE@ABV01@@Z
InsertFn_004344e0 g_insert_004344e0 = &Outer_004344e0::insert;
