// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
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
// State, not source: instantiating any second vector<T>::insert in the same file
// (33 different element types were tried, before or after) flips this function
// from the 99.7% shape to the 91.5% one, while unrelated functions do not. So
// the original's shape (and the three different shapes of the neighbouring
// instantiations 0x4758c0 and 0x475ef0) depends on what else the original
// translation unit had compiled; it needs the regroup-into-original-files phase.
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
