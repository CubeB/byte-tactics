// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040cc40>::_Destroy(first, last): empty, since the
// element's destructor is trivial.
// The element is a map cell and its sort key, as 0x40a7b0 and 0x40a260 use
// it (its copy constructor is 0x40a5b0). Its callers (0x40a7b0, 0x40ca50)
// inline vector::insert and call 0x40d5b0 (_Ufill), 0x40cc40 (_Ucopy) and
// 0x40cc30 (_Destroy) with ecx set to the vector.
#include <vector>

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

typedef std::vector<Elem_0040cc40> Vec_0040cc30;
typedef void (Vec_0040cc30::*DestroyFn_0040cc30)(Vec_0040cc30::iterator, Vec_0040cc30::iterator);

struct Access_0040cc30 : Vec_0040cc30 {
    static DestroyFn_0040cc30 fn;
};

// FUNCTION: 0x40cc30 ?_Destroy@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEXPAUElem_0040cc40@@0@Z
DestroyFn_0040cc30 Access_0040cc30::fn = &Access_0040cc30::_Destroy;
