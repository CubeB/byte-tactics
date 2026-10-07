// Decompiled by Opus. Names are provisional.
// std::allocator<Elem_00434360>::destroy(pointer), out of line: the element's
// implicit destructor is ~vector<Elem_00434020>, which frees _First and
// zeroes the three pointers. Its caller (0x4330b0, the destroy loop of a
// vector of vectors of these elements) sets ecx to the allocator before each
// call, so this is the allocator member, not std::_Destroy. The element type is
// the one 0x434770 (operator= of the middle vector) copies with 0x4345e0,
// vector<Elem_00434020>::operator=.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::allocator<Elem_00434360> Alloc_00434400;
typedef void (Alloc_00434400::*DestroyFn_00434400)(Elem_00434360*);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x434400 ?destroy@?$allocator@UElem_00434360@@@std@@QAEXPAUElem_00434360@@@Z
DestroyFn_00434400 g_destroy_00434400 = &Alloc_00434400::destroy;
