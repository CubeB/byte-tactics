// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0043c390>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its callers (0x43bc90, 0x43c050)
// inline vector::reserve on the global vector of 25-byte records at
// 0x512340 and call this with ecx set to it.
#include <vector>

#pragma pack(push, 1)
struct Elem_0043c390 {
    char data[0x19];
};
#pragma pack(pop)

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef void (Vec_0043c390::*DestroyFn_0043c390)(Vec_0043c390::iterator, Vec_0043c390::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_0043c390 : Vec_0043c390 {
    static DestroyFn_0043c390 fn;
};

// FUNCTION: 0x43c390 ?_Destroy@?$vector@UElem_0043c390@@V?$allocator@UElem_0043c390@@@std@@@std@@IAEXPAUElem_0043c390@@0@Z
DestroyFn_0043c390 Access_0043c390::fn = &Access_0043c390::_Destroy;
