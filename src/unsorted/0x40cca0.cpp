// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Retry verification: best remains 99.5%; the only diff is the pointer-sum
// grouping in _Ucopy, and the existing 128-header sweep found no match.
// Partial: 99.5%. This is std::vector<Elem_0040cfb0>::insert(iterator, size_type,
// const Elem&) from MSVC 5's <vector>, emitted out of line by taking the
// member's address. The only remaining difference is one pointer sum in the
// reallocation path's tail _Ucopy(_P, _Last, _Q + _M): the original groups it
// as (dest + _P) - _Q - _M (lea eax,[ebp+ecx]; sub eax,edx; sub eax,edi) and
// ours as (dest - _Q) + _P - _M (mov eax,ecx; sub eax,edx; add eax,ebp;
// sub eax,edi). Both equal _P; it is the compiler's induction-variable
// canonicalisation, and tools/headers.py tries all 128 header sets at 99.5%.
#include <vector>
struct Elem_0040cfb0 { char a, b, c; };
typedef std::vector<Elem_0040cfb0> Vec;
typedef void (Vec::*Insert)(Vec::iterator, unsigned int, const Elem_0040cfb0&);
// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
Insert insert_0040cca0=&Vec::insert;
