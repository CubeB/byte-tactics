// Decompiled by Opus. Names are provisional.
// std::vector<std::vector<Elem_00434020> >::~vector() from MSVC 5's <vector>,
// out of line (the same class as the erase at 0x434020): each inner vector's
// elements go through the empty out-of-line FUN_00434430 (std::_Destroy
// overload below), then its _First is freed and its three pointers zeroed;
// finally the outer _First is freed and zeroed. Its callers (0x433270,
// 0x4340f0) are the destroy loops of a vector of these vectors, next to calls
// to 0x434770 (this class's operator=), where the inliner stops; taking the
// address of the next level's operator= makes the compiler emit this
// destructor out of line the same way.
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

typedef std::vector<Elem_00434020> Inner_00433a80;
typedef std::vector<Inner_00433a80> Middle_00433a80;
typedef std::vector<Middle_00433a80> Outer_00433a80;
typedef Outer_00433a80& (Outer_00433a80::*AssignFn_00433a80)(const Outer_00433a80&);

AssignFn_00433a80 g_assign_00433a80 = &Outer_00433a80::operator=;

// FUNCTION: 0x433a80 ??1?$vector@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@V?$allocator@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@@2@@std@@QAE@XZ
