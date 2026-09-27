// Decompiled by Opus. Names are provisional.
// std::allocator<std::vector<Elem_00434020> >::destroy(pointer), out of line:
// the inlined ~vector frees _First and zeroes the three pointers. Its caller
// (0x4330b0, the destroy loop of a vector of these vectors) sets ecx to the
// allocator before each call, so this is the allocator member, not
// std::_Destroy (0x434440 is the byte-identical _Destroy, called without ecx).
// The element type is the one 0x434770 (operator= of the middle vector)
// copies with 0x4345e0, vector<Elem_00434020>::operator=. Taking the member's
// address makes the compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00434400;
typedef std::allocator<Inner_00434400> Alloc_00434400;
typedef void (Alloc_00434400::*DestroyFn_00434400)(Inner_00434400*);

// FUNCTION: 0x434400 ?destroy@?$allocator@V?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@@std@@QAEXPAV?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@2@@Z
DestroyFn_00434400 g_destroy_00434400 = &Alloc_00434400::destroy;
