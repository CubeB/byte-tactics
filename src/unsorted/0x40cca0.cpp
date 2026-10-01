// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// Refinement by GPT-6.1-sol: best remains 99.5%; two element-type variants
// left the same pointer-sum grouping in _Ucopy; a prior 128-header sweep also
// found no match.
// 2026-09-30 GPT-6.1-sol retry: manual vector-clone variants failed before
// compile; the saved source still checks at 99.5% (781/781).
// deepseek-v4.1: the full 768-set tools/headers.py --cpp sweep, a char[3] and
// an unsigned char element, a class instead of a struct (which mangles as V,
// not U, so it cannot be used), an explicit `template class
// std::vector<Elem_0040cfb0>;` (with operator< and operator== supplied), and a
// TU that instantiates a global vector plus an insert call before taking the
// member's address all leave the same 8 bytes and the same 99.5%.
// 2026-10-01 deepseek-v4.1-flash retry: no new variant reaches the residual
// 8 bytes; the file was re-checked and retains 99.5% (781/781). Only the
// inlined _Ucopy cursor rematerialization (lea eax,[ebp+ecx] vs our
// mov/sub/add tree) still differs; the calling file text cannot set it.
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
