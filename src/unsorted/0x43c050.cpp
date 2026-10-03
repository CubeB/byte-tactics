// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, edited by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash. Names are provisional.
// Claude Code / opus (#4961, 60 min): best stays 92.0% (732/753). New findings,
// none of which beats the file below yet:
//  * This function is FUN_0043bc90(DAT_004fd288, 1) inlined. 0x43bc90's other three
//    callers are exactly the registration calls made here (0x403180, 0x406bf0,
//    0x415b20). Compiling 0x43bc90 as `reserve(size() + count); std::copy(from,
//    from + count, std::back_inserter(v)); std::sort(v.begin(), v.end(),
//    FUN_0043c020);` with the real <vector>/<algorithm>, and this function as the
//    call, reproduces every call and inline decision of both functions: here
//    _Destroy and size() out of line and both _Insertion_sort_1 calls out of line;
//    in 0x43bc90 only the first is out of line. push_back, index or explicit-iterator
//    loops change the inline budget and get other patterns. Its registers score
//    80.7% to 84.3% here.
//  * A hand-written std::vector in namespace std, as 0x473d50.cpp does (insert
//    declared only, reserve's size() calling Class_0043c360::FUN_0043c360, _Destroy
//    with its real empty-loop body), gives this file's exact 92.0% with every
//    reference named as data/symbols.csv expects; the file below still calls the
//    real vector::size, which that file names differently.
//  * Every variant that calls _Destroy (budget, nudge, or that hand-written vector
//    with _Destroy declared only) lands on 82.8% to 84.3%. The original loads
//    [_Last]/[_First] for _Destroy into ebp/eax, the copy loop's registers, so its
//    `je` over the empty loop skips the reloads; ours always reloads into eax/ecx at
//    the join.
//  * Permuter lead (hand-written vector with _Destroy declared only, 17 min, 10292
//    candidates, nothing else found; headers.py flat): routing the do/while test
//    through `static inline bool NotEnd(Elem* p) { return DAT_004fd2a1 != p; }` fixes the
//    whole sort block's allocation, 90.6% at 759 bytes (the extra 6 bytes are its
//    setne/test). A function-local static end pointer gives 87.2% at exactly 753.
//  * Functions defined after the target in the same file shift its allocation by
//    about 2 points (functions before it do not), so the rest of the original
//    translation unit (0x43c350 and the sort templates after it) matters.
// DeepSeek V4.1 Flash (issue #4789): best stays 92.0% (732/753), confirmed
// stuck. A 3-minute permuter run (1865 candidates) found nothing. An empty
// `static inline` nudge placed after reserve (or at any of ten statement
// boundaries) does tip the /Ob2 budget and emit the out-of-line `call
// _Destroy` at 752 bytes, but every such variant lands at 84.3%: the tail
// reloads `_Last` into ecx and `_First` into eax (original: ebp/eax) and the
// sort's `mov esi, ebx` copy plus its downstream homes shift. Removing the
// explicit `template class std::vector<Elem_0043c390>;` with the nudge is
// worse (82.8%). Reconfirmed the two independent residuals: reserve's
// inlined-empty `_Destroy` and the sort block's `_F` home.
// deepseek-v4.1-flash (#4014, 10 min): best stays 92.0% (732/753). Two further
// levers tested against it, both worse: (1) the 0x43bc90 copy-loop shape
// (`iterator ins = end(); do { insert(ins,1,*p); ins = end(); ++p; }`) collapses to
// 31.2% / 915 bytes: it makes vector::insert an out-of-line call per iteration where
// the original calls 0x43c3a0 exactly once, from the inlined push_back.
// (2) routing the reserve through the Access `grow` member with a declared non-static
// `_Destroy(iterator,iterator)` does emit the wanted `call 0x43c390` (verified against
// ctx.py: the original calls 0x43c390 at 0x43c108) and lands 752/753 bytes, but only
// 82.8% with 8 hunks: the reserve tail flips its scratch temp to edx (original: ecx
// before the `mov ecx,this`) and the sort's register homes shift, i.e. the call and the
// allocation are not independently steerable. Same conclusion as the four prior
// sessions, which confirmed with /Fa that the inline verdict is a whole-function
// pre-pass threshold: the 732 byte body sits just under it, the 753 byte original just
// over, and the 20-byte `_Destroy` call is itself what it is measured against.
// deepseek-v4.1-flash (#3676, 10 min): best stays 92.0% (732/753 bytes). Two levers
// tested against it, both worse. Deleting `template class std::vector<Elem_0043c390>;`
// (to mirror 0x43bc90's TU, which does emit the out-of-line `_Destroy` call) kept 732
// bytes but fell to 89.3%: the reloc/register plan changes without the missing call.
// Routing reserve through the Access `grow` member with a non-static declared
// `_Destroy(iterator,iterator)` produced the call block and 752/753 bytes, but the
// surrounding register allocation collapsed to 84.3%. Still differing: the single
// ~20-byte `mov ebp,[_Last] / mov eax,[_First] / push ebp / push eax / mov ecx,this /
// call _Destroy` block in reserve's reallocation tail (ours has no call, only a 4-byte
// `mov [esp+0x18],eax`), everything else is jump-displacement fallout.
// deepseek-v4.1-flash (#3340, 10 min): reconfirmed 92.0% (732/753 bytes). Mirrored 0x43bc90's undeclared Access_0043c390::_Destroy/insert member declarations in place of the unused grow fallback: byte-identical, 92.0% / 732 bytes, so the Access declaration shape is not what keeps reserve's _Destroy inline. Remaining gap is the single 20-byte _Destroy call block at original +0xb1 minus our 4-byte `mov [esp+0x18],eax` burst; all other hunks are displacement fallout.
// GPT-6.1-sol retry (issue #3130, 2026-10-01): best remains 92.0% (732/753). Direct insert(end(), 1, value) expanded insertion to 913 bytes / 31.2%; while and index-loop spellings tied at 92.0%, with the index form changing the target jne to jl. Preserve the baseline. Remaining differences: reserve omits the empty out-of-line _Destroy call and shifts register allocation / branch targets; vector::size resolves under the mismatched symbol noted below. No MATCH observed.
// deepseek-v4.1-flash (#2950 retry): still 92.0% (732 of 753 bytes). The only code
// gap is a trivially-empty out-of-line callee (ret N COMDAT) plus the register spill
// that call forces; removing the explicit template instantiation scores 89.3%,
// named size_type/`_FF = _F`/insert-loop forms are byte-identical or worse. This is
// the /Ob2 whole-function inline-weight threshold, not source-reachable.
//
// RETRY 2 (deepseek-v4.1-flash, 2026-09-30, 10 min): 89.8% -> 92.0% (753
// against 732 bytes). The fix was in `_Sort_0043c050`: write the loop test as
// `_L - _F > 16` instead of `16 < _L - _F`. That spelling makes MSVC 5 fold
// the `_Sort_0` `if (_L - _F <= 16)` test together with the inlined `_Sort`
// entry test, so the redundant `cmp edx, 0x10 / mov ebp, esi / jle` prologue
// (7 bytes) disappears and the sort's `_F` home lands in ebp/esi the way the
// original has it. The same loop merged by hand into `_Sort_0`, and the
// inverted `if (_L - _F > 16) { ... } else ...` form, are both worse (85.9%
// and 90.1%). What still differs: reserve's out-of-line `_Destroy` call is
// still inlined empty (the tail reloads `_First` into eax/ecx instead of
// ebp/eax), and the sort block is missing the `mov esi, ebx` copy so its
// post-loop `_F` reload lands in eax instead of esi. The nudge experiments
// (a zero-trip inline helper at seven positions) still just trade this for a
// worse global allocation (v6/v8: 84.3% at 752 bytes).
//
// RETRY (deepseek-v4.1-flash, 2026-09-30, 10 min): retained the 89.8% / 745 byte
// version. Confirmed once more that the only missing bytes are reserve's
// out-of-line `call _Destroy` (0x43c390) plus the register reallocation it
// forces. Tried an address-taken static member pointer to _Destroy in this TU
// (same trick as src/unsorted/0x43c390.cpp): no change, still inlines. Tried
// empty inline nudges at five positions (before reserve, after reserve, after
// the push_back loop, after sort, at the end). Every nudge that tips the /Ob2
// budget emits the _Destroy call but reallocates _First/_Last into eax/ecx
// (765 bytes, 82.2%); the end-of-function nudge instead drops an unrelated
// inline (683 bytes, 69.6%). Hand-written Access::grow with the same nudge
// gives the identical 765 byte shape, so the grow source is not the lever. What
// still differs: reserve's _Destroy call is inlined empty, and the sort block
// homes _F/_L in the swapped slots (esi/ebx instead of ebp/esi) with the
// `cmp ebp, [esp+0x10]` memory compare in the insertion tail. No further
// progress in the timebox.
// deepseek-v4.1 (2026-09-30, independent rerun): baseline 89.8% (745/753)
// reproduced. Evidence: 0x43c390 MATCHES as the empty (3-byte `ret 8`)
// out-of-line copy, so the element type really is trivial (its
// `for (; _F != _L; ++_F) allocator.destroy(_F);` loop is removed by VC5) and
// the missing call is pure inliner bookkeeping, not a type difference.
// Probes, all worse: an empty user destructor `~Elem() {}` (775 bytes, 81.9%,
// _Destroy becomes a loop with ??_G); a declared-only destructor `~Elem();`
// (900 bytes, 72.1%, the loop is inlined at the call site); one extra inline
// helper (zero-trip loop) right after `reserve(N)` DOES restore the
// out-of-line `call _Destroy` (765 bytes, 82.2%) but flips the _First/_Last
// pair into eax/ecx and reorders the `_End = _S + N` / `size()` stores, so the
// net is worse; the same helper at the end of the function does not flip
// _Destroy at all, it makes MSVC drop a bigger inline instead (688 bytes,
// 70.9%). The 89.8% shape below is still the best.
//
// GPT-6 retry: retained 89.8%. Native insert spellings and 768 header sets
// did not recover the missing out-of-line _Destroy call. The vector::size
// symbol discrepancy described below still needs orchestrator review.
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
{for (; _L - _F > 16; )
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

template<class _RI, class _Pr>
inline void sort_0043c050(_RI _F, _RI _L, _Pr _P)
{_Sort_0_0043c050(_F, _L, _P, (Elem_0043c390*)0); }

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

    sort_0043c050(DAT_00512340.begin(), DAT_00512340.end(), FUN_0043c020);

    FUN_00406bf0();
    FUN_00415b20();
    FUN_00406f00();
    FUN_00403180();
}
