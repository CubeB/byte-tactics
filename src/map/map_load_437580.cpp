// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<MapCacheEntry>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, for the 8-byte {string handle, int} element of the
// static vector at 0x5122c0 (see 0x434a30.cpp); its only caller, 0x4373a0,
// does a push_back. The element's copy constructor (0x437820) and operator=
// (0x437800) are out-of-line calls. Taking the member's address makes the
// compiler emit the template instantiation out of line.
//
// BYTES MATCH (Claude Opus 5.5, #4416). check.py still reports the three
// operator= calls (fill and copy_backward, +0x1ec, +0x241, +0x25f) as a wrong
// reference: data/symbols.csv names 0x437800 `Class_00437800::Class_00437800`,
// but it is this element's operator= (??4Class_00437820@@QAEAAV0@ABV0@@Z):
// it assigns the handle through Assign, copies field_4 and returns
// *this, and insert calls it on existing elements. It needs a data/aliases.csv
// row (or the rename), not a source change.
//
// How it matched. Without /Gi the grow arm's third copy is always built dest
// first and _P loses edi (79.3% was the best in eight earlier passes, notes
// in git history). /Gi gives the _P-first affine the original has (as for
// 0x46eba0 and the 4-byte inserts of field-notes Part 7), but with the real
// header it also leaves the sixth std::_Construct (the third arm's first
// _Ucopy) as an out-of-line call (34.7 to 46%). That is MSVC's per-function
// inline budget, not anything specific to _Construct: under /Gi, deleting
// any one earlier expansion (arm 2's fill, _Destroy, or one of the size()
// calls) lets it inline, and copy_backward or fill after it do not matter.
// Writing out one of the four inlined size() calls as its body (any of the
// three that are not the first operand of _N; the first changes the order _N
// is evaluated in) frees enough, and the whole function then matches with
// header-order helpers. The element must declare its copy constructor and
// operator=: with implicit ones the freed budget inlines operator= into
// fill and copy_backward.
#include <climits>
#include <memory>
#include <xutility>

class Class_004c9390 {
public:
    char* data;
    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* ptr;
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

class MapCacheEntry {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4

    MapCacheEntry(const MapCacheEntry& other);
    MapCacheEntry& operator=(const MapCacheEntry& other);
};

namespace std {

// MSVC 5's vector, cut down to what insert uses; the one difference from the
// header is the written-out size() in the grow arm's final _Last (see above).
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
            iterator _Q = _Ucopy(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            _Ucopy(_P, _Last, _Q + _M);
            _Destroy(_First, _Last);
            allocator.deallocate(_First, _End - _First);
            _End = _S + _N;
            _Last = _S + (_First == 0 ? 0 : _Last - _First) + _M;
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

typedef std::vector<MapCacheEntry> Vec_00437580;
typedef void (Vec_00437580::*InsertFn_00437580)(
    Vec_00437580::iterator, Vec_00437580::size_type, const MapCacheEntry&);

// FUNCTION: 0x437580 ?insert@?$vector@VMapCacheEntry@@V?$allocator@VMapCacheEntry@@@std@@@std@@QAEXPAVMapCacheEntry@@IABV3@@Z
InsertFn_00437580 g_insert_00437580 = &Vec_00437580::insert;
