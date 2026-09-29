// Decompiled by space-bunny-free. Names are provisional.
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
			iterator _Q = _Ucopy_i(_First, _P, _S);
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
	iterator _Ucopy_i(const_iterator _F, const_iterator _L, iterator _P)
		{for (; _F != _L; ++_P, ++_F)
			allocator.construct(_P, *_F);
		return (_P); }
	void _Destroy(iterator _F, iterator _L);
	iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P);
	void _Ufill(iterator _F, size_type _N, const _Ty& _X);
	_A allocator;
	iterator _First, _Last, _End;
};
}

#pragma pack(push, 1)
struct Elem_0046faf0 {
	unsigned char type;
	unsigned char arg;
	int id;
	int field_6;
	int field_a;
};
#pragma pack(pop)

extern "C" int __cdecl FUN_0044fe00();
extern "C" void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

class Class_0046cc10 {
public:
	int field_0;
	char unknown_4[8];
	std::vector<Elem_0046faf0> vec;
	void FUN_0046cc10(Elem_0046faf0* param_1, Elem_0046faf0* param_2);
};

// FUNCTION: 0x46cc10
// The local reference below is load bearing: writing vec.insert(vec.end(), ...)
// directly makes the front end keep the end() load rooted at ecx+0x14, which
// blocks the load CSE with the insert's own [esi + 8] read of _Last and costs
// the original's single `mov edi, ecx` in the capacity block.
void Class_0046cc10::FUN_0046cc10(Elem_0046faf0* param_1, Elem_0046faf0* param_2)
{
	param_2->id = ++field_0;
	std::vector<Elem_0046faf0>& _v = vec;
	_v.insert(_v.end(), 1, *param_2);
	FUN_00451bc0(FUN_0044fe00(), (unsigned int)param_1, param_2, 0xe);
}
