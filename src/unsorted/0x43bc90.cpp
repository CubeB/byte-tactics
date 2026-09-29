// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
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

// The name the comparator uses is at +0x15.
#pragma pack(push, 1)
struct Elem_0043c390 {
    char unknown_0[0x15];
    char* name;                        // +0x15
};
#pragma pack(pop)

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef int (__stdcall* Pred_0043bc90)(const Elem_0043c390&, const Elem_0043c390&);

// _Destroy and insert are protected in std::vector, so a class derived from
// it re-declares them: that is what gives the calls their out-of-line
// instantiations.
class Access_0043c390 : public Vec_0043c390 {
public:
    void _Destroy(Vec_0043c390::iterator f, Vec_0043c390::iterator l);
    void insert(Vec_0043c390::iterator p, unsigned int n, const Elem_0043c390& x);
};

extern Access_0043c390 DAT_00512340;

// _Median returns its result through a hidden first argument, and the callers
// hand it the predicate twice: the second copy is the one left on the stack
// when the function pops only what its own declaration promises, and the
// partition call that follows pops it.
Elem_0043c390 __stdcall FUN_0043ca70(Elem_0043c390 a, Elem_0043c390 b,
                                      Elem_0043c390 c, Pred_0043bc90 pred,
                                      Pred_0043bc90 pred2);
Elem_0043c390* __stdcall FUN_0043cb20(Elem_0043c390* first, Elem_0043c390* last,
                                       Elem_0043c390 pivot, Pred_0043bc90 pred);
void __stdcall FUN_0043c720(Elem_0043c390* first, Elem_0043c390* last,
                            Pred_0043bc90 pred, Elem_0043c390*);
void __stdcall FUN_0043c940(Elem_0043c390* last, Elem_0043c390 value,
                            Pred_0043bc90 pred);
void __stdcall FUN_0043c990(Elem_0043c390* first, Elem_0043c390* last,
                            Pred_0043bc90 pred, Elem_0043c390*);

// Out of line because the sort hands it on to the helpers above.
static int __stdcall FUN_0043c020(const Elem_0043c390& a, const Elem_0043c390& b)
{
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

// FUNCTION: 0x43bc90
void __stdcall FUN_0043bc90(Elem_0043c390* from, int count)
{
    DAT_00512340.reserve(DAT_00512340.size() + count);
    for (Elem_0043c390* p = from; p != from + count; ++p)
        DAT_00512340.insert(DAT_00512340.end(), 1, *p);

    Elem_0043c390* _F = DAT_00512340.begin();
    Elem_0043c390* _L = DAT_00512340.end();
    if (_L - _F <= 16) {
        FUN_0043c990(_F, _L, FUN_0043c020, (Elem_0043c390*)0);
        return;
    }
    // _Sort(_F, _L, _P, (_Ty*)0), inlined; its recursive calls are the
    // out-of-line copy 0x43c720.
    for (Elem_0043c390* _FF = _F; 16 < _L - _FF; ) {
        Elem_0043c390* _M = FUN_0043cb20(_FF, _L,
            FUN_0043ca70(*_FF, *(_FF + (_L - _FF) / 2), *(_L - 1),
                         FUN_0043c020, FUN_0043c020), FUN_0043c020);
        if (_L - _M <= _M - _FF)
            FUN_0043c720(_M, _L, FUN_0043c020, (Elem_0043c390*)0), _L = _M;
        else
            FUN_0043c720(_FF, _M, FUN_0043c020, (Elem_0043c390*)0), _FF = _M;
    }

    // _Insertion_sort(_F, _F + 16, _P), inlined.
    if (_F != _F + 16)
        for (Elem_0043c390* _M = _F; ++_M != _F + 16; ) {
            Elem_0043c390 _V = *_M;
            if (!FUN_0043c020(_V, *_F))
                FUN_0043c940(_M, _V, FUN_0043c020);
            else {
                for (Elem_0043c390* _i = _M; _i != _F; ) {
                    --_i;
                    _i[1] = *_i;
                }
                *_F = _V;
            }
        }
    // for (_F += 16; _F != _L; ++_F) _Unguarded_insert(_F, *_F, _P)
    for (_F += 16; _F != _L; ++_F) {
        Elem_0043c390 _V = *_F;
        for (Elem_0043c390* _M = _F; FUN_0043c020(_V, *--_M); _F = _M)
            *_F = *_M;
        *_F = _V;
    }
}
