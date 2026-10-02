// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// DeepSeek V4.1 Flash pass (issue 4851): 83.0% RETAINED, 795 of 794 bytes, no
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
