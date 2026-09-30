// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// GPT-6 retry: 78.9%, 646 of 632 bytes; pointer and buffer constness did not change the saved register family or spilled insertion pointer.
// std::vector<Elem_00476490>::insert(Elem_00476490* _P, size_type _M,
// const Elem_00476490& _X), the game's reallocating insert.
//
// NOT MATCHING: 78.9 percent, 646 bytes against 632 (space-bunny-free retry
// for #1190; the 78.9 itself is from the Sonnet 5.5 retry for #1081 below).
//
// Retry finding: writing _Destroy out as an inline loop with the end cached
// (`iterator _e = _Last;`) after the deallocate took 75.1 to 78.9 and made the
// whole tail (delete call, size() recompute, three pointer stores) match except
// the original's dead spill of _First into the _X argument slot. A scripted
// sweep of about 700 more variants (helper parameter orders and loop shapes at
// every _Ucopy/_Ufill site, statement orders, local copies of _P/_M/_X, size
// expression spellings, manual third-copy loops) never moved it further: the
// one remaining cause is that the original keeps _P in edx (loaded right after
// the operator new call, before _First) so the fill counter must be ebp with
// &_X reloaded, while this build reloads _P into ecx after each rep movsd.
// Older notes follow.
//
// The whole 14-byte difference is one allocator decision, and it is worth
// naming precisely: the original loads the insert argument _P into EDX right
// after the operator new call (`mov edx, [esp+0x24]`, before it spills _S) and
// keeps it in EDX to the end of the suffix copy, where the source pointer is
// built in place (`sub edx,ebx / add edx,eax / sub edx,ecx` from _P). This
// build keeps _P in its argument slot, and everything else follows from that:
//   10 bytes  two extra `mov ecx, [esp+0x20]` reloads of _P, one in the
//             prefix copy's preheader and one per turn of its loop,
//   9 bytes   two allocator spills in the tail (`mov [esp+0x20],ecx` and
//             `mov [esp+0x20],eax`, both into the now-dead _P slot) because
//             size() has to land in ECX here and in EAX in the original,
//   -5 bytes  the original's dead `mov [esp+0x2c], eax` (the _First argument
//             of operator delete spilled over the &_X slot), which this build
//             elides.
// Fixing the reload and the two spills without the dead store is the whole job.
//
// What this pass added, all scored for free with check.py --sym, none of it
// moving the number (each is a dead end, do not repeat):
//   - The clone of 0x476210.cpp's file, which is itself 99.6% (one SIB byte),
//     scores 60.8% and 636 bytes here. It is a different register family
//     (`mov edi, ecx` after the four pushes, this in EDI, _P reloaded into EDI
//     each turn), so the two functions really are one source with two
//     allocations, as the guide's "register family" note says. Dropping its
//     default constructor changes nothing (60.8%, 636 bytes), so the family is
//     not decided by the ctor.
//   - Writing the third inlined _Ucopy by hand as a loop with the destination
//     first (the guide's 0x425480 trick) gives 38.8% and 652 bytes, source
//     first 37.9% and 663: for 0x476490 the stock `_Ucopy(_F, _L, _P)` spelling
//     is the right family and the guide's trick is exactly wrong here.
//   - The same with the helper's own parameter order reversed (dest first at
//     all four sites): 36.6%, 649 bytes.
//   - `_Destroy(_First,_Last)` written out as a call before the deallocate is
//     the 0x44ec30/0x46cc10 form and drops to 60.6%; the same call after the
//     deallocate gives 75.1%. Only the inline loop with the cached end
//     `_e = _Last`, after the deallocate, reaches this family at all.
//   - `iterator _p = _P;` used through the whole reallocating branch: 78.9%,
//     646 bytes, identical, so a named local is not what puts _P in a register.
//   - The prefix copy written out by hand, the fill written out by hand, `int`
//     instead of `size_type` for _N, and the three pointer stores reordered:
//     78.8 / 78.9 / 78.9 / 57.0.
//   - Deleting the trivially-destructible _Destroy loop outright is the only
//     way to reach the original's exact 632 bytes here, but it drops to 63.0
//     percent and flips the family (`mov edi, ecx` after the pushes, this in
//     EDI, _N added as `add eax, edx` rather than `lea edi, [edx + eax]`), so
//     the loop is a source-level register-allocation lever whose body the
//     compiler elides; it is not emitted, it is what keeps `this` in ECX.
//     Moving it before the deallocate (the stock header order) gives 60.6.
//
// Previous state: 74.9 percent, 644 bytes against 632. The byte count is 12 too
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
            {
                iterator _d = _First;
                iterator _e = _Last;
                for (; _d != _e; ++_d)
                    allocator.destroy(_d);
            }
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
