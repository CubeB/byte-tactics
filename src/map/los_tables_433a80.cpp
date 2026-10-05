// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00434360>::~vector() from MSVC 5's <vector>, out of line
// (the same class as the erase at 0x434020), where Elem_00434360 is a struct
// holding one std::vector<Elem_00434020>: each element's held vector has its
// elements go through the empty out-of-line FUN_00434430 (std::_Destroy
// overload below), then its _First is freed and its three pointers zeroed;
// finally the outer _First is freed and zeroed. The std::_Destroy overload for
// Elem_00434360 runs the held vector's destructor directly, one inline level
// shallower than the implicit destructor, as in 0x434020.cpp. Its callers
// (0x433270, 0x4340f0) are the destroy loops of a vector of these vectors,
// next to calls to 0x434770 (this class's operator=), where the inliner
// stops; taking the address of the next level's operator= makes the compiler
// emit this destructor out of line the same way.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

void __stdcall FUN_00434430(int);

namespace std {
inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}
}

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434020> Inner_00433a80;

namespace std {
inline void _Destroy(Elem_00434360* p)
{
    p->v.~Inner_00433a80();
}
}

typedef std::vector<Elem_00434360> Middle_00433a80;
typedef std::vector<Middle_00433a80> Outer_00433a80;
typedef Outer_00433a80& (Outer_00433a80::*AssignFn_00433a80)(const Outer_00433a80&);

AssignFn_00433a80 g_assign_00433a80 = &Outer_00433a80::operator=;

// FUNCTION: 0x433a80 ??1?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAE@XZ
