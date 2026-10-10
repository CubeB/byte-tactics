// Decompiled by Claude Opus 5.5, Sonnet, Haiku, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<MapCacheEntry>::insert(iterator, size_type, const T&), for the
// 8-byte {string handle, int} element of the static vector at 0x5122c0 (see
// map_list.cpp); its only caller, 0x4373a0, does a push_back. The element's
// copy constructor (0x437820) and operator= (0x437800) are out-of-line calls.
#include <climits>
#include <memory>
#include <xutility>

// The reference-counted string handle: a pointer to the characters with the
// reference count in the int just before them. The views in this file are the
// copy constructor 0x4c91a0, the assignment 0x4c93b0 and ReleaseRef 0x4c9390.
class StringRef {
public:
    char* ptr;                         // +0x0, count in the dword before

    StringRef(const StringRef& other);
    StringRef(const char* text);
    StringRef(const char* text, int len);
    ~StringRef() { ReleaseRef(); }
    void ReleaseRef();
    void Assign(const StringRef& param);
    StringRef* Append(const StringRef& other);
    StringRef* MakeLower();
    StringRef* MakeUpper();
    StringRef* AssignText(const char* text);
    char* GetUnique();
    int IsEmpty() const;
    StringRef SubString(int start, int end) const;
};

// Declares its own copy constructor and operator=: implicit ones get inlined into insert.
class MapCacheEntry {
public:
    StringRef handle;                  // +0x0
    int tntChecksum;                   // +0x4

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
            // size() is written out here: it frees inline budget for the copies.
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

// Out of line, as insert calls them: with inlining they are expanded into it.
#pragma auto_inline(off)
// FUNCTION: 0x437800
MapCacheEntry& MapCacheEntry::operator=(const MapCacheEntry& other)
{
    ((StringRef*)this)->Assign(other.handle);
    tntChecksum = other.tntChecksum;
    return *this;
}

// FUNCTION: 0x437820
MapCacheEntry::MapCacheEntry(const MapCacheEntry& other)
    : handle(other.handle), tntChecksum(other.tntChecksum)
{
}
#pragma auto_inline()
