// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free. Names are provisional.
// Claude Sonnet 5.5 pass (#624): compiler state ruled out (0 to 400 unused `extern
// int` declarations in steps of 8: all 199 bytes and 93.4%). Scored without effect
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
