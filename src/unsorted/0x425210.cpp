// Decompiled by Space Bunny Free, finished by LongCat 2.5 Preview Free, verified by GPT-6.1-sol, sixth pass by space-bunny-free, edited by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<unsigned short>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector>, emitted out of line through a member pointer. The caller
// is 0x424c00.
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector<unsigned short> members the TU
// instantiates: with a reserve use (as below), a resize or a copy constructor
// this MATCHes (docs/field-notes.md Part 7). Without /Gi the best file, a hand
// copy of the <vector> class, reached 99.6%: one SIB byte at 0x4252d1
// (`lea eax,[ecx+ebx]` against the original's `lea eax,[ebx+ecx]`), which
// Part 6 took for a different compiler build. The earlier passes are in git
// history.
#include <vector>

typedef std::vector<unsigned short> Vec_00425210;
typedef void (Vec_00425210::*InsertFn_00425210)(
    Vec_00425210::iterator, Vec_00425210::size_type, const unsigned short&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or resize, or the
// copy constructor).
void __stdcall Grow_00425210(Vec_00425210* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x425210 ?insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z
InsertFn_00425210 g_insert_00425210 = &Vec_00425210::insert;
