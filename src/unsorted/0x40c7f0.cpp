// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: identical bytes, but 0x40d4c0 is named Class_0040d4c0::FUN_0040d4c0 instead of this vector's size().
#include <vector>
struct Elem_0040d550 { int value; };
typedef std::vector<Elem_0040d550> Vec;
typedef void (Vec::*Resize)(unsigned int, const Elem_0040d550&);
// FUNCTION: 0x40c7f0 ?resize@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@QAEXIABUElem_0040d550@@@Z
Resize resize_0040c7f0=&Vec::resize;
