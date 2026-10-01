// Decompiled by space-bunny-free, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Retry (deepseek-v4.1-flash, ten-minute box): spelling the first fill loop
// as i = 0; while (i < count) { ...; i++; } is byte-identical at 72.2 %
// (882 bytes), so the count-in-ebp vs count-reload tie is not the loop
// statement spelling. This file remains the best known (72.2 %, 882 of
// 895 bytes); the first-loop register/frame-slot tie below is still the gap.


// Probe (deepseek-v4.1-flash, ten-minute box): `int* pc = &count;` used only
// in the first loop guard is folded away by MSVC (byte-identical, 72.2 % /
// 882 bytes), so an address-taken spelling does not force the original's
// count spill either.

// GPT-6.1-sol retry: tools/headers.py tried all 128 header sets with no improvement; best remains 72.2%. The first-loop count/ptr1 register and low-frame-slot allocation still differ from the original.
//
// deepseek-v4.1-flash, seventh pass, still 72.2 % (882 of 895 bytes). The rest
// of the function is byte identical; only the first loop and the four low
// frame slots differ (see below). New variants tried this pass, all scored with
// check.py --sym, none better:
//   - first loop with a `char** p1` cursor for the ptr1 store and the backslash
//     test still on `ptr1[i]`: 900 bytes, 55.2 % (the cursor and the base both
//     stay live, so MSVC puts the test on the base and adds a reload).
//   - `p1[i]` alias for the store with the test on `ptr1[i]`: 882 bytes, 72.2 %,
//     byte identical (MSVC folds the alias into ptr1).
//   - both lists walked with `*p1++` / `*p2++` cursors and the test on the
//     cursor: 911 bytes, 52.6 %.
//   - `int n = -1;` and `int swapped = 0;` initialisers at the declarations
//     instead of the later assignment: 884 bytes, 71.6 % (worse, the
//     initialiser moves a slot).
// The block below is a byte-identical 882-byte body, so the compiler state for
// this shape is fixed. The decision to reproduce is still the preheader one:
// the original loads `count` into edx (scratch) for the guard only, keeps ptr1
// in ebx, the ptr2-ptr1 stride in ebp, the cursor in edi and `i` in esi, so
// the latch reloads `count` from the parameter slot [esp+0x44] (0x4af043).
// This file instead keeps `count` in ebp, gives the stride ebx, and spills ptr1
// to slot 0x1c: MSVC coalesced ptr1's birth register with the loop cursor
// (edi), so ptr1's base is not live across the loop and loses the callee-saved
// register to count. That is why the frame slots come out
// 0x10 swapped / 0x14 ptr2 / 0x18 n / 0x1c ptr1 here, instead of the original
// 0x10 ptr2 / 0x14 ptr1 / 0x18 swapped / 0x1c n. Everything else (the two
// allocation sizes and call order, both bubble passes, the start/end split at
// the last absolute path, the join loop with the inlined strcpy and the two
// memcpy calls, and the four frees in ptr2/ptr1/buf2/buf1 order) is identical.
//
// Prior passes already exhausted: every declaration order of the nine locals
// (24 permutations, no slot change), renaming/retyping (unsigned, const) every
// local, register on the pointers, do/while and while(1)+break rewrites, a
// distinct index variable per loop, `i != count` / `count > i`, an extra
// `if (count > 0)` guard (flips the tie but costs 2 instructions, 916 bytes),
// `*p++` cursor shapes, point-of-use declarations, a static single-use
// FillLists helper, and 128 header sets via headers.py. The one untried lever
// noted by earlier passes is a shape that keeps ptr1's base live across the
// loop without adding instructions; I did not find it.
//
// deepseek-v4.1, fifth pass, still 72.2 % (882 of 895 bytes). Everything that
// follows is a byte-identical 882-byte body (verified with check.py on scratch
// copies, same 94/92-line diff), so the compiler state is fixed for this shape:
// declarations at point of use for the four allocations, `char** const` /
// `register` on the pointers, `const int count`, `unsigned count`, a dummy-extern
// TU-state sweep (N = 1, 8, 16, 64, 128, 240, 256, all inert), do/while or
// while(1)+break rewrites of both bubble passes, `int diff` / `char* t` hoisted
// to function scope, the same swap written with two temps, `if (keys != 0)`,
// and an inlined SwapP/SwapI helper for the swap block (that one is 882 bytes
// but 71.6 %, its diff re-aligns). New shapes that DID move bytes, all worse:
// a p1/p2 cursor in the first loop with `ptr2[i]` left indexed (887 bytes,
// 68.7 %, gives ptr2 a cursor in ebx and keeps count in ebp), both cursors
// (882/71.9), `char* s` for the first call (892/69.6), `if (count > 0)` around
// the for (916/47.9), while(i<count) with the guard (892/65.7), and i = 0
// moved inside the if of a do/while (889/70.2). A micro repro shows the
// mechanism the original used: an inlined two-list copy loop (same store to
// ptr1[i], guarded second list, ptr2[i] via a stride) keeps count in memory and
// reloads it at the latch (`mov eax,[count]; cmp esi,eax`) exactly like the
// original, but only while the stride has to live in a stack slot; our full
// function gives the stride ebx and promotes count to ebp, so the tie is which
// of ptr1 (live over the loop only for the post-loop sort setup) and the bound
// keeps the fourth callee-saved register. Nothing in the source shape tried
// moves that tie: the diff is still the same slot permutation (0x10/0x14/0x18/
// 0x1c = ptr2/ptr1/swapped/n in the original, swapped/ptr2/n/ptr1 here) and the
// first-loop register swap that follows from it. Next lever candidates: an
// inline helper around the whole first loop, or a shape that makes ptr1's
// post-loop uses cheaper than a count reload.
//
// deepseek-v4.1, fourth pass, still 72.2 % (882 of 895 bytes). No new best
// variant, but the search space is now mapped: the first loop's ONE allocator
// tie is reachable from source shape, and every shape that flips it costs
// instructions elsewhere.
// Measured with `tools/wcl /Fa` listings (free), scored with check.py:
// - all 24 permutations of the four low declarations (ptr2, ptr1, swapped, n)
//   compile to the identical 882-byte body, so the slot order is not
//   declaration order (confirmed, not guessed).
// - an extra `if (count > 0)` before the first loop DOES flip the tie: count
//   drops to memory (loop tail becomes `mov eax,[esp+0x44]; cmp esi,eax` as in
//   the original) and ptr1 keeps esi, but the duplicate test costs 2
//   instructions (916 bytes, 47.9 %).
// - `*p++` cursor shapes (`*p = FUN(list1,i); ... p++;` and the for-increment
//   spelling) also un-promote count, but MSVC then gives ptr2 its own cursor
//   (`[ebx]`) instead of the `ptr2[i]` stride, 66.0 to 68.7 %.
// - a distinct index variable per loop (i1/j1/j2) un-promotes count too, but
//   the ptr1 store becomes `[esi+edi*4]` (no strength reduction), 69.8 %.
// So the wanted combination (indexed stores, one cursor, count in memory)
// needs count to lose the promotion without any of those structural costs.
// The current best keeps ptr1's home at 0x1c and count in ebp.
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
// space-bunny-free, fourth pass, still 72.2 % (882 of 895 bytes), and the diff
// is now fully explained as ONE allocator decision, in a form precise enough
// to search for directly. The original and this file are structurally
// identical everywhere (same instruction counts in every block, verified with
// a two-column address-aligned disassembly); what differs is which value owns
// each callee-saved register inside the FIRST loop, and the four low frame
// slots that follow from it:
//   register   original (0x4aefe5..0x4af04d)     this file
//   esi        i, the loop index                 i
//   edi        the cursor, ptr1 + i*4            ptr1 AND the cursor (coalesced)
//   ebx        ptr1's base, live across the loop the ptr2 - ptr1 stride
//   ebp        the ptr2 - ptr1 stride           count, reloaded nowhere
//   memory     count (reloaded from 0x44 at the latch, 0x4af043)
// So the original spends four callee-saved registers on {i, cursor, ptr1,
// stride} and squeezes `count` out to memory, and we spend them on
// {i, ptr1+cursor, stride, count}. Because ptr1 and the cursor share one
// register here, MSVC can fold the cursor's initialisation into the preheader
// (`sub ebx, edi`, one instruction) instead of copying it (`mov ebp, eax /
// mov edi, ebx / sub ebp, ebx`, three), and that is the whole 13-byte
// difference. Two rank inversions have to be undone at once: the induction
// variable must outrank ptr1 (so the cursor gets edi of its own and ptr1
// keeps ebx), and the stride must outrank count (so count goes to memory).
// New shapes tried this pass, all scored free with check.py --sym, none better
// than the 882-byte 72.2 % body:
//   - a named `char** p1` cursor with the increment in the body, in the for
//     increment, and declared `register`, each with the test on `(*p1)[0]`:
//     887 bytes, 68.7 to 69.0 % (MSVC gives the stride a cursor and keeps
//     count in ebp).
//   - both cursors `*p1++ / *p2++` with the test on `*p1`: 882 bytes, 72.2 %,
//     byte identical (the two cursors fold back into ptr1 and ptr2).
//   - `char* s = FUN_004b6af0(...)` with the test on `s[0]`: 892 bytes, 69.3 %
//     (s has to survive the second call, so it gets a slot).
//   - the backslash test moved above the `if (list2)` block: 889 bytes, 70.1 %.
//   - `if (count > 0)` in front of the loop: 916 bytes, 47.9 % (flips the tie
//     but the duplicate guard costs two blocks of instructions).
//   - extra graph references to `count` that fold away in the final code, as a
//     way to demote it to memory: `end = count; end--;`, `end = count;
//     end -= 1;`, `for (i = 0; i <= count - 1; i++)`, `i != count`,
//     `while (i < count)` with the increment in the body, and a second index
//     variable for the join loop: all five give the byte identical 882-byte
//     body, so at this size `count` has one rank and no spelling of it moves.
//   - the first loop with its own index (`i1`) whose value is passed to both
//     calls and used for both stores: 882 bytes, 72.2 %, byte identical.
//   - `while (1) { if (i >= count) break; ... i++; }` on the first loop (the
//     guide's item 9 form): 850 bytes, 55.6 %, the loop stops being a loop
//     MSVC strength reduces; `do { ... } while (i < count)` after an `if`:
//     882 bytes, 72.2 %, byte identical; `while (i++ < count)` with `i - 1`
//     for every index use: 910 bytes, 66.1 %.
// Conclusion for the next pass: the lever is not in the first loop's spelling.
// It has to be a construct that adds a live node ABOVE `count` in the priority
// order without emitting an instruction, or a loop form in which MSVC keeps
// the induction variable out of ptr1's register. Two-column diff tool left in
// build/scratch/0x4aefa0/cmp2.py (prints the original and our disassembly side
// by side by address, marking every line that differs).
//
// deepseek-v4.1, sixth pass, still 72.2 % (882 of 895 bytes). New evidence on
// which value the allocator keeps across the first loop: the original uses
// ptr1 in ebx AFTER the loop at 0x4af07c (`lea esi,[ebx+4]`, the pass 1 cursor)
// and 0x4af14a (`lea esi,[ebx+edx*4+4]`, the pass 2 cursor), so ptr1 stays in
// ebx across the whole first loop while `count` is re-read from the argument
// slot at the latch (0x4af043 `mov eax,[esp+0x44]`). Our body does the
// opposite: ptr1 is home in edi and edi is recycled as the loop cursor, so ptr1
// is re-read from its slot at both of those sites and count is promoted to ebp.
// Twenty source shapes were compiled and measured this pass, every one a
// byte-identical 882-byte body at 72.2 %: `i != count` and `count > i` guards,
// `*ptr1[i]` for the backslash test, `i = 0;` split out of the for initializer,
// the increment moved into the body, `register int i`, `for (n = -1, i = 0; ...)`,
// `if (list2 != 0)`, a separate `i1` index for the first loop only, point-of-use
// declarations for buf1/buf2/ptr1/ptr2, `char** const cp = ptr1;` used for the
// store with the test still on ptr1 (MSVC folds cp into ptr1), a static
// single-use FillLists helper that gets inlined back to the same code,
// reordering the whole local declaration list, and `#include <windows.h>`.
// The pass 1 setup also confirms the slots: 0x10 ptr2, 0x14 ptr1, 0x18 swapped,
// 0x1c n, with keys (arg3) at 0x40 overwritten by ptr2-ptr1 and the pass 1 trip
// count at 0x2c.
#include <string.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
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
