// Decompiled by space-bunny-free, deepseek-v4.1-flash and GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, verified by GPT-6. Names are provisional.
// FLAGS: /Gi
// std::vector<int>::insert(iterator, size_type, const _Ty&), stock MSVC 5
// <vector>, emitted out of line through a member pointer. The caller
// (0x46d6c0) appends one int at a time with insert(end(), 1, x).
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
