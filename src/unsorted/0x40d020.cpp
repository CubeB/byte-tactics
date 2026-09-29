// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// std::vector<short>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, fill and copy_backward all inlined.
// 0x409160 calls it from the inlined resize() of the vector at +0x7d (with
// size() 0x40d000 and erase() 0x40d240). Taking the member's address makes
// the compiler emit the template instantiation out of line.
//
// Hand-rolled std::vector (the 0x425480 / 0x425210 lever): with the real
// <vector> this build puts `this` in ebx and the count in ebp (57.9%), while
// the original keeps `this` in ebp and _M in ebx. Writing the growth branch's
// third _Ucopy as a loop in the insert body with the destination declared
// before the source (dest = _Q + _M, src = _P) flips that allocation, so the
// prologue now matches exactly (80.5%).
//
// Still different: the registers of the first _Ucopy's source end and of the
// third _Ucopy. The original leaves _P in ecx after the first copy and later
// reuses ecx as the third loop's source induction variable; ours leaves _P in
// edi and builds the third source as (dest + _P) - _Q - _M in eax, with dest
// in ecx where the original has dest in eax (and caches _Last in esi). This is
// the allocator wall the sibling family records (0x425480, 0x4732e0): the
// source-first declaration order and an explicit `const_iterator _l = _Last`
// cache both revert `this` to ebx (57-60%), and the loop-form variants
// (for / if-do-while / pure do-while) all leave the same swap. A pure
// do-while scores 81.1% but drops the original's pre-test (cmp/je before the
// loop), so it is not kept. headers.py does not reach it.
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
			{ iterator _d = _Q + _M; const_iterator _s = _P; for (; _s != _Last; ++_d, ++_s) allocator.construct(_d, *_s); }
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

typedef std::vector<short> Vec_0040d020;
typedef void (Vec_0040d020::*InsertFn_0040d020)(
    Vec_0040d020::iterator, Vec_0040d020::size_type, const short&);

// FUNCTION: 0x40d020 ?insert@?$vector@FV?$allocator@F@std@@@std@@QAEXPAFIABF@Z
InsertFn_0040d020 g_insert_0040d020 = &Vec_0040d020::insert;
