// Decompiled by GPT-6 Astra. Names are provisional.
struct Point16 { short x, y; };
struct Elem_0040cc40 {
    Point16 pos;
    float key;
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
};
void __stdcall FUN_0040d5e0(Elem_0040cc40*, const Elem_0040cc40*);
namespace std {
inline void _Construct(Elem_0040cc40* dest, const Elem_0040cc40& src) { FUN_0040d5e0(dest,&src); }
}
#include <vector>
typedef std::vector<Elem_0040cc40> Vec;
typedef Vec::iterator (Vec::*Insert)(Vec::iterator, const Elem_0040cc40&);
// FUNCTION: 0x40ca50 ?insert@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QAEPAUElem_0040cc40@@PAU3@ABU3@@Z
Insert insert_0040ca50=&Vec::insert;
