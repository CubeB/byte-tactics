// Decompiled by GPT-5.6-Terra, finished by muse-spark-1.3-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Retry #1784: GPT-6.1-sol confirmed 56.5% after twelve worker checks; the final combined check also did not MATCH.
//
// Best checked source this pass: 56.5% (check.py, 488 bytes against the original's 494); no MATCH. The scan-index form scan = entries + j raises the reported score, but the disassembly still differs broadly and this appears to be a similarity-alignment artifact, not a close match. The
// control flow, the two inlined strlens, the two tolower pairs and both loop
// shapes are right. What is left is one allocator state, clearest in the
// prologue:
//
//   original: mov ebx,[edx+4] / mov [esp+0x1c],ebx / ... / mov dl,[esi]
//   here:     mov edx,[edx+4] / mov esi,edx / mov [esp+0x20],edx / mov bl,...
//
// The original keeps `entries` in EBX, a callee-saved register, spills it once
// into the obj parameter slot at [esp+0x1c], and so EBX is free later for the
// first tolower result; its `text` pointer stays memory resident in the index
// parameter slot at [esp+0x20] and is reloaded after each call. This version
// keeps `entries` in the volatile EDX and a register copy of `text` in EBX, so
// the first tolower result has to take EDI, which pushes the scan index `j`
// into a stack slot. Forcing `text` to memory (address taken, v9/v11 in
// build/scratch/0x4a05e0) changed nothing, so the remaining lever is making
// `entries` land in EBX. The dead `if (entry == 0) return;` is there because
// one extra live reference there is worth 2.2 points.
//
// Correction (deepseek-v4.1-flash, issue 1177): the claim above that the
// original keeps `entries` in EBX is wrong. Disassembly shows EBX holds the
// live-across-call first-tolower temp `a` (mov ebx,eax at 0x4a0710), and
// `entries` is reloaded from the obj-slot spill [esp+0x1c] at 0x4a0752 and
// 0x4a076a. All four callee-saved registers are loop state in the original
// (i=ebp, scan=esi, j=edi, a=ebx), so BOTH `entries` ([esp+0x1c]) and `text`
// ([esp+0x20]) are memory-homed. The remaining lever is to make MSVC rank
// i/j/scan/a above `text`, not to put `entries` in EBX.
//
// Tried this round and rejected (all in build/scratch/0x4a05e0):
//  a1 swapping the declaration order of `length` and `entry`,
//  a2 wrapping the second chain in `if (entry->type == type)` (MSVC CSEs both
//     loads and folds the branch to a plain jmp, the original keeps `cmp dl,dl`),
//  a3 `for (scan = entries, j = 0; ...)` as one declaration,
//  a4 reading `text` through a second local before the strlen,
//  b2 `text = &entry->u.text[0]` instead of array decay,
//  b3 an extra `if (entries == 0) return;` (50.6%),
//  b4 an extra `if (scan == 0) break;` in the inner loop (51.2%),
//  b1 `Entry_004a05e0* const entries` does not compile (C++ const pointer).
//  a1 to a4 and b2 are byte-identical to the base, so they are free dead ends.
//
// Other known differences:
//  * the original has `cmp dl,dl / je 0x4a0659` where this source gives a plain
//    `jmp` over the `else if (type != 1) return;` test,
//  * the original's `entry` frame slot is [esp+0x10] and `length` is [esp+0x14];
//    here they are the other way round, and declaration order does not change it,
//  * the type==5 arm falls into the common strlen block in the original but
//    jumps to it here.
//
// deepseek-v4.1-flash re-checked the allocation levers and confirmed 52.2% is
// the ceiling for this shape. The whole diff is still the single choice of
// which long-lived pointer gets EBX: the original gives EBX to `entries` and
// spills `text` to the index-argument slot; here EBX goes to `text` (and to
// `type` before `text` is defined) and `entries` is spilled to that same slot.
// Everything else (the DL vs BL type, the entry/length slot swap, `j` in a
// stack slot) follows from that one choice.
// New levers tried and rejected this round (all in build/scratch/0x4a05e0/ds):
//  * no `type` local at all (use entry->type directly): byte-identical to base,
//  * `Entry_004a05e0* scan = entries + j;` inside the loop instead of the
//    pointer `scan++`: difflib jumps to 56.5% but it is a difflib artifact
//    (the original increments a scan pointer by 0x15b, so this is further from
//    MATCH, not closer),
//  * `short length`: difflib 53.1% but it changes how the strlen result is
//    used (test cx,cx / movsx), so again not closer to MATCH,
//  * `int count = entries->u.count;` / `short count`: 46.9%,
//  * `entry->u.text` recomputed in the loop: 39%,
//  * `if (type == 1 && (flags & 0x10000) != 0) return;` for the first test, and
//    `if (type != 1 && type != 5) return;` split forms: no change,
//  * more dead references to `entries` (before `length`, after the type read,
//    `if (entries == scan)`): all dropped to 49 to 51%,
//  * declaring `text` first or swapping its declaration with `entries`: no
//    change.
// The remaining lever is a source construct that makes MSVC 5 prefer to keep
// `entries` in a register and leave `text` in memory; the dead `if (entry == 0)`
// check is the only known thing that moves the number at all (52.2 with it,
// 50.0 without).
//
// space-bunny-free round (all variants in build/scratch/0x4a05e0/w2, base 52.2%):
// Confirmed the diff is one allocator state and nothing else. The two register
// homes the original needs are `entries` and `text`; this source homes
// `entries` and `j` instead, and the frame slots and the `jmp` for `cmp dl,dl`
// all follow from that swap.
// Rejected, all free scratch scores, none better than the base:
//  * declaration order is irrelevant here. 8 permutations of the local block
//    (entry first, j first, text first, entries after text, ...) give either
//    the base allocation or a slightly worse one, never a new allocation. C1
//    is ranking these locals by something other than declaration order.
//  * defining `entries` or `entry` in their own declaration (mixed
//    declarations after the `index == -1` test): byte identical.
//  * no `entries` local at all, `obj->data->entries` written at each of its
//    four use sites, hoping for a CSE commutator with a stack home the way the
//    original's [esp+0x1c] looks: 42.1%. With `entries` kept and only `text`
//    inlined it is 43.8%, with only `entries` inlined it is 48.8%. Both locals
//    are needed.
//  * no `text` local either, `entry->u.text` at every use: 40.7%.
//  * no `entry` local, `(entries + index)->` at every use: 50.0%.
//  * the two tolower calls compared directly with no `a` and `b` temporaries
//    (which is what `cmp ebx, eax` in the original looks like): 47.0%, and 507
//    bytes, so the named temporaries are right.
//  * `if (text == 0) return;` after `text` is defined, to demote `text` out
//    of EBX: 50.6%. A live reference to `scan` after the inner loop: 51.9%.
//    `if (type == 1) type = entry->type; else type = entry->type;`: 51.4%.
//  * exit block reading `entry->u.text[i]` instead of `text[i]`, to cut two
//    references to `text` and so change its rank: 50.9%. Inner loop as
//    `for (j = 0; ; j++) { if (j > count) break; ... }` on top of that: 50.9%.
//    The same loop compared the other way round, `count >= j`: 50.3%.
//  * `while` instead of `for` for either loop: byte identical.
//  * arms swapped so the `type == 5` arm comes first: 51.6%.
//  * an empty nested `if (type == 5) { }` at the end of the `type == 5` arm,
//    to try to make C1 emit the original's redundant `cmp dl,dl / je`: C1
//    deletes the empty if and still emits the plain `jmp`, byte identical.
//    A `switch (type)` with `case 5 / case 1 / default: return` gives 49.4%.
//  * `char* const text` and `Entry_004a05e0* const entries` do not compile
//    (confirmed again, same as the earlier b1 attempt).
// So `cmp dl,dl / je` at 0x4a064c still has no source construct: an empty if is
// deleted, and the redundant compare in the second chain at 0x4a0679 only
// survives because that nested if has a real body.
// Open: something makes the original's C1 keep `entries` in a callee-saved
// register and give `text` a home, while here `text` outranks both `entries`
// and `j`. Nothing tried this round moves that ranking.
//
// Second pass (deepseek-v4.1-flash, issue 1542) confirmed 52.2% is the local
// maximum. New levers tried and rejected this round (all in
// build/scratch/0x4a05e0/ds2):
//  * `register` on entries / text / j: byte-identical to the base (and on a
//    struct pointer it does not compile),
//  * `char type` instead of `unsigned char type`: byte-identical,
//  * `entry = &entries[index];` instead of `entries + index`: byte-identical,
//  * a foldable extra `if (j > entries->u.count) break;` at the top of the
//    inner body: 49.2% (and it grows the code),
//  * reordering the declarations to put i, j, scan first: 49.7%,
//  * initialising every local at its declaration: 48.9%,
//  * a dummy static function or global placed before this one, and forward
//    declarations of the siblings FUN_004a0570 / FUN_004a07d0: byte-identical,
//  * tools/headers.py: all 128 include sets give 52.2% or less.
// The single remaining difference is still that MSVC gives the fourth
// callee-saved register to `text` here and to `j` (with `entries` and `text`
// both memory-homed) in the original.
#include <string.h>

int __cdecl tolower(int);

#pragma pack(push, 1)
struct Entry_004a05e0 {                 // 0x15b bytes
    unsigned char type;                 // +0x0
    char unknown_1[0x1b - 0x1];
    unsigned int flags;                 // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                    // +0xb6 (entry 0)
        char text[0x15b - 0xb6];        // +0xb6
    } u;
};

struct Data_004a05e0 {
    int unknown_0;
    Entry_004a05e0* entries;            // +0x4
};

struct Object_004a05e0 {
    char unknown_0[0x18];
    Data_004a05e0* data;                // +0x18
};
#pragma pack(pop)

// FUNCTION: 0x4a05e0
void __stdcall FUN_004a05e0(Object_004a05e0* obj, int index)
{
    Entry_004a05e0* entries;
    Entry_004a05e0* scan;
    char* text;
    int length;
    Entry_004a05e0* entry;
    int i;
    int j;
    unsigned char type;

    if (index == -1)
        return;

    entries = obj->data->entries;
    entry = entries + index;
    if (entry == 0)
        return;
    type = entry->type;
    if (type == 1) {
        if ((entry->flags & 0x10000) != 0)
            return;
    }
    if (type == 5) {
        if (strlen(&entry->u.text[0x136 - 0xb6]) == 0)
            return;
    } else if (type != 1) {
        return;
    }

    if (type == 1) {
        if (entry->u.text[0x136 - 0xb6] != 0) {
            entry->u.text[0x13a - 0xb6] = 0;
            return;
        }
        if (type == 1) {
            if (strlen(entry->u.text) == 0)
                return;
            entry->u.text[0x13a - 0xb6] = 0;
            text = entry->u.text;
        }
    } else if (type == 5) {
        text = entry->u.text;
        entry->u.text[0x147 - 0xb6] = 0;
    }

    length = strlen(text);
    if (length == 0)
        return;

    for (i = 0; i < length; i++) {
        if (text[i] != ' ') {
            for (j = 0; j <= entries->u.count; j++) {
                scan = entries + j;
                if (scan->type == 1) {
                    int a = tolower((signed char)scan->u.text[0x13a - 0xb6]);
                    int b = tolower((signed char)text[i]);
                    if (a == b)
                        break;
                } else if (scan->type == 5) {
                    int a = tolower((signed char)scan->u.text[0x147 - 0xb6]);
                    int b = tolower((signed char)text[i]);
                    if (a == b)
                        break;
                }
            }
            if (j > entries->u.count) {
                if (entry->type == 1) {
                    entry->u.text[0x13a - 0xb6] = text[i];
                    return;
                }
                if (entry->type == 5) {
                    entry->u.text[0x147 - 0xb6] = text[i];
                    return;
                }
                return;
            }
        }
    }
}
