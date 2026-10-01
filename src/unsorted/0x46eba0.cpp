// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Sonnet 5.5. Names are provisional.
// PARTIAL 81.4%, 928 of 936 bytes (Sonnet 5.5 retry, #3079; was 81.2% / 924 bytes).
// std::vector<Packet_0046cef0>::insert(iterator, size_type, const T&) of MSVC 5's
// <vector> (the same template as 0x476210 and 0x46f7a0), emitted out of line by
// taking the member's address. Like those files this uses a hand-written clone
// of the vector class template. 14-byte packed element, so the empty allocator
// keeps the three pointers at +4, +8 and +0xc.
// What changed against the previous best: the file used a hand-made class with
// `::operator delete(first)`; the real allocator.deallocate(_First, _End - _First)
// is what produces the original's dead `mov [esp+0x28], eax` before the delete
// call (the inlined deallocate parameter spilled into the dead _X slot), so the
// clone with the real header body is the right base. Of the three-argument
// _Ucopy spellings, the real (_F, _L, _P) order is kept for the two in-place
// branches. The first copy and the tail copy are written destination-first
// (_Ucopy_dst(_P, _F, _L)); scored with 216 helper-order combinations, any
// other order for the tail copy drops to 58 to 79 percent because it flips
// which of `this` and _M gets ebp/ebx, and the order of the first copy does
// not change the score.
// Still differs (all register allocation, same size class as the other
// instantiations of this template):
// 1. The tail copy. The original builds the source start as an affine sum,
//    `(_P - _Q) + (_Q + _M*14) - _M*14` (sub, add, sub, mov eax, esi: 12 bytes
//    this build lacks) which is what the real source-first _Ucopy(_P, _Last,
//    _Q + _M) produces in the clone, but spelling the tail that way also puts
//    `this` in ebx and _M in ebp (939 bytes, 72.9% at best), so dst-first wins.
// 2. In the first loop the original keeps the new buffer's walker in edx and
//    _P in esi, with ebp as the data temp (so `this` is reloaded from
//    [esp+0x10] afterwards); this build puts the walker in ecx.
// 3. The original keeps _N in the dead _M argument slot and _S in the frame
//    slot at [esp+0x18]; this build does the reverse.
// Tried without effect: `if (0 < _N) do {...} while (--_N)` for _Ufill (flips
// _M into ebx in the source-first spelling too, 71.4%, but loses the rest),
// `if (_M)` around the fill, the real <vector> header (940 bytes, 62.7%),
// explicit and member-pointer instantiation, ~1500 random combinations of
// expression-order, size() spelling and helper-order tweaks.
#include <algorithm>
#include <memory>
#include <xutility>

#pragma pack(push, 1)
struct Packet_0046cef0 { // 0xe bytes
    unsigned char type;
    unsigned char arg;
    unsigned int id;
    int field_6;
    int field_a;
};
#pragma pack(pop)

namespace std {

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
            iterator _Q = _Ucopy_dst(_S, _First, _P);
            _Ufill(_Q, _M, _X);
            _Ucopy_dst(_Q + _M, _P, _Last);
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
    iterator _Ucopy_dst(iterator _P, const_iterator _F, const_iterator _L)
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


typedef std::vector<Packet_0046cef0> Vec_0046eba0;
typedef void (Vec_0046eba0::*InsertFn_0046eba0)(
    Vec_0046eba0::iterator, Vec_0046eba0::size_type,
    const Packet_0046cef0&);

void __cdecl FUN_0046ebb0(Vec_0046eba0* v, Packet_0046cef0* p,
                  Vec_0046eba0::size_type n, const Packet_0046cef0& x)
{
    InsertFn_0046eba0 f = &Vec_0046eba0::insert;
    (v->*f)(p, n, x);
}

// FUNCTION: 0x46eba0 ?insert@?$vector@UPacket_0046cef0@@V?$allocator@UPacket_0046cef0@@@std@@@std@@QAEXPAUPacket_0046cef0@@IABU3@@Z