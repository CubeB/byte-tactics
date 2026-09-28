// Decompiled by mimo-v2.6-flash. Names are provisional.
// std::vector<Unit*>::insert(iterator _P, const T& _X) from MSVC 5's <vector>
// (the two-argument overload, VECTOR line 146): remembers the offset, calls
// insert(_P, 1, _X) (inlined here), then returns begin() + offset. This is the
// overload the push_back sites inline; taking its address emits it out of
// line. Element type is Unit* (0x406c00.cpp).
//
// The original's file was compiled with __stdcall as the default, so its
// std::_Construct instantiation (0x406c70, ret 8) pops its own arguments; a
// non-template overload forwarding to it reproduces that (0x405d90.cpp).
struct Unit;
void __stdcall FUN_00406c70(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { FUN_00406c70(dest, &src); }
}
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_0048ddc0;
typedef Vec_0048ddc0::iterator (Vec_0048ddc0::*InsertFn_0048ddc0)(
    Vec_0048ddc0::iterator, Unit* const&);

// FUNCTION: 0x48ddc0 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEPAPAUUnit@@PAPAU3@ABQAU3@@Z
InsertFn_0048ddc0 g_insert_0048ddc0 = &Vec_0048ddc0::insert;
