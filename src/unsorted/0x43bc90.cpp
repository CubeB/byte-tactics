// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// GPT-6.1-sol (#3130 retry): best remains 83.6% (909 bytes), confirmed twice. Splitting reserve size into a `needed` local scored 80.0% (914 bytes); incrementing the held insertion iterator after insert scored 71.8% (894 bytes). Four checker runs completed, no MATCH. Remaining mismatch is primarily begin/end pointer register and stack-slot allocation through reserve and the inlined sort; see prior notes below.
// deepseek-v4.1-flash (#2950 retry): still 83.6% (909 bytes, exact). Root cause:
// both versions hoist one global pointer into ecx before `sub esp`; the original
// hoists `_First` (0x512344) and keeps the `_Last` load inside the size() ternary
// branch, ours hoists `_Last` (0x512348) out of the branch and puts `_First` in ebx.
// That single scheduling choice cascades into every downstream slot. All
// reserve-argument/insert-loop rewrites tie or drop; headers.py 128 sets flat.
//
// deepseek-v4.1-flash (2026-09-30 retry): 75.9% -> 83.6% (909 bytes, exact
// size). Two source changes over the previous best, both in the insert loop:
//  * an explicit `Vec_0043c390::iterator ins` held across the call and
//    reassigned from `end()` after each `insert(ins, 1, *p)` (75.9 -> 82.3):
//    the reload after the insert call now lands in a register instead of
//    re-reading [0x512348] at the top of every iteration, and reserve's
//    out-of-line `call _Destroy` reappears, closing the 13-byte gap.
//  * an index loop `for (int i = 0; i < count; ++i)` with `from[i]` instead of
//    the pointer loop (82.3 -> 83.6, and exactly 909 bytes). It changes the
//    downstream register allocation so the sort's slots and the _Median/
//    _Unguarded_partition calls line up much better, at the cost of a counted
//    loop shape (dec edi / jne) where the original compares pointers
//    (add esi,0x19 / cmp esi,edi / jne).
// What still differs, all register/slot allocation, no semantic gap:
//  * prologue: original loads _First into ecx before `sub esp` and _Last into
//    ebp before the pushes and tests ecx there; ours loads _First into ebx
//    after `push ebx` and _Last inside the size branch, testing ebx. This is
//    the root of the remaining ~16%: every downstream register and the
//    [esp+0x10]/[esp+0x34]/[esp+0x38] local homes follow from it (original
//    homes the sort's _F in [esp+0x38] and _Last in [esp+0x10], ours in
//    [esp+0x34]/[esp+0x38]).
//  * reserve's reallocation tail uses ebx/ebp/ecx in the original, eax/ecx/
//    ebx in ours.
// Tried this session with no gain: hand-written `grow()` copy of reserve
// (same 82.3), `<windows.h>` (80.2), `size_type N = size()+count` local
// (same 82.6), keeping the `ins` iterator live through the sort (81.5),
// pointer loop instead of index (82.6), plain `extern Vec_0043c390` without
// the Access class (26.6, wrong insert/name resolution), `push_back(from[i])`
// (69.5).
//
// deepseek-v4.1-flash (retry 2, 10 min): the 83.6 version is still the best.
// Tested against it, all worse: begin()/end() read into locals before the
// reserve call (68.1, reserve stops inlining cleanly), the same with an index
// loop (68.1), pointer loop with inline end() (73.8, 896 bytes), and pointer
// loop with the explicit `ins` iterator (82.6, 918 bytes). The 909-byte index
// loop with `ins` stays the best; the remaining gap is still the prologue
// allocation: original ecx=begin before `sub esp`, ebp=end after push ebp,
// ours ecx=end/ebx=begin.
//
// deepseek-v4.1 (2026-09-30, rerun): baseline 75.9% / 896 bytes confirmed
// unchanged. New probes, none better: declaring the global as a plain
// `Vec_0043c390` instead of the Access class ties at exactly 75.9% / 896; a
// `Vec_0043c390&` local for every access drops to 73.8%; giving the sort
// helpers the real <algorithm> two-level shape (`sort` wrapper and
// `_Insertion_sort` -> `_Insertion_sort_1`) gives 73.7% / 899; adding
// code-free inline helper calls at the three `_Sort` call sites gives 73.8% /
// 896 without flipping the call below. The 13-byte size gap is still exactly
// reserve's out-of-line `call _Destroy` (0x43c390) plus the 4-byte `_First`
// spill it removes, so /Ob2 sits one inline-weight unit under the threshold.
//
// deepseek-v4.1 (2026-09-30): 75.9% (896 bytes against 909), best so far. The one
// change over the 75.6% below is the insertion sort's copy_backward written
// exactly as MSVC 5's <algorithm> template, `while (_F != _L) *--_X = *--_L;`,
// which flips that loop's cmp to the original's `cmp ebp, ebx` (75.6 -> 75.9).
// Still differs:
//  * prologue and reserve: the original loads _First into ecx before `sub esp`
//    and _Last into ebp between the ebx and esi pushes; ours loads _First into
//    ebx after `push ebx` and _Last into ecx. Reserve's rebuild keeps the new
//    _Last in ebp in the original, so its push_back loop pushes ebp and reloads
//    it after each insert call; ours stores it and reloads [vec+8] into ecx at
//    the top of every iteration.
//  * downstream of the same allocation: the original homes the sort's _F/_L in
//    [esp+0x38]/[esp+0x10] (the dead count and _N slots), ours in
//    [esp+0x34]/[esp+0x38], and the insertion tail's slot numbers follow.
//  * the original calls _Destroy (0x43c390, a 3-byte `ret 8` for this trivial
//    element) out of line from reserve; ours inlines the empty body. That call
//    is exactly the 13-byte size gap. Same whole-function inline-weight
//    threshold documented in 0x43c050.cpp.
// Tried on top of 75.9 with no byte change: reserve argument via a named local
// (`int N = DAT_00512340.size() + count;`), a plain `extern Vec_0043c390`
// global without the Access derived class, `_Val_type`-shaped inline calls for
// the recursive _Sort arguments, and an explicit
// `template class std::vector<Elem_0043c390>;` (with the dummy operators so it
// compiles). Worse: explicit `insert(end(), 1, *p)` (73.8), header-shaped
// _Copy_backward/_Insertion_sort_1 templates (73.8), real <algorithm> std::sort
// (57.2 at 905 bytes).
//
// Older note (GPT-6, 75.6% version, kept for the record): corrected sort cursor
// lifetimes and median-call ABI; quicksort preserves the original end for the
// insertion tail and unguarded insertion shifts through a separate cursor; the
// median takes one predicate, matching its 0x5c stack cleanup. The older 76.8%
// attempt sketched below had those semantic errors and is superseded.
//
// NOT a match: 76.8% (933 bytes against 909). Adds `count` copies of a run of
// 25-byte name records (the run at param_1) to the global std::vector at
// 0x512340, then sorts the whole table with the introsort from MSVC 5's
// <algorithm>. The first _Sort level is inlined here (0x43c720 is the
// recursive out-of-line copy), _Median is 0x43ca70, _Unguarded_partition
// 0x43cb20, _Insertion_sort 0x43c990 and _Unguarded_insert 0x43c940; they pop
// their own arguments (__stdcall).
//
// This version is the big step up from the old hand-rolled sort: writing
// _Sort as its own loop over `_FF = _F` (so the original _F survives for the
// insertion-sort tail) took it from 56.9% to 76.8%. What still differs is
// register allocation in the inlined reserve and the insert loop:
//   * The original keeps _Last in ebp from the prologue (`mov ebp,[0x512348]`
//     before the pushes) and reuses it for `_Last - _First`; ours reloads it.
//   * `lea esi, [edx+edi]` (size+count) in the original, `[edi+edx]` in ours.
//   * After the insert loop the original already holds _First in ecx and
//     _Last in ebp; ours reloads both and shuffles them into ebx/ecx.
//   * The original emits `_Sort`'s own entry compare a second time
//     (`cmp edx,0x10 / mov ebx,ecx / jle`) before the median block; ours
//     merges it with the _Sort_0 guard.
//   * reserve's tail uses ebp for `_S + size()*25` in the original versus eax
//     in ours.
// Tried and did NOT change the score: a `Access_0043c390& v` local reference,
// a static inline `Sort_0043bc90` helper, `_Fp = &_F`, and reloading _FF with
// `DAT_00512340.begin()` (that last one dropped to 60.2%).

#include <algorithm>
#include <string.h>
#include <vector>

#pragma pack(push, 1)
struct Elem_0043c390 {
    char unknown_0[0x15];
    char* name; // +0x15
};
#pragma pack(pop)

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef int(__stdcall* Pred_0043bc90)(const Elem_0043c390&, const Elem_0043c390&);

class Access_0043c390 : public Vec_0043c390 {
  public:
    void _Destroy(Vec_0043c390::iterator f, Vec_0043c390::iterator l);
    void insert(Vec_0043c390::iterator p, unsigned int n, const Elem_0043c390& x);
};

extern Access_0043c390 DAT_00512340;

// Hidden result pointer, three padded records and one predicate: ret 0x5c.
Elem_0043c390 __stdcall FUN_0043ca70(Elem_0043c390 a, Elem_0043c390 b, Elem_0043c390 c,
                                     Pred_0043bc90 pred);
Elem_0043c390* __stdcall FUN_0043cb20(Elem_0043c390* first, Elem_0043c390* last,
                                      Elem_0043c390 pivot, Pred_0043bc90 pred);
void __stdcall FUN_0043c720(Elem_0043c390* first, Elem_0043c390* last, Pred_0043bc90 pred,
                            Elem_0043c390*);
void __stdcall FUN_0043c940(Elem_0043c390* last, Elem_0043c390 value, Pred_0043bc90 pred);
void __stdcall FUN_0043c990(Elem_0043c390* first, Elem_0043c390* last, Pred_0043bc90 pred,
                            Elem_0043c390*);

static int __stdcall FUN_0043c020(const Elem_0043c390& a, const Elem_0043c390& b) {
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

template <class _BI1, class _BI2>
_BI2 CopyBackward_0043bc90(_BI1 _F, _BI1 _L, _BI2 _X) {
    while (_F != _L)
        *--_X = *--_L;
    return (_X);
}

static inline void InsertionInline(Elem_0043c390* _F, Elem_0043c390* _L) {
    if (_F != _L)
        for (Elem_0043c390* _M = _F; ++_M != _L;) {
            Elem_0043c390 _V = *_M;
            if (!FUN_0043c020(_V, *_F))
                FUN_0043c940(_M, _V, FUN_0043c020);
            else {
                CopyBackward_0043bc90(_F, _M, _M + 1);
                *_F = _V;
            }
        }
}
template <class _RI, class _Ty, class _Pr> void _Sort_0043bc90(_RI _F, _RI _L, _Pr _P, _Ty*) {
    for (; 16 < _L - _F;) {
        _RI _M = FUN_0043cb20(
            _F, _L, FUN_0043ca70(_Ty(*_F), _Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
        if (_L - _M <= _M - _F)
            FUN_0043c720(_M, _L, _P, (_Ty*)0), _L = _M;
        else
            FUN_0043c720(_F, _M, _P, (_Ty*)0), _F = _M;
    }
}

template <class _BI, class _Ty, class _Pr> void _Unguarded_insert_0043bc90(_BI _L, _Ty _V, _Pr _P) {
    for (_BI _M = _L; _P(_V, *--_M); _L = _M)
        *_L = *_M;
    *_L = _V;
}

template <class _RI, class _Ty, class _Pr> void _Sort_0_0043bc90(_RI _F, _RI _L, _Pr _P, _Ty*) {
    if (_L - _F <= 16)
        FUN_0043c990(_F, _L, _P, 0);
    else {
        _Sort_0043bc90(_F, _L, _P, (_Ty*)0);
        InsertionInline(_F, _F + 16);
        for (_F += 16; _F != _L; ++_F)
            _Unguarded_insert_0043bc90(_F, _Ty(*_F), _P);
    }
}

// FUNCTION: 0x43bc90
void __stdcall FUN_0043bc90(Elem_0043c390* from, int count) {
    DAT_00512340.reserve(DAT_00512340.size() + count);
    Vec_0043c390::iterator ins = DAT_00512340.end();
    for (int i = 0; i < count; ++i) {
        DAT_00512340.insert(ins, 1, from[i]);
        ins = DAT_00512340.end();
    }

    Vec_0043c390::iterator first = DAT_00512340.begin();
    Vec_0043c390::iterator last = DAT_00512340.end();
    _Sort_0_0043bc90(first, last, FUN_0043c020, (Elem_0043c390*)0);
}
