// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<Elem_0043c390>::insert(iterator, size_type, const _Ty&) of
// MSVC 5's <vector>, the three-argument insert, emitted out of line: the two
// callers (0x43bc90 and 0x43c050) insert one 25-byte record at a time into the
// global table at 0x512340. It calls the global operator new (0x4b4f10) and
// delete (0x4b4f20). The element is the same 25-byte record as the one
// _Destroy (0x43c390) destroys.
#include <vector>

struct Elem_0043c390 {
    char data[0x19];

    // Only so the explicit instantiation of the rest of the vector compiles.
    int operator<(const Elem_0043c390&) const { return 0; }
    int operator==(const Elem_0043c390&) const { return 0; }
    int operator!=(const Elem_0043c390&) const { return 0; }
};

typedef std::vector<Elem_0043c390> Vec_0043c3a0;

// Explicit instantiation: the member is emitted itself, not inlined into a wrapper.
template class std::vector<Elem_0043c390>;

// FUNCTION: 0x43c3a0 ?insert@?$vector@UElem_0043c390@@V?$allocator@UElem_0043c390@@@std@@@std@@QAEXPAUElem_0043c390@@IABU3@@Z
