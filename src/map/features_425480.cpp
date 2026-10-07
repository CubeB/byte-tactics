// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, verified by GPT-6.1-sol, finished by space-bunny-free, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<TdfFile*>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector>, emitted out of line through a member pointer. The caller
// is 0x4222e0 (push_back on the global feature vector).
#include <vector>

#include "../util/tdf.h"

typedef std::vector<TdfFile*> Vec_00425480;
typedef void (Vec_00425480::*InsertFn_00425480)(
    Vec_00425480::iterator, Vec_00425480::size_type, TdfFile* const&);

// An assignment on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate operator=.
void __stdcall Assign_00425480(Vec_00425480* a, const Vec_00425480* b)
{
    *a = *b;
}

// FUNCTION: 0x425480 ?insert@?$vector@PAVTdfFile@@V?$allocator@PAVTdfFile@@@std@@@std@@QAEXPAPAVTdfFile@@IABQAV3@@Z
InsertFn_00425480 g_insert_00425480 = &Vec_00425480::insert;
