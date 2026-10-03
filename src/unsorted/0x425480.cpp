// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, verified by GPT-6.1-sol, finished by space-bunny-free, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Class_004c2ea0*>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector>, emitted out of line through a member pointer. The caller
// is 0x4222e0 (push_back on the global feature vector).
//
// The original's translation unit was built with /Gi, and the bytes also
// depend on which other members of this vector type the TU instantiates: with
// an operator= use (as below) this MATCHes; with reserve, resize, the copy
// constructor or no other use it is 58.0% (docs/field-notes.md Part 7). The
// best file without /Gi, a hand copy of the <vector> class with a renumbered
// third copy, reached 84.1%; those passes are in git history.
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
};

typedef std::vector<Class_004c2ea0*> Vec_00425480;
typedef void (Vec_00425480::*InsertFn_00425480)(
    Vec_00425480::iterator, Vec_00425480::size_type, Class_004c2ea0* const&);

// An assignment on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate operator=.
void __stdcall Assign_00425480(Vec_00425480* a, const Vec_00425480* b)
{
    *a = *b;
}

// FUNCTION: 0x425480 ?insert@?$vector@PAVClass_004c2ea0@@V?$allocator@PAVClass_004c2ea0@@@std@@@std@@QAEXPAPAVClass_004c2ea0@@IABQAV3@@Z
InsertFn_00425480 g_insert_00425480 = &Vec_00425480::insert;
