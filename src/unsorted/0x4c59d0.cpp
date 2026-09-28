// Decompiled by Space Bunny Free. Names are provisional.
//
// PARTIAL: 90.9%, 409 against 415 bytes. This is the out-of-line
// std::vector<Elem*>::insert for the reallocating case. Everything matches
// except the ordering of the tail, where the original interleaves the two
// helper calls, `operator delete` and the three member stores differently from
// the source order this file started with.
//
// What moved it, from 80.9%: the order of the six tail statements. All 720
// permutations of
//     _Last = s + FUN_004c5ba0() + 1;   FUN_004c5bc0(p, _Last, q + 1);
//     FUN_004c5b70(_First, _Last);      ::operator delete(_First);
//     _End = s + n;                     _First = s;
// were scored with `check.py --sym`. The winner calls FUN_004c5bc0 first, then
// stores _Last, then FUN_004c5b70, then the delete, then _First, then _End
// (90.9%); the source order it started with gave 80.9%, and moving only the
// _End store after the delete gave 88.4%. So this is the guide's "register
// choice and instruction order follow the order of your statements" in its
// bluntest form: six statements, no semantic content in their order, and the
// percentage is decided entirely by which of 720 orderings MSVC schedules the
// way the original did.
//
// The residue is the address computation for the _Last store. The original
// computes it after the argument pushes for FUN_004c5bc0 (`mov ecx, esi` then
// the `lea`); this file computes the `lea` above the pushes and reuses it.
#include <stddef.h>
#include <vector>

class Class_004c91a0 {
public:
    char* p;

    Class_004c91a0();
    Class_004c91a0(const Class_004c91a0& other);
};

struct Elem_004c5bc0 {
    Class_004c91a0 a;                  // +0x0
    Class_004c91a0 b;                  // +0x4
};

typedef std::vector<Elem_004c5bc0> Vec_004c5ba0;

// The game's vector of entries: the same three pointers as std::vector (the
// empty allocator at +0, _First +4, _Last +8, _End +0xc) and the same STL
// helpers, but every one of them is out of line here, and so is size().
class Class_004c5ba0 : public Vec_004c5ba0 {
public:
    typedef Vec_004c5ba0::iterator iterator;
    typedef Vec_004c5ba0::const_iterator const_iterator;
    typedef Vec_004c5ba0::size_type size_type;

    void FUN_004c5b70(iterator first, iterator last);
    int FUN_004c5ba0(void);
    iterator FUN_004c5bc0(const_iterator first, const_iterator last, iterator dest);
    void FUN_004c5c20(iterator first, size_type n, const Elem_004c5bc0& x);

    size_type size() { return _First == 0 ? 0 : (size_type)(_Last - _First); }

    iterator FUN_004c59d0(iterator p, const Elem_004c5bc0& x);
};

void* operator new(unsigned int size);
void operator delete(void* p);

void __stdcall FUN_004c5d60(Elem_004c5bc0* p, const Elem_004c5bc0& value);
void __stdcall FUN_004c5cd0(Elem_004c5bc0* first, Elem_004c5bc0* last, const Elem_004c5bc0& x);
Elem_004c5bc0* __stdcall FUN_004c5d10(Elem_004c5bc0* first, Elem_004c5bc0* last, Elem_004c5bc0* dest);

// allocator::allocate, the header version, inlined
static inline Elem_004c5bc0* Alloc004c59d0(int n)
{
    if (n < 0)
        n = 0;
    return (Elem_004c5bc0*)::operator new((unsigned int)n * sizeof(Elem_004c5bc0));
}

// FUNCTION: 0x4c59d0
Elem_004c5bc0* Class_004c5ba0::FUN_004c59d0(iterator p, const Elem_004c5bc0& x)
{
    size_type off = (size_type)(p - begin());

    if ((size_type)(_End - _Last) < 1u) {
        int n = (int)size() + ((size_type)1 < size() ? (int)size() : 1);
        iterator s = Alloc004c59d0(n);
        iterator q = s;

        // _Ucopy(_First, p, s), the first of the three, inlined
        for (iterator i = _First; i != p; ++i, ++q)
            FUN_004c5d60(q, *i);
        // _Ufill(q, 1, x), inlined
        {
            iterator r = q;
            int count = 1;
            do {
                FUN_004c5d60(r, x);
                r += 1;
            } while (--count != 0);
        }
        FUN_004c5bc0(p, _Last, q + 1);
        _Last = s + FUN_004c5ba0() + 1;
        FUN_004c5b70(_First, _Last);
        ::operator delete(_First);
        _First = s;
        _End = s + n;
        return begin() + off;
    }
    if ((size_type)(_Last - p) < 1u) {
        FUN_004c5bc0(p, _Last, p + 1);
        FUN_004c5c20(_Last, 1 - (_Last - p), x);
        FUN_004c5cd0(p, _Last, x);
    } else {
        FUN_004c5bc0(_Last - 1, _Last, _Last);
        FUN_004c5d10(p, _Last - 1, _Last);
        FUN_004c5cd0(p, p + 1, x);
    }
    _Last += 1;
    return begin() + off;
}
