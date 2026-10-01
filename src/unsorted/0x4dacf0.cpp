// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash and GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry: rechecked the retained 55.6% variant; moving/aliasing the zero init through `memset` or `wraps` keeps the same score, and branch-local initialization drops to 43.3%. The persistent mismatch is register allocation around `res = 0`: this build materializes zero in ebp, while the original keeps the initial stack zero immediate and uses esi as wraps.
// Started by space-bunny-free, continued by deepseek-v4.1-flash and GPT-6; deepseek-v4.1 retry.
// deepseek-v4.1-flash retry 2 (best 55.6, unchanged): swept the `res` declaration position
// (right after size, after map, after wraps) all stay 55.6; before lock drops to 51.5 because
// lock moves to ebx. The blocker is unchanged: `res = 0` compiles to `xor ebp,ebp` and ebp is
// then reused for the `map->count` compare, so `want` cannot take ebp; the original stores the
// 0s as immediates and compares against the just-zeroed esi (wraps).
// NOT A MATCH: measured 52.2 percent, ours 778 bytes against the original 780, frame 0x68 (correct,
// and every callee/data reference resolves at the same place). The body is instruction for
// instruction right except for register allocation:
//   * the original homes lock/need/want/wraps in edi/ebx/ebp/esi and keeps res (E+0x24) in memory
//     only; this build homes lock and map in ebx, want in edi, res in ebp and need in memory at
//     E+0x20, and the loop counter wraps shares ebp as the zero source.
//   * consequences of that one difference: `cmp dword ptr [map+0xc],esi / jbe` becomes
//     `cmp dword ptr [map+0xc],ebp / jbe`; `push 0` for the record ctor's last argument becomes
//     `push ebp`; the pad branch's `lea eax,[ebx+edi]` becomes `lea ecx,[ebx+edi]`; the b iterator
//     home slides from E+0x1c to E+0x10 (n2 stays at E+0x14); the early `mov dword ptr [esp+0x24],0`
//     becomes `xor ebp,ebp / mov [esp+0x24],ebp`.
//   * tried without effect: removing the `= 0` initialisers on base/len/key, splitting one Pair into
//     the two the original has, swapping the iterator declarations, `wraps` before `map`, the
//     `!(a == b)` Neq form, and changing the local declaration order. Every attempt leaves the
//     ebx/ebp/edi assignment as above, so this looks like MSVC5 allocator state the local source
//     does not steer (same class as 0x4a6ae0 and 0x4866d0).
// GPT-6 retry: repaired shared page commitment for reused blocks, zeroed base on
// failed reservation, fixed the 48-byte record layout and iterator hidden-return ABI, and corrected
// matched callee owners. Earlier notes below describe the superseded reconstruction. Progress over the 49.7 percent version came from removing the `= 0`
// initialisers on base/len/key (the original does not zero them, and the extra zero store was the
// spilled home that both pushed the frame to 0x6c and took the register allocation away from
// map/wraps) and from splitting the one Pair into the two the original has
// (p at E+0x30, the lower_bound query q at E+0x38), plus masking n before the
// negate in pad.
// The frame is still 0x6c where the original is 0x68: our locals relative to E
// match the original exactly (res 0x24, lock 0x28, need 0x2c, p 0x30, q 0x38,
// ins 0x40, rec 0x48) but E sits 4 bytes lower, so the extra dword is reserved
// above the record, most likely a distinct hidden-return slot for the
// FUN_004dbd00(cur) erase call (the original aliases it onto E+0x30, the p
// slot). Changing the local declaration order changes nothing.
// Worked: want/need split as a Pair; no `= 0` on base/len/key; base loaded from
// DAT_005289d4 before the `if (base == 0)`; pad = (0 - (n & 0xfff)) & 0xfff.
// Still differs: frame 0x6c; the record ctor's last argument is pushed as a
// register (ebp) instead of the immediate 0; a lea lands in ecx where the
// original uses eax in the commit path.
// The original's frame (E = esp after `sub esp,0x68` and the four pushes):
//   0x10 want        0x14 n2 (lower_bound out)   0x18 cur    0x1c b, then
//   reused as prev  0x20 len   0x24 res   0x28 the CRITICAL_SECTION pointer,
//   then `need` (both use that one home)   0x2c NOT referenced at all, a
//   genuine hole   0x30/0x34 the pair, which is ALSO the hidden-return slot of
//   FUN_004dbe10, FUN_004dbd80 and FUN_004dbd00 (all three pass &slot first
//   and 0 second)   0x38/0x3c the lower_bound query pair   0x40 the insert
//   result, reused for the second map's insert   0x48 the 48-byte record, whose
//   tail overlaps the four saved registers (0x4d8849 stores the 4th ctor
//   argument to [esi+0x2c] = E+0x74 = saved edi, which the epilogue pops and
//   discards, so the original gets away with it).
// What worked (all scored free with `check.py --sym`):
//   * `FUN_004da780()` returns the CRITICAL_SECTION pointer itself, and the
//     size argument is NOT written back: `unsigned int size = n; if (!size)
//     size = 1;`. That gives the original's `test esi,esi / jne / mov esi,1`
//     with esi live. Assigning to the parameter instead (v0, 48.5%) produced
//     `mov [esp+0x80],1`, a store into the argument slot, and turned the
//     `test` into a `cmp` against the register holding the zero.
//   * Pair_004dacf0 with NO constructors at all: a user-provided default
//     constructor zero-initialises the pair at the top of the function, and
//     the original has no such stores (v0 had three `mov [esp+X],ebp`).
//   * `if (cur == (Class_004dbe10(map->head)))`: the original's head test is
//     `mov ebx,[edi+4] / xor edx,edx / cmp eax,ebx / sete dl / test dl,dl /
//     je`, i.e. the End() iterator is a VALUE compared through operator==.
//     Comparing `cur.ptr == map->head` loads the head straight into the cmp
//     and drops the sete/test (49.7 -> 47.7 when reverted).
// Tried and did NOT work:
//   * declaring `wraps` before `map` to swap the esi/edi roles the original has
//     (map in edi, wraps in esi): no effect at all, 47.7 either way.
//   * the other change folded into the same run as the head test (both
//     together): 47.9, worse than the head test alone, so the two do not
//     compose.
// Still differs:
//   * `need` and `want` are in the opposite callee-saved registers (the
//     original has need in ebx and want in ebp, this file has need in ebp and
//     want in ebx, and therefore the `cmp` is the other way round).
//   * the original stores `need` into the lock's home at 0x28 and compares in
//     registers; this file gives `need` a home of its own, one dword high.
//   * the original keeps `map->count <= wraps` as `cmp [edi+0xc],esi / jbe`
//     and does not fold the known zero; this file folds it to `test eax,eax`.
//   * the trailing half (the two insert calls, VirtualAlloc, the record and
//     its ctor) has not been revisited since v0.
// deepseek-v4.1-flash retry: spelling `if (!size)` as `if (size < 1)` is the
// only change that scored better (52.2 percent, 778 bytes) because MSVC folds
// the `size = 1` fixup into the test (`cmp esi,1`). tools/headers.py (128 sets)
// and a dummy-declaration sweep of N = 0..400 all stay flat at 51.1/51.5, and
// every res/need/want declaration-order permutation keeps lock in ebx and the
// zero from `res = 0` in ebp, so the edi/ebx/ebp rotation above still stands.
// Carve a block out of the reservation allocator: enter the lock, round the
// request up (need) and double it (want), then walk the free-block map for a
// block of at least want bytes, splitting it around the allocation point and
// committing the pages with VirtualAlloc. Same std::map idiom as 0x4db000,
// 0x4db450 and 0x4db1c0: the block is the map's value_type, a base and a length.
// deepseek-v4.1-flash pass 2 (best 55.6 with `#include <memory.h>` added, 55.2
// without; tools/headers.py picks <windows.h> <memory.h> as the closest set and
// the 55.6 state is the same one 28 to 52 unused declarations reach): moved
// `unsigned int res = 0;` from the
// top of the locals down to just after `cur` and spelled the size fixup
// `if (size == 0) size = 1;` (which restores the original's `test esi,esi /
// jne / mov esi,1`; `if (size < 1)` had folded it to `cmp esi,1 / jae` and was
// what the earlier 52.2 version used). Those two moves took lock/map from ebx
// to edi and pushed the code from 52.2 to 55.2 percent.
// What is left is ONE register choice and its cascade: MSVC materialises the
// constant 0 in ebp at the size test (`xor ebp,ebp / cmp esi,ebp`) and then
// reuses ebp for `DAT_005289d4 = 0`, `q.length = 0` and the two zero pushes.
// The original never keeps 0 in a register: it stores the 0s as immediates and
// uses esi (wraps, just zeroed) for `q.length` and the FUN_004dbe10 argument.
// Because ebp is held by that constant here, `want` is pushed out to ebx, and
// with ebx occupied the find path's iterator temporaries land in edx/ecx where
// the original uses ebx and the head test's flag lands in cl where the
// original uses dl. Give ebp to `want` and the whole function should snap into
// place; the original's callee-saved homes are edi = lock then map, esi = size
// then wraps, ebp = want, with ebx left for temporaries and `need` and `res`
// memory only.
// Tried this pass, all scored with check.py --sym:
//   * demoting `res` by reading it once into a separate variable (`blk`,
//     `void* blk`, or a plain copy): 48.0, the compiler coalesces the copy and
//     keeps res in ebp; `res` declared at the very top again (w1): 51.1.
//   * `if (n == 0)`, `if (!size)`, `size = n ? n : 1`: all still materialise
//     the ebp zero (55.2/53.7/55.2).
//   * replacing the loop's `DAT_005289d4 = 0` with `= wraps`, and/or
//     `q.length = wraps`, to remove the constant: 52.2 and worse; the constant
//     still appears.
//   * swapping the need/want declarations, declaring need+want before size,
//     declaring res after wraps, after ins, after b, after cur: 53.7 to 55.2.
//   * a dummy-declaration sweep of N = 0..400 in steps of 4 flips between
//     exactly two states, 55.6 and 46.8, with no intermediate value, so the
//     source shape is the only thing left to change.
//   * defining the real preceding function 0x4dabb0 above this one (the guide's
//     state technique) gives 55.6, the same state as the dummy sweep, so it
//     buys 0.4 points and is not worth the extra code here.
// The zero-register theory to try next: make `res = 0` not a foldable
// constant, or get the size test to emit `test` before any zero exists. Every
// spelling of the fixup tried so far emits the cmp.
#include <windows.h>
#include <memory.h>

extern unsigned int DAT_005289d4;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289d0;
extern unsigned int DAT_00528a04;

struct Node_004dacf0 {
    Node_004dacf0* left;   // +0x0
    Node_004dacf0* parent; // +0x4
    Node_004dacf0* right;  // +0x8
    unsigned int key;      // +0xc
    int length;            // +0x10
    int color;             // +0x14
};

class Class_004dbe10 {
  public:
    Node_004dacf0* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    Class_004dbe10* FUN_004dbe10(Class_004dbe10*, int);
    Class_004dbe10* FUN_004dbd80(Class_004dbe10*, int);

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dbe10 a, Class_004dbe10 b) { return !(a == b); }
};

struct Pair_004dacf0 {
    unsigned int offset; // +0x0
    int length;          // +0x4
};

struct Ins_004dacf0 {
    Node_004dacf0* ptr;
    unsigned char inserted;
};

class Class_004db610 {
  public:
    char unknown_0[4];
    Node_004dacf0* head; // +0x4
    char unknown_8[4];
    int count; // +0xc
    int total; // +0x10
    char unknown_14[20];

    void FUN_004dbbc0(Ins_004dacf0* out, Pair_004dacf0* v);
};

class Class_004db450 {
  public:
    bool FUN_004db450(unsigned int);
};
class Class_004db000 {
  public:
    Node_004dacf0* FUN_004db1c0(unsigned int);
    void FUN_004db000(Pair_004dacf0);
};
class Class_004dbd00 {
  public:
    Class_004dbe10 FUN_004dbd00(Class_004dbe10);
};
class Class_004dbeb0 {
  public:
    Class_004dbe10* FUN_004dbeb0(Class_004dbe10*);
};
class Class_004dc620 {
  public:
    Class_004dbe10* FUN_004dc620(Class_004dbe10*, Pair_004dacf0*);
};

class Class_004d8820 {
  public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d, const char* e);
};

class Class_004dc680 {
  public:
    void FUN_004dc680(Ins_004dacf0* out, Class_004d8820* v);
};

class CritSec_004da780 {
  public:
    CRITICAL_SECTION cs;
};

CritSec_004da780* FUN_004da780();
unsigned int __cdecl FUN_004da8a0(unsigned int size);
unsigned int __cdecl FUN_004da8c0(unsigned int size);
Class_004db610* FUN_004db610();
Class_004dc680* FUN_004da8d0();
void __cdecl FUN_004da7d0(unsigned int size);
char FUN_004db760();
int FUN_004db7c0();
void __cdecl FUN_004d82c0(void* at, int value, unsigned int count);

// FUNCTION: 0x4dacf0
unsigned int __cdecl FUN_004dacf0(unsigned int n, unsigned int arg2) {
    CritSec_004da780* lock = FUN_004da780();
    EnterCriticalSection(&lock->cs);
    unsigned int size = n;
    if (size == 0)
        size = 1;
    unsigned int need = FUN_004da8c0(size);
    unsigned int want = FUN_004da8a0(size);
    if (want < need) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }

    Class_004db610* map = FUN_004db610();
    unsigned int wraps = 0;
    unsigned int base;
    unsigned int len;
    unsigned int key;
    Pair_004dacf0 p;
    Pair_004dacf0 q;
    Ins_004dacf0 ins;
    Class_004dbe10 n2;
    Class_004dbe10 b;
    Class_004dbe10 cur;
    unsigned int res = 0;

    if (map->count <= wraps)
        goto alloc_new;

    q.offset = DAT_005289d4;
    q.length = 0;
    ((Class_004dc620*)map)->FUN_004dc620(&n2, &q);
    ((Class_004dbeb0*)map)->FUN_004dbeb0(&b);
    if (n2.Neq(n2, b)) {
        b.ptr = n2.ptr;
        b.FUN_004dbe10((Class_004dbe10*)&p, 0);
        if (DAT_005289d4 >= b.ptr->key && want + DAT_005289d4 <= b.ptr->key + b.ptr->length)
            n2.ptr = b.ptr;
    }
    cur.ptr = n2.ptr;

    for (;;) {
        if (cur == (Class_004dbe10(map->head))) {
            ((Class_004dbeb0*)map)->FUN_004dbeb0(&b);
            cur.ptr = b.ptr;
            DAT_005289d4 = 0;
            DAT_00528a00++;
            wraps++;
        }
        if (cur.ptr->length >= want)
            break;
        cur.FUN_004dbd80((Class_004dbe10*)&p, 0);
        if (wraps >= 2)
            goto alloc_new;
    }

    key = cur.ptr->key;
    len = cur.ptr->length;
    ((Class_004dbd00*)map)->FUN_004dbd00(cur);
    base = DAT_005289d4;
    if (base == 0) {
        base = key;
        DAT_005289d4 = base;
    }
    if (base < key)
        base = key;
    if (base + want > key + len)
        base = key;
    if (base > key) {
        p.offset = key;
        p.length = base - key;
        map->FUN_004dbbc0(&ins, &p);
    }
    if (base + want < key + len) {
        p.offset = base + want;
        p.length = key + len - base - want;
        map->FUN_004dbbc0(&ins, &p);
    }
    DAT_005289d4 = base + want;
    goto commit_block;

alloc_new:
    if (((Class_004db450*)map)->FUN_004db450(want))
        base = (unsigned int)((Class_004db000*)map)->FUN_004db1c0(want);
    else
        base = 0;
commit_block:
    if (base) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT, PAGE_READWRITE);
        if (res == 0) {
            p.offset = base;
            p.length = want;
            ((Class_004db000*)FUN_004db610())->FUN_004db000(p);
        }
    }

    if (res == 0) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }
    unsigned int pad = (0 - (n & 0xfff)) & 0xfff;
    if (FUN_004db760()) {
        FUN_004d82c0((void*)res, FUN_004db7c0(), pad);
        res += pad;
    } else {
        FUN_004d82c0((void*)(res + n), FUN_004db7c0(), pad);
    }
    {
        Class_004d8820 rec(res, n, DAT_00528a04, arg2, 0);
        FUN_004da8d0()->FUN_004dc680(&ins, &rec);
    }
    FUN_004da7d0(n);
    DAT_005289f0 += (n + 0xfff) & 0xfffff000;
    if (DAT_005289f0 > DAT_005289d0)
        DAT_005289d0 = DAT_005289f0;
    LeaveCriticalSection(&lock->cs);
    return res;
}
