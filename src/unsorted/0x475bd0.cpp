// Retry (deepseek-v4.1-flash, issue 3795): still 99.7, the same single SIB byte
// at 0x475d01. Three more spellings of the second arm's third _Ucopy
// destination (a local `iterator _D = _P + _M;`, `&_P[_M]` and `_M + _P`) are
// all byte-identical to the file best, so the mirror byte is not reachable from
// the destination expression. Best version (99.7) kept.


// the same single mirror lea byte at 0x475d01: _Ucopy with all three parameters
// declared iterator (const dropped from _F/_L) and `iterator _Ps = _P;` used
// only by the third copy of the realloc arm. Neither moves the operand order, so
// the constness of the copy bounds and a fresh third-copy source local are both
// ruled out; the best version (99.7) is kept.

// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash retry (#3691): still 99.7, the same single mirror lea byte
// in the second arm (`_Last - _P < _M`): at 0x475d01 the original emits
// `lea eax,[esi+edx]` where esi and ebx are duplicate _P loads and edx =
// _P + _M*0x3c, ours emits `lea eax,[edx+esi]`. Binding that _Ucopy's
// destination to an `iterator _R = _P + _M;` local is byte-identical (99.7,
// 791 bytes, same byte), so the arm-2 tree operand order is not set by a named
// destination either.
// deepseek-v4.1-flash retry (#3483): still 99.7, the same single lea SIB byte
// (original `lea eax,[esi+edx]`, ours `lea eax,[edx+esi]` at 0x475d01).
// Re-measured the `_M + _Q` spelling of the third _Ucopy destination: the
// emitted code and the diff are byte-identical to the file best, so the operand
// order inside that reassociated induction add is not a caller-visible left/right
// expression-order artifact either.
// Retry 1 (GPT-6.1-sol, issue 3129): the destination-first inline _Ucopy loop
// lowered similarity to 72.0% through broad register/frame changes. Restored
// the 99.7% best; only remaining mismatch is the source pointer lea SIB
// operand order documented below.
// Retry 7 (deepseek-v4.1-flash, issue 2932): no change, 99.7%, the same single
// SIB byte (original `lea eax,[esi+edx]`, ours `lea eax,[edx+esi]`). New
// measurements via check.py --sym: changing the _Ucopy loop shape (increment
// order swapped, split increment into the body, while-body, `_P++`) all stay
// 99.7, so the operand order is not set by the helper's body. A destination-
// first helper `_Ucopy3(_Q+_M, _P, _Last)` (for/while) is 75.1. Count-based
// _Ucopy loops (`_C = _L - _F` as size_type or difference_type, for/while) all
// collapse to 27.3. The sibling 0x4758c0 explicit-fill trick in insert is 95.7
// here. None of the ten third-argument spellings (iterator/static_cast casts,
// `_M*1`, `1*_M`, `&*_P`) moved the byte. This remains the compiler-state SIB
// case: the SIB base/index assignment inside the optimizer's reassociated copy
// is not reachable from any caller-visible spelling, and the file needs the
// regroup-into-original-files phase. Best version (99.7) kept. 
// Retry 6 (deepseek-v4.1-flash, issue 2826): no change, 99.7%, the same one SIB
// byte ([esi+edx] original, [edx+esi] ours). New measurements with check.py
// --sym: any second std::vector<T>::insert instantiation in this file (1 to 4
// extra address-taken instantiations, placed before or after the target) flips
// the whole reallocation arm to the 91.5% four-instruction source pointer, so
// rebuilding the immediately preceding sibling 0x4758c0 above this one is not
// the answer. Unused function prototypes at N = 0, 100, ..., 3200 alternate the
// same two shapes (99.7 / 91.5), never a third, matching the earlier extern-int
// sweep. Thirteen more spellings of the third _Ucopy (destination and _M locals,
// casts, &_Q[_M], iterator(...), manual element-pointer loops) are all 99.7% or
// worse (manual loops 72%). The SIB base/index order is chosen after the
// optimizer's reassociation of the strength-reduced copy and no caller-visible
// spelling reaches it; this needs the regroup-into-original-files phase.
// Retry 5 (deepseek-v4.1-flash, issue 2784): still 99.7%, the same one SIB
// byte. Three more variants, all measured with check.py --sym: replacing
// _Ufill in the reallocation arm with an explicit `for (_C = _M; 0 < _C;
// --_C, ++_Q) construct(_Q, _X)` loop (the trick that moved sibling
// 0x4758c0) shifts the whole arm and scores lower; showing the third copy's
// destination (`iterator _R = _Q + _M;`) or its source (`iterator _R = _P;`)
// in a local after _Ufill leaves the exact same 99.7 one-byte diff. The SIB
// base/index order is chosen during the optimizer's reassociation of the
// strength-reduced copy, not by any spelling of the call, so this file needs
// the regroup-into-original-files phase.
// Retry 4 (deepseek-v4.1-flash, issue 2521): still 99.7%, one instruction. The
// remaining diff is the identical one described below: the original's third
// copy source pointer is `lea eax, [esi + edx]`, this build emits
// `lea eax, [edx + esi]` (same sum, base/index swapped). Adding one header at
// the top (<windows.h>, <stdio.h>, <string.h>, <math.h>, <stdlib.h>, <stddef.h>,
// <memory.h>, <xmemory>, <algorithm>) kept the function at 99.7% with the same
// diff, consistent with the earlier 225/3800 header sweep. No new source
// spelling was found; this is the compiler-state SIB case from the guide and
// needs the regroup-into-original-files phase.
// Retry 2 (deepseek-v4.1, issue 1186, 17 more check.py runs): the mirror lea is
// immune to the destination spelling. Still 99.7% with the identical diff for:
// _Q + 1 * _M, _Q + _M + 0, this->_Last, static_cast<iterator>(_Q + _M),
// iterator _R = _Q; _R += _M; _Ucopy(_P, _Last, _R), _R = _Q + _M declared before
// _Ufill, a size_type _MM local copy of _M, a local copy of _P used for the first
// and third copies, and a while-loop _Ucopy. Swapping _Ufill and the third copy
// is 51.4% (register allocation changes), _R before _Ufill is 74.7%. The lea's
// base/index order is decided inside the optimizer's reassociated copy loop
// (source = dest + (_P - (_Q + _M))), not by anything the caller can spell, so
// this file needs the original translation unit's other instantiations.
// Also 99.7% with the identical diff (runs 14 to 17): the loop guard reversed
// (_L != _F, 98.3% instead, it reorders the compares), the increments swapped
// (++_F, ++_P), and _Ufill's decrement before the increment. Writing the copy
// as `*_P = *_F` instead of allocator.construct collapses the whole arm to
// 49.9%: the rep movsd that the placement-new construct inlines is what the
// optimizer's reassociated source-pointer tree is built around.
// std::vector<Record_00475bd0>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward inlined. The
// vector's 0x3c-byte element is the frame record of 0x4743a0 (the exe's only
// caller passes end(), 1 and a record). Taking the member's address emits the
// template instantiation out of line, as the original file did.
//
// PARTIAL (99.7%, 791 of 791 bytes): one operand order is left. In the third
// _Ucopy of the reallocation branch the original computes the source pointer
// as `lea eax, [esi + edx]; sub eax, ebx; sub eax, ecx`; this build emits
// `lea eax, [edx + esi]`, the same bytes with the two lea registers swapped.
//
// What moved this from 91.5% (792 bytes) to 99.7%: the vector is written out
// below instead of `#include <vector>`. Merely adding `#include <stdexcept>`
// (or <string>) in front of the hand-written class flips the file back to the
// 91.5% shape (`mov eax,edx; sub eax,ebx; add eax,esi; sub eax,ecx`), so the
// real header is not a faithful stand-in for the original's state.
//
// Measured with check.py --sym, none of it moved the last lea (each is 99.7%):
// the six parameter orders of _Ucopy (and of a separate helper used only for the
// third copy, with for/while/reversed-compare loops), the third copy written as
// explicit loops with _d or _s declared first (72% and 60%: this and _M swap
// registers), the destination in a local, _Q/_S/_N declared at the top or split,
// _M + _Q and &_Q[_M], iterator/const_iterator/_Ty* parameter types, every
// member order of the class (400 shuffles), removing capacity/begin/end and
// each typedef (512 subsets), adding resize/push_back/insert(_P, _X) and other
// members of the real header, 225 single and 3800 multi-header prefixes (the
// result is only ever 99.7% or 91.5%), 400 random names for the typedefs and
// the global, 600 random 60-byte element layouts (arrays, mixed widths,
// pointers, floats), and a statement-order hill climb over all three arms.
//
// Retry (deepseek-v4.1, issue 1186): 12 more spellings of the third call and of
// _Ucopy, each still 99.7% with the identical one-instruction diff: increment
// order swapped, `&_Q[_M]`, a named destination local, `_M + _Q`,
// `_Q + size_type(_M)`, `&*(_Q + _M)`, `_P + 0`, a local copy of _P, a local copy
// of _Q, a local copy of _M, and `_Ucopy(_P3, _Last, _Q + _M)` with a
// const_iterator local. Also tried: `_Ucopy(_P, end(), _Q + _M)` (71.7%),
// `_Ucopy(_First + (_P - _First), ...)` and `_Ucopy(_Last - (_Last - _P), ...)`
// (both 53.3%, they change the whole register allocation), and
// `_S + size_type(_P - _First) + _M` as the destination (72.8%, 820 bytes).
// Same TU-state SIB wall as 0x44ec30, 0x408f30, 0x425210, 0x46e640 and 0x476210.
// State, not source: instantiating any second vector<T>::insert in the same file
// (33 different element types were tried, before or after) flips this function
// from the 99.7% shape to the 91.5% one, while unrelated functions do not. So
// the original's shape (and the three different shapes of the neighbouring
// instantiations 0x4758c0 and 0x475ef0) depends on what else the original
// translation unit had compiled; it needs the regroup-into-original-files phase.
// Retry 3 (deepseek-v4.1, issue 2393): the last SIB byte is a compiler state
// toggle and the toggle is a plain declaration count, but only two shapes are
// reachable. Padding the file at file scope with K dummy declarations flips
// this function between exactly two forms: K in the first window (0 to 49)
// gives the 99.7 percent shape above, K from 50 on gives the 91.5 percent
// four-instruction source pointer (mov eax, ecx / sub eax, edx / add eax, ebx
// / sub eax, edi). Borders at K = 50, 320, 576, 832, 1088, 1344, 1600, so the
// pad alternates with period 256 above the first border. The same two shapes
// for dummy typedefs, dummy static ints and dummy one-line functions, and for
// padding before the class or after the global. The original's third form
// (lea eax, [esi + edx] plus the subs) never appears at any K, so it is not a
// declaration-count state: the file still needs the regroup-into-original-files
// phase. Also 99.7 with the identical diff this pass: iterator(_Q + _M),
// &_Q[_M] and (_Q) + (_M) as the third copy's destination.
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



struct Record_00475bd0 {
    int field_00;
    int field_04;
    int field_08;
    int field_0c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
    int field_28;
    int field_2c;
    int field_30;
    int field_34;
    int field_38;
};

typedef std::vector<Record_00475bd0> Vec_00475bd0;
typedef void (Vec_00475bd0::*InsertFn_00475bd0)(
    Vec_00475bd0::iterator, Vec_00475bd0::size_type, const Record_00475bd0&);

// FUNCTION: 0x475bd0 ?insert@?$vector@URecord_00475bd0@@V?$allocator@URecord_00475bd0@@@std@@@std@@QAEXPAURecord_00475bd0@@IABU3@@Z
InsertFn_00475bd0 g_insert_00475bd0 = &Vec_00475bd0::insert;
