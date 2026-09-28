// Decompiled by Space Bunny Free. Names are provisional.
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
    if (_L - _F <= 16) {
        FUN_0043c990(_F, _L, FUN_0043c020, (Elem_0043c390*)0);
        return;
    }
    for (; 16 < _L - _F; ) {
        Elem_0043c390* _M = FUN_0043cb20(_F, _L,
            FUN_0043ca70(*_F, *(_F + (_L - _F) / 2), *(_L - 1),
                         FUN_0043c020, FUN_0043c020), FUN_0043c020);
        if (_L - _M <= _M - _F)
            FUN_0043c720(_M, _L, FUN_0043c020, (Elem_0043c390*)0), _L = _M;
        else
            FUN_0043c720(_F, _M, FUN_0043c020, (Elem_0043c390*)0), _F = _M;
    }

    // _Insertion_sort(_F, _F + 16, pred), inlined.
    if (_F != _F + 16) {
        for (Elem_0043c390* _M = _F; ++_M != _F + 16; ) {
            Elem_0043c390 _V = *_M;
            if (!FUN_0043c020(_V, *_F))
                FUN_0043c940(_M, _V, FUN_0043c020);
            else {
                Elem_0043c390* _I = _M;
                if (_F != _I) {
                    do {
                        --_I;
                        _I[1] = *_I;
                    } while (_I != _F);
                }
                *_F = _V;
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

