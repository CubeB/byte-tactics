// Decompiled by Space Bunny Free. Names are provisional.
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
