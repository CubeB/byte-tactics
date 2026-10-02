// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// 2026-10-02 (Space Bunny Free): 58.1% -> 67.6%, 1843 bytes. Six source changes,
//   every one scored on its own and in combination. The first two are worth far
//   more together than apart.
//   1. The `out` block tests the scroll-up case positively
//      (`if (point.y < r.y0) goto scroll_up; goto scroll_down;`), so scroll_up
//      is the out block's fall-through and lands before scroll_down: the
//      original's `cmp edi,ebx / jge 0x4a3def` at 0x4a3d27. Alone 57.5%, and with
//      (2) it is most of the jump from 58.1.
//   2. scroll_up clamps the field, not a local (`if (me->field_ba < 0)
//      me->field_ba = 0;`), which is what produces the original's
//      `mov cx, word [ebp+0xba] / test cx,cx / jge`.
//   3. `} else if ((flags & 0x20) | 0x80) {`: the original's
//      `mov ecx,eax / and ecx,0x20 / or cl,0x80 / test cl,cl`. The older note
//      here that this spelling scores worse was measured on the 52.3% base; on
//      this base it is 0.2 points worse alone, but it is the only spelling that
//      emits those three instructions at all, so it stays.
//   4. The 0x10 block reads `if ((short)line >= 0) { big } else { restore }`,
//      so its two-instruction restore block is laid out after the second loop,
//      where the original puts it at 0x4a3c18.
//   5. `+ (unsigned short)me->field_bc` in the FIRST FUN_004ab570 block's line
//      addition only. That mixed signedness is what gives the original's
//      `mov cx, word [ebp+0xbc] / ... / add eax, ecx / ... / movsx edx, cx /
//      lea eax, [eax + ecx - 1]`: one use of field_bc is zero-extended and one is
//      sign-extended. The same cast in the 0x10 block costs 12 points (55.8%), so
//      only the first block wants it.
//   6. The 0x20/0x80 walk's second pointer is a `char*` stepped in bytes
//      (`*(char**)(itemp + 0x28)`, `itemp += 4`) instead of an `int*` stepped in
//      words: 67.0% -> 67.6%. It also folds the pointer into the
//      `lea [eax+ecx*4+0x28]` the original has at 0x4a3c85.
//   The two FUN_0049fc50/FUN_004ab690 tails no longer merge: each keeps the
//   original's `push N / mov edx,[esp+0x54] / push edx / call / jmp`. The nudge
//   (`if (i < 1) break;`) is still what buys the esi/edi split; deleting it drops
//   67.0% -> 58.7%, so it stays as a diagnostic.
//
// What still differs, biggest first:
//   (a) The second loop (`type == 2 && kind == me->kind`) is one register
//       rotation: the original keeps `entries` in esi and the loop counter in
//       edi, here the counter keeps esi and `entries` takes edi, and the walk
//       pointer is ecx where the original uses eax. The instruction sequence is
//       otherwise identical, down to the `[reg-0xc0]` displacements, so the
//       pointer is biased to field_c0 either way. The cause is that this is the
//       same variable `i` the first loop used, so its web wins the preferred
//       register. Giving the second loop a fresh counter (`int j`, in block
//       scope or at function scope, with or without its own walk pointer) is
//       byte-identical: MSVC coalesces the two counters. A folded extra use of
//       `entries` (to outrank the counter) also changes nothing.
//   (b) The 0x20/0x80 prologue. The original builds only the pointer its flag8
//       path uses, with a dead `mov esi, obj` on the other path and a dead `je`
//       that reuses the outer test's flags; here both pointers are built. Every
//       spelling that builds only one of them (`char* fixed;` plus
//       `if (flag8) fixed = ...;`, and its mirror) collapses the score to about
//       43%, because MSVC then has to re-test flag8 inside the loop.
//   (c) The flags word. The original spills `me->flags` to [esp+0x20] and keeps
//       it in eax for the 0x20/0x80 test, here it stays in ecx and the test
//       copies it to edx. Declaring `flags` at function scope, as an int, as an
//       unsigned, or through a second local is byte-identical or far worse
//       (43%). `unsigned flags` is in this file only because it turns the
//       `sar ecx,7` into the original's `shr eax,7`.
//   (d) The prologue's point.x/point.y schedule, unchanged by every earlier pass:
//       the original reloads point.x into edi, subtracts, stores it, then
//       reloads point.y into that same edi; here point.x's subtraction is done
//       in ecx and point.y's reload is hoisted above it. Subtracting y first
//       (66.6%), copying the two differences into plain ints, a separate copy
//       statement, and a hoisted loop bound are all within 0.4 points of this
//       file; none of them moves the schedule.
//   (e) 11 bytes are missing, so every internal jump target from the first loop
//       on is out by the same amount. Two spellings account for all of it: the
//       extra `mov edx, ecx / and edx, 0xffff` before `add eax, ecx` in the
//       first ab570 block (5 bytes: MSVC only skips that zero-extend when the
//       register is already known clean, and every cast or local spelling of
//       the addend either keeps it or costs 8 points), and the nudge's
//       `cmp esi,1 / jl` (5 bytes, the price of the esi/edi split). LEAD:
//       build/scratch/0x4a3780/v_w1b.cpp scores the same 67.6% at 1831 bytes,
//       one byte under the original, by clamping scroll_up's index in an `int
//       sel` local instead of in the field. With the sizes that close the
//       remaining diffs are register names only, so that is worth trying first.
//   (f) Smaller, unchanged: `mov [esi+0xcca], 1` (the original has obj in esi
//       there, this compile reloads it), `mov cx, [esp+0x10]` against `mov dx`,
//       `lea eax, [ecx+eax-1]` operand order in scroll_down, and `push esi`
//       against `push ecx` for the field_ce callback.
// permute.py was run on the 63.7% version of this file and found nothing better
//   than 64.0%; its best.cpp keeps a `tmp0` temporary and an `inl4` helper that
//   just returns its argument, so nothing was taken from it. Scoring harness
//   and every variant tried: build/scratch/0x4a3780/ (sc.sh scores a scratch
//   file, gen.py holds the substitutions, gv.sh does both, asm.sh writes the
//   listing, hunks.py ranks the diff hunks).
// 2026-10-01 retry 9 (deepseek-v4.1-flash, #3660): best stays 58.1% (this file,
//   the nudge version). About 100 check.py runs, no new best. What this pass
//   established, all scored:
//   - MSVC 5 folds dead arithmetic and duplicate/CSE'd comparisons BEFORE the
//     register allocator, so they add no weight. Byte-identical to the 52.3%
//     no-nudge base: `n += i - i`, `i = i`, `int j = i;`, `entries[i - 1 + 1]`,
//     `entries[i + 0]`, `entries[i * 1]`, `(int)(unsigned)i`, a duplicate
//     `i < count+1` in the for header, `(rr = i) < count+1`, `rr = i` in the
//     increment, and `if (i < 1) return 0;` before the loop (i is constant 1
//     there, so it is folded away before allocation).
//   - Only a LIVE emitted branch on i flips the esi/edi rotation, and every
//     natural construct that emits one adds the same 2 instructions as the
//     nudge: `if (i >= count+1) break;` at the body top (57.5%, cmp/jge),
//     `e->type == 7 && i >= 1` (57.9%, cmp esi,1/jl), `i < count+1 && i <
//     count+1` in the header (57.5%, cmp esi,eax/jge). None beats 58.1%.
//   - The 0x4bcb50 dead-call-assignment lever does not apply: the loop's only
//     call (FUN_004c1420) is on the break path, and an inline wrapper taking i
//     there (`SelectAt_004a3780(entries, i)`) is fully coalesced (byte-
//     identical to no-nudge). Inline getters never fold: the exact-negation
//     check compiles to `setge/dec/and` (53.2%, G3/z2 shape), a returning
//     getter spills the walk pointer to the stack (49.8%).
//   - The 0x10-block nsel/bc swap does not move either: `short nsel =
//     me->field_c0;` (plain, `int`, via an inline getter, or declared right
//     after orig_sel) all compile byte-identically to this file; MSVC
//     rematerialises nsel into ax at the clamp and gives si to field_bc.
//   - Loop spellings with a live extra use that keep the walk compiler-made
//     (`while`, `do/while`, goto, `++i` in the condition) are 50.7-51.9% and
//     move the entry test's branch polarity.
//   - Source reorderings of the point.x/y subtractions with `int i = 1;`
//     between them are byte-identical (58.1%); y-first drops to 57.7%.
//   - The flip is NOT a plain use count. An extra LIVE arithmetic use of i in
//     the latch (`i - 1 < entries[0].count`, which emits `lea edx,[edi-1]` in
//     every iteration) does NOT flip (49.9%). Only an extra BRANCH directly on
//     i in the first loop flips (nudge, `i >= 1`, duplicate header compare).
//     A branch on i in the second loop (q9a/q9c) or after the loop (q9b) does
//     not flip, so the weight is local to the first loop's web.
//   - The tie is against point.y: replacing the four point.y compares in the
//     two FUN_004ab510 blocks with another variable flips the whole function
//     to the original's allocation without the nudge (point.y -> orig_sel
//     58.3%, -> span 47.2%), while removing two of the four does not (52.6%).
//     So one loop branch on i weighs about four straight-line point.y uses.
//   - Dead-looking locals do not count: `int py = point.y;` inside the ab510
//     blocks, `int r = FUN_004c1420(...)` with the callee redeclared int, an
//     inline InRect(x, y, r) helper, an inline Self(i) returning i used in the
//     loop header, `(rr = i) < bound`, and nested-if / continue / else / goto
//     spellings of the body are all byte-identical to the no-nudge base.
//   Remaining diffs unchanged: (a) the nudge's cmp esi,1/jl, (b) the point.x/y
//   schedule (point.x in ecx here vs edi in the original, point.y hoisted),
//   (c) the 0x10 block nsel/bc swap, (d) the FUN_004ab690 tail merge. The
//   nudge still needs a natural BRANCH on i in the first loop that emits no
//   code; nothing in this pass found one.
// 2026-10-01 retry 7 (deepseek-v4.1-flash): 52.3 -> 58.1%, 1800 bytes. The esi/edi
//   rotation is BROKEN. Root cause found: MSVC 5 weights register priority by uses, and
//   this loop counter is exactly one live use short of point.y, so point.y takes esi and
//   the counter takes edi. Any extra live use of the counter inside the loop flips the
//   whole function to the original's allocation (point.x temp in edi, counter in esi,
//   point.y in edi, loop pointer in ecx, n folded from memory) and the code aligns with
//   the original from the loop exit to the end of the function.
//   Levers that flip it (all scored): `if (i < 1) break;` at the loop bottom (58.1%,
//   this file), `if (i >= 1) { ... }` around the body (57.9%), `i < count+1 && i >= 1`
//   in the for condition (57.9%), a getter with a range check (53.2%), `if (i < 1)
//   break;` before the type check (57.9%). Levers that do NOT flip it: an extra use
//   after the loop, an empty `if` (removed before allocation), a duplicate CSE'd
//   comparison outside the loop, `i - 0`, pointer walks, `e = &entries[i]`, the helper
//   returning i (`int i = FindGroup(...)`, 49.0%), swapping declarations, moving
//   `int i;`/`int i = 1;` (all 52.3%), headers.py (768 sets, all 52.3%).
//   The flip needs a LIVE use of the counter inside the loop, so the extra branch is a
//   diagnostic nudge, not the original's source; the original has no such branch and
//   its loop is 2 instructions shorter. THE NEXT STEP: find the natural construct that
//   adds one live counter use and folds away (docs/agent-guide.md "Register priority":
//   an inlined sibling getter with its own range check inside an identical explicit
//   check). Every getter spelling tried emitted a real range check (setge/dec/and or
//   cmp/jge), so the fold has not been found yet.
//   Remaining diff with the nudge in place: (a) the 2 extra nudge instructions
//   (`cmp esi,1 / jl`); (b) the point.x/y schedule: the original computes point.x in
//   edi (the register point.y then reuses) and loads point.y late, here point.x uses
//   ecx and the point.y load is hoisted; (c) the 0x10 block keeps bc in si and reloads
//   nsel from memory, the original keeps nsel in si and bc in cx. Everything from the
//   loop exit to the epilogue already matches instruction for instruction.
//   Scratch files: build/scratch/0x4a3780/wG.cpp (52.3%, y1 split + pointer walk,
//   allocation still wrong), p3.cpp (58.1%, the file), h3b/p3b asm+dumps.
// 2026-10-01 retry 5 (deepseek-v4.1-flash): best stays 52.3%, 1806 bytes. Swapping the point.x/point.y subtract order (51.1%) and moving `int n = 0;` below the r.y1/r.y0 setup (byte-identical, 52.3%) do not move the py/esi register rotation. Still differs: py in esi here vs edi in the original, one rotation only.
// 2026-10-01 retry 6 (deepseek-v4.1-flash): best stays 52.3%, 1806 bytes. Confirmed the
//   remaining gap is the one esi/edi rotation (original keeps point.y in edi and reloads
//   obj / point.x through esi; this compile swaps the two, so py takes esi and the loop
//   counter plus the obj/px reloads take edi). The swap is 1:1, so the bytes differ all
//   through the function, and that is where most of the missing 48% goes. In the
//   original the loop init (`mov esi,1`, 0x4a383d) is scheduled BEFORE point.y's load
//   (0x4a3853) and even before the point.x subtract, so the loop counter owns esi first
//   and point.y has to reuse edi; here point.y's definition comes first and claims esi.
//   New this pass: MSVC 5 does fold `- 1 - 3` into lea -4 (so the original's
//   `lea eax,[ecx+ebx-1] / sub eax,3` for r.y1 cannot come from one expression), and it
//   also folds a `r.y1 -= 3;` placed directly after the assignment. The only fold barrier
//   found is a statement that writes a value the y1 expression reads:
//   `r.y1 = me->field_19 + r.y0 - 1; r.y0 += 2; r.y1 -= 3;` DOES give the original's
//   `lea -1 / sub eax,3 / add ebx,2 / store` (1809 bytes) but the schedule moves
//   point.y's load up into esi and the score drops to 49.2%, so the base spelling
//   (`r.y1 = me->field_19 + r.y0 - 4;`) scores higher and is kept.
//   Also tried, all worse or byte-identical: a `short nsel = me->field_c0;` local used
//   for both the !=0 test and the nsel-1 clamp (byte-identical: MSVC folds it to
//   `cmp word ptr [ebp+0xc0],0`, so the original's `mov si,[ebp+0xc0] / test si,si /
//   dec esi` is still unexplained); aliasing obj into a local pointer (coalesced away);
//   `int n = 0;` moved between the y1 statements as a fold barrier (folded anyway);
//   `} else if ((flags & 0x20) | 0x80) {` instead of the `||` spelling: this DOES give
//   the original's `mov ecx,eax / and ecx,0x20 / or cl,0x80 / test cl,cl` at 0x4a3c2b
//   and takes the file to the original's 1812 vs 1832 bytes, but the checker's similarity
//   reads 52.1% (the register swap elsewhere in that block costs more than the three
//   matching lines gain), so it is not kept. The `| 0x80` (value, not mask) looks like
//   the original programmers meant `(flags & 0x20) | (flags & 0x80)`; as written the
//   test is always true, which is why the original's `je 0x4a3cd3` is dead;
//   point.x before point.y (51.1%); the point.y subtraction after the selection loop
//   (42.1%); hoisting `int i = 1;` above the point copy (38.0%: the local's slot moves
//   and every [esp+N] shifts, so a hoisted i needs to keep the merged slot the current
//   `int i;` has).
// PARTIAL: 52.3%, 1806 bytes against 1832, frame 0x3c like the original.
//
// What the Sonnet 5.5 pass (#3246) found, 35.4 to 52.3:
//  1. x0, y0, x1, y1 are ONE `Rect` local, not four ints. The original's slots
//     0x24 x0, 0x28 (never touched: y0 lives in ebx), 0x2c x1, 0x30 y1 are four
//     consecutive dwords, and the dead one is the member of a struct local whose
//     other members are in memory. With four separate ints the frame was 0x34
//     and every [esp+N] was 8 low; with the struct the frame is exactly 0x3c.
//  2. The three early `return 1` paths (no 0x200 flag, negative line, strncmp
//     mismatch) all jump to the ONE `mov eax,1; ret` block that belongs to
//     `if (me->flags & 0x40) return 1;` at the end, while the `return 0` ones are
//     duplicated epilogues. `goto ret1;` to a label inside that last `if` gives
//     the shared block (1841 to 1806 bytes). `int flags` has to be declared above
//     the gotos (C2362).
//  3. `short da = me->field_da; int span = (da != 0) ? da : size + 1;` gives the
//     original's `mov cx,[da]; test cx,cx; movsx ecx,cx; jne; lea ecx,[eax+1]`.
//  4. The second-loop clamp is int arithmetic (`int v = e->field_c0 - 1;
//     if (v >= ba) v = ba;`), not `short v`.
//  5. 46.4 to 52.3: the duplicate `|| point.x < r.x0` / `|| point.x > r.x1` in the
//     two `goto out` tests after `after:`. The duplicate folds late and the four
//     jumps become the original's `jl out / jg out / jl scroll_up / jg out`.
//     The same duplicates in the first chains or inside an `&&` chain do nothing.
// Structure still to apply (scratch z4, 49.0%, 1784 bytes: the block layout is
// the original's but a register web moves and the score drops): after the three
// compare blocks the original is
//     if (obj->focus == index) {
//         if (!FUN_004ab5b0(obj, 3)) obj->focus = -1;
//         if (px >= x0 && px <= x1 && py >= y0 && py <= y1) { flags block;
//             select check; if (flags & 0x40) { ret1: return 1; } field_cca = 1; }
//         else if (py < y0) { scroll up, tail FUN_004a1b40 + FUN_004a2be0 }
//         else if (py > y1) { scroll down, same tail }
//     }
//     return obj->field_60 != -1;
// (scroll up is laid out BEFORE scroll down; with `goto out` the compiler puts
// scroll down first). Also the second block's condition is `flags & 0x20 | 0x80`
// (always true, compiles to `and ecx,0x20 / or cl,0x80 / test cl,cl`, +6 bytes),
// and scroll up's argument is `short sel = ba; (sel < 0) ? 0 : sel`.
// What still differs: ONE register rotation. The original keeps py in edi and
// the loop index / obj / px reloads in esi; this compiles with py in esi and the
// others in edi (a toy test shows MSVC 5 hands esi, edi, ebx, ebp in that order
// to the webs with the most references, loops weigh little: a loop counter with
// 5 references loses to a variable with 6 straight-line ones). Also the
// original has a short local `nsel = me->field_c0` in esi in the first block
// (`mov si,[ebp+0xc0] / test si,si`, a `mov esi,[esp+0x34]` reload on the
// field_c0 == 0 path); every spelling here folds it to a memory compare, and the
// reload block is what keeps the two FUN_004ab690 call tails from merging.
// Tried without effect (flat or worse): `int i = 1` in four positions, duplicate
// `||` uses of i, n, py, `do/while(0)` and while forms of the loop, pointer walk,
// inline Rect::Set (37.8%, whole function reallocates), dummy extern-int sweep
// 0..400 step 8 (flat).
#include <string.h>


#pragma pack(push, 1)
struct Entry_004a3780 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x13 - 0x02];
    short field_13;                    // +0x13
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0)
        int field_b6;                  // +0xb6 (the scroll repeat timer)
    };
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    void* field_c2;                    // +0xc2
    int field_c6;                      // +0xc6
    char unknown_ca[0xce - 0xca];
    void (__stdcall* field_ce)(void*, void*);  // +0xce
    char unknown_d2[0xd6 - 0xd2];
    int field_d6;                      // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct List_004a3780 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3780 {
    int current;                       // +0x00
    Entry_004a3780* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3780* list;               // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Rect_004a3780 { int x0, y0, x1, y1; };

struct Point_004a3780 {                // 0x18 bytes, copied with rep movsd x6
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];                 // +0x08
};

struct Object_004a3780 {
    char unknown_00[0x18];
    Holder_004a3780* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a3780 point;              // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                      // +0x60
    int focus;                         // +0x64
    char unknown_68[0xcca - 0x68];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern Holder_004a3780* DAT_0051fba4;
extern char DAT_00502a20[];

void __stdcall FUN_004c1420(int id);
int FUN_004c1450();
void* __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004b6af0(void* text, int line);
int FUN_004b6340();
int __stdcall FUN_004ab570(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab510(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, int mask);
void __stdcall FUN_004ab690(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall FUN_004a1b40(Object_004a3780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a3780* obj, int index);

// FUNCTION: 0x4a3780
int __stdcall FUN_004a3780(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    Rect_004a3780 r;
    if (me->type == 0) {
        r.x0 = 0;
        r.y0 = 0;
    } else {
        r.x0 = me->field_13;
        r.y0 = me->field_15;
    }
    r.x1 = me->field_17 + r.x0 - 1;
    int n = 0;
    r.y1 = me->field_19 + r.y0 - 1;
    r.y0 += 2;
    r.y1 -= 3;
    Point_004a3780 point = obj->point;
    point.x -= entries[0].field_13;
    point.y -= entries[0].field_15;
    int i;
    Entry_004a3780* e = &entries[1];
    for (i = 1; i < entries[0].count + 1; i++, e++) {
        if (e->type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].field_d6);
                break;
            }
            n++;
        }
        if (i < 1) break;
    }
    if (i == entries[0].count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
        : (*(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    int flag8 = 0;

    if (FUN_004ab570(obj, 1)) {
        if (point.x < r.x0 || point.x > r.x1 || point.y < r.y0 || point.y > r.y1)
            goto after;
        if (me->field_c0 != 0) {
            if (!(me->flags & 0x200))
                goto ret1;
            {
                int line = (point.y - r.y0) / span + (unsigned short)me->field_bc;
                me->field_ba = (short)line;
                if ((short)line < 0)
                    goto ret1;
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if ((short)me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) != 0)
                    goto ret1;
                me->field_ba = orig_sel;
                return 0;
            }
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

after:
    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x < r.x0 || point.x < r.x0)
        goto out;
    if (point.x > r.x1 || point.x > r.x1)
        goto out;
    if (point.y < r.y0)
        goto scroll_up;
    if (point.y > r.y1)
        goto out;
    {
        obj->holder->field_20 = index;
        unsigned flags = me->flags;
        if (flags & 0x10) {
            int line = (point.y - r.y0) / span + me->field_bc;
            me->field_ba = (short)line;
            if ((short)line >= 0) {
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    Entry_004a3780* e = &entries[i];
                    if (e->type == 2 && e->kind == me->kind) {
                        int v = e->field_c0 - 1;
                        if (v >= me->field_ba)
                            v = me->field_ba;
                        e->field_ba = v;
                    }
                }
            } else {
                me->field_ba = orig_sel;
            }
        } else if ((flags & 0x20) | 0x80) {
            flag8 = (flags >> 7) & 1;
            int bc = me->field_bc;
            char* fixed = (char*)(me->field_c6 + bc * 0x18);
            char* itemp = (char*)(me->field_c6 + bc * 4);
            int remain = point.y - r.y0 - 2;
            int n2 = 0;
            int k = bc;
            while (1) {
                char* row = flag8 ? fixed : *(char**)(itemp + 0x28);
                int h = (me->field_da != 0) ? span : *(unsigned short*)(row + 2);
                remain -= h;
                if (remain <= 0) {
                    me->field_ba = (short)(n2 + bc);
                    break;
                }
                n2++;
                k++;
                fixed += 0x18;
                itemp += 4;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
    }

select_check:
    if (orig_sel != me->field_ba) {
        FUN_004a1b40(obj, index);
        if (me->field_ce)
            me->field_ce(obj, me);
    }
    if (me->flags & 0x40) {
ret1:
        return 1;
    }
    obj->field_cca = 1;
    goto end;

out:
    if (point.y < r.y0)
        goto scroll_up;
    goto scroll_down;

scroll_up:
    if (me->field_bc > 0 && me->field_b6 < FUN_004b6340()) {
        me->field_b6 = FUN_004b6340() + 2;
        if (me->field_ba > me->field_bc)
            me->field_ba = me->field_bc;
        me->field_ba--;
        me->field_bc--;
        if (me->field_c2 != 0) {
            if (me->field_ba < 0)
                me->field_ba = 0;
            char* s = FUN_004b6af0(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
        goto finish;
    }
    if (me->field_ba > 0) {
        me->field_ba = 0;
        goto finish;
    }
    goto end;

scroll_down:
    if (point.y <= r.y1)
        goto end;
    if (me->field_bc >= me->field_be)
        goto end;
    if (me->field_b6 >= FUN_004b6340())
        goto end;
    me->field_b6 = FUN_004b6340() + 2;
    me->field_bc++;
    {
        short sel = (short)(step + me->field_bc - 1);
        me->field_ba = (short)sel;
        if (me->field_c2 != 0) {
            char* s = FUN_004b6af0(me->field_c2, sel);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
    }

finish:
    FUN_004a1b40(obj, index);
    FUN_004a2be0(obj, index);

end:
    return obj->field_60 != -1;
}