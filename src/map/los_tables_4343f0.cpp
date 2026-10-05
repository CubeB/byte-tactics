// Decompiled by Haiku. Names are provisional.
// std::allocator<Elem_00434020>::destroy(pointer), out of line: empty, since
// the element type is trivial. Its only caller (0x433540) sets ecx to the
// allocator first, so this is the allocator member, like 0x434400 one level
// up. Taking the member's address makes the compiler emit it.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::allocator<Elem_00434020> Alloc_004343f0;
typedef void (Alloc_004343f0::*DestroyFn_004343f0)(Elem_00434020*);

// FUNCTION: 0x4343f0 ?destroy@?$allocator@UElem_00434020@@@std@@QAEXPAUElem_00434020@@@Z
DestroyFn_004343f0 g_destroy_004343f0 = &Alloc_004343f0::destroy;
