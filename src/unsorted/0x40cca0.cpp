// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: 98.9%. The second inlined copy computes its source with mov/sub/add instead of lea/sub.
#include <vector>
struct Elem_0040cfb0 { char a, b, c; };
typedef std::vector<Elem_0040cfb0> Vec;
typedef void (Vec::*Insert)(Vec::iterator, unsigned int, const Elem_0040cfb0&);
// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
Insert insert_0040cca0=&Vec::insert;
