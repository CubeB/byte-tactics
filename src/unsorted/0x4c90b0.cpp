// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free (third pass), edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
// #3006 retry by GPT-6.1-sol: six checks retained 93.4%; unsigned-short and
// string/operand variants did not improve the saved source.
// Second pass (deepseek-v4.1-flash): no new lever moved the +1/+5 split.
// Tested and all folded to a single `lea ecx,[ecx+edx+6]` (193 bytes, 86.1%):
// `static __inline int TotalLen(a,b){return a+b+1;}` with `malloc(TotalLen(n,m)+5)`,
// `__inline AllocBlock(len)` returning `malloc(len+5)`, `int m`/`int n` swapped,
// `size_t` casts, and `sizeof(int)+1` spellings. So the split is not an inlining
// boundary or a type spelling, which rules out the "inlined helper" theory too.
// headers.py: all 128 header sets, closest 93.4% (<windows.h>, <ddraw.h>). The
// `short` below is still the only spelling that splits the adds; it is codegen
// only (see the note further down).
// Claude Sonnet 5.5 pass (#624): compiler state ruled out (0 to 400 unused `extern
// int` declarations in steps of 8: all 199 bytes and 93.4%, and none of the 128
// header sets of headers.py gets past 93.4%). Scored without effect
// on the +1 / +5 split (all 193 bytes and 86.1%, i.e. folded to one `lea ecx,
// [ecx+edx+6]`, and without the `short` they are shorter than the original by the
// separate `add eax, 5`): a `static inline int* AllocBlock(int len)` that does the
// malloc(len + 5) and the `*block = 1` with `AllocBlock(n + m + 1)` at the call
// (also `len + 4 + 1`), a named `int len = n + m + 1;`, `len = n + m; len++;`,
// `len += m; len += 1;` and `len += 5; malloc(len)`. `int len = n ? n + m + 1 :
// n + m + 1;` (or on m) does split it into `lea eax,[ecx+esi+1]` plus `add eax,5`,
// but adds a `test; jne; lea eax,[ecx+1]` branch (201 to 203 bytes, 84.0 to 84.5%).
// The arithmetic says the original allocates n + m + 6 bytes: one for the
// terminator and five for `AllocBlock`-style header plus terminator again, as in
// 0x4c9290 (`malloc(len + 5)` for len = strlen) and 0x4c91b0, so the caller passes
// n + m + 1 to something that adds 5 and the compiler did not fold it. Note the
// original also keeps other.ptr in esi from the very first instruction (`push esi`
// before the emptiness test), which this file matches; only the sum, the position of
// the n store and the register of the second strcpy destination differ.
// space-bunny-free pass: convention checked and correct (thiscall, ret 4; malloc and
// free are cdecl, add esp,4 after each, as declared). A member `static char* Alloc(int
// len)` with `len + 4 + 1` and the call `Alloc(n + m + 1)` also folds (1 run, worse).
// Reading of the two facts together (space-bunny-free, from the bytes, not yet
// tested beyond the ternary above): the +1 and +5 stay apart in the original only
// when `n + m + 1` is a value MSVC keeps as its own node. Every single-use spelling
// folds; the one spelling that splits is the one that evaluates the same sum on two
// paths, which makes it a shared subexpression the reassociator will not reopen.
// So the original most likely uses `n + m + 1` twice, with the second use removed
// by the optimiser or living in code this reconstruction lacks. Look for what else
// could consume that sum (a length store, a bound check) before trying more
// spellings of the allocation itself.
// Append of the reference-counted string handle: concatenates other's
// characters onto this handle's, allocating a new block whose first int is
// the reference count, then releasing the old block. The handle points at the
// characters; the count is the int just before them (see 0x4c91a0, 0x4c9290,
// 0x4c93b0, 0x4c93f0). The class is named after this address because it has
// no name in data/symbols.csv.
//
// The emptiness test is a bool returned by a helper (IsEmpty below, a free
// function or a bool local both work): it is the only spelling found that
// gives the original's `xor ecx,ecx; cmp byte ptr [esi],0; sete cl; test
// cl,cl; jne end` and keeps other.ptr in esi, as the original does. A plain
// `if (*other.ptr != 0)` is shorter (check.py 91.4%) but drops those three
// instructions; a bool local materialises them in al instead of cl and moves
// other.ptr into edx (80%).
//
// Still differs from the original (best 93.4%):
//  - the allocation size is `lea eax,[ecx+edx+1]; add eax,5` in the original,
//    i.e. the terminator (+1) and the block header (+5) are two separate
//    additions, while every plain spelling of the sum folds into one
//    `lea ecx,[ecx+edx+6]`. 7840 orderings and parenthesisations of the four
//    terms (n, m and constants totalling 6) were tried, plus every type and
//    sizeof spelling; MSVC 5 always reassociates the constants. What does
//    reproduce the split is a value MSVC cannot reassociate into the lea: a
//    narrowing conversion (the `short` below) or a value from a merged branch
//    (`x ? n + m + 1 : n + m + 1`, which costs a phi and emits no code). The
//    `short` cast is a codegen experiment, NOT the original's code: it also
//    leaves a `movsx` the original has not got, and it truncates the size for
//    strings over 32k. Delete it once the real spelling is found (86.1%).
//  - with that cast the store of n (`mov [esp+0x10],edx`) lands before the
//    `push ebp` instead of after it, and the size lands in edx rather than eax.
//  - the destination of the second strcpy comes out `lea edx,[ebp+eax]` where
//    the original has `lea edx,[eax+ebp]` (same address, base/index swapped).
//    `#include <windows.h>` does not change it here.
//  - the original reloads n from the stack right after the first strcpy's
//    `rep movsd`; this version reloads it after the `rep movsb`.
//
// space-bunny-free pass 3 (1795): two things are now settled and one is a lead
// worth more than this pass had time for.
// 1. SAME-SIZE CASTS DO NOT BLOCK THE MERGE. All of these fold to one
//    `lea ecx,[ecx+edx+6]` (193 bytes, 86.1%), so the backend looks straight
//    through them: `(size_t)(n+m+1) + 5`, `(unsigned)(n+m+1) + 5`,
//    `(int)(n+m+1) + 5`, `(long)(n+m+1) + 5`, `(__int64)(n+m+1) + 5`,
//    `n + m + 1 + 5L`, `n + m + 1 + 5U`, an int->char*->int round trip, and a
//    local `int buf[2]` holding the sum read back as `buf[0] + 5`. A widening
//    or signedness cast is not the missing node, only a real narrowing is.
// 2. WRITING THE STRLENS INSIDE THE malloc ARGUMENT DOES NOT SPLIT IT.
//    `malloc((int)strlen(ptr) + (int)strlen(other.ptr) + 1 + 5)` with the `m`
//    local still present CSEs against it and folds (193 bytes, 86.1%); with
//    `m` dropped the first strlen is simply recomputed (205 bytes, 79.5%).
// 3. THE LEAD: the matched 0x4c91b0 writes
//    `malloc(strlen(text) + 1 + sizeof(int))` and gets `not ecx; add ecx, 4`:
//    its `+1` is swallowed by MSVC's strlen+1 idiom and the `+4` survives as a
//    separate add, so two constants in one `+` chain are NOT always merged by
//    the front end, and a "special" node blocks that merge for free. Here
//    both lengths are plain strlen values (`repne scasb; not ecx; dec ecx`,
//    no absorbed `+1`), so the `+1` is a genuine add that lands in the LEA
//    displacement. That means the node blocking the `+5` sits between the
//    `+1` and the `+5` and costs zero instructions, and the only ones found
//    so far are a narrowing (costs a movsx) and a phi (costs a branch). A
//    third possibility nobody has tried: a construct that makes MSVC treat
//    the value as an address operand, so the `+1` goes into the LEA and the
//    `+5` cannot be folded into a displacement, e.g. a size computed as a
//    `char*` difference or with the `+1` written as a pointer step
//    (`chars + n + m + 1` style) rather than as an integer constant.
// deepseek-v4.1 pass (2097): the +1/+5 split resists every pure-arithmetic
// spelling. Newly tested and all folded to one `lea ecx,[ecx+edx+6]` (193
// bytes, 86.1%), i.e. worse than the `short` version kept below: the operand
// orders `n + 1 + m + 5`, `1 + 4 + n + m + 1`, `n + m + 1 + 1 + 4`,
// `m + n + sizeof(int) + 1 + 1`, `n + m + sizeof(char) + sizeof(int) +
// sizeof(char)`, a `len += sizeof(int) + 1;` second statement, the multiply
// `sizeof(char) * (n + m + 1 + sizeof(int))`, the address-operand form
// `(size_t)((char*)(n + m + 1) + 5)`, and `static`/`static inline` helpers
// `AllocBlock(int len)`, `AllocBlock(int a, int b)` and `AllocChars(int len)`
// called with `n + m + 1` or `(n, m)`. The one new variant that keeps the two
// adds apart without a movsx is a file-scope `static int term = 1;` used as
// `n + m + term + sizeof(int) + 1`: the front end cannot substitute the static,
// and the backend emits `mov eax,[term]; add eax,ecx; lea ecx,[eax+edx+5]`
// (200 bytes, 91.0%). That pins the mechanism down: the front end folds
// constants only within one parse tree (here `sizeof(int) + 1` folds to a
// single +5, exactly as in the matched 0x4c9230 `malloc(len + 1 + sizeof(int))`
// -> `lea eax,[edi+5]`), so the original's `+1` and `+5` must come from two
// trees, one of whose values the front end could not substitute and the backend
// then folded the `+1` into the address of `n + m`. A plain local (promoted
// front end), a helper call, a cast and every constant spelling are all
// transparent; only a real memory value blocks it, and a load would cost bytes
// the original has not got, so what is still missing is a construct that yields
// the value `n + m + 1` as a value the front end will not re-fold with the `+5`.
// deepseek-v4.1-flash pass (2437): 40 more spellings, all folded to one lea
// (+6, 193 bytes, 86.1%), so the split is not reachable by wording: every
// parenthesisation of 1 + sizeof(int) + 1 in any order (including
// `(n + m + 1) + (sizeof(int) + 1)`, `n + m + 1 + sizeof(int) + sizeof(char)`,
// `(n + m + 1) + (int)(char)5`), an `enum { ONE = 1 }` constant, a 32-bit
// bitfield and a union round trip, a struct-return helper, `const`/`const&`
// locals, an address-taken local (`int* lp = &len; *lp + 5`), a `size_t` or
// `unsigned` mixed with an `int` length, pointer-difference and `&((char*)0)[k]`
// forms, and `__inline` helpers `Pad5()`/`Pad4()`/`TotalLen()` (also constant
// first). `unsigned short`/`char` narrowings are worse (202/203 bytes). The
// `short` below stays the best at 199 bytes / 93.4%; the residual is exactly
// the four diffs listed above (early n store, movsx, lea operand order, jump).
// The one mechanism still untested is a value the front end keeps opaque that
// costs zero instructions; no such spelling was found.
#include <string.h>
#include <stdlib.h>

class Class_004c90b0 {
public:
    char* ptr;              // refcount lives in the dword before ptr

    bool IsEmpty() const { return *ptr == 0; }

    Class_004c90b0* FUN_004c90b0(const Class_004c90b0& other);
};

// FUNCTION: 0x4c90b0
Class_004c90b0* Class_004c90b0::FUN_004c90b0(const Class_004c90b0& other)
{
    if (!other.IsEmpty()) {
        int n = (int)strlen(ptr);
        int m = (int)strlen(other.ptr);
        short len = n + m + 1;      // codegen only, see the note above
        int* block = (int*)malloc(len + 5);
        *block = 1;
        char* chars = (char*)(block + 1);
        strcpy(chars, ptr);
        strcpy(chars + n, other.ptr);
        ((int*)ptr)[-1]--;
        int* old = (int*)ptr - 1;
        if (((int*)ptr)[-1] == 0) {
            free(old);
        }
        ptr = chars;
    }
    return this;
}
