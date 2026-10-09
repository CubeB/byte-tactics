// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// Slot 4 of NanoParticles (vtable 0x4fd5b8, family in 0x471cc0.cpp; the rest
// of the class is in nano_particles.cpp, which uses the real <vector>): makes
// room in the std::vector<NanoParticle> at +0xc for five particles a beat,
// then per particle picks a random point in the box at +0x28 around the
// centre at +0x1c and one in the box at +0x40 around the point at +0x34,
// appends a record holding both points, the step that divides the segment
// between them (so a particle moves about 4 units a tick), an expiry and a
// per-particle kind, and finally pushes the clock at +0x8 one tick ahead.
// The expiry is the high half of (length << 16) / 0x40000, so it is 2^16 /
// step ticks away; a zero step drops the particle.
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

void* __cdecl operator new(size_t n);
void __cdecl operator delete(void* p);

extern char* g_game;                   // 0x511de8, frame at +0x38a47

struct Vec3_00473d50 {
    int x;
    int y;
    int z;

    Vec3_00473d50 operator-(const Vec3_00473d50& o) const
    {
        Vec3_00473d50 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    int Length() const
    {
        float fx = x;
        float fy = y;
        float fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

// 16.16 fixed point seen as the short above the short below.
union Fix_00473d50 {
    int whole;
    short half[2];
};

struct NanoParticle {
    Vec3_00473d50 pos;                 // +0x00
    Vec3_00473d50 tgt;                 // +0x0c
    Vec3_00473d50 vel;                 // +0x18
    int unused;                        // +0x24
    int flags;                         // +0x28
    int endTime;                       // +0x2c
};

// The vector's own out-of-line size() (0x475840) under the name
// data/symbols.csv gives that address. It is this vector's first pointer at
// +0x04 and its second at +0x08, so the cast below is a no-op.
class NanoParticleVector {
public:
    unsigned int GetCount() const;
};

namespace std {

// MSVC 5's _Allocate: the negative-count guard is what the insert's
// `jns` before the new[] and its deallocate counterpart come from.
template <class _Ty> inline
_Ty* _Alloc(ptrdiff_t _N, _Ty*)
    {if (_N < 0)
        _N = 0;
    return ((_Ty*)::operator new((size_t)_N * sizeof(_Ty))); }

template <class _Ty> class allocator {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Ty* pointer;
    typedef const _Ty* const_pointer;
    typedef _Ty& reference;
    typedef const _Ty& const_reference;
    typedef _Ty value_type;
    pointer allocate(size_type _N, const void*)
        {return ((pointer)_Alloc((ptrdiff_t)_N, (pointer)0)); }
    void deallocate(pointer _P, size_type)
        {::operator delete((void*)_P); }
};

template <class _FI, class _Ty> inline
void fill(_FI _F, _FI _L, const _Ty& _X)
    {for (; _F != _L; ++_F)
    *_F = _X; }

template <class _BI1, class _BI2> inline
_BI2 copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
    {while (_F != _L)
    *--_X = *--_L;
    return (_X); }

// <vector> with the members the original calls left undefined: the three
// buffers are _First, _Last and _End, behind the empty allocator at +0x00.
// It derives from the size() view, which is the same object, so the out-of-line
// GetCount is reached without a cast (the empty base costs nothing).
template <class _Ty, class _A = allocator<_Ty> > class vector : public NanoParticleVector {
public:
    typedef vector<_Ty, _A> _Myt;
    typedef _A allocator_type;
    typedef _A::size_type size_type;
    typedef _A::difference_type difference_type;
    typedef _A::pointer pointer;
    typedef _Ty* iterator;
    typedef const _Ty* const_iterator;
    typedef _Ty& reference;
    typedef const _Ty& const_reference;
    typedef _Ty value_type;

    _A alloc;                          // +0x00
    iterator _First;                   // +0x04
    iterator _Last;                    // +0x08
    iterator _End;                     // +0x0c

    explicit vector(const _A& _Al = _A())
        : alloc(_Al), _First(0), _Last(0), _End(0) {}

    iterator begin()
        {return (_First); }
    iterator end()
        {return (_Last); }
    // The one size() the original expands rather than calls.
    size_type raw_size() const
        {return (_First == 0 ? 0 : _Last - _First); }
    size_type size() const
        {return (this->GetCount()); }
    void reserve(size_type _N);
    void push_back(const _Ty& _X)
        {insert(end(), 1, _X); }
    void insert(iterator _P, size_type _M, const _Ty& _X)
        {if (_End - _Last < _M)
            {size_type _N = size() + (_M < raw_size() ? size() : _M);
            iterator _S = alloc.allocate(_N, (void*)0);
            iterator _Q = _Ucopy(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            _Ucopy(_P, _Last, _Q + _M);
            _Destroy(_First, _Last);
            alloc.deallocate(_First, _End - _First);
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
    iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P);
    void _Ufill(iterator _P, size_type _N, const _Ty& _X);
    void _Destroy(iterator _F, iterator _L);
};

}

class NanoParticles {
public:
    char unknown_0[4];                          // +0x00
    int deadline;                               // +0x04
    int time;                                   // +0x08
    std::vector<NanoParticle> records;           // +0x0c
    Vec3_00473d50 center;                       // +0x1c
    Vec3_00473d50 radius;                       // +0x28
    Vec3_00473d50 target;                       // +0x34
    Vec3_00473d50 spread;                       // +0x40

    void Emit();
};

// FUNCTION: 0x473d50
void NanoParticles::Emit()
{
    int grow = deadline - *(int*)(g_game + 0x38a47) + 1;
    if (grow > 0)
        records.reserve(records.raw_size() + grow * 5);

    int i = 0;
    for (int n = 5; n != 0; n--) {
        NanoParticle e;
        e.pos.x = (int)(((__int64)rand() * radius.x) / 0x8000) + center.x;
        e.pos.y = (int)(((__int64)rand() * radius.y) / 0x8000) + center.y;
        e.pos.z = (int)(((__int64)rand() * radius.z) / 0x8000) + center.z;
        e.tgt.x = (int)(((__int64)rand() * spread.x) / 0x8000) + target.x;
        e.tgt.y = (int)(((__int64)rand() * spread.y) / 0x8000) + target.y;
        e.tgt.z = (int)(((__int64)rand() * spread.z) / 0x8000) + target.z;

        e.vel = e.tgt - e.pos;
        int len = e.vel.Length();
        Fix_00473d50 step;
        step.whole = (int)(((__int64)len << 16) / 0x40000);
        short s = step.half[1];
        if (s != 0) {
            // Divided straight into e.vel, no named Vec3: fixes the vel store order.
            e.vel.x = e.vel.x / s;
            e.vel.y = e.vel.y / s;
            e.vel.z = e.vel.z / s;
            e.endTime = *(int*)(g_game + 0x38a47) + s;
            e.unused = 0x100;
            e.flags = i % 7 + 0xa1;
            // push_back, not insert(end(), e): the insert position is copied out of ecx.
            records.push_back(e);
        }
        i++;
    }
    time = *(int*)(g_game + 0x38a47) + 1;
}
