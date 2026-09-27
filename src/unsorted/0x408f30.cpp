// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<Unit*>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, fill and copy_backward all inlined. Its
// callers are push_back sites (0x40ab36 on the vector at +0x5 of the
// 0x409160 object, and 0x407786 on a local vector). Taking the member's
// address makes the compiler emit the template instantiation out of line.
// The element type is a guess: any 4-byte type compiles to the same code.
//
// Still differs (88.7%): in the third _Ucopy (_P, _Last into _Q + _M) the
// original starts the source pointer with `lea eax, [ebx + ecx]` (P + dest,
// then - Q - M*4); ours computes `mov eax, ecx; ... add eax, ebx`
// (dest - Q + P - M*4). Everything else is identical. No header set, element
// type, earlier instantiation order or preceding function changed it; it is
// probably compiler state from the rest of the original file, whose
// end-of-file template instantiations this function belongs to.
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00408f30;
typedef void (Vec_00408f30::*InsertFn_00408f30)(
    Vec_00408f30::iterator, Vec_00408f30::size_type, Unit* const&);

// FUNCTION: 0x408f30 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEXPAPAUUnit@@IABQAU3@@Z
InsertFn_00408f30 g_insert_00408f30 = &Vec_00408f30::insert;
