// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by mimo-v2.6-pro, retried by space-bunny-free. Names are provisional.
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
// 2026-10-02 space-bunny-free retry. Two facts measured here that the notes
// above do not have. (1) The two neighbouring shapes are BOTH one step from the
// original, in different ways, and the shape oracle is the length of that one
// lea: `lea eax,[ebp+ecx]` is 4 bytes (SIB base ebp forces mod=01, disp8=0),
// `lea eax,[ecx+ebp]` is 3 bytes (base ecx, no disp), and the shipped <vector>
// form is 4 instructions of 8 bytes, so the totals tie at 781 and the
// percentage cannot tell them apart. (2) The hand-written clone of
// build/scratch/40cca0/base.cpp scores 92.5% only because of that 3-byte lea:
// 305 instructions against the original's 305, every jump target shifted by
// one. So the clone is the better platform (one SIB byte from a MATCH) and the
// shipped <vector> is the worse one (four instructions from it).
// 2026-10-02 space-bunny-free, second pass. The blocker is now identified, and
// it is not a source or a translation-unit question. THE SHAPE IS NOT A PROPERTY
// OF THE SOURCE. In the exe, 0x408f30 (vector<Unit*>::insert, the wanted
// `lea eax,[ebx + ecx] / sub eax,edx / sub eax,edi`) and 0x4c4d70
// (vector<Class_004c3e40*>::insert, the mov form
// `mov eax,ecx / sub eax,edx / add eax,ebx / sub eax,edi`, and MATCHED at
// 100.0% by src/unsorted/0x4c4d70.cpp) are the same template instantiation on
// the same 4-byte dword-copied element, and instruction for instruction they
// differ ONLY in that group: 225 against 226 instructions, one replace and one
// insert (diff of the two disassembly lists, jump targets masked). Both
// register assignments agree too (_P in ebx, _Q in edx, dest in ecx, _M*4 in
// edi, _Last in esi). So the exe's own compiler emitted both shapes from the
// same source, and all ten of the exe's MATCHED 3-argument vector
// inserts produce its mov form (0x433b20,
// 0x433db0, 0x4340f0, 0x43c3a0, 0x488fb0, 0x4b7b00, 0x4be6c0, 0x4c4d70,
// 0x4c51e0 and 0x4dd8c0), while the six that are stuck
// (0x408f30, 0x40cca0, 0x425210, 0x44ec30, 0x46e640, 0x476210) all want the
// lea form. This build reaches {mov form, lea with the basic IV in the SIB
// base} and never the lea with _P in the base, and that is a property of the
// build, not of the file: measured here, all flat and all negative.
//   * The element type is not the variable either, which settles the family
//     question the notes above leave open. Taking the MATCHED 0x4c4d70.cpp file
//     verbatim and changing only the element type to `Unit*` (structurally the
//     same 4-byte dword-copied pointer element that 0x408f30 uses, declared
//     exactly as src/unsorted/0x408f30.cpp declares it) still compiles to the
//     mov form, 547 bytes, `mov eax,ecx / sub eax,edx / add eax,ebx / sub
//     eax,edi`. So with one file style both of the exe's pointer-element
//     instantiations come out the same, while the exe has one at 546 bytes with
//     the lea form and one at 547 with the mov form. Two source files of the
//     game's build, one STL source, two shapes: it is the translation unit.
//   * Source: about 70 spellings of the third copy's destination, of _Ucopy's
//     loop (increment order, while/do-while, post-increment in the body,
//     swapped comparison operands, argument orders with the call sites updated,
//     the loop body reading through a subscript, a dead `(_P - _F)` in the body)
//     and of the fill, each crossed with 3 to 5 pad counts, all give the mov form
//     or the lea with the IV in the base; none gives the wanted one. The pad
//     count, not the spelling, decides which of the two comes out: for 15
//     spellings the flip is at exactly the same count (17 unused prototypes),
//     one spelling moves it to 16. That is compiler state, so stop rewriting.
//   * TU state: 15 filler kinds (unused prototypes to 6000, structs, globals,
//     static functions, template instantiations, classes with virtuals,
//     try/catch, string literals, typedefs/enums, externs, inline functions,
//     macros, unions/bitfields, functions taking structs, calls into the
//     class) at counts 0 to 400 each, plus a whitespace control, all with the
//     residual read as an instruction sequence rather than a percentage: always
//     exactly two shapes, never a third. A genuinely large TU instead of
//     filler was built from real game code (our own matched src/unsorted files
//     appended to this file and to the clone, up to 832 KB and 32306 lines with
//     about 700 of them, each one kept only after the whole file still compiled,
//     plus a <list>/<map>/<set>/<deque>/<string> instantiation block before,
//     after and around this file): all the mov form. The sibling 0x408f30, whose
//     wanted and our forms are both three-byte leas, so one SIB byte is the whole
//     oracle (0x0b against 0x19), is the same story: 300 prototype counts give
//     126 builds at our 0x19 and 174 at the mov form, never 0x0b.
//     Twelve parallel compiles of the same file produce byte-identical code
//     (build/scratch/40cca0/determinism.py), so the flat sweeps are facts and
//     not noise.
//   * The compiler build: BT_TOOLCHAIN=msvc5-rtm gives the same two shapes on
//     both bases, and the RTM and SP3 INCLUDE/VECTOR headers are identical, so
//     neither the patch level nor the STL revision is the difference.
//   * 24 flag variations (/Zp1..16, /Ob0..3, /Gs*, /G6, /G7, /Z7, /Zl, /W3,
//     /W4, /D_DEBUG, /DNDEBUG, /GF, /Zc): all the mov form except /Ob0 and
//     /Ob3, which stop inlining the helpers and make the function 395 bytes.
//     check.py's /O2 /Ob2 /MT /Gz is not hiding the byte behind a flag.
// So the wanted byte is a one-byte difference in this compiler's handling of the
// loop optimiser's synthesised add (which of its two registers goes in the SIB
// base slot when the emitter has a free register for the result), and it is not
// reachable from the source or from any translation unit we can build. Keep the
// shipped <vector> below: it is the best score and, unlike the clone, it has the
// original's length. The family (0x408f30, 0x425210, 0x44ec30, 0x46e640,
// 0x476210, this one) all sit on this same byte.

#include <vector>
struct Elem_0040cfb0 { char a, b, c; };
typedef std::vector<Elem_0040cfb0> Vec;
typedef void (Vec::*Insert)(Vec::iterator, unsigned int, const Elem_0040cfb0&);
// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
Insert insert_0040cca0=&Vec::insert;
