// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Claude Opus 5.5 pass (#5015): 83.0% RETAINED under the checker's flags,
// body unchanged, BUT THE ORIGINAL SOURCE IS FOUND: IT MATCHES WITH /Gi.
// The plain real header plus one ordinary use of the vector's operator= is
// byte-identical, references included, when compiled with /Gi (incremental
// compilation) added to the flags:
//     uv run tools/check.py 0x475ef0 <file> --flags "/O2 /Ob2 /MT /Gz /Gi"
//   prints MATCH (794 of 794 bytes) for
//     #include <vector>
//     struct Element_00475ef0 { int data[0x11]; };
//     typedef std::vector<Element_00475ef0> Vec_00475ef0;
//     void __stdcall Assign_00475ef0(Vec_00475ef0* to, const Vec_00475ef0* from)
//     { *to = *from; }
//     (the InsertFn_00475ef0 typedef and the annotated line at the end
//     of this file, unchanged)
// A `resize(n)` use instead of operator= also MATCHes; reserve, copy ctor,
// erase, destructor or no other use give 83.0% under /Gi. Under the default
// flags that file is 82.9% (796 bytes), below this clone, so it is not
// committed; switch to it if the orchestrator adopts /Gi for this TU.
// The same recipe matches every insert of this TU under /Gi: 0x4758c0
// (operator= or resize), 0x476490 (operator= or resize), 0x475bd0 and
// 0x476210 (reserve or copy ctor), and also 0x46e640 (vector<int>, reserve,
// copy ctor or resize), one of the six that field-notes Part 6 closed as
// unreachable. So the "lea family" byte and the _P-in-edx family are not a
// different compiler build: they are /Gi plus which other vector members
// the TU instantiates. Mechanism, from the /Fa listing: /Gi numbers IL
// symbols per function (`__N$5966$5`, temps from 0x400000) instead of with
// the TU-wide counter (`__N$2336`, `$T2576`), which reverses the ties that
// pick _P's register and the _N/_S frame slots. /Gi is not uniform across the
// exe: 15 of 48 sampled MATCHED functions (and 11 of the 12 matched inserts,
// with their current files) stop matching under it.
// Without /Gi nothing moves this function: with the real <vector> all of
// these are flat at 82.9% (the 0x43c3a0 control's esi family): 8 filler kinds
// at 0..300, fillers between the class instantiation and the insert, the 102
// matched files of 0x471000..0x475ef0 compiled first in one TU, 12
// instantiations in one file, element declarations (ctor, dtor, memcpy copy
// ctor and operator=, nested short pairs, char/short/int members, pack
// 1/2/4/8), earlier uses of every member above, helpers emitted out of line
// first, an explicit member specialisation, RTM, /YX, /Z7, /Zd, /Zi, /Gm,
// /G3 to /G5 and source path length. This clone is flat at 83.0 for _N/_S/_Q
// declaration order, self-assignments at every position, helper
// pre-instantiation and third-copy spellings.
//
// Space Bunny Free pass (issue 4896): 83.0% RETAINED, 795 of 794 bytes, body
// unchanged. This pass was pointed at the frame-slot reading in stackcmp's
// table (_N at -0x8 here against -0x4 in the original, _S the other way) and
// settles three things the notes below did not have.
//
// 1. THE SLOT SWAP IS REACHABLE FROM SOURCE, BUT NOT ON ITS OWN. Three
// spellings do put _N at cv-4 and _S at cv-8, the original's layout: the
// third copy written as an inline destination-first loop
//     {iterator _D = _Q + _M; const_iterator _E = _P;
//      for (; _E != _Last; ++_D, ++_E) allocator.construct(_D, *_E);}
// (69.3%, 799 bytes), a destination-first `_Ucopy` for the first call only
// (82.6%, 793 bytes, but then the third call hands the destination to the
// source parameter, so it no longer computes this function), and the three
// member stores reordered with `_End = _S + _N` last (80.5%, 801 bytes).
// Every one of them also moves `this` out of ebp or _P out of edx, so the slot
// order is not a free parameter of the allocation: it follows the IR, and the
// only shapes that swap the slots break something the original has.
//
// 2. THE ONE SPELLING THAT PUTS _P IN edx AND _S IN esi, EXACTLY AS THE
// ORIGINAL, IS A FUSED FILL INCREMENT, AND IT COSTS THE FILL LOOP. With
// `_Ufill`'s body as
//     {for (; 0 < _N; --_N) allocator.construct(_F++, _X); }
// or the same loop written inline at the call site, the load after
// `operator new` becomes `mov edx, [esp+0x24]` and the allocate result goes to
// esi and is spilled to cv-8, which is the original's whole head. But the fill
// then walks the destination in eax and the source in esi (two induction
// variables) instead of reloading &_X inside a one-variable loop, so it is
// 800 bytes and 72-73%. A grid of 3240 variants (5 _Ucopy loop shapes x 8
// _Ufill loop shapes x 3 first-copy forms x 4 fill-call forms x 5 third-copy
// forms) and 216 further _Ucopy/_Ufill/_Destroy parameter-order permutations
// never produce edx together with the original's one-variable fill loop. That
// pair is the whole wall: edx is forced there, because inside the two copy
// loops eax and ebx are the walking pointers, ecx the rep count, esi and edi
// the rep operands and ebp holds `this` until the reload from cv-0xc, so edx is
// the only register that can carry _P across both loops. This build spends
// esi instead and pays for it twice (a reload of _P at the end of the prefix
// loop and another after the fill), which is the whole 41-line residual.
//
// A lead worth one more pass: the third copy inlined as above does reproduce
// the original's FILL loop, invariant &_X re-loaded inside the loop with no
// register hoisted for it, which nothing else here does. What it still gets
// wrong is that _P then takes ebp and `this` stays in edi, so the fill counter
// lands in edx where the original has ebp. The shape that has both the
// original's fill loop and _P in edx is the fixed point neither shape reaches.
//
// 3. OUR SHAPE IS A COMPILATION OF THIS TEMPLATE THAT THE EXE ITSELF
// CONTAINS. Re-reading all 29 `insert@?$vector` instantiations off the exe
// corrects the census in the notes below: three MATCHED ones do hold _P in edx
// (0x488fb0, 0x4be6c0, 0x4c51e0), but all three are the class-element shape
// with an out-of-line `_Ucopy`. Among the memcpy-shape ones the matched
// siblings read _P in ebx (0x433b20, 0x4c4d70), ecx (0x433db0, 0x4340f0,
// 0x4b7b00) and esi (0x43c3a0), and 0x43c3a0's realloc-arm head is
//     call 0x4b4f10 / mov esi,[esp+0x24] / mov [esp+0x1c],eax / mov ebx,eax
// which is this build's head, instruction for instruction, with _S spilled to
// cv-4 and _N stored at cv-8. So the residual is not a shape no C++ produces:
// it is the 0x44-byte instantiation of a template the original built one way
// here and another way at 0x4758c0 and 0x43c3a0.
//
// Everything measured this pass, all inert or worse than 83.0%:
//  - file-scope padding, 6 forms (int globals, functions, typedefs, structs,
//    enums, classes) x 45 counts from 1 to 704, on this clone and on a
//    header-shaped clone: two shapes only, 795B/83.0% and 796B/82.9%, never
//    edx. 0x46f7a0's note records padding flipping that instantiation; it does
//    not flip this one, so padding is not the lever here either;
//  - the element declaration, 11 forms (int array, 17 int members, char[68],
//    long[17], short[34], int plus char, a nested struct, unsigned, a union),
//    all byte-identical; and 32 element sizes from 4 to 128 bytes, which give
//    _P in esi or ecx and never edx;
//  - the toolchain's own header: `#include <vector>` with and without
//    `template class std::vector<Element>;`, with and without comparison
//    operators on the element, and with the whole vector class text pasted into
//    the file (all 796 bytes, 82.9%, _P in esi). Removing any of 18 member
//    groups from that pasted class (92 compilable combinations) is still
//    796B/esi, and adding any single group to this 795B clone is still
//    795B/esi. The single byte between the clone and the header is the third
//    copy's induction arithmetic, `dest + _P - _Q - _M*size` in the clone,
//    `(dest - _Q) + _P - _M*size` in the header and
//    `(_P - _Q) + dest - _M*size` in the original, and neither spelling moves
//    the register;
//  - unused declarations: a local, three locals, an unused member function,
//    reordered helpers, reordered data members, an extra typedef, a static
//    member, all byte-identical;
//  - accessor spellings: `_Ucopy(begin(), _P, _S)` 79.4%/796B,
//    `_Ucopy(_P, end(), _Q + _M)` 72.7%, `_Destroy(begin(), end())`
//    77.2%/802B; the `end()`-only spellings in arms 2 and 3 are
//    byte-identical, as is `iterator& _R = _P;` aliasing the parameter in
//    either arm;
//  - the `_N` expression: swapped operands, `>=` instead of `<`, `<` reversed,
//    `+ 0` and an explicit cast are byte-identical or worse;
//  - translation-unit state: the pasted header class plus one, two or three
//    extra `std::vector<E>::insert` instantiations at 0x34, 0x3c and 0x5c
//    before ours, all 796B/esi;
//  - `_Ucopy` and `_Ufill` as static members taking the allocator, byte-
//    identical;
//  - `permute.py 0x475ef0 --stack _N,_S` for 20 minutes, nothing above the
//    start.
// The body text is confirmed against the toolchain itself:
// toolchain/msvc5-sp3/INCLUDE/VECTOR has this `insert` (lines 137-158) and
// these `_Ucopy`, `_Ufill` and `_Destroy` (lines 219-231) character for
// character, so what is left is not a source question.// DeepSeek V4.1 Flash pass (issue 4851): 83.0% RETAINED, 795 of 794 bytes, no
// source shape found that moves _P into edx. Ran `permute --stack _N` and
// `permute --stack _N,_S` for 3 minutes each (4515 candidates, 0 gain) and ~35
// hand variants, none above 83.0: split and fused fills; named locals for the
// first copy's limit _P, its source _First, its dest _Q and the third copy's
// dest; no-op self-assignments on _N, _S, _P, _M, _Last and _End; a temp then
// `_N = _T` to try to move the frame slot; the second and third arm's
// _Ufill/fill/_Last DAG rewritten; and flipping `a < b` to `b > a` on all three
// branch guards. New measurement that closes the frame-slot route: with the
// locals split, this build always puts the FIRST-defined local in the deeper
// slot (`_T` at -0x8, `_S` at -0x4 in T1), while the original puts _N (defined
// first, at 0x475f89) in the shallower -0x4 and _S in -0x8, so the original's
// pair is the same inverted allocator choice as edx, not an independent slot
// decision. The rest of the diff below (arms 2 and 3) is only the +1 jump-target
// shift, so the whole 41-line residual is this one allocation.
// Space Bunny Free pass (issue 4147 follow-up): 83.0% RETAINED, 795 of 794 bytes,
// unchanged body. This pass ran the masked-byte twin test it was pointed at and
// then showed that on THIS function the twin test's 0 hits is a false negative
// and must not be read as closing it.
//
// TWIN TEST, MASKED BYTES (the decisive form). Masking every rel8 and rel32
// branch and call displacement of the 794 bytes at 0x475ef0 and searching all
// of .text returns 0 hits in 1,026,560 bytes, so no byte-identical compilation
// of this shape exists anywhere in TotalA.exe and there is no sibling to copy.
// TWIN TEST, SIZE LEVEL: of the 29 insert@?$vector instantiations the linker
// names, 12 are matched (449/467/547/622/649/755/773/785) and 17 are partial
// (477/532/537/544/546/632/636/639/779/781/791/794/798/936). 794 appears only
// among the partials, and no partial size appears among the matched ones at all.
//
// BUT THE RESIDUAL IS A REGISTER, NOT AN INSTRUCTION, SO 0 HITS PROVES NOTHING
// HERE, and this pass established that instead of assuming it. The whole
// residual is one choice at instruction 71: the original loads _P into edx, and
// edx is the only register the first copy loop leaves free, because eax and ebx
// are its two walking pointers, ecx is the rep count and esi and edi are the rep
// movsd operands. A register choice is legal C++, so it needs no twin to exist
// for it to be reachable. The axis provably varies: reading the family straight
// off the exe for all 29 gives eax (4 functions, 2 matched), ebx (2, 2), ecx
// (1, 1), edi (7, 4), ebp (1, 0), esi (10, 3), edx (4, 0). Five of the seven
// registers appear in MATCHED functions, so edx's absence from the matched set
// is absence from a set that provably varies, not from a constant set. The three
// CONSECUTIVE functions 0x4758c0 (779 bytes, _P in eax), 0x475bd0 (791, esi) and
// this one (794, edx) are one template at element sizes 0x34, 0x3c and 0x44, so
// the family varies inside a single instantiation run, which is why a size census
// cannot settle it either.
//
// edx IS REACHABLE FROM SOURCE. Two independent knobs, neither recorded in the
// notes below, put _P in edx AND delete the in-loop reload that is the single
// extra byte:
//   1. fusing the fill's increment into the construct call, either through the
//      member _Ufill(_Q, _M, _X); written as
//          for (; 0 < _N; --_N) allocator.construct(_F++, _X);   (800 bytes)
//      or inline through a local
//          {iterator _Q2 = _Q; for (size_type _C = _M; 0 < _C; --_C)
//           allocator.construct(_Q2++, _X); }                    (800 bytes);
//   2. a bare do-while fill, do { construct(_F2, _X); ++_F2; }
//      while (0 < --_C);                                         (781 bytes).
// Item 1 and the inline local form are equivalent to _Ufill(_Q, _M, _X) and are
// correct C++; item 2 is NOT, it spins for 2^32 iterations when _M is 0, which
// is also why it loses the `test edi,edi / jbe` entry guard the original has,
// so it is a lever reading, not a candidate.
// Independently, spelling the third copy's source as a difference of two live
// pointers also gives edx: _Ucopy(_Last - (_Last - _P), _Last, _Q + _M) is 810
// bytes and _Ucopy(_Q - (_Q - _P), _Last, _Q + _M) is 844. The same mechanism
// applied to _Ucopy itself moves the register without reaching edx:
// allocator.construct(_P++, *_F) gives ebp (787 bytes),
// allocator.construct(_P, *_F++) gives ebp (775), and construct(_P++, *_F) with
// ++_F, ++_P still in the comma gives ebx (789). All seven registers are
// reachable from this one body, so "not reachable from the source" as recorded
// below is wrong and is withdrawn.
//
// WHY edx IS STILL NOT THE ANSWER: every edx shape costs more elsewhere than it
// gains. The postfix _Ufill is 800 bytes and 112 differing lines; dropping
// _Destroy on top of it reaches 796 bytes and check.py scores that one 70.5%
// against this file's 83.0% (78.3% if internal jump-target shifts are ignored),
// and it is not a candidate anyway because it deletes a line the original
// plainly has. Its listing has edx right and three things wrong at once: `this` and _M become live ACROSS the first copy loop, so
// they are re-materialised inside it (mov ebp,[esp+0x10] and mov edi,[esp+0x24]
// where the original has them after it) and the restore of _S into esi
// disappears; &_X lands in edx and evicts _P, which then has to be reloaded from
// [esp+0x20]; and the third copy's destination and source registers stay swapped
// (lea edx,[ebx+ecx] and cmp eax,esi where the original wants lea eax,[ebx+ecx]
// and cmp edx,ebp).
//
// MEASURED AND INERT THIS PASS. Compile-only, through the 0.5 s harness at
// build/scratch/475ef0/, so these are differing-line distances from the
// original's disassembly, not percentages, and `d=0` would mean byte-identical:
//  - element size, the axis the family actually varies on: 24 sizes from 4 to
//    128 built with the real <vector> give esi, edi, ecx, eax and never edx, so
//    the 0x44 element is not the cause;
//  - the real <vector> with `template class std::vector<E>;`, the recipe of
//    matched sibling 0x43c3a0, at a 0x44-byte element: 796 bytes, 65 differing
//    lines, the same esi family plus a dead pre-delete spill. Not the lever;
//  - translation-unit state: one extra address-taken vector<T>::insert, before or
//    after, at any of 32 element sizes, and pairs of them, all give the same
//    796-byte, 65-line, esi-family shape. Two shapes, never a third, which is
//    what 0x475bd0's notes record for the neighbouring template;
//  - declaration order for _N and _S, both orders and both split and unsplit:
//    byte-identical. The _N/_S frame-slot swap (original _S at [esp+0x14] and _N
//    at [esp+0x18], this build the reverse) is the FIRST difference in the diff
//    and is not declaration-driven here;
//  - inert at 795/41 with the faithful _Ufill: the third copy's destination (a
//    named local, _Q + _M + 0, an explicit cast, &_Q[_M], _Q + _M * 1), its
//    source (_P + 0, &*_P, a cast, a named local), the three member stores in
//    three orders, the allocate spelling, splitting the _N and _S declarations,
//    dropping _Destroy, moving the stores ahead of _Destroy and deallocate, an
//    extra _Ucopy member, an extra reserve member, an operator[], and four
//    single includes including <windows.h>;
//  - ten fill spellings crossed with six _Ucopy parameter and loop-shape orders
//    and twelve other arm knobs, 144 combinations: this file's body (795 bytes,
//    d=41) is still the best of all of them.
#include <climits>
#include <memory>
#include <xutility>

struct Element_00475ef0 {
    int data[0x11];
};

namespace std {

// MSVC 5's <vector> class template, with only the members insert needs. The
// name and the two template parameters have to match the real ones for the
// member to mangle as ?insert@?$vector@UElement_00475ef0@@...
template<class _Ty, class _A = allocator<_Ty> >
class vector {
public:
    typedef vector<_Ty, _A> _Myt;
    typedef _A allocator_type;
    typedef _A::size_type size_type;
    typedef _A::difference_type difference_type;
    typedef _A::pointer iterator;
    typedef _A::const_pointer const_iterator;
    typedef _A::reference reference;
    typedef _A::const_reference const_reference;
    typedef _Ty value_type;
    vector() : allocator(), _First(0), _Last(0), _End(0) {}
    size_type size() const
        {return (_First == 0 ? 0 : _Last - _First); }
    iterator begin() { return (_First); }
    iterator end() { return (_Last); }
    void insert(iterator _P, size_type _M, const _Ty& _X)
        {if (_End - _Last < _M)
            {size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void *)0);
            iterator _Q = _Ucopy(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            _Ucopy(_P, _Last, _Q + _M);
            _Destroy(_First, _Last);
            allocator.deallocate(_First, _End - _First);
            _End = _S + _N;
            _Last = _S + size() + _M;
            _First = _S; }
        else if (_Last - _P < _M)
            {_Ucopy(_P, _Last, _P + _M);
            _Ufill(_Last, _M - (_Last - _P), _X);
            fill(_P, _Last, _X);
            _Last += _M; }
        else if (0 < _M)
            {_Ucopy(_Last - _M, _Last, _Last);
            copy_backward(_P, _Last - _M, _Last);
            fill(_P, _P + _M, _X);
            _Last += _M; }}
protected:
    void _Destroy(iterator _F, iterator _L)
        {for (; _F != _L; ++_F)
            allocator.destroy(_F); }
    iterator _Ucopy(const_iterator _F, const_iterator _L,
        iterator _P)
        {for (; _F != _L; ++_P, ++_F)
            allocator.construct(_P, *_F);
        return (_P); }
    void _Ufill(iterator _F, size_type _N, const _Ty& _X)
        {for (; 0 < _N; --_N, ++_F)
            allocator.construct(_F, _X); }
    _A allocator;
    iterator _First, _Last, _End;
    };

} // namespace std

typedef std::vector<Element_00475ef0> Vec_00475ef0;
typedef void (Vec_00475ef0::*InsertFn_00475ef0)(
    Vec_00475ef0::iterator, Vec_00475ef0::size_type, Element_00475ef0 const&);

// FUNCTION: 0x475ef0 ?insert@?$vector@UElement_00475ef0@@V?$allocator@UElement_00475ef0@@@std@@@std@@QAEXPAUElement_00475ef0@@IABU3@@Z
InsertFn_00475ef0 g_insert_00475ef0 = &Vec_00475ef0::insert;
