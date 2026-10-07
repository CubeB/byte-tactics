// Decompiled by Opus. Names are provisional.
// std::vector<Elem_0040cc40>::_Ufill(first, n, value): copy-constructs n
// copies of value into raw storage at first.
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

typedef std::vector<Elem_0040cc40> Vec_0040d5b0;
typedef void (Vec_0040d5b0::*UfillFn_0040d5b0)(
    Vec_0040d5b0::iterator, Vec_0040d5b0::size_type, const Elem_0040cc40&);

struct Access_0040d5b0 : Vec_0040d5b0 {
    static UfillFn_0040d5b0 fn;
};

// FUNCTION: 0x40d5b0 ?_Ufill@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEXPAUElem_0040cc40@@IABU3@@Z
UfillFn_0040d5b0 Access_0040d5b0::fn = &Access_0040d5b0::_Ufill;
