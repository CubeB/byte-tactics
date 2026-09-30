// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6. Names are
// provisional. GPT-6 retry: corrected sort cursor lifetimes and median-call ABI. Quicksort now
// preserves the original end for the insertion tail, and unguarded insertion shifts through a
// separate cursor. The median takes one predicate, matching its 0x5c stack cleanup. Current
// score: 75.6%. Older 76.8% attempt below had these semantic errors and is superseded.
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

static inline void InsertionInline(Elem_0043c390* _F, Elem_0043c390* _L) {
    if (_F != _L)
        for (Elem_0043c390* _M = _F; ++_M != _L;) {
            Elem_0043c390 _V = *_M;
            if (!FUN_0043c020(_V, *_F))
                FUN_0043c940(_M, _V, FUN_0043c020);
            else {
                for (Elem_0043c390* _i = _M; _i != _F;) {
                    --_i;
                    _i[1] = *_i;
                }
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
    for (Elem_0043c390* p = from; p != from + count; ++p)
        DAT_00512340.push_back(*p);

    _Sort_0_0043bc90(DAT_00512340.begin(), DAT_00512340.end(), FUN_0043c020, (Elem_0043c390*)0);
}
