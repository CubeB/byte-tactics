// Decompiled by space-bunny-free, deepseek-v4.1-flash and GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// deepseek-v4.1 pass (2409): re-verified 546 bytes / 99.6%, still only the
// swapped SIB byte at 0x46e708 (original lea eax,[ebx+ecx], ours
// lea eax,[ecx+ebx]). New measurements, all scored with check.py --sym on
// scratch copies of this exact file, none reached MATCH:
//   * dense padding scan, extern int declarations, every count from 1 to 100
//     and 300 to 420 (the earlier passes stepped by 8): every build is either
//     546/99.6 (same SIB diff) or 547/89.6. The counts do not fall into two
//     clean regimes as the older note says: 313 to 317 alternate between the
//     shapes one count at a time, so the dependence is hash-like, not
//     monotone, and no count in these ranges gives shape A.
//   * 12 padding kinds (extern int, static int, typedef, struct, enum,
//     function body, extern "C", namespace, class, typedef of vector<int>,
//     global int, mixed) crossed with counts 1 to 32: 384 builds, all either
//     99.6 or 89.6, same single-byte diff. 240 further builds with random
//     mixes of those kinds at random counts and at two insertion points
//     (before namespace std and before the typedef): same result.
//   * windows.h, and stdio/stdlib/string/math/time headers, in five
//     positions each: 99.6, the SIB does not flip. The guide's windows.h
//     SIB note does not apply to this lea.
//   * 42 exotic spellings of the third _Ucopy call (source as _P+0,
//     _Last-(_Last-_P), (_P-_Last)+_Last, _First+(_P-_First), _P+(_Q-_Q),
//     dest as _Q+_M, &_Q[_M], _Q+_M+0, _M+_Q, casts on either argument, the
//     result stored back into _Q, and loop forms with a forced temp):
//     anything that keeps 546 bytes keeps exactly this diff.
//   * class layout knob that does matter: moving _Ufill's definition before
//     _Ucopy's flips the build to 547/89.6 without touching the lea, which
//     confirms shape choice is a whole-TU/class-order hash, not a property
//     of the copy expression.
// So the remaining byte is a compiler-state artifact: shape A is not
// reachable from any source spelling of this function in a TU whose only
// content is this template. Best source kept below unchanged.
// GPT-6.1-sol review: fresh check.py run confirms the best source is 99.6%;
// the sole mismatch is the swapped base/index register order in the LEA at
// 0x46e708. Prior notes below record extensive unsuccessful source-level probes.
// std::vector<int>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, _Destroy, fill and copy_backward all
// inlined. Its one caller, 0x46d6c0, walks the 0x5c-byte entries of a
// Class_0046e000 list and pushes, for each of the entry's two std::vector<int>
// members (at +0x4 and +0x14, so _First at entry+8 and +0x18), the value of
// a packet field at +0x6 or +0xa, the count 1, and the member's _Last.
//
// The template is reproduced here rather than included from <vector>, because
// the codegen of the third inlined _Ucopy in the grow path depends on the
// rest of the class and on the include set: the real header (which also
// instantiates rbegin/rend, hence reverse_iterator) emits 547 bytes and a
// four-instruction source pointer, this one emits 546 bytes and the lea the
// original has. Still one byte differs: at 0x46e708 the original has
// `lea eax, [ebx + ecx]` (the source pointer built from the source pointer
// first) and this one has the same lea with the two registers swapped, so
// MSVC 5 built the loop's source start value in the order (dest, source)
// where the original built it (source, dest). Spelling the source start any
// other way in the source, moving a statement, changing the loop's increment
// order, and adding a live local (which demotes this from ebx to ebp) all
// move it further away.
//
// What the byte is, and what is left to try. The third _Ucopy copies
// [_P, _Last) to _Q + _M. MSVC 5 makes the destination the loop's basic
// induction variable (its start is the one-instruction lea ecx,[edx+edi])
// and re-derives the source from it, so the source start is the linear form
// `_P + destIV - _S - _M*4`, emitted as lea, sub, sub. Only the order of the
// two terms in that sum is in question, and the sum is built by the loop
// optimiser, not by the source: everything below was measured and none of it
// moves the byte.
//   * The grow branch on its own, as a free function over the same class,
//     reproduces `lea eax,[ecx+ebx]` exactly (build/scratch/0x46e640/micro),
//     so the choice is made inside this loop and not by the branches, the
//     spill of `this` or anything else in the function.
//   * Spelling: _Q + _M as _M + _Q, &_Q[_M], (_Q + _M), _Q + _M*1,
//     _Q + (int)_M, _Q + (long)_M; _P as _P + 0, *(&_P), &_P[0], (int*)_P,
//     &_Last[0]; a local for either start (that kills the lea and costs a
//     byte, 547); a static __inline accessor for either (no effect at all).
//   * The loop: ++_P,++_F either way round, _P += 1, increments in the body,
//     a while form, _F != _L either way round, _F < _L, a separate IV
//     local, dest-first parameter order, a static _Ucopy, raw int* parameters.
//   * The class: the real header's members bisected one group at a time
//     (build/scratch/0x46e640/vec.cpp), the _Ufill loop's three shapes, and
//     spelling size() once into a local. All 546-byte builds agree on the
//     swapped SIB byte.
//   * Includes: all 128 sets of the seven C headers, all 768 sets of
//     headers.py --cpp, and by hand the real header's own set
//     (<climits> <memory> <stdexcept> <xutility>, 547 bytes) plus
//     <stdexcept>, <climits>, <algorithm>, <xmemory>, <new>, <exception>,
//     <utility>, <typeinfo> and <string> in every position. Nothing matches.
//   * The one lever that does work is global state that has nothing to do
//     with this source: the *number of functions defined in the translation
//     unit* before insert is compiled flips the third _Ucopy between two
//     association shapes. Below one threshold (e.g. one dead static
//     function, or <stdexcept>, or a second std::vector<T> instantiation)
//     it is 547 bytes: `mov eax,ecx; sub eax,edx; add eax,ebx; sub eax,edi`,
//     a different association. Between one and about fourteen units it is
//     546 bytes with the swapped SIB byte, and above fourteen it is 547
//     again. A dead loop-bodied function, fourteen chained inline calls and
//     fourteen nested loops all put it in the 546 regime, so the regimes are
//     countable but the source-anchored one is not among the reachable
//     values. The original's association therefore needs a compiler state
//     this file cannot reproduce, most likely its own big translation unit.
// Claude Sonnet 5.5 pass (#601): the compiler-state probe (N unused `extern int
// dummyK;` lines after the includes, K = 8 to 400 step 8, scored with check.py
// --sym, not committed) is NOT flat here, unlike the other two functions in this
// issue: 546 bytes and 99.6 percent for N = 8 to 56 and again for N = 320 to 400,
// 547 bytes and 89.6 percent for N = 64 to 312. So the third _Ucopy's code does
// depend on how many declarations precede it, in a periodic way, but no N gives
// MATCH (the swapped SIB byte stays). plain headers.py, 128 sets: best 99.6,
// nothing matches, as the notes above say.
// space-bunny-free pass (1875): the 0x4732e0 recipe does NOT reach the SIB
// byte here, and this is now measured, not guessed.
//   * The growth branch's third copy written as an explicit loop in insert,
//     dest declared first, (iterator _D = _Q + _M; const_iterator _C = _P;
//     for (; _C != _Last; ++_D, ++_C) allocator.construct(_D, *_C);), keeps the
//     identical tree (lea ecx,[_Q+_M*4]; lea eax,[dest+_P]; sub; sub; then the
//     two adds and the cmp in the same order) with only the registers renamed,
//     and demotes this from ebx to ebp (_M takes ebx): 541 bytes, 62.8%. So on
//     this function that lever moves the register allocation, not the lea
//     operand order, which is why it took 0x4732e0 from 57.9% to 81.1% and
//     leaves 0x46e640 alone.
//   * A dest-first _Ucopy(iterator _P, const_iterator _F, const_iterator _L)
//     with all four call sites rewritten to match: the same ebx/ebp demotion,
//     526 bytes, 53.0%.
//   * The authentic MSVC 5 STL spellings of the same body (the size_type cast
//     in the first if, begin()/end() in _Ucopy and _Destroy,
//     _Q = _Ucopy(_P, end(), _Q + _M);, deallocate(begin(), end() - begin()))
//     cost 10 bytes, 536, 68.8%: the begin()/end() pair drops two instructions
//     from the grow branch, so the real <vector> header really is not what
//     produced this 546-byte build, and only _First/_Last with a discarded
//     _Ucopy result reproduce it.
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

typedef std::vector<int> Vec_0046e640;
typedef void (Vec_0046e640::*InsertFn_0046e640)(
    Vec_0046e640::iterator, Vec_0046e640::size_type, int const&);

// FUNCTION: 0x46e640 ?insert@?$vector@HV?$allocator@H@std@@@std@@QAEXPAHIABH@Z
InsertFn_0046e640 g_insert_0046e640 = &Vec_0046e640::insert;
