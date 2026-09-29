// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Appends one 25-byte record from the static table at 0x4fd288 to the global
// std::vector<Elem> at 0x512340 (element: 0x19 bytes, char* name at +0x15),
// std::sorts it with the __stdcall name compare 0x43c020, then calls four
// registration helpers. The original inlines vector::reserve and std::sort's
// _Sort_0; the recursive _Sort (0x43c720), _Median (0x43ca70),
// _Unguarded_partition (0x43cb20) and _Insertion_sort (0x43c990) stay
// out-of-line, so those are called by name here.
//
// 89.8% (753 against 745 bytes; previous best 84.5%). Two changes, both in the
// grow block, and the second is the real find:
//  - the element type must be TRIVIAL (no destructor). With a user destructor
//    vector::_Destroy inlines as a loop calling the scalar deleting destructor
//    (`??_GElem_0043c390`), which clobbers edi, so the N reload
//    (`mov edi, [esp+0x10]` at 0x43c0e8) is lost and everything downstream
//    shifts. Trivial makes _Destroy's inlined copy empty, the reload returns
//    (it matches the original) and the frame shrinks to the original's.
//  - call the real `reserve()` from <vector> rather than the hand-written
//    `grow()` copy: real reserve gives the original's ebp = old _Last,
//    ebx = _S allocation in the copy loop (0x43c0c2/0x43c0c8). The hand copy
//    allocates those two the other way round and no amount of source shuffling
//    moved it.
//
// The one remaining structural diff: the original calls the out-of-line
// `_Destroy` at 0x43c108 (3-byte `ret 8` COMDAT, 0x43c390) where ours inlines
// the empty body. To get the call out of line, the function's /Ob2 inline
// budget must run out before reserve's nested `_Destroy`. 0x43bc90.cpp, which
// compiles vector::reserve with the same trivial element and DOES emit the
// call (its /Fa at ?FUN_0043bc90 shows `call ?_Destroy@?$vector...`), is the
// proof it is reachable; its extra budget comes from the real <algorithm>
// std::sort it inlines. Using <algorithm> std::sort here does NOT reproduce it
// (it inlines _Sort_0 and _Sort together and re-derives the loop), and adding
// dead inline helpers or <algorithm>/<string>/<map> to this file did not flip
// it either.
//
// SECOND issue, and why this partial cannot MATCH as written: real reserve
// calls the vector's `size()` (mangled UElem_0043c390::?$vector::size) while
// data/symbols.csv names 0x43c360 `Class_0043c360::FUN_0043c360`. The checker
// will call that reference a mismatch. The symbol at 0x43c360 is exactly
// vector::size ((first==0) ? 0 : (last-first)/0x19); every other vector size in
// the table is named UElem_<addr>::?$vector::size, so the 0x43c360 row looks
// wrong. The hand-written grow below sidesteps it by calling
// ((Class_0043c360*)this)->FUN_0043c360() and keeps the reference correct (that
// version is 86.7%: right name, but the grow registers swap). If 0x43c360 is
// aliased to ?size, real reserve gives the original's bytes.
//
// The sort block itself still has the entry register difference the previous
// note recorded (original loads _L into ebp, ours into esi) plus the _Median
// argument setup, both downstream of the same allocation state.
//
// ROOT CAUSE of that structural diff (found 2026-09-30, deepseek-v4.1-flash):
// it is the /Ob2 whole-function inline-WEIGHT threshold, not a source detail.
// A scratch build with one extra inlined loop anywhere in the function (before
// the reserve call, or at the very end after the four registration calls)
// makes the compiler emit the out-of-line `_Destroy` call, confirmed in the
// /Fa. So the decision is made on the whole function in a pre-pass, not in
// source order, and the original is just over the threshold while this 745
// byte version is just under (the missing 8 bytes ARE that call). Forcing an
// out-of-line instantiation through a global member pointer, dropping the
// `template class std::vector<Elem_0043c390>;` instantiation, dropping the
// unused Elem member operators, and a derived class re-declaring `_Destroy`
// all leave reserve's own call inlined. The fix is genuine original inline
// code (most likely the true std::_Sort_0 / std::_Unguarded_insert expansion
// that keeps _Sort out of line), not added bytes. Merging the quicksort driver
// straight into _Sort_0 and calling the real <algorithm> std::sort both score
// worse (85.9% and 78.5%).
//
// Earlier dead ends, kept for the record:
//  - std::sort from <algorithm>: the recursive call is the mangled std::_Sort,
//    which does not resolve to FUN_0043c720 in data/symbols.csv.
//  - the real <algorithm> std::sort instead of the hand-rolled templates:
//    82.7%, it inlines _Sort_0 AND _Sort together and loses the spill/reload.
//  - writing the _Sort loop as an explicit do/while inside _Sort_0: 78.1%.
//  - a while-form tail (`_F += 16; while (_F != _L) {...}`): 81.9%.
//  - an extra live local for _F across the _Sort call: 83.5% and 81.6%.
//  - routing the sort's begin()/end() through accessors or named locals: 84.5%.
//  - giving the element a user destructor: makes _Destroy a ??_G loop (worse).
#include <vector>
#include <string.h>

#pragma pack(push, 1)
struct Elem_0043c390 {
    char unknown_0[0x15];
    char* name;                        // +0x15

    int operator<(const Elem_0043c390&) const { return 0; }
    int operator==(const Elem_0043c390&) const { return 0; }
    int operator!=(const Elem_0043c390&) const { return 0; }
};
#pragma pack(pop)

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef int (__stdcall* Pred_0043c050)(const Elem_0043c390&, const Elem_0043c390&);

extern Vec_0043c390 DAT_00512340;
extern Elem_0043c390 DAT_004fd288;
extern Elem_0043c390 DAT_004fd2a1;

class Class_0043c360 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;
    int FUN_0043c360(void);
};

// The hand-written copy of reserve, kept as the reference-correct fallback:
// it calls FUN_0043c360 by name instead of the mangled vector::size.
struct Access_0043c390 : Vec_0043c390 {
    void grow(size_type N) {
        if (capacity() < N) {
            iterator S = allocator.allocate(N, (void*)0);
            _Ucopy(_First, _Last, S);
            _Destroy(_First, _Last);
            allocator.deallocate(_First, _End - _First);
            _End = S + N;
            _Last = S + ((Class_0043c360*)this)->FUN_0043c360();
            _First = S;
        }
    }
};

template class std::vector<Elem_0043c390>;

extern "C" void FUN_00406bf0();
extern "C" void FUN_00415b20();
extern "C" void FUN_00406f00();
extern "C" void FUN_00403180();
void __stdcall FUN_0043c720(Elem_0043c390*, Elem_0043c390*, Pred_0043c050, Elem_0043c390*);
void __stdcall FUN_0043c990(Elem_0043c390*, Elem_0043c390*, Pred_0043c050, int*);
Elem_0043c390 __stdcall FUN_0043ca70(Elem_0043c390, Elem_0043c390, Elem_0043c390, Pred_0043c050);
Elem_0043c390* __stdcall FUN_0043cb20(Elem_0043c390*, Elem_0043c390*, Elem_0043c390, Pred_0043c050);

int __stdcall FUN_0043c020(const Elem_0043c390& a, const Elem_0043c390& b)
{
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

template<class _RI, class _Ty, class _Pr>
void _Sort_0043c050(_RI _F, _RI _L, _Pr _P, _Ty *)
{for (; 16 < _L - _F; )
    {_RI _M = FUN_0043cb20(_F, _L,
        FUN_0043ca70(_Ty(*_F), _Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
    if (_L - _M <= _M - _F)
        FUN_0043c720(_M, _L, _P, (_Ty *)0), _L = _M;
    else
        FUN_0043c720(_F, _M, _P, (_Ty *)0), _F = _M; }}

template<class _BI, class _Ty, class _Pr>
void _Unguarded_insert_0043c050(_BI _L, _Ty _V, _Pr _P)
{for (_BI _M = _L; _P(_V, *--_M); _L = _M)
    *_L = *_M;
*_L = _V; }

template<class _RI, class _Ty, class _Pr>
void _Sort_0_0043c050(_RI _F, _RI _L, _Pr _P, _Ty *)
{if (_L - _F <= 16)
    FUN_0043c990(_F, _L, _P, 0);
else
    {_Sort_0043c050(_F, _L, _P, (_Ty *)0);
    FUN_0043c990(_F, _F + 16, _P, 0);
    for (_F += 16; _F != _L; ++_F)
        _Unguarded_insert_0043c050(_F, _Ty(*_F), _P); }}

// FUNCTION: 0x43c050
void FUN_0043c050()
{
    int N = DAT_00512340.size() + 1;
    DAT_00512340.reserve(N);

    Elem_0043c390* p = &DAT_004fd288;
    do {
        DAT_00512340.push_back(*p);
        ++p;
    } while (p != &DAT_004fd2a1);

    _Sort_0_0043c050(DAT_00512340.begin(), DAT_00512340.end(), FUN_0043c020, (Elem_0043c390*)0);

    FUN_00406bf0();
    FUN_00415b20();
    FUN_00406f00();
    FUN_00403180();
}
