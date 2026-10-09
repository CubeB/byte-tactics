// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// Stays in its own file: it is built with /Gi, and its hand-written cut-down
// std::vector cannot share unit_sync.cpp's real <vector>.
// std::vector<UnitSyncPlayer>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, for the 0x5c-byte element (four vectors and a sub-struct,
// see unit_sync.cpp) of the vector that 0x46dad0 appends to with an
// inlined push_back. The element's copy constructor (0x470390), destructor
// (0x46ded0) and operator= (0x470040) are out-of-line calls.
#include <climits>
#include <memory>
#include <xutility>

struct SyncPlayerRecord {
public:
    char unknown_0[0x5c];
    SyncPlayerRecord& operator=(const SyncPlayerRecord& rhs);
};

// Must declare its copy constructor and operator=: the original calls both.
class UnitSyncPlayer : public SyncPlayerRecord {
public:
    UnitSyncPlayer(const UnitSyncPlayer& other);
    ~UnitSyncPlayer();
    UnitSyncPlayer& operator=(const UnitSyncPlayer& rhs)
    {
        return (UnitSyncPlayer&)SyncPlayerRecord::operator=(rhs);
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
            // Written out instead of _Ucopy: frees inline budget for a later call.
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
