// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by mimo-v2.6-pro. Names are provisional.
// 2026-10-01 mimo-v2.6-pro retry (60 min timebox, ~150 scored scratch
// variants): the kept <vector> member-pointer build still scores 99.5%
// (781/781), so no new best, but the residual IS steerable from this file,
// contrary to the notes below. Using 0x408f30's clone trick (a hand-written
// copy of the <vector> class in namespace std, from <memory> + <xutility>,
// no <vector>) with the third copy still spelled `_Ucopy(_P, _Last, _Q + _M)`
// compiles the source-start derivation as
//     lea eax, [ecx + ebp] / sub eax, edx / sub eax, edi
// (780 bytes, 92.5%) instead of the library's four-instruction
//     mov eax, ecx / sub eax, edx / add eax, ebp / sub eax, edi
// (99.5%). Everything else matches instruction for instruction. So the only
// residual on the clone base is ONE SIB byte: the original wants
// `lea eax, [ebp + ecx]` (base ebp, mod=01 disp8=0, 4 bytes) and the clone
// emits base ecx (3 bytes); every later jump target then shifts by 1. That
// makes this the same commutative lea base/index wall as 0x408f30, 0x425210,
// 0x44ec30 and 0x46e640 (see those files). Note the wanted order is
// (older value _P first): every other lea in the function, matching or not,
// puts the older value in the SIB base.
// Measured this pass, all on the clone base unless said otherwise:
//   * secondary instantiations (vector<unsigned short>::insert,
//     vector<int>::insert at four positions; vector<Elem_0040cc40>::size/
//     capacity/insert and vector<Elem_0040cfb0>::size/erase in the original's
//     emission order before insert) flip the clone to the 99.5% mov form but
//     never to [ebp + ecx]; <climits> in front changes nothing.
//   * tools/headers.py on the clone (128 sets): no match, best 99.5% with
//     <ddraw.h> (the mov form). Prefix x extern-count scan (windows.h,
//     ddraw.h, climits combos x 24 counts to 256) produces only the same two
//     shapes, lea-swapped (92.5%) and mov form (99.5%).
//   * third-copy spellings on the clone: a named `iterator _d = _Q + _M`
//     local, `++_F, ++_P` increment swap, `(size_type)_M` cast, a
//     `const_iterator _s = _P` temp, source-first `_d`/`_s` locals, `_M + _Q`
//     and a cached `_Last` local are all byte-identical at 780 bytes / 92.5%
//     with the same [ecx + ebp]. Source text does not reach the operand order;
//     the remaining lever is TU state of the original's whole file (untried:
//     wide single-granularity declaration padding on the clone base at the
//     0x4b6c30 flip scale, thousands of declarations).
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
// 2026-10-01 deepseek-v4.1-flash retry 2: the insert body is the shipped
// <vector>, so the calling file has no lever for the residual; re-checked
// at 99.5% (781/781) with the same single hunk retained.

#include <vector>
struct Elem_0040cfb0 { char a, b, c; };
typedef std::vector<Elem_0040cfb0> Vec;
typedef void (Vec::*Insert)(Vec::iterator, unsigned int, const Elem_0040cfb0&);
// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
Insert insert_0040cca0=&Vec::insert;
