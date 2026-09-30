// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are
// provisional.
// Best verified retry: 89.6% (546/547 bytes); no MATCH. The remaining mismatch
// is the third _Ucopy source-pointer expression after vector reallocation.
// A hand-written explicit insert specialization scored 86.3% and added a
// compare in that loop, so the standard-library instantiation remains best.
// std::vector<Unit*>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, fill and copy_backward all inlined. Its
// callers are push_back sites (0x40ab36 on the vector at +0x5 of the
// 0x409160 object, and 0x407786 on a local vector). Taking the member's
// address makes the compiler emit the template instantiation out of line.
// The element type is settled by 0x40ad80, which calls this, _Ucopy, _Ufill
// and size on one vector of units (#135; see 0x406c00.cpp).
//
// Still differs (89.6%, 546 vs 547 bytes): in the third _Ucopy (_P into
// _Q + _M) the source start differs. The original computes it as
// `lea eax, [ebx + ecx]` then `sub eax, edx` / `sub eax, edi`, that is
// (P + dest) - Q - M; ours computes (dest - Q) + P - M as
// `mov eax, ecx` / `sub eax, edx` / `add eax, ebx` / `sub eax, edi`. Every
// other instruction and register is identical. This is the same third-_Ucopy
// source sum that 0x40d290's note says flips with compiler state, but here it
// is stuck: all 128 tools/headers.py sets, plain <vector> with 0 to 304 extra
// dummy declarations, <windows.h> plus <ddraw.h> and/or <math.h>, extra
// out-of-line member instantiations (size, begin, push_back) and an explicit
// `template class std::vector<Unit*>;` all give the `mov`/`add` form. A
// hand-written explicit member specialisation whose third _Ucopy is written
// as a loop (guide 1833) was not tried; compiler state from the rest of the
// original translation unit is the likely cause.
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00408f30;
typedef void (Vec_00408f30::*InsertFn_00408f30)(
    Vec_00408f30::iterator, Vec_00408f30::size_type, Unit* const&);

// FUNCTION: 0x408f30 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEXPAPAUUnit@@IABQAU3@@Z
InsertFn_00408f30 g_insert_00408f30 = &Vec_00408f30::insert;
