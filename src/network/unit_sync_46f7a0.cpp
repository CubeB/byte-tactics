// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<UnitSyncPlayer>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, for the 0x5c-byte element (four vectors and a sub-struct,
// see 0x46eaa0.cpp) of the vector that 0x46dad0 appends to with an inlined
// push_back. The element's copy constructor (0x470390), destructor (0x46ded0)
// and operator= (0x470040) are out-of-line calls. Taking the member's address
// makes the compiler emit the template instantiation out of line.
//
// MATCH (Claude Opus 5.5, #5042). Without /Gi this was stuck at 83.8% for
// eleven passes (notes in git history): the grow arm's third copy was always
// built dest first and _P lost edi. /Gi gives the original's _P-first
// affine, as for its twin 0x46eba0 in this translation unit, but with the
// real header it also leaves the sixth std::_Construct as an out-of-line call
// (52%): that is MSVC's per-function inline budget under /Gi (see
// map_load.cpp). Freeing one expansion fixes it, and which one decides the
// last byte. Writing out a size() (as 0x437580 does) gives 99.7%, with the
// third copy's destination `lea esi, [eax + ebx]` (_M*0x5c as the base);
// which of the last three size() calls is written out, a full header-order
// clone and any one use of
// push_back, operator=, reserve, resize, erase, the destructor or the copy
// constructor leave that byte as it is. Writing out the grow arm's first
// _Ucopy, as below, frees the budget and gives `lea esi, [ebx + eax]` (_Q as
// the base), which matches. Writing out _Destroy (91.7%), arm 2's fill
// (88.7%), the allocate (51.6%), the deallocate (31.3%) or the third _Ucopy
// (40.4%) instead does not. The element must declare its copy constructor and
// operator=, as for 0x437580.
#include <climits>
#include <memory>
#include <xutility>

struct Class_0046eaa0 {
public:
    char unknown_0[0x5c];
    Class_0046eaa0& operator=(const Class_0046eaa0& rhs);
};

class UnitSyncPlayer : public Class_0046eaa0 {
public:
    UnitSyncPlayer(const UnitSyncPlayer& other);
    ~UnitSyncPlayer();
    UnitSyncPlayer& operator=(const UnitSyncPlayer& rhs)
    {
        return (UnitSyncPlayer&)Class_0046eaa0::operator=(rhs);
    }
};

namespace std {

// MSVC 5's vector, cut down to what insert uses; the one difference from the
// header is the grow arm's first _Ucopy, written out as its body (see above).
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
    explicit vector(const _A& _Al = _A())
        : allocator(_Al), _First(0), _Last(0), _End(0) {}
    size_type size() const
        {return (_First == 0 ? 0 : _Last - _First); }
    iterator begin() { return (_First); }
    iterator end() { return (_Last); }
    void insert(iterator _P, size_type _M, const _Ty& _X)
        {if (_End - _Last < _M)
            {size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void *)0);
            iterator _Q = _S;
            for (const_iterator _F = _First; _F != _P; ++_Q, ++_F)
                allocator.construct(_Q, *_F);
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

} // namespace std

typedef std::vector<UnitSyncPlayer> Vec_0046f7a0;
typedef void (Vec_0046f7a0::*InsertFn_0046f7a0)(
    Vec_0046f7a0::iterator, Vec_0046f7a0::size_type,
    const UnitSyncPlayer&);

// FUNCTION: 0x46f7a0 ?insert@?$vector@VUnitSyncPlayer@@V?$allocator@VUnitSyncPlayer@@@std@@@std@@QAEXPAVUnitSyncPlayer@@IABV3@@Z
InsertFn_0046f7a0 g_insert_0046f7a0 = &Vec_0046f7a0::insert;
