// Decompiled by space-bunny-free, deepseek-v4.1-flash and GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<int>::insert(iterator, size_type, const _Ty&), stock MSVC 5
// <vector>, emitted out of line through a member pointer. The caller
// (0x46d6c0) appends one int at a time with insert(end(), 1, x).
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector<int> members the TU instantiates: with a
// reserve use (as below), a resize or a copy constructor this MATCHes; with
// operator= it is 58.0%. Without /Gi the best file reached 99.6%, one swapped
// SIB byte at 0x46e708 (`lea eax,[ebx+ecx]` against ours `lea eax,[ecx+ebx]`)
// that docs/field-notes.md Part 6 took for a different compiler build. It is
// not: it is /Gi plus the TU's other member uses (Part 7 there).
#include <vector>

typedef std::vector<int> Vec_0046e640;
typedef void (Vec_0046e640::*InsertFn_0046e640)(
    Vec_0046e640::iterator, Vec_0046e640::size_type, int const&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or resize, or the
// copy constructor).
void __stdcall Grow_0046e640(Vec_0046e640* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x46e640 ?insert@?$vector@HV?$allocator@H@std@@@std@@QAEXPAHIABH@Z
InsertFn_0046e640 g_insert_0046e640 = &Vec_0046e640::insert;
