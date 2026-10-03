// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by mimo-v2.6-pro, retried by space-bunny-free, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Elem_0040cfb0>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on a 3-byte element, emitted out of line through a member
// pointer. Callers 0x409160 and 0x409730 call it.
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector members the TU instantiates: with a
// reserve use (as below) or a copy constructor this MATCHes
// (docs/field-notes.md Part 7). Without /Gi the best file reached 99.5%: the
// library's `mov / sub / add / sub` source-start derivation against the
// original's `lea eax,[ebp+ecx] / sub / sub`, which Part 6 took for a
// different compiler build. The earlier passes are in git history.
#include <vector>

struct Elem_0040cfb0 { char a, b, c; };
typedef std::vector<Elem_0040cfb0> Vec;
typedef void (Vec::*Insert)(Vec::iterator, unsigned int, const Elem_0040cfb0&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or the copy
// constructor).
void __stdcall Grow_0040cca0(Vec* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
Insert insert_0040cca0 = &Vec::insert;
