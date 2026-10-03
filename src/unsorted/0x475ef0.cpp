// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Element_00475ef0>::insert(iterator, size_type, const _Ty&),
// stock MSVC 5 <vector>, emitted out of line through a member pointer. The
// caller (0x474880) grows the vector with reserve and fills it with insert.
//
// The original's translation unit was built with /Gi (#5035): /Gi numbers
// internal symbols per function instead of TU-wide, which flips the ties that
// put _P in edx and _N/_S in the original's frame slots. The bytes also depend
// on which other vector members the TU instantiates: with an operator= use (or
// resize) this MATCHes; with only reserve, a copy constructor, erase or no
// other use it is 83.0%. Without /Gi no spelling got past 83.0% (the earlier
// passes, in git history, measured that wall in detail).
#include <vector>

struct Element_00475ef0 {
    int data[0x11];
};

typedef std::vector<Element_00475ef0> Vec_00475ef0;
typedef void (Vec_00475ef0::*InsertFn_00475ef0)(
    Vec_00475ef0::iterator, Vec_00475ef0::size_type, Element_00475ef0 const&);

// A use of operator= on this vector type, standing in for the original TU's
// own; the insert's bytes need the TU to instantiate it.
void __stdcall Assign_00475ef0(Vec_00475ef0* to, const Vec_00475ef0* from)
{
    *to = *from;
}

// FUNCTION: 0x475ef0 ?insert@?$vector@UElement_00475ef0@@V?$allocator@UElement_00475ef0@@@std@@@std@@QAEXPAUElement_00475ef0@@IABU3@@Z
InsertFn_00475ef0 g_insert_00475ef0 = &Vec_00475ef0::insert;
