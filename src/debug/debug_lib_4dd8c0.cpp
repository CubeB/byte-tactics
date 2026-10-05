// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// std::vector<Elem, Alloc>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector> (the body at lines 152-170), emitted out of line by taking the
// member's address, the same family as 0x408f30 / 0x40d020 / 0x40d290.
//
// The real header, not a hand-rolled copy, is what fixes the register family:
// the original keeps `this` in esi, n in edi, _Last in ebx and the byte count in
// ebp, and the plain instantiation reproduces all of that (the hand-written
// version in the previous revision rotated the saved registers and scored 32%).
//
// The object layout is the header's own: the empty allocator byte sits at +0
// (padded to 4), so _First/_Last/_End land at +4/+8/+0xc. The allocator is the
// game's GlobalAlloc/GlobalFree one, retrying through the out-of-memory handler
// at DAT_005289bc; its deallocate tests the pointer before freeing, which is the
// last instruction block the match needed (without the `if (_P != 0)` the
// function was four bytes short and the _First spill was scheduled after the
// push instead of before the branch).
//
// Previously suspected as a bug: the in-place path where n > (_Last - _P) copies
// the tail forward with _Ucopy(_P, _Last, _P + _M). It is safe, because that
// branch guarantees _P + _M > _Last so the ranges do not overlap, and the other
// in-place path uses copy_backward. This is the standard library's own code, so
// nothing here looks like a Cavedog mistake.
#include <windows.h>
#include <vector>

extern void (*DAT_005289bc)();

struct Elem_004dd8c0 {
    unsigned int w[0xc];               // +0x0, 0x30 bytes
};

class Alloc_004dd8c0 {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef Elem_004dd8c0* pointer;
    typedef const Elem_004dd8c0* const_pointer;
    typedef Elem_004dd8c0& reference;
    typedef const Elem_004dd8c0& const_reference;
    typedef Elem_004dd8c0 value_type;

    pointer allocate(size_type _N, const void* = 0)
    {
        pointer _P;
        do {
            _P = (pointer)GlobalAlloc(0, _N * sizeof(value_type));
            if (_P == 0 && DAT_005289bc != 0)
                DAT_005289bc();
        } while (_P == 0 && DAT_005289bc != 0);
        return _P;
    }
    void deallocate(pointer _P, size_type)
    {
        if (_P != 0)
            GlobalFree(_P);
    }
    void construct(pointer _P, const value_type& _V)
    {
        std::_Construct(_P, _V);
    }
    void destroy(pointer) {}
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }
};

typedef std::vector<Elem_004dd8c0, Alloc_004dd8c0> Vec_004dd8c0;
typedef void (Vec_004dd8c0::*InsertFn_004dd8c0)(
    Vec_004dd8c0::iterator, Vec_004dd8c0::size_type, const Elem_004dd8c0&);

// FUNCTION: 0x4dd8c0 ?insert@?$vector@UElem_004dd8c0@@VAlloc_004dd8c0@@@std@@QAEXPAUElem_004dd8c0@@IABU3@@Z
InsertFn_004dd8c0 g_insert_004dd8c0 = &Vec_004dd8c0::insert;
