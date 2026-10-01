// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. finished by Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// Sonnet 5.5 retry (#3079): still 83.8%, 798 bytes. Diagnosis: in the original the
// loaded value of _P is one register web (edi) from the load after operator new
// through the _Ufill loop to the tail copy, and its home slot [esp+0x20] is
// reused as the _Ufill counter; here _P is reloaded from its slot after each
// constructor call (ecx scratch) and the counter takes edi. Tried without
// effect (all 83.8% or collapsing to 773 to 795 bytes): 216 combinations of
// _Ucopy parameter order for the four call sites, do-while / guarded / while /
// reversed-increment forms of _Ufill (whole-class or realloc-branch only),
// the same forms for _Ucopy, <vector> with explicit instantiation, ~140 random
// pairs of expression-order and local-copy tweaks.
// deepseek-v4.1-flash retry (2026-10): still 83.8% (798 bytes, the original's
// size). Re-confirmed the wall is the one register-allocation decision, not a
// source form. Tested (all scored with check.py on scratch copies):
//   v2 separate _Ufill counter local, v3 named _Ucopy end local, v5 named
//   _Ucopy start local -> all collapse to 790-792 bytes / 40-45 percent, so
//   the helper bodies must stay byte-for-byte as the MSVC 5 header writes
//   them; v4/v6-v14 (cached _Pe/_L/_Cp copies, _P + 0, &_P[0], uninitialised
//   _S/_Q declarations, while-form _Ufill, local copies of _P used in both
//   _Ucopy calls) all stay byte-identical to the current build at 83.8%.
//   msvc5-rtm gives 799 bytes / 80.1%, so the sp3 clone is the right toolchain.
// The original keeps _P in edi across the prefix loop and the _Ufill loop and
// spills the _Ufill counter to [esp+0x20]; ours keeps the counter in edi and
// reloads _P from [esp+0x20]. This matches the guide's row "Out-of-line
// vector::insert copies ... differ from each other in the original in the order
// of their pointer sums and in register choice, and source, type and flag
// changes don't reach most of those spots. Treat them as compiler state."
// deepseek-v4.1-flash: replaced the <vector> include with a hand-written clone
// of the vector class template (the trick that matched 0x476210). The clone
// compiles to exactly 798 bytes, the original's size, and lifts this function
// from 80.1% (799 bytes) to 83.8%. Six include-set variants (<climits>,
// <memory>+<xutility>, all four, <windows.h> first) and three source variants
// (a cached _Q + _M local, an indexed _Ufill loop, reordered pointer stores)
// all scored 83.8% or worse, so the clone was kept.
// Still differs: the reallocation branch's register allocation. The original
// keeps _P in edi across the prefix loop and the _Ufill loop and spills the
// _Ufill counter to [esp+0x20]; ours keeps the counter in edi and reloads _P
// from [esp+0x20]. Everything downstream (suffix loop direction, the tail
// block, the in-place branch's scratch registers) follows from that one
// choice. Earlier notes: the copy assignment's base must remain a struct to
// preserve its U mangling; every remaining difference is this single
// register-allocation decision.
// deepseek-v4.1-flash (issue 3379), still 83.8%, 798 bytes: confirmed the wall is
// not reachable from source. The two builds are byte-identical for the first
// 0xbe bytes; the first divergence is the load of _P after operator new, which
// takes edi in the original and ecx here. That one choice then forces everything
// else: here the _Ufill counter takes edi (so _P must be reloaded from its
// argument slot on the prefix loop's back edge) and the destination of the tail
// _Ucopy takes edi, while the original spills the counter to the _P argument slot
// and keeps _P in edi throughout. Since the emitted code is identical up to that
// point, the allocator's live-range metadata must differ, and no source-form
// change reaches it. Tested this run (all scored by check.py, none above 83.8%):
// _Ufill counter as a reference to its own parameter (790 bytes / 40.3%),
// counter as a separate size_type loop index (790 / 40.3%), _Ucopy as a while
// loop with a braced body (798 / 83.8%), _Ufill with a bare _N test, with the
// decrement moved into the body, and with the for-update order swapped (all 798
// / 83.8%, i.e. no effect at all), _Ufill defined before _Destroy and _Ucopy
// (798 / 83.8%), helpers defined after the data members (798 / 83.8%), data
// members before the allocator (792 / 70.1%).
#include <algorithm>
#include <memory>
#include <xutility>

struct Class_0046eaa0 {
public:
    char unknown_0[0x5c];
    Class_0046eaa0& operator=(const Class_0046eaa0& rhs);
};

class Class_0046ded0 : public Class_0046eaa0 {
public:
    Class_0046ded0(const Class_0046ded0& other);
    ~Class_0046ded0();
    Class_0046ded0& operator=(const Class_0046ded0& rhs)
    {
        return (Class_0046ded0&)Class_0046eaa0::operator=(rhs);
    }
};

namespace std {

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

typedef std::vector<Class_0046ded0> Vec_0046f7a0;
typedef void (Vec_0046f7a0::*InsertFn_0046f7a0)(
    Vec_0046f7a0::iterator, Vec_0046f7a0::size_type,
    const Class_0046ded0&);

void __cdecl FUN_0046f7b0(Vec_0046f7a0* v, Class_0046ded0* p,
                  Vec_0046f7a0::size_type n, const Class_0046ded0& x)
{
    InsertFn_0046f7a0 f = &Vec_0046f7a0::insert;
    (v->*f)(p, n, x);
}

// FUNCTION: 0x46f7a0 ?insert@?$vector@VClass_0046ded0@@V?$allocator@VClass_0046ded0@@@std@@@std@@QAEXPAVClass_0046ded0@@IABV3@@Z