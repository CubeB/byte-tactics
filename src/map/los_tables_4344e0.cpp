// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<Elem_00434360>::vector(const vector&), the copy constructor
// from MSVC 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: copy the empty allocator byte, allocate size()
// elements and copy them with the placement-new construct. The held
// vector<Elem_00434020> copy constructor copies every element through the
// out-of-line allocator<Elem_00434020>::construct (0x4345c0).
// The only caller is the outer vector<vector<Elem_00434360>>::insert
// (0x4340f0), one level above 0x434470's caller.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

void __stdcall FUN_00434430(int);
void __stdcall FUN_004345c0(int* p, const int* q);

namespace std {
// Non-trivial body forces the out-of-line construct call (0x4345c0).
inline void _Construct(Elem_00434020* p, const Elem_00434020& v)
{
    FUN_004345c0((int*)p, (const int*)&v);
}

inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434020> Inner_004344e0;
typedef std::vector<Elem_00434360> Middle_004344e0;
typedef std::vector<Middle_004344e0> Outer_004344e0;
typedef void (Outer_004344e0::*InsertFn_004344e0)(
    Outer_004344e0::iterator, Outer_004344e0::size_type, const Middle_004344e0&);

// Taking insert's address emits this copy constructor out of line.
// FUNCTION: 0x4344e0 ??0?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAE@ABV01@@Z
InsertFn_004344e0 g_insert_004344e0 = &Outer_004344e0::insert;
