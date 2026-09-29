// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// std::vector<Elem_00476490>::insert(Elem_00476490* _P, size_type _M,
// const Elem_00476490& _X), the game's reallocating insert.
//
// NOT MATCHING: 74.9 percent, 644 bytes against 632. The byte count is 12 too
// high, so the shape is still wrong somewhere, not just a register order.
//
// The class below is a hand-written clone of the primary `std::vector` template
// rather than the real one from <vector>. The template parameter names and the
// default allocator argument are spelled identically, so the member still
// mangles as
//   ?insert@?$vector@UElem_00476490@@V?$allocator@UElem_00476490@@@std@@@std@@
//    @QAEXPAUElem_00476490@@IABU3@@Z
// which check.py confirms. The reason for the clone is that <vector> drags in
// <stdexcept>, and with that header present MSVC 5 builds the third copy's
// source pointer as `mov eax,edx / sub eax,ebx / add eax,edi / sub eax,ecx`
// where the original wants the single-instruction form. Both sibling functions
// in this issue (0x475ef0 and 0x476210) are the same template and the same
// include is the same lever there.
//
// Written as an explicit SPECIALISATION of the real std::vector, which also
// preserves the symbol, this scores 29.6 percent (637 bytes): MSVC gives `this`
// a callee-saved register (`mov edi, ecx` after the four pushes) and loads
// _End and _Last through edi, where the original leaves `this` in ecx, spills it
// to [esp+0x10] and reloads it. That one difference permutes ebx/ebp/esi/edi
// for the whole function. The clone avoids it.
//
// The statement order inside the reallocating branch is deliberate: the
// `allocator.deallocate(_First, _End - _First)` comes BEFORE `_Destroy`, which
// is not the stock <vector> order. That reorder alone was worth 60.7 to 74.9
// percent, because it is what flips the entry shape above. It is
// behaviourally identical here because Elem_00476490 is trivially
// destructible, so _Destroy is a no-op.
//
// The element type is NOT the problem. int[8], char[32], eight separate ints,
// pointer+Vec3+four ints, long long[4], double[4], a nested array, a bitfield,
// and in-class memcpy copy constructor and operator= all compiled to the same
// bytes. That was established against the stock header and is the main reason
// the previous pass's element-type sweep came up empty.
//
// What still differs, all of it inside the reallocating branch:
//   - _P is parked in EDX in the original, from the first instruction after the
//     operator new call to the end of the suffix copy, so the prefix-copy loop
//     never reloads its bound. Here _P is spilled and reloaded, which forces
//     the different frame slots and a different source expression in the third
//     copy.
//   - The _Ufill loop's counter is EBP in the original, with &_X reloaded from
//     [esp+0x28] on each turn. Here &_X is hoisted into ebp and EDX counts.
//   - The third copy's source pointer form (see above).
//   - The epilogue's size() recompute lands in ECX where the original uses EAX.
//   - A dead `mov [esp+0x2c], eax` store that the original does not have.
#include <memory>
#include <algorithm>
#include <string.h>
#include <climits>

struct Elem_00476490 { int dwords[8]; };

namespace std {
template<class T, class A = allocator<T> >
class vector {
public:
    typedef A allocator_type;
    typedef typename A::size_type size_type;
    typedef T* iterator;
    typedef const T* const_iterator;

    void insert(iterator _P, size_type _M, const T& _X)
    {
        if (_End - _Last < _M) {
            size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void *)0);
            iterator _Q = _Ucopy(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            _Ucopy(_P, _Last, _Q + _M);
            allocator.deallocate(_First, _End - _First);
            _Destroy(_First, _Last);
            _End = _S + _N;
            _Last = _S + size() + _M;
            _First = _S;
        } else if (_Last - _P < _M) {
            _Ucopy(_P, _Last, _P + _M);
            _Ufill(_Last, _M - (_Last - _P), _X);
            fill(_P, _Last, _X);
            _Last += _M;
        } else if (0 < _M) {
            _Ucopy(_Last - _M, _Last, _Last);
            copy_backward(_P, _Last - _M, _Last);
            fill(_P, _P + _M, _X);
            _Last += _M;
        }
    }
    size_type size() const { return (_First == 0 ? 0 : _Last - _First); }

private:
    void _Destroy(iterator _F, iterator _L)
        { for (; _F != _L; ++_F) allocator.destroy(_F); }
    iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P)
        { for (; _F != _L; ++_P, ++_F) allocator.construct(_P, *_F); return (_P); }
    void _Ufill(iterator _F, size_type _N, const T& _X)
        { for (; 0 < _N; --_N, ++_F) allocator.construct(_F, _X); }

    allocator_type allocator;
    iterator _First, _Last, _End;
};
}

typedef std::vector<Elem_00476490> Vec_00476490;
typedef void (Vec_00476490::*InsertFn_00476490)(
    Vec_00476490::iterator, Vec_00476490::size_type, const Elem_00476490&);

// FUNCTION: 0x476490 ?insert@?$vector@UElem_00476490@@V?$allocator@UElem_00476490@@@std@@@std@@QAEXPAUElem_00476490@@IABU3@@Z
InsertFn_00476490 g_insert_00476490 = &Vec_00476490::insert;
