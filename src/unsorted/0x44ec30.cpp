// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::vector<Elem_0044ec30>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward inlined.
// Elem_0044ec30 is the 4-byte element (two unsigned shorts) the only caller
// builds: 0x44da00 pushes the vector's end(), the count 1 and the address of
// a stack temporary whose two fields are written as adjacent WORDs, on the
// vector at +0x8 of its object. Taking the member's address makes the
// compiler emit the template instantiation out of line.
//
// Partial (99.6%, 546 of 546 bytes, one SIB byte). The body is
// instruction-for-instruction identical to the original, including the
// 4-byte element size, the register assignment (this in ebx, _M in ebp),
// both calls to operator new and operator delete, and every loop. The one
// byte left is the SIB of the lea that starts the source pointer of the
// third _Ucopy, `_Ucopy(_P, _Last, _Q + _M)`, whose terms commute and cancel
// to _P:
//
//   original:  lea eax, [ebx + ecx]   ; _P + dest
//   ours:      lea eax, [ecx + ebx]   ; dest + _P
//
// MSVC 5 makes the destination the loop's induction variable and re-derives
// the source from it as `_P + dest - _Q - _M*4`; only the order of the two
// addends is in question, and the loop optimiser builds it, not the source.
// This is the same one-byte wall as 0x408f30.cpp (the identical body for
// vector<Unit*>, which sits at 88.7% built from the real header), 0x425210.cpp
// and 0x46e640.cpp (both 99.6%). Those files record the search that ruled
// out reaching the order from source: every spelling of the destination and
// the source, every _Ucopy/_Ufill/_Destroy out-of-line combination, the class
// bisected member by member, all 128 header sets of tools/headers.py and all
// 768 with --cpp, /Zp and /O variants, and filler declarations. The real
// <vector> gives the 547-byte `mov eax,ecx; sub eax,edx; add eax,ebx; sub
// eax,edi` association instead; writing the class out (as below) gives the
// 546-byte shape with the swapped SIB. The wanted order needs the compiler
// state of the game's own translation unit, not this file.
//
// The class body below is MSVC 5's <vector> written out, exactly as
// 0x425210.cpp and 0x46e640.cpp do, because the real header emits the
// 547-byte shape.
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

struct Elem_0044ec30 { unsigned short a; unsigned short b; };
typedef std::vector<Elem_0044ec30> Vec_0044ec30;
typedef void (Vec_0044ec30::*InsertFn_0044ec30)(
    Vec_0044ec30::iterator, Vec_0044ec30::size_type, const Elem_0044ec30&);

// FUNCTION: 0x44ec30 ?insert@?$vector@UElem_0044ec30@@V?$allocator@UElem_0044ec30@@@std@@@std@@QAEXPAUElem_0044ec30@@IABU3@@Z
InsertFn_0044ec30 g_insert_0044ec30 = &Vec_0044ec30::insert;
