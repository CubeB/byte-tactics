// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by
// GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry (2546, 2026-09): re-tested the SIB wall. _Ucopy
// parameters as iterator/const_iterator, the exact library _Construct spelling,
// _Ufill while/for/count-down spellings, include sets and orders, a source or
// destination local at the third copy, and a free-template _Ucopy/_Ufill
// restructure all stay at 99.6% with the same single lea SIB byte (or worse).
// The remaining byte is the commutative lea operand order described below.
// deepseek-v4.1-flash retry (3073, 2026-10-01): re-ran headers.py (128 sets,
// all 99.6%) and two fresh scratch shapes (_M + _Q for the third _Ucopy's
// destination sum, and an intermediate iterator _R = _Q + _M local). Both stay
// 99.6% with the same single SIB byte. Confirmed unreachable compiler state;
// the best version (this one) is unchanged.
// GPT-6.1-sol refinement stopped on watchdog: 99.6% remains best. All 128
// C-header variants tied; four C++ headers dropped to 89.6%. The partial C++
// header sweep was stopped at about 118 variants. The source-start LEA's SIB
// order remains the single differing byte. No MATCH.
// 2026-09-30 retry check: 99.6% (546/546 bytes). Alias, single-use offset
// helper, and hoisted-local variants did not change the SIB operand order.
// 99.6%, 546 of 546 bytes, ONE SIB byte left (was 89.6%, 546 of 547).
// The fix is 0x476210's clone trick, and it works here: this file is NOT an
// include of <vector> but a hand-written copy of the <vector> class template
// in namespace std, built from <climits> + <memory> + <xutility> with no
// <vector> and no <stdexcept>. The library insert compiles the third _Ucopy's
// source start as four instructions
//     mov eax, ecx / sub eax, edx / add eax, ebx / sub eax, edi
// (547 bytes, 89.6%), the clone as three
//     lea eax, [ecx + ebx] / sub eax, edx / sub eax, edi
// (546 bytes, 99.6%). Nothing else moves: the clone's body below is the
// library's own text, only the members insert needs are declared, and the
// class and template parameter names have to match the real ones so the
// member mangles as ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@.
// Which element type, member names or loop spellings are used does not matter
// (0x476210's ~1900 variants agree), the header state is the lever.
// std::vector<Unit*>::insert(iterator, size_type, const T&) with _Ucopy,
// _Ufill, fill and copy_backward all inlined. Callers are push_back sites
// (0x40ab36 on the vector at +0x5 of the 0x409160 object, and 0x407786 on a
// local vector); taking the member's address makes the compiler emit the
// instantiation out of line. The element type is settled by 0x40ad80, which
// calls this, _Ucopy, _Ufill and size on one vector of units (#135, 0x406c00).
// Stack layout: MSVC 5 hands this __thiscall's declared parameters to
// ASCENDING slots, _P in [entry esp+4] (reloaded at 0x408fac and 0x409019),
// _M in [esp+8] (dead after 0x408f8e, so the new buffer pointer is stored
// there), &_X in [esp+0xc] (read by both _Ufill loops at 0x4090ac/0x409136).
//
// Still differs (99.6%): the operand order of the third _Ucopy's source-start
// lea. The original adds the source to the destination, we add the
// destination to the source:
//     lea eax, [ebx + ecx]        ; (_P + (_Q + _M * 4)) - _Q - _M * 4
//     lea eax, [ecx + ebx]        ; this build, the same value
// followed by the same `sub eax, edx` and `sub eax, edi` in both. The two
// operands are plain registers (ebx = _P, ecx = the destination), the two
// orders are commutative and the value is the same, so nothing in this
// function's source reaches the choice: MSVC 5 canonicalises the pair (this
// build puts the lower-numbered register in the SIB base) and the original's
// translation unit did not. This is 0x476210's wall (SIB 0x17 against 0x3a
// there), so the two are the same residual and the same
// regroup-into-original-files question.
//
// Tried here and rejected: every int and char* spelling of the sum, with _P
// first and with _Q first, `(int)_P + (int)(_Q + _M) - (int)_Q - (int)(_M *
// 4)`, `((char *)_P + (int)(_Q + _M)) - ...` and the reversed ones: 99.6% with
// the same SIB byte. The untried shape from the previous pass's note,
// `_P + ((_Q + _M) - _Q - _M)`, is 66.4% / 561 bytes: the compiler folds it
// and then loses the whole register family. `(_P + (_Q + _M)) - _Q - _M` as
// written is not C++ at all, error C2110 "cannot add two pointers": a lea of
// two pointers has no C++ spelling, so the original's form is MSVC 5's
// reassociation of the value, not of this function's source.
//
// deepseek-v4.1 (2368, 2026-09): re-walked the wall. A file-scope padding
// sweep of 29 dummy declaration counts (0 to 1024, step 16/32) toggles only
// between the 99.6% lea and a 547-byte 89.6% shape whose source pointer is
//     mov eax, ecx / sub eax, edx / add eax, ebx / sub eax, edi
// (K <= 48, 384 to 512 and 832 to 1024 give 99.6%; 64 to 352 and 576 to 768
// give 89.6%; <windows.h> and 300 dummies before the includes give the 89.6%
// shape). No count gives the original base-ebx lea. Swapping the increments
// in _Ucopy's loop, a destination local at the third copy, an int cast on _M,
// &_Q[_M], a while spelling, a dummy pointer loop above the class, and moving
// begin()/end() after size() all keep the same one byte. Swapping _Ucopy's
// parameter order (destination first) collapses the function to 526 bytes /
// 40.0%, so the parameter order reaches the code but not this byte.
#include <climits>
#include <memory>
#include <xutility>

struct Unit {
    int unknown_0;
};

namespace std {

// MSVC 5's <vector> class template, with only the members insert needs. The
// name and the two template parameters have to match the real ones for the
// member to mangle as ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEXPAPAUUnit@@IABQAU3@@Z
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
    vector() : allocator(), _First(0), _Last(0), _End(0) {}
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
    iterator _Ucopy(const_iterator _F, const_iterator _L,
        iterator _P)
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

typedef std::vector<Unit*> Vec_00408f30;
typedef void (Vec_00408f30::*InsertFn_00408f30)(
    Vec_00408f30::iterator, Vec_00408f30::size_type, Unit* const&);

// FUNCTION: 0x408f30 ?insert@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEXPAPAUUnit@@IABQAU3@@Z
InsertFn_00408f30 g_insert_00408f30 = &Vec_00408f30::insert;
