// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::vector<T>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, fill and copy_backward all inlined. The
// element type is 0x44 (68) bytes; the copies are rep movsd of 0x11 dwords.
#include <vector>

struct Element_00475ef0 {
    int data[0x11];
};

typedef std::vector<Element_00475ef0> Vec_00475ef0;
typedef void (Vec_00475ef0::*InsertFn_00475ef0)(
    Vec_00475ef0::iterator, Vec_00475ef0::size_type, Element_00475ef0 const&);

// FUNCTION: 0x475ef0 ?insert@?$vector@UElement_00475ef0@@V?$allocator@UElement_00475ef0@@@std@@@std@@QAEXPAUElement_00475ef0@@IABU3@@Z
InsertFn_00475ef0 g_insert_00475ef0 = &Vec_00475ef0::insert;
