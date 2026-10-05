// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by
// GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Unit*>::insert(iterator, size_type, const _Ty&), stock MSVC 5
// <vector>, emitted out of line through a member pointer. Callers 0x407560,
// 0x40aa40 and 0x40ad80 call it.
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector<Unit*> members the TU instantiates: with a
// reserve use (as below), a resize or a copy constructor this MATCHes
// (docs/field-notes.md Part 7). Without /Gi the best file, a hand copy of the
// <vector> class, reached 99.6%: one SIB byte in the `lea` that starts the
// third _Ucopy, which Part 6 took for a different compiler build. The earlier
// passes are in git history.
#include <vector>

struct Unit {
    int unknown_0;
};

typedef std::vector<Unit*> Vec_00408f30;
typedef void (Vec_00408f30::*InsertFn_00408f30)(
    Vec_00408f30::iterator, Vec_00408f30::size_type, Unit* const&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or resize, or the
// copy constructor).
void __stdcall Grow_00408f30(Vec_00408f30* units, int extra)
{
    units->reserve(extra + units->size());
}

// FUNCTION: 0x408f30 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEXPAPAUUnit@@IABQAU3@@Z
InsertFn_00408f30 g_insert_00408f30 = &Vec_00408f30::insert;
