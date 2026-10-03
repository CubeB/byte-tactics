// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// space-bunny-free (#4896, 2026-10-03): still 99.6%, 636 bytes, the one SIB
// byte. New result worth keeping, because it closes off the last two theories
// and fixes what the clone in this file really is. There are THREE possible
// codegen shapes for the third copy's derived source pointer, and this
// compiler emits only the first two:
//   (a) lea eax, [dest + src0]; sub eax, _Q; sub eax, n   636 bytes, 99.6%
//   (b) mov eax, dest; sub eax, _Q; add eax, src0; sub eax, n   637, 89.6%
//   (c) lea eax, [src0 + dest]; sub eax, _Q; sub eax, n   what the original has
// (src0 is `_P`, dest is `_Q + _M * 32`, n is `_M * 32`.) Every source spelling
// in this file reaches (a), the state axis reaches only (a) and (b), and the
// original is in (c). So the missing byte is the difference between two shapes
// that are both reachable in the exe's own code, not a misreading of it.
//
// (1) It is a per-source-file compiler state, not an element-size property, and
// the exe's own matched siblings split by translation unit, not by size:
// 0x433b20 and 0x4c4d70 (4-byte elements), 0x43c3a0 (25-byte) and 0x4dd8c0
// (48-byte) all MATCH with the real <vector> and all have shape (b), while
// 0x408f30, 0x425210, 0x44ec30, 0x46e640, 0x475bd0 and this one are all stuck
// at 99.6% and all have shape (c). Same template, same header, both forms.
//
// (2) The hand-written clone below is NOT equivalent to the real <vector>, and
// that is the reason the clone is in this file. Control experiment: rewriting
// 0x433b20 (which MATCHes with the real header) as this same clone gives 89.6%
// and shape (a), `lea eax, [ecx + ebx]`, where the original has shape (b). So a
// function whose original is in shape (b) must use the real header, and one whose
// original is in shape (a) or (c) cannot use it at all: for this 32-byte element
// the real header compiles to shape (b) (637 bytes) and the clone to shape (a)
// (636 bytes), so neither carrier can reach the original's (c). The file is the
// best of the two, but the last byte needs the original file's compiler state.
//
// (3) The state axis is a count, not a hash: five kinds of unused declaration
// (`extern int`, `extern void f();`, `typedef int`, `struct`, `extern int v`)
// flip at the same N (48-50, 306-312, 369, 377, ...) and 0 to 2800 of them give
// only (a) and (b). Real code before behaves the same way: 1 to 3 small template
// instantiations keep (a), 4 or more give (b), and any of the exe's neighbouring
// vector::insert instantiations (0x4758c0, 0x475bd0, 0x475ef0, 0x476490,
// 0x46eba0, 0x40a7b0, alone or all together) gives (b); 1 to 5 plain functions
// before or after the annotation change nothing. The real <vector> carrier is
// pinned at (b) for 0 to 400 declarations.
//
// (4) Flags never reach (c): /Ob1, /Ob0, /Ob3, /O1, /G3../G8, /Gr, /Gd, /Gm,
// /Ot, /Gs all stay at (a) or far worse (/G6 gives 93.5%, 641 bytes).
//
// (5) Flat this pass, all at 99.6% with the same SIB byte: 11 spellings of the
// third copy's destination and source, 9 spellings of _Ucopy's loop, 24
// _Ufill/_Destroy/_Last spellings, 13 element types (int[8], long[4], double[4],
// char[32], short[16], float[8], eight named fields, a nested struct, a
// user-defined operator=, a user-defined copy constructor), 17 include sets
// (adding <new>, <algorithm>, <utility>, <string>, <exception>, <map>, <list>,
// <iostream>, dropping <climits>, every order of the three STL headers), member,
// typedef and function order inside the class, register hints on every pointer,
// a fresh local for either pointer, `copy` and `uninitialized_copy` instead of
// _Ucopy, 16 dead statements before and after the copy, 16 hand-written
// third-copy loops with the two pointers declared in both orders and four
// increment orders, a 2D sweep of 20 source spellings x 105 declaration counts,
// and a 18-minute permuter run (2069 candidates, 793 of them commutative operand
// swaps). headers.py's 256 sets are flat as before.
//
// (6) Correction for the note below from 0x40cca0's file: here the wanted
// operand order is not simply "the older value in the base". All four leas in
// this function, in the original, do put the older value in the base (ebx, ebp,
// and `_P` in edi), and shape (a) puts the freshly computed dest (edx) there
// instead, so shape (a) is the odd one out in both builds. What flips is which
// leaf of the derived expression comes first, and it is the same tie-break in
// both this file and 0x408f30, 0x425210, 0x44ec30, 0x46e640 and 0x475bd0.
//
// Tooling note for the next pass: for this family, screen the lea encodings
// instead of the score. `objdump -d -M intel` on the object and grep for `lea`
// prints every base/index pair in one line, which says in 0.5 s whether a variant
// reached (b) or (c); check.py's diff only shows the one SIB byte, and the ratio
// cannot tell (a) from (c). build/scratch/0x476210/screen.py does this.
//
// deepseek-v4.1-flash (#4851, 2026-10): two 3-minute permuter runs (6258
// candidates, seeds default and 12345) and manual third-copy source spellings
// (a difference-of-pointers source `_Last - (_Last - _P)`, an explicit
// `(_Q + _M) - ((_Q + _M) - _P)`, and a named destination local) all stay at
// 99.6%, 636 bytes. Still only the single SIB byte: `lea eax, [edi + edx]`
// (SIB 0x17) against this build's `lea eax, [edx + edi]` (SIB 0x3a).
// Confirmed compiler state; needs the regroup-into-original-files phase.
// deepseek-v4.1-flash (#3287): tried the real-<vector> recipe of the matched
// siblings 0x43c3a0/0x433db0 (explicit instantiation, same element size): 89.6%,
// 637 bytes, worse than this clone's 99.6%, 636 bytes. A dest local of iterator
// or const_iterator type at the third _Ucopy, `_Q += _M` before the call, and a
// difference_type cast on _M are all byte-flat at 99.6.
// deepseek-v4.1-flash (#2924 retry, 2026-10): still 99.6%, 636/636 bytes. Two
// more source variants at the third _Ucopy (a fresh "iterator _D = _Q + _M;"
// local, and a separate "size_type _Off = _M;" used as "_Q + _Off") both keep
// the same "lea eax, [edx + edi]" (SIB 0x3a) against the original's
// "lea eax, [edi + edx]" (SIB 0x17). This confirms the wall is compiler state,
// as the long note below already records; the fix needs the regroup phase.
// GPT-6.1-sol refinement (#2306): best remains 99.6% (636/636 bytes); only the commutative LEA SIB operand order differs. Header-order and <algorithm> substitutions kept the same mismatch.
// GPT-6 retry: 99.6%, 636 bytes; pointer and buffer constness and allocator pointer typedef variants left the same third-copy LEA base/index byte different.
// Sonnet 5.5 retry (#1081): /Gz and /Gr give the same 99.6% (it is a method), the
// third-copy loop written out by hand with the destination declared first is
// 51%, and 3000 more random variants (helper parameter orders and loop shapes at
// each _Ucopy/_Ufill site, size and tail spellings) never leave the same one SIB byte.
// std::vector<Elem_00476210>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, emitted out of line for a 32-byte trivially copyable element. The
// body is the template with _Ucopy, _Ufill, fill and copy_backward inlined: the
// free-space check splits into the reallocating branch (allocate, copy the
// prefix, fill _M copies, copy the suffix, deallocate, reset the three
// pointers) and the two in-place branches (shift the tail right when it is
// shorter than _M, or roll the last _M elements and shift the middle with
// copy_backward). Taking the member's address makes the compiler emit the
// template instantiation out of line, as in the original translation unit.
//
// The class below is a hand-written clone of the <vector> class template, not
// an include of <vector>, because the only difference left is decided by what
// the surrounding translation unit instantiates: including <vector> (which
// pulls in <stdexcept> and its std::string instantiations) makes this
// compiler build the third copy's source pointer as
//     mov eax, edx / sub eax, ebx / add eax, edi / sub eax, ecx
// (637 bytes) while dropping <stdexcept> makes it build the same value as one
// lea plus the two subs (636 bytes). Nothing else about the class matters:
// the element type (int[8], eight int members, char[32], short[16], long
// long[4], float[8], double[4], a nested struct, a class, typedef'd arrays),
// the parameter and member names, the loop shapes of _Ucopy/_Ufill, the order
// of the increments, the include order, the header set (about 1000
// combinations) and the amount of other code in the file all give exactly the
// same 636 bytes. A copy of this file with no STL headers at all (its own
// std::allocator, fill and copy_backward) is 635 bytes, and the msvc5-rtm
// compiler, whose C1XX.DLL is a different build, gives the same code as the
// sp3 one, so this is not a compiler or header version difference.
//
// Still differs: 99.6%, ours 636 bytes against the original's 636, a single
// SIB byte. The original adds the copy's source pointer to the destination
// first, ours adds the destination to it:
//     lea eax, [edi + edx]        ; _P + (_Q + _M * 32)   original, SIB 0x17
//     lea eax, [edx + edi]        ; (_Q + _M * 32) + _P   this build, SIB 0x3a
// followed by the same `sub eax, ebx` (_Q) and `sub eax, ecx` (_M * 32) in
// both, which cancel the destination again. The two operands are commutative
// and both are plain registers here, so nothing in the source reaches the
// choice: about 1900 variants (source spellings, loop shapes, parameter
// orders, header sets, dummy code, template instantiations) all produce the
// [edx + edi] order, and every variant that keeps the lea produces exactly
// this one byte out. The other instantiations of this template in the exe
// (0x408f30, 0x40cca0, 0x40d020, 0x40d290) want the same order, so this is
// the same wall as the one the guide records for them.
//
// One further attempt, on the theory that the add is built inside
// <algorithm>'s copy_backward and so could be re-spelled: replacing that call
// with a hand-written `cb_00476210(_F, _L, _R)` whose body is
// `_BI _D = _R - (_L - _F); while (_F != _L) *--_D = *--_L;` gives 675 bytes
// and 57.6 percent, so the <algorithm> version is closer to the original than
// any re-spelling of it. (Naming it `copy_backward` instead of a unique name
// is a compile error, ambiguous against the header's overloads, so if you try
// this again give it a distinct name.) Given that the clone trick above removed
// <stdexcept> successfully, the remaining one byte looks like another header
// side effect rather than anything reachable from this function's own source.
//
// deepseek-v4.1-flash re-verified the wall. Prepending each of 57 single
// STL/C++ headers (ALGORITHM through XUTILITY, the C++ ones and <windows.h>)
// to this file gives 99.6% or 89.6% and never 100; two (VECTOR, QUEUE) fail
// to compile in that arrangement. Eleven more source variants at the third
// copy all keep the [edx + edi] SIB: `_Q + _M` in a local, `_M + _Q`, a
// static_cast, explicit loops with the source or the destination declared
// first (42% to 51%), an index loop, a cached `_Last` (87.5%), a
// `(const_iterator)` cast, a reordered include block, and `_Ucopy`'s loop
// with a `_p` local (57.1%) or reversed increments. A second
// vector<other>::insert instantiation above this one makes it worse (89.6%,
// 637 bytes). The neighbouring instantiations 0x4758c0/0x475bd0/0x475ef0
// are themselves at 88.9%/99.7%/83.0%, so prefixing them cannot reproduce
// the original translation unit's state either. This is compiler state, as
// the guide says for this family; it needs the regroup-into-original-files
// phase.
//
// deepseek-v4.1-flash (#1401, 2026-09): re-confirmed the same wall. Prepending
// <windows.h> gives 637 bytes / 89.6%; headers.py tried all 128 combinations of
// the seven common headers and none match (best still 99.6%). No new source
// lever: the difference remains the single `lea eax, [edi + edx]` (SIB 0x17)
// against this build's `[edx + edi]` (SIB 0x3a).
// GPT-6.1-sol refinement: following the shared SIB-order finding, declared fresh
// destination/source locals immediately before the third _Ucopy call, destination
// first. The variant remained 99.6% with the same SIB byte; restored the best form.
// deepseek-v4.1 (#2496, 2026-09): the lea is the affine flatten of
// src + dst - _Q - _M*32 and its leaf order is not source reachable. Swapping
// _Ucopy's declared parameter order to (iterator _P, const_iterator _F,
// const_iterator _L) with the calls reordered accordingly collapses to 65.3%
// (645 bytes); ++_F,++_P, `_L != _F`, `*_P = *_F` instead of construct,
// `&_P[0]`, `&_Q[_M]` and a fresh source local all stay 99.6% with the same
// [edx + edi] byte. Still differs: only `lea eax, [edi + edx]` (SIB 0x17)
// against this build's `lea eax, [edx + edi]` (SIB 0x3a).
#include <climits>
#include <memory>
#include <xutility>

struct Elem_00476210 {
    int dwords[8];                     // 0x20 bytes
};

namespace std {

// MSVC 5's <vector> class template, with only the members insert needs. The
// name and the two template parameters have to match the real ones for the
// member to mangle as ?insert@?$vector@UElem_00476210@@...
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

typedef std::vector<Elem_00476210> Vec_00476210;
typedef void (Vec_00476210::*InsertFn_00476210)(
    Vec_00476210::iterator, Vec_00476210::size_type, const Elem_00476210&);

// FUNCTION: 0x476210 ?insert@?$vector@UElem_00476210@@V?$allocator@UElem_00476210@@@std@@@std@@QAEXPAUElem_00476210@@IABU3@@Z
InsertFn_00476210 g_insert_00476210 = &Vec_00476210::insert;
