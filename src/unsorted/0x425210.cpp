// Decompiled by Space Bunny Free, finished by LongCat 2.5 Preview Free, verified by GPT-6.1-sol, sixth pass by space-bunny-free, edited by deepseek-v4.1-flash. Names are provisional.
// Ninth pass (deepseek-v4.1-flash, #3897): a live `iterator _P0 = _P;` copy
// passed as the third _Ucopy's source, and a `size_type _N0 = _M;` alias for
// the _Ufill count, are both byte-identical at 544 bytes / 99.6 percent, so
// the one remaining byte at 0x4252d1 (`lea eax, [ebx + ecx]` wanted,
// `[ecx + ebx]` emitted) is not liveness, value-number or copy-propagation
// steered. Moving `_End = _S + _N;` before `_Destroy` regresses to 95.6
// percent. Best shape unchanged.
// Eighth pass (deepseek-v4.1-flash, 2 scratch scores on top of the saved
// 99.6% base): writing the third _Ucopy as the hand loop
// `{ iterator _d = _Q + _M; const_iterator _s = _P; do {} while (_s != _Last); }`
// collapses to 530 bytes / 62.2 percent, and swapping the class member order to
// `iterator _First, _Last, _End; _A allocator;` gives 538 bytes / 75.1 percent,
// so neither the loop spelling (the 0x425480 shape) nor the member offsets
// touch the SIB byte. Restored to the 544-byte / 99.6 percent base below:
// still only `lea eax, [ebx + ecx]` (original) vs `[ecx + ebx]` (ours) at
// 0x4252d1 differs, and no BAD references. The wanted base/index order needs a
// translation-unit state this build cannot reach.
// Tenth pass (mimo-v2.6-pro, 2026-10-01, 60 min): re-verified the 544-byte /
// 99.6 percent base with one fresh check.py run. New measurements this pass,
// all scored via build/scratch/0x425210/scan.py harnessing check.py's own
// compile and compare (several thousand builds, none MATCH and none reaching
// the wanted [ebx+ecx]):
//   * dense TU-state scan (every count, not step 8) of unused extern int
//     padding after the includes: 0-1600 gives exactly two shapes, the 544
//     [ecx+ebx] one (0-59, 317-500 at one-count granularity) and the 545
//     association (60-316, and sparse singles at 380/388/444/452), so the
//     step-8 boundaries hide no third shape. The A/B flip is count-based,
//     not name-based: the prefixes pad425210_/zq_/aardvark_ give identical
//     regimes at identical counts.
//   * other padding kinds: void prototypes to 6000 (the 0x4b6c30 flip scale
//     was 2700-5400 prototypes; only two shapes at any count), class decls,
//     struct decls, typedefs, globals, and dead function definitions.
//   * insertion points: before the includes, after the includes, at namespace
//     std, before the typedef and appended at end of file (only the
//     before-instantiation positions matter; the same two shapes).
//   * #include <windows.h> in front of the file crossed with 200 counts of
//     extern padding: still only the same two shapes (so the guide's
//     windows.h SIB lever does not reach this lea).
//   * the 0x41ace0 technique (no includes at all: operator new/delete
//     hand-declared, own allocator and the header's fill/copy_backward
//     spellings) puts the whole function in the OTHER register family
//     (this in ebp, _M in ebx, 503-504 bytes) at every count 0-400 with and
//     without windows.h, so the no-include state is a family flip, not a
//     SIB lever here.
//   * original instantiation order state: instantiating vector<Class_004c2ea0*>._Destroy
//     and vector<unsigned short>.size before insert (matching the original's
//     emission order 0x4251e0, 0x4251f0, 0x425210) flips the build to the
//     545-byte association in every order tried (destroy+size before and
//     destroy+erase+insert<Class*> after), and a decls-only variant without
//     the size instantiation stays 99.6 percent with the same SIB byte. The
//     state flips at single-declaration granularity (adding just the
//     DestroyFn typedef to the decls block moves it A to B).
//   * two untried spellings of the third _Ucopy body: a value local
//     `value_type _V = *_F; construct(_P, _V)` (558 bytes, 46.4 percent, the
//     temp survives) and a fresh `const_iterator _G = _F` read through
//     (byte-identical 544, same SIB byte).
// Verdict unchanged and reinforced: the wanted `lea eax, [ebx + ecx]` is a
// third state of the optimizer's commutative-operand pick that no TU state
// reachable from this file produces; every reachable state gives either
// [ecx+ebx] in the 544-byte shape or the 545-byte association.
// #2343 retry by GPT-6.1-sol: baseline remains 99.6% after four check.py
// invocations. Restrict, byte-offset, and single-use destination probes left
// the SIB operand order at 0x4252d1 unchanged. No MATCH.
// GPT-6.1-sol refinement: five checks kept 99.6%. Three twin-inspired source
// start expressions scored 57.2%, 61.2% and 66.4%; the exact best was restored.
// Only the SIB operand order at 0x4252d1 remains different. No MATCH.
// Sonnet 5.5 retry (#679), no change to the score (99.6%). What it added:
// the operand order of that sum is fixed by the order in which the inlined
// copy's variables are numbered, which the source controls through the
// parameter order of the inlined _Ucopy (arguments are bound right to left)
// or, written as a loop in the body, the declaration order of its locals. The
// build has four outcomes and no more: destination-first gives this in ebp
// (the 0x425480 family, 53.8% here), and every source-first shape gives one of
// `lea eax, [ecx + ebx]` (this file, 99.6%), the 545-byte `mov eax, ecx; sub
// eax, edx; add eax, ebx` (89.6% to 92.9%) or 99.1% (one extra store of S).
// The wanted `lea eax, [ebx + ecx]` was not produced by: all 6 parameter
// orders per _Ucopy site (1296 site combinations), 12 loop/local forms per
// site in 6000 random combinations of the four sites (inline, helper with the
// destination or the end first, hand-written loops with 2 or 3 locals in any
// declaration order), permuted _Ufill and _Destroy parameters, locals hoisted
// to function scope in all 720 orders, copies of _P for the first _Ucopy, and
// 3000 random files with up to 60 dead declarations (two outcomes only).
// std::vector<unsigned short>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. 0x424c00 calls it from the first inlined resize() of its feature
// type remap table, next to size() (0x4251f0) and erase() (0x425430).
// Taking the member's address makes the compiler emit the template
// instantiation out of line. The class below is MSVC 5's <vector> written out
// (the same shape 0x46e640.cpp uses), which reproduces 543 of the 544 bytes
// where the real <vector> reproduced 540; the insert body is character for
// character the header's, so only the surrounding class differs.
//
// Partial (99.6%, 543 of 544 bytes): every instruction matches, including the
// register allocation (this in ebx, _M in ebp, the opposite of 0x40d020's
// original), both calls to operator new and operator delete, and every one of
// the loops. The single byte left is the SIB byte of one lea, the source start
// of the third _Ucopy, `_Ucopy(_P, _Last, _Q + _M)`, where the operands of the
// sum commute and the values cancel to _P:
//
//   original:  lea eax, [ebx + ecx]   ; _P + dest
//   ours:      lea eax, [ecx + ebx]   ; dest + _P
//
// MSVC 5's loop optimiser keeps the destination as the loop's induction
// variable and re-derives the source from it as _P + (dest - _Q - 2 * _M), so
// the lea is the two addends of that sum in the order the optimiser built them.
// Only the order is in question, and the build has exactly two shapes for it,
// of which the 544-byte one always puts the induction variable first:
//   * this 544-byte shape, `lea eax, [ecx + ebx]`, reached by writing the
//     class out as here. The real <vector> gives the 545-byte shape below.
//   * 545 bytes, `mov eax, ecx; sub eax, edx; add eax, ebx; sub eax, edi`,
//     a different association, reached by the real <vector> (and by
//     0x425210's previous file), by <stdexcept> in front of <memory> and
//     <xutility>, by a second std::vector instantiation, by a real caller
//     that inlines this insert, and by enough unrelated declarations before
//     the class (see below).
// Fourth pass (space-bunny-free, 900 s budget, 1 real check run): the file
// above is unchanged and still scores 99.6%. The guide's advice for this
// register family ("write the third copy as a loop with the destination
// declared first, or a _Ucopy helper with the destination parameter first")
// is measurably WORSE here than the header's own _Ucopy: written out in insert
// as {iterator _D = _Q + _M; const_iterator _S = _P; for (; _S != _Last;
// ++_D, ++_S) allocator.construct(_D, *_S);} it gives 537 bytes and 62.7%,
// with the source declared first 530 bytes and 54.6%, and as a _Ucopy with the
// destination parameter first and every call site's arguments swapped 507 bytes
// and 32.1%. That last figure is the interesting one: 507 bytes is not a
// near-miss, it is the whole function one notch down the register family (this
// in ebp and _M in ebx, the 0x425480 shape the header's comment records at
// 53.8%), and the same collapse happens if _Ucopy's body is a plain store,
// `*_P = *_F`, instead of `allocator.construct(_P, *_F)` (507 bytes, 32.1%).
// So the allocator's placement new is what holds this register family up, and
// the two shapes that would move the byte are 37 bytes away, not one: this file
// is not one byte from a shape the source can reach, it is on a cliff whose
// edges are the two sizes above.
// Nothing in the source reaches the wanted order. Measured, all of it giving
// the same single byte: the destination spelled _Q + _M, _M + _Q, &_Q[_M],
// (_Q + _M), _Q + _M * 1, _Q + (int)_M, &_Q[0] + _M; the source _P, _P + 0,
// *(&_P), &_P[0], (int *)_P, (&_P)[0]; _Ucopy with the increments in either
// order, in the body, as post-increments, in while and do-while forms, with
// const_iterator or iterator parameters, with a local for the destination,
// with the destination as the first parameter, and counting to a length; the
// element type unsigned short, short, a 2-byte struct, a 2-byte class with a
// copy constructor, int, int* and unsigned char; size(), capacity(), begin(),
// end(), _Ucopy, _Ufill and _Destroy emitted out of line in all sixteen
// combinations; an explicit class instantiation; the real header's class with
// 50 of its 68 members deleted; 0 to 30 dead functions, classes, namespaces,
// templates, typedefs, extern variables, unions, enums, static strings and
// virtual classes before and after the class (the shape flips to 545 bytes at
// no fixed count, so it is a hash of the file, not a size); all 128 header sets
// of tools/headers.py and all 768 with --cpp; and /Zp1 /Zp2 /Zp4 /Zp8 /Zp16,
// /G3 to /G7, /Gr /Gd /Gs /Gx /Zc /Os /Ot /Oi /Ob1 and /Ob3. The same byte
// stays wrong for 0x46e640 (vector<int>) and 0x408f30 (vector<Unit*>), so it
// wants the state of the game's own translation unit, as 0x4732e0.cpp says.
// Fifth pass (longcat-2.5-preview-free, 20 check runs): characterised, verdict
// unreachable, same class as 0x46e640. Evidence, all free /Fa screens:
//   * Isolation micro (build/scratch/0x425210/micro): the growth branch alone
//     as a free function over the same class emits the identical 544-byte
//     block, including `lea eax,[ecx+ebx]`. So both the association and the
//     base/index pick are decided inside the third _Ucopy's loop-block
//     transformation, not by the other branches, the spill of `this`, or the
//     tail stores.
//   * In that micro, the 6 parameter orders of the third _Ucopy give exactly
//     two shapes: FLP (the header's own order) -> the 544-byte [ecx+ebx], the
//     other five -> the 545-byte `mov eax,ecx; sub eax,edx; add eax,ebx; sub
//     eax,edi` association. Parameter order flips the association, never the
//     base/index pair inside the 544-byte shape.
//   * TU-state probe, N unused `extern int dummyN;` lines after the includes
//     (N = 0..400, step 8, mirror of the 0x46e640 probe): 0..56 -> 544 bytes
//     [ecx+ebx]; 64..312 -> 545 bytes; 320..400 -> 544 bytes [ecx+ebx]. The
//     periodic two-regime structure is exactly 0x46e640's, and the 544-byte
//     regime ALWAYS puts the induction variable (ecx) first; [ebx+ecx] never
//     occurs at any N.
//   * msvc5-rtm (the unpatched compiler) gives the same [ecx+ebx].
//   * Surrounding live code screens (member order before/after insert, a
//     hoisted _Old = size() local, _Last hoisted to a local, a _P copy for
//     the third copy, _S + (_M + size()) association, a _Sz local): all stay
//     [ecx+ebx]. The live-node "demote one step" lever would change the
//     function's (matching) register allocation, so it is not a route to the
//     wanted byte either.
// Conclusion: the wanted `lea eax,[ebx+ecx]` needs a compiler state this TU
// cannot produce (most likely the state of the game's own big translation
// unit, as 0x4732e0.cpp demonstrated for its function). 99.6% is the ceiling
// for source-reachable shapes here.
// Sixth pass (space-bunny-free, 10 min budget, 0 check.py runs, 10 scratch
// scores): still 99.6%, the same single SIB byte, and the 544-byte regime
// turned out to be a plateau rather than a slope: nine further spellings of
// the third copy and its helpers compile to a byte-for-byte IDENTICAL 544-byte
// block, induction variable first in the lea every time, so the whole
// interesting question is whether anything can leave the plateau at all.
// Measured, all 99.6% with the same one byte: _Ucopy's parameters all plain
// `iterator` instead of `const_iterator`; `static` on _Ucopy; only _F as a
// plain `iterator` and _L left const; _Ucopy's body as a `while` with two
// separate increments instead of the for's comma list; the body spelling as
// explicit placement new `::new ((void *)_P) _Ty(*_F)` instead of
// `allocator.construct(_P, *_F)` (so it is the CALL to construct, not its
// body, that the loop reads); the third copy's destination hoisted into
// `iterator _D = _Q + _M;`; the third copy's source taken as a fresh
// `const_iterator _Ps = _P;`; and `<stdexcept>` in front of `<memory>`, which
// the fifth pass recorded as a way to reach the 545-byte shape but which on
// this file now stays in the 544-byte regime, so that flip is a hash of the
// whole file like the TU-state probe found, not an include order.
// Two spellings do not even compile, which is worth recording so nobody
// retries them: binding the source element to a local reference inside the
// loop (`const _Ty& _V = *_F;` then `construct(_P, _V)`) is rejected with
// C2065 at the use, and an accessor that returns the allocator by reference
// (`_Al().construct(...)`) fails too. The reference-to-a-local lever the
// 0x4a7290 note relies on has no handle here: the third copy's source start
// is already a callee-saved register (ebx) and the induction variable already
// ecx, both exactly as the original, so there is no register split left to
// influence the base/index pick, and with both operands unscaled registers
// the pick is decided by the order the loop optimiser built the affine source
// expression, which no source shape here reaches.
// Eighth pass (deepseek-v4.1-flash, 10 min budget, 0 new check.py runs, 16
// scratch scores): still 99.6%, same single SIB byte, still no BAD
// references. Tested and left on the plateau: the real headers <windows.h>,
// <string.h>, <stdio.h>, <math.h>, <memory.h> and the string/stdio/math
// triple prepended (all 99.6%, [ecx+ebx]); the third copy's source and
// destination as declare-then-assign locals in every combination (99.6%);
// source as a fresh const_iterator local. Consistent with the seventh pass
// verdict: the wanted [ebx+ecx] needs compiler state this TU cannot produce.
// Seventh pass (deepseek-v4.1-flash, 10 min budget, 5 scratch scores, 0 new
// check.py runs on the file): still 99.6%, same single SIB byte. Two more
// spellings tested and both stayed on the 544-byte plateau with
// `lea eax,[ecx+ebx]`: swapping _Ucopy's increments to `++_F, ++_P` (99.6%),
// and commuting the third copy's destination to `_M + _Q` (99.6%). Written
// out as a named `iterator _R = _Q + _M;` before the call also stayed 99.6%,
// and moving the destination increment into the construct argument
// (`allocator.construct(_P++, *_F)`) dropped to 547 bytes / 47.2%. None of
// the source levers move the base/index pick, matching the earlier verdict
// that the wanted `[ebx+ecx]` needs a compiler state this TU cannot reach.
#include <memory>
#include <xutility>

namespace std {
template<class _Ty, class _A = allocator<_Ty> >
class vector {
public:
	typedef vector<_Ty, _A> _Myt;
	typedef _A allocator_type;
	typedef _A::size_type size_type;
	typedef _A::difference_type difference_type;
	typedef _A::pointer _Tptr;
	typedef _A::const_pointer _Ctptr;
	typedef _A::reference reference;
	typedef _A::const_reference const_reference;
	typedef _A::value_type value_type;
	typedef _Tptr iterator;
	typedef _Ctptr const_iterator;

	size_type size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	size_type capacity() const
		{return (_First == 0 ? 0 : _End - _First); }
	iterator begin()
		{return (_First); }
	iterator end()
		{return (_Last); }
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
	iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P)
		{for (; _F != _L; ++_P, ++_F)
			allocator.construct(_P, *_F);
		return (_P); }
	void _Ufill(iterator _F, size_type _N, const _Ty& _X)
		{for (; 0 < _N; --_N, ++_F)
			allocator.construct(_F, _X); }
	_A allocator;
	iterator _First, _Last, _End;
};
}

typedef std::vector<unsigned short> Vec_00425210;
typedef void (Vec_00425210::*InsertFn_00425210)(
    Vec_00425210::iterator, Vec_00425210::size_type, const unsigned short&);

// FUNCTION: 0x425210 ?insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z
InsertFn_00425210 g_insert_00425210 = &Vec_00425210::insert;
