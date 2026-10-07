// Decompiled by GPT-6 Astra. Names are provisional.
struct Elem_0040d4f0 { char value; };
void __stdcall CopyOneByte(Elem_0040d4f0*, const Elem_0040d4f0*);
namespace std {
inline void _Construct(Elem_0040d4f0* dest, const Elem_0040d4f0& src) { CopyOneByte(dest,&src); }
}
#include <vector>
typedef std::vector<Elem_0040d4f0> Vec;
typedef void (Vec::*Resize)(unsigned int, const Elem_0040d4f0&);
// FUNCTION: 0x40c600 ?resize@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@QAEXIABUElem_0040d4f0@@@Z
Resize resize_0040c600=&Vec::resize;
