// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash,
// finished by space-bunny-free. Names are provisional.
//
// space-bunny-free, third pass, still 72.2 % (882 of 895 bytes), headers.py
// tried all 128 sets (best 72.2 %, <string.h> already as good as any). What
// still differs, stated as the one register-priority decision that causes all
// of it, in the order the allocator meets it:
//   original: ptr1 is born in ebx at 0x4aefe5 and STAYS in ebx for the rest
//   of its first life: it is only read at 0x4af016 (`sub ebp, ebx`, ptr2 -
//   ptr1 into ebp), then again at 0x4af07f and 0x4af14a (`sub eax, ebx`,
//   inside the two bubble-loop setups) before ebx is finally reused for the
//   stride at 0x4af081/0x4af150. So the original spends 4 callee-saved
//   registers on ptr1 (ebx), the first loop's cursor (edi), the ptr2-ptr1
//   stride (ebp) and i (esi), and `count` keeps its parameter home, which is
//   why the latch compare reloads it from 0x44 (0x4af043).
//   ours: ptr1 is born in ebx AND edi, the preheader `sub ebx, edi` makes ebx
//   the stride and kills the ebx copy, so ptr1 dies as a register at 0x4af016
//   and only its slot (0x1c) survives; `count` then gets the ebp that the
//   original needed for the stride, which is why our preheader is one
//   instruction longer and both bubble setups reload ptr1 from its slot.
// The cause of the tie is the register taken at 0x4aeffa: the original loads
// `count` into edx, uses it for `test edx, edx` and lets it die (it reloads
// it inside the loop), we load it into ebp and it lives to the latch. So the
// lever is still "make count's live range in the first loop end at the guard
// test", not the frame slots and not the declaration order (both exhausted
// twice). Not tried here (no time): making the guard a `do`/early-continue,
// using `count` as a `while (i != count)` with the body's `n = i` inside a
// separate block, and giving the first loop its own `char** p1`/`p2` cursors
// while keeping the `ptr1[i]`/`ptr2[i]` indexing in the body so ptr1 keeps a
// register copy (that is the shape the original's [ebx+edi] store implies).
//
// deepseek-v4.1-flash, second pass, 72.2 % (unchanged). Confirmed the
// permutation is allocator-intrinsic: reordering the nine declarations,
// renaming every local, and respelling the first loop (while with the
// increment in the body, `i != count`, char** cursor locals) each compile to
// byte-identical output (882 bytes, 72.2 %). Forcing extra liveness on ptr1
// (cursor locals kept alongside ptr1, a null test of ptr1 after the loop, a
// separate `lim = count` loop bound) only added instructions (71.0 to 71.5 %).
// Nothing here moves the first-loop register choice (original keeps ptr1 in
// ebx and spills count; this file keeps count in ebp and spills ptr1).
//
// 72.2 %. Everything structural is in place, and the
// one place I expected an MSVC 5 bug to stop me came out byte identical on
// its own: the swap of the second pointer list in the `_strcmpi` branch loads
// its base from [esp+0x1c] (0x4af0b2, 0x4af189), which in the original holds
// the split index `n`, not the `ptr2 - keys` difference that the numeric
// branch uses from [esp+0x20]. MSVC 5 produces that load from the plain
// `ptr2[i] = ptr2[i+1]` source, so it needs no trickery here.
//
// What still differs is one thing: MSVC hands out the four low local slots in
// a different order, and the register allocation in the first loop follows.
//   slot   original            this file
//   0x10   ptr2 (PTR LIST2)    swapped
//   0x14   ptr1 (PTR LIST1)    ptr2
//   0x18   swapped             n
//   0x1c   n (split index)     ptr1
// Slots 0x20 (the `ptr2 - keys` temp), 0x24 (buf2), 0x28 (buf1), 0x2c (the
// trip-count temp, then `end`) and 0x30 (`start`) are right, and the copy
// loop's `out2` and counter land in the dead parameter slots 0x40 and 0x44
// as in the original. The frame is the right 0x24.
//
// The frame order is NOT the declaration order. Verified: rewriting the nine
// declarations into exactly the order the original's slots come out in
// (swapped, ptr2, n, ptr1, buf2, buf1, end, start) leaves every slot
// unchanged, so MSVC 5 is picking these from something intrinsic (liveness
// and the first store), not from source order.
//
// The first loop names ONE cause for both the slot permutation and the
// register difference, and it is not the slots. The original SPILLS `count`:
// it reloads it from the parameter slot 0x44 at the latch compare (0x4af043),
// which frees the fourth callee-saved register, so `ptr1` keeps a register
// copy in ebx across the loop and the `ptr2 - ptr1` stride demotes to ebp.
// This file keeps `count` in ebp (0x4aeffa) and compares against it directly,
// so it spills `ptr1` instead: the stride gets ebx and `ptr1` is re-read from
// its slot at 0x4af06d. The same spill is why the bubble loop's setup is one
// instruction longer here (`mov edx, ecx`, `sub edx, eax`, `lea esi, [eax+4]`)
// where the original reuses the surviving ebx copy of `ptr1` (`sub eax, ebx`
// at 0x4af07f, `lea esi, [ebx+4]` at 0x4af07c). So whatever makes MSVC 5
// treat `count` as the lower priority of the two is the thing to try next: it
// would fix the first loop, the bubble setup and maybe the frame order at
// once. Adding a named local live across the `FUN_004b6af0` calls in that
// loop (the guide's live-node lever) makes it worse, 69.3 %.
//
// Knock-on effects of the permutation, all in the first loop and the copy
// loop: the original keeps ptr1 in ebx and the ptr2-ptr1 stride in ebp and
// spills `count` (it reloads it from 0x44 for the latch compare), while this
// file keeps ptr1 in edi coalesced with its own induction variable, the
// stride in ebx and `count` in ebp. Tried and all still permuted: every
// declaration order of the nine locals (the order is not the declaration
// order, it does not move), declaring `swapped` inside the `if`, one index
// variable per loop (all six name permutations), `while (i < count)` with the
// increment in the body, `count > i`, an `if (count > 0)` wrapper, braces
// around the `if (list2)`, `int n, swapped;` in one declaration, and walking
// the two lists with `char**` cursors. A temporary for the first line's
// result does move two of the four slots (ptr2 lands on 0x10) but costs
// instructions elsewhere, 105 differing instead of 93.
//
// The rest of the function is byte identical, including:
// - the two allocation sizes (0x17700 for the SORTED LIST buffers, 0x2ee0 for
//   the PTR LIST buffers) and the order of the four calls,
// - the first loop: both `FUN_004b6af0` calls, the `if (list2)` guard on the
//   second, and the `*ptr1[i] == '\\'` test that records the LAST absolute
//   path index (see the bug note below),
// - the two bubble passes as separate do/while(swapped) loops, the first over
//   `n` elements from 0 and the second over `start = n + 1` to `end = count-1`
//   (so together they sort every element, split at the absolute path),
// - the comparison `keys[i] - keys[i+1]` when keys is non-null and
//   `_strcmpi(ptr1[i], ptr1[i+1])` otherwise, both ascending, with the ptr1,
//   ptr2 and keys swaps in that order and the `if (list2)` / `if (keys)`
//   guards on the last two,
// - the `start`/`end` computation in the `if (n != -1)` block,
// - the join loop: `strcpy` then `p += strlen(s) + 1` for each of the two
//   lists (MSVC 5 inlines strcpy as strlen + rep movsd/movsb of len+1 bytes, so
//   the extra +1 on the advance is what the original does), the two
//   `memcpy` back over the caller's buffers, and the four frees in the order
//   ptr2, ptr1, buf2, buf1.
//
// Suspected original bug: `n` is overwritten by every line that starts with a
// backslash (0x4af03f) instead of only the first, and if no line is absolute
// the sort is skipped entirely while the buffers are still rebuilt (the jump
// at 0x4af05a). So the split index is the LAST absolute path, not the first.
#include <string.h>

void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);
char* __stdcall FUN_004b6af0(char* list, int index);
int __cdecl _strcmpi(const char* a, const char* b);

// FUNCTION: 0x4aefa0
void __stdcall FUN_004aefa0(char* list1, char* list2, int* keys, int count)
{
    char** ptr2;
    char** ptr1;
    int swapped;
    int n;
    char* buf2;
    char* buf1;
    int end;
    int start;
    int i;

    buf1 = (char*)FUN_004d83b0("SORTED LIST1", 0x17700);
    buf2 = (char*)FUN_004d83b0("SORTED LIST2", 0x17700);
    ptr1 = (char**)FUN_004d83b0("PTR LIST1", 0x2ee0);
    ptr2 = (char**)FUN_004d83b0("PTR LIST2", 0x2ee0);
    n = -1;
    for (i = 0; i < count; i++) {
        ptr1[i] = FUN_004b6af0(list1, i);
        if (list2)
            ptr2[i] = FUN_004b6af0(list2, i);
        if (ptr1[i][0] == '\\')
            n = i;
    }
    if (n != -1) {
        do {
            swapped = 0;
            for (i = 0; i < n; i++) {
                int diff;
                if (keys)
                    diff = keys[i] - keys[i+1];
                else
                    diff = _strcmpi(ptr1[i], ptr1[i+1]);
                if (diff > 0) {
                    char* t = ptr1[i];
                    ptr1[i] = ptr1[i+1];
                    ptr1[i+1] = t;
                    if (list2) {
                        t = ptr2[i];
                        ptr2[i] = ptr2[i+1];
                        ptr2[i+1] = t;
                    }
                    if (keys) {
                        int u = keys[i];
                        keys[i] = keys[i+1];
                        keys[i+1] = u;
                    }
                    swapped = 1;
                }
            }
        } while (swapped);
        start = n + 1;
        end = count - 1;
        do {
            swapped = 0;
            for (i = start; i < end; i++) {
                int diff;
                if (keys)
                    diff = keys[i] - keys[i+1];
                else
                    diff = _strcmpi(ptr1[i], ptr1[i+1]);
                if (diff > 0) {
                    char* t = ptr1[i];
                    ptr1[i] = ptr1[i+1];
                    ptr1[i+1] = t;
                    if (list2) {
                        t = ptr2[i];
                        ptr2[i] = ptr2[i+1];
                        ptr2[i+1] = t;
                    }
                    if (keys) {
                        int u = keys[i];
                        keys[i] = keys[i+1];
                        keys[i+1] = u;
                    }
                    swapped = 1;
                }
            }
        } while (swapped);
    }
    {
        char* p = buf1;
        char* q = buf2;
        for (i = 0; i < count; i++) {
            strcpy(p, ptr1[i]);
            p += strlen(ptr1[i]) + 1;
            if (list2) {
                strcpy(q, ptr2[i]);
                q += strlen(ptr2[i]) + 1;
            }
        }
        memcpy(list1, buf1, p - buf1);
        if (list2)
            memcpy(list2, buf2, q - buf2);
    }
    FUN_004d85a0(ptr2);
    FUN_004d85a0(ptr1);
    FUN_004d85a0(buf2);
    FUN_004d85a0(buf1);
}
