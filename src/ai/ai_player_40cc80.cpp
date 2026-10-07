// Decompiled by Sonnet. Names are provisional.
// std::vector<Elem_0040cfb0>::size() for the 3-byte element type (the `/ 3`
// is the pointer difference). 0x409160 calls it with ecx set to its vector at
// +0x65, from the inlined resize() around the insert (0x40cca0) and erase
// (0x40cfb0).
#include <vector>

struct Elem_0040cfb0 {
    char a;                            // +0x0
    char b;                            // +0x1
    char c;                            // +0x2
};

typedef std::vector<Elem_0040cfb0> Vec_0040cc80;
typedef Vec_0040cc80::size_type (Vec_0040cc80::*SizeFn_0040cc80)() const;

// FUNCTION: 0x40cc80 ?size@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QBEIXZ
SizeFn_0040cc80 g_size_0040cc80 = &Vec_0040cc80::size;
