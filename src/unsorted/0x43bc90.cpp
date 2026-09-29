// Decompiled by Space Bunny Free, finished by space-bunny-free. Names are provisional.
//
// NOT a match: 56.9% (901 bytes against 909). What still differs, and what was
// tried, is at the bottom of this file. In short: the structure and the callee
// sequence are right, but the compiler gives us a 0x3c-byte frame where the
// original has 0x20, and that one allocation difference shifts every [esp+X]
// displacement in the sort tail.
//
// Adds `count` copies of a run of 25-byte name records (the run at param_1)
// to the global std::vector at 0x512340, then sorts the whole table by name.
// The reserve and the insert are the STL's <vector> (its _Destroy is 0x43c390,
// insert 0x43c3a0, size 0x43c360, all out of line), and the sort is the
// introsort of <algorithm>: the comparator (0x43c020), _Insertion_sort
// (0x43c990), _Sort (0x43c720), _Median (0x43ca70) and _Unguarded_partition
// (0x43cb20) are out of line, while _Unguarded_insert, the _Insertion_sort
// after the quicksort and the whole _Sort_0 driver are inlined here. The
// helpers pop their own arguments, so every one of them is __stdcall.

#include <algorithm>
#include <string.h>
#include <vector>

// The name the comparator, and the sorts inlined here, compare is at +0x15.
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
    Access_0043c390& v = DAT_00512340;
    v.reserve(v.size() + count);
    for (Elem_0043c390* p = from; p != from + count; ++p)
        v.insert(v.end(), 1, *p);

    Elem_0043c390* _F = v.begin();
    Elem_0043c390* _L = v.end();
    Elem_0043c390** _Fp = &_F;
    if (_L - _F <= 16) {
        FUN_0043c990(_F, _L, FUN_0043c020, (Elem_0043c390*)0);
        return;
    }
    for (; 16 < _L - *_Fp; ) {
        Elem_0043c390* _M = FUN_0043cb20(_F, _L,
            FUN_0043ca70(**(_Fp), *(*_Fp + (_L - *_Fp) / 2), *(_L - 1),
                         FUN_0043c020, FUN_0043c020), FUN_0043c020);
        if (_L - _M <= _M - *_Fp)
            FUN_0043c720(_M, _L, FUN_0043c020, (Elem_0043c390*)0), _L = _M;
        else
            FUN_0043c720(_F, _M, FUN_0043c020, (Elem_0043c390*)0), _F = _M;
    }

    // _Insertion_sort(_F, _F + 16, pred), inlined.
    if (*_Fp != *_Fp + 16) {
        for (Elem_0043c390* _M = *_Fp + 1; _M != *_Fp + 16; ++_M) {
            Elem_0043c390 _V = *_M;
            if (!FUN_0043c020(_V, **(_Fp)))
                FUN_0043c940(_M, _V, FUN_0043c020);
            else {
                Elem_0043c390* _I = _M;
                if (*_Fp != _I) {
                    do {
                        --_I;
                        _I[1] = *_I;
                    } while (_I != *_Fp);
                }
                **(_Fp) = _V;
            }
        }
    }

    // for (_F += 16; _F != _L; ++_F) _Unguarded_insert(_F, *_F, pred)
    for (_F += 16; _F != _L; ++_F) {
        Elem_0043c390 _V = *_F;
        Elem_0043c390* _M = _F;
        Elem_0043c390* _Q = _F;
        for (; FUN_0043c020(_V, *--_M); _Q = _M)
            *_Q = *_M;
        *_Q = _V;
    }
}

// ---------------------------------------------------------------------------
// Still to fix (56.9% baseline, from check.py):
//
// 1. FRAME SIZE. Ours is `sub esp, 0x3c`, the original is `sub esp, 0x20`. Every
//    slot the original puts in a dead INCOMING ARGUMENT slot (+0x34, +0x38) lands
//    in ours at +0x50, +0x54 instead, and the address computed for the _Median
//    return slot is `lea eax, [esp + 0x90]` against the original's `[esp + 0x70]`,
//    a difference of exactly the 0x20 of extra frame. Locals are the finish
//    pointer at +0x10, the 25-byte temp at +0x14 and the _Median return slot, so
//    the extra slack is for something the listing never names. Getting the frame
//    to 0x20 is the single upstream cause of most of the remaining diffs.
//
// 2. THE ORIGINAL SPILLS `first` AND KEEPS THE LOOP IV IN A REGISTER. It does
//    `mov [esp+0x38], ecx` (a copy of begin()) right after the size division,
//    and reloads it at the top of both tail loops; the `first + 16` bound lives
//    in another dead argument slot, while the insertion-sort induction variable
//    lives in ebx and the unguarded-insert one in ebp. Taking `&_F` into a
//    local pointer variable (`_Fp`) does move `first` out of ebx and was worth
//    0.9 points, but the reloads it produces are of the wrong value: the
//    original reloads the SAME slot the quicksort loop advanced, while ours
//    reloads the slot the insert loop never touched.
//
// 3. THE INSERT LOOP'S RELOAD ORDER. The original reloads `end` immediately
//    after the insert call (before `add esi, 0x19`); we reload both `begin` and
//    `end` at the latch instead.
//
// 4. THE SORT TAIL'S FLAG REGISTER. The original's guarded comparison ends in
//    `jne` to the shift block with `xor ecx,ecx / test / setl cl / mov eax,ecx
//    / test / jne`, and uses ecx where we use edx. Spelling `if (pred(...))
//    { shift }` does move the flag temp to ecx as predicted, but the score
//    drops from 56.0% to 52.6%, so the original really is spelled with the
//    `!pred` form and the ecx/edx difference has another cause.
//
// 5. THE `else` ARM OF THE QUICKSORT. The original picks the shorter side with
//    `cmp edx, ecx / jg` on the two element counts, and on the `jg` target it
//    pushes `last` then `esi` (the right half); ours matches the instruction
//    order but the branch bodies come out in the other order.
//
// Tried and did NOT help:
//   * Declaring FUN_0043ca70 as returning `Elem*` through a hidden first
//     pointer argument (vA). 53.8%, and the frame stays 0x3c, so the extra
//     frame is NOT the hidden struct-return slot.
//   * Inverting the guarded comparison to `if (pred(...)) { shift } else
//     FUN_0043c940(...)`: moves the flag temp to ecx but drops to 52.6%.
//   * Initialising the insertion-sort induction variable as `_F + 1` instead of
//     pre-incrementing in the test: byte-identical output, 56.0% either way.
//   * Declaring `_L` before `_F`: 53.8%.
//   * A self-referential `Elem** _Fp = &(*_Fp);` scores 61.7% but is undefined
//     behaviour (reading an uninitialised pointer); do not use it, the extra
//     points are an artifact of the UB and not reproducible.
