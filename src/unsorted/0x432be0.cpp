// Decompiled by Opus. Names are provisional.
// std::vector<T>::size() from MSVC 5's <vector>, out of line, for a vector of
// 4-byte string handles (its destructor is 0x432ba0). Taking the member's
// address makes the compiler emit the template instantiation out of line.
#include <vector>

struct Elem_00432be0 {
    char* data;                        // +0x0
};

typedef std::vector<Elem_00432be0> Vec_00432be0;
typedef Vec_00432be0::size_type (Vec_00432be0::*SizeFn_00432be0)() const;

// FUNCTION: 0x432be0 ?size@?$vector@UElem_00432be0@@V?$allocator@UElem_00432be0@@@std@@@std@@QBEIXZ
SizeFn_00432be0 g_size_00432be0 = &Vec_00432be0::size;
