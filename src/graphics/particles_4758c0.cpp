// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, verified by GPT-6. Names are provisional.
// Issue #4931 retry: check.py MATCH on the current main translation-unit setup.
// FLAGS: /Gi
// std::vector<Class_00473590>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on a 0x34-byte record, emitted out of line through a member
// pointer. The caller (0x4737c0) grows the vector with reserve and fills it
// with insert.
//
// The original's translation unit was built with /Gi (#5035), and the bytes
// also depend on which other vector members the TU instantiates: with an
// operator= use (or resize) this MATCHes; with only reserve or a copy
// constructor it is 84.3%. Without /Gi the best file reached 88.9%, stuck on
// _P's register in the realloc arm (the earlier passes, in git history,
// measured that wall in detail).
#include <vector>

struct Class_00473590 {
    void* field_0;
    int f04, f08, f0c, f10, f14, f18, f1c, f20, f24, f28, f2c, f30;
};

typedef std::vector<Class_00473590> Vec_00473590;
typedef void (Vec_00473590::*InsertFn_00473590)(
    Vec_00473590::iterator, Vec_00473590::size_type, const Class_00473590&);

// A use of operator= on this vector type, standing in for the original TU's
// own; the insert's bytes need the TU to instantiate it.
void __stdcall Assign_00473590(Vec_00473590* to, const Vec_00473590* from)
{
    *to = *from;
}

// FUNCTION: 0x4758c0 ?insert@?$vector@UClass_00473590@@V?$allocator@UClass_00473590@@@std@@@std@@QAEXPAUClass_00473590@@IABU3@@Z
InsertFn_00473590 g_insert_00473590 = &Vec_00473590::insert;
