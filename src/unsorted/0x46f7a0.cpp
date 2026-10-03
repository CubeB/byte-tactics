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
// deepseek-v4.1-flash (#3860): file-scope declaration-count padding flips this
// instantiation between exactly two shapes, the kept 83.8 percent / 798 bytes
// one and an 80.1 percent / 799 bytes one (320 dummies after the includes;
// 576 dummies are back at 83.8), so padding is not the lever here either.
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
// deepseek-v4.1-flash (issue 3811, 2026-10-01): still 83.8%, 798 bytes. Tested the
// dst-first family that carries 0x46eba0 at this element size: adding a
// _Ucopy_dst(iterator _P, const_iterator _F, const_iterator _L) helper and calling
// it at the tail (_Ucopy_dst(_Q + _M, _P, _Last)) collapses to 63.6% / 777 bytes,
// using it at the prefix copy too is byte-identical to that (63.6%), a named _Pc
// for the prefix source is flat at 83.8%, and the guide's dest-declared-first
// inline tail loop ({iterator _d = _Q + _M; const_iterator _s = _P; for (; _s !=
// _Last; ++_d, ++_s) allocator.construct(_d, *_s);}) collapses to 40.4% / 793.
// Prefix inline dst-first loop alone: 80.1% / 799. So for this 0x5c-byte element
// the standard source-first _Ucopy must stay at all call sites.
// deepseek-v4.1-flash (#4002): re-checked, still 83.8% / 798 bytes. Swapping the
// two in-place else-if branches (0 < _M first) drops to 62.7%, so the retained
// header order is load bearing.
// deepseek-v4.1-flash (#4088, 2026-10): re-checked, still 83.8% / 798 bytes.
// Frame reading of the original: _P is never spilled (edi, live across the
// prefix loop's constructor call), and the inlined _Ufill counter takes its
// home in the dead _P argument slot [esp+0x20]; this build gives edi to the
// counter and reloads _P from that slot. One allocator decision, no source
// form found for it in this pass.
// Space Bunny Free (issue 4164, 2026-10-03): still 83.8%, 798 bytes, unchanged.
// Same wall, and this pass added the evidence that it is not this function's
// source. The register family differs per instantiation of this template in
// the exe, all of them from the same MSVC 5 <vector>: the matched 0x4c51e0
// (8-byte element) keeps _P in ecx and reloads it inside the copy loop and
// gives edi to the _Ufill counter, exactly what this file produces; 0x46f7a0
// (0x5c-byte element, a constructor call in every loop) keeps _P in edi and
// spills the counter into the dead _P argument slot; 0x46eba0 in this same
// issue (14-byte element) keeps _P in esi and spills the counter into a frame
// slot. Three shapes from one template, so the choice is settled per
// instantiation and not by the wording of insert.
// Measured this pass, every one scored with check.py on a scratch copy, none
// above 83.8% (83.8 / 798 bytes kept):
//   the guide's declaration sweep (N unused `extern int dummyN;`, N = 0 to
//   700 step 4) reaches only the two known shapes: 83.8 at N = 0, 80.1 /
//   799 bytes for every N from 16 up;
//   all 128 C header sets (windows.h, stdio.h, stdlib.h, string.h, math.h,
//   memory.h, ddraw.h) on top of the three includes: 83.8 without them, 80.1
//   with any set that has math.h. tools/headers.py itself cannot run on this
//   file: it strips the file's own includes and none of its sets supply
//   std::allocator, so it reports every variant failed to compile;
//   the C++ include set: <stdexcept> in any position, the real <vector>, and
//   <climits> + <memory> + <stdexcept> + <xutility> all give 80.1 / 799 bytes,
//   so the plain three-include clone is the only 798-byte build;
//   translation-unit perturbation: an unused inline vector member, a dummy
//   free function and a second std::vector<int>::insert are flat, a second
//   vector<Class>::insert instantiation gives 80.1;
//   the element class: a statement-body operator=, `class` instead of `struct`
//   for the base and private inheritance are flat, dropping the derived
//   operator= altogether collapses to 40.3 / 790 because copy_backward then
//   calls something else;
//   helper shapes: five of the six _Ufill parameter orders, _Ucopy with
//   non-const bounds, _Ucopy and _Destroy in while form, `if (0 < _M)` around
//   the fill call are flat, while (0 < _N--) in _Ufill gives 62.9 / 817 and
//   `if (_M != 0)` 77.7 / 802;
//   tree-only rewrites that keep the code: a named local for the max() term
//   of _N, size() hoisted into a local, (size() + _M) + _S, named locals for
//   the allocate size, the deallocate count, the fill destination, the third
//   copy's destination and each branch's difference, and all four orders of
//   the three trailing pointer stores (83.4 / 81.8 / 74.8 / 77.7); the same
//   spellings with begin() / end() instead of _First / _Last are worse (63.6
//   to 74.9);
//   dead stores and self-stores at six points in the grow arm, do {} while (0)
//   and a discarded _Ucopy return are flat;
//   hand-written fill / copy_backward loops in place of <algorithm>'s give
//   777 bytes (75.4), so the real <algorithm> stays;
//   destination-first copies: a _Ucopy3(dest, src, end) helper at the third
//   copy (63.6 / 777, void or returning), the guide's inline
//   destination-first loop (40.4 / 793), at the prefix copy only (41.2 /
//   794 void, 73.8 / 794 returning, 44.4 / 795 inline), at the two in-place
//   copies (flat);
//   the parameter renamed _Pp with a local `iterator _P = _Pp;` used through
//   the whole grow arm is flat;
//   permute.py for 25 minutes (insert, size, _Destroy, _Ucopy, _Ufill
//   mutated): 4254 candidates, 83.8% -> 83.8%.
// One change kept, because its output is byte-identical (same diff, 83.8%):
// the out-of-line emission is now a member-pointer initialiser,
// `InsertFn_0046f7a0 g_insert_0046f7a0 = &Vec_0046f7a0::insert;`, as in the
// matched 0x4c51e0 and 0x4b7b00, replacing the FUN_0046f7b0 wrapper, whose
// name pointed at 0x46f7b0, an address inside this very function.
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

// FUNCTION: 0x46f7a0 ?insert@?$vector@VClass_0046ded0@@V?$allocator@VClass_0046ded0@@@std@@@std@@QAEXPAVClass_0046ded0@@IABV3@@Z
InsertFn_0046f7a0 g_insert_0046f7a0 = &Vec_0046f7a0::insert;
