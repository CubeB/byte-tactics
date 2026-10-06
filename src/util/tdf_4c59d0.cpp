// Decompiled by space-bunny-free, deepseek-v4.1-flash, deepseek-v4.1, mimo-v2.6-pro, claude-sonnet-5-5 and Haiku. Names are provisional.
// std::vector<TdfField>::insert(iterator, const Elem&)
// from MSVC 5's <vector> (insert(_P, 1, _X) inlined into insert(_P, _X)), for
// the reallocating, shift-up and in-place arms. It is the out-of-line
// instantiation the 0x4c54f0 map code calls.
//
// What the earlier passes (stuck at 93.3 percent for ten retries) were
// missing, found by claude-sonnet-5-5 in issue 4641:
//  * The tail of the reallocating arm is the header's own order,
//        _Destroy(_First, _Last);
//        alloc.deallocate(_First, _End - _First);
//        _End = _S + _N; _Last = _S + size() + _M; _First = _S;
//    and the function ends in ONE shared `return begin() + _O;` after the
//    whole if / else if / else chain (arms 2 and 3 do their own `_Last += 1`).
//    Both were already known, but together with the next item they were
//    never tried in the same file: the "authentic" order alone scored 84.2.
//  * The deallocation must be the header's allocator call with its unused
//    second argument, `alloc.deallocate(_First, _End - _First)`, a real
//    member that discards the count. With a bare `::operator delete(_First)`
//    the delete argument lands in edx and the whole tail rotates its
//    registers (n, the _End sum and the _Last sum), which is what the old
//    "allocation tie" notes were chasing. The dead `_End - _First` costs no
//    code but keeps the register order.
//  * The vector is a cut-down hand-written std::vector in namespace std
//    (as in 0x470c10) whose _Ucopy, _Ufill and _Destroy are protected and only
//    declared. That keeps them out-of-line calls and gives them the exe's
//    mangled names, so check.py's reference column is ok for them. size() is
//    the header's inline one; the last use calls the out-of-line copy at
//    0x4c5ba0 (the inline budget ran out there in the original).
//  * The first _Ucopy and the one-element _Ufill are written out as loops
//    over FUN_004c5d60 (inlined in the original); the rest are calls.
#include <stddef.h>

class Class_004c91a0 {
public:
    char* p;

    Class_004c91a0();
    Class_004c91a0(const Class_004c91a0& other);
};

struct TdfField {
    Class_004c91a0 a;                  // +0x0
    Class_004c91a0 b;                  // +0x4
};

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// A cut-down MSVC 5 <vector>: only the members this function reaches. The
// three helpers are declared and not defined, so they stay out-of-line calls
// with the exe's mangled names (std::vector<Elem>::_Ucopy and friends).
namespace std {
    template<class _Ty> inline
    _Ty* _Allocate(ptrdiff_t _N, _Ty*)
        {if (_N < 0)
            _N = 0;
        return ((_Ty*)operator new((size_t)_N * sizeof(_Ty))); }

    template<class _Ty> class allocator {
    public:
        typedef size_t size_type;
        typedef _Ty* pointer;
        pointer allocate(size_type _N, const void*)
            {return ((pointer)_Allocate((ptrdiff_t)_N, (pointer)0)); }
        void deallocate(void* _P, size_type)
            {operator delete(_P); }
    };

    template<class _Ty, class _A = allocator<_Ty> > class vector {
    public:
        typedef _A::size_type size_type;
        typedef _Ty* iterator;
        typedef const _Ty* const_iterator;
        iterator begin()
            {return (_First); }
        size_type size() const
            {return (_First == 0 ? 0 : _Last - _First); }
    protected:
        iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P);
        void _Ufill(iterator _P, size_type _N, const _Ty& _X);
        void _Destroy(iterator _F, iterator _L);
        _A alloc;
        iterator _First;
        iterator _Last;
        iterator _End;
    };
}

// The game's vector of entries. size() is also reached through the out-of-line
// copy at 0x4c5ba0 for the last use below.
class Class_004c5ba0 : public std::vector<TdfField> {
public:
    int FUN_004c5ba0(void);

    iterator FUN_004c59d0(iterator p, const TdfField& x);
};

void __stdcall FUN_004c5d60(TdfField* p, const TdfField& value);
void __stdcall FUN_004c5cd0(TdfField* first, TdfField* last, const TdfField& x);
TdfField* __stdcall FUN_004c5d10(TdfField* first, TdfField* last, TdfField* dest);

// FUNCTION: 0x4c59d0
TdfField* Class_004c5ba0::FUN_004c59d0(iterator p, const TdfField& x)
{
    size_type off = (size_type)(p - begin());

    if ((size_type)(_End - _Last) < 1u) {
        size_type n = size() + ((size_type)1 < size() ? size() : 1);
        iterator s = alloc.allocate(n, (void*)0);
        iterator q = s;
        for (iterator i = _First; i != p; ++i, ++q)
            FUN_004c5d60(q, *i);
        {
            iterator r = q;
            size_type count = 1;
            do {
                FUN_004c5d60(r, x);
                r += 1;
            } while (--count != 0);
        }
        _Ucopy(p, _Last, q + 1);
        _Destroy(_First, _Last);
        alloc.deallocate(_First, _End - _First);
        _End = s + n;
        _Last = s + FUN_004c5ba0() + 1;
        _First = s;
    } else if ((size_type)(_Last - p) < 1u) {
        _Ucopy(p, _Last, p + 1);
        _Ufill(_Last, 1 - (_Last - p), x);
        FUN_004c5cd0(p, _Last, x);
        _Last += 1;
    } else {
        _Ucopy(_Last - 1, _Last, _Last);
        FUN_004c5d10(p, _Last - 1, _Last);
        FUN_004c5cd0(p, p + 1, x);
        _Last += 1;
    }
    return begin() + off;
}
// The original calls this from 0x4c59d0 rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4c5ba0
int Class_004c5ba0::FUN_004c5ba0(void)
{
    if (_First == 0) {
        return 0;
    }
    return _Last - _First;
}
#pragma auto_inline(on)
