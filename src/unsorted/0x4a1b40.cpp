// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5. Names are provisional.

// PARTIAL (73.1%), issue 4354. This pass went 66.5% -> 73.1% and the compiled size is
// back to the original's 2160 bytes. What moved it, in order of size:
//  (A) 66.5 -> 67.4. The FUN_004be950 colour parameter is `int`, not `unsigned char`.
//      With a byte parameter MSVC knows only AL matters and emits a bare
//      `mov al,[ebx+0x8be]`; with an int it must materialise the value and emits the
//      original's `xor reg,reg` + `mov regL,[ebx+0x8be]` in all four calls
//      (0x4a2300, 0x4a2320, 0x4a2349, 0x4a2374). That is the whole 8-byte shortfall.
//  (B) 67.4 -> 67.6. The non-language glyph loop is a GUARDED do-while:
//        unsigned char c = *q;
//        unsigned short* g = 0;
//        if (c != 0) { do { g = ...; c = *(++q); if (0 != g) w += *g; } while (c != 0); }
//      not `while (c != 0) { ... }`. The guard lets both loop exits share one join and
//      moves the `mov esi,[x1] / mov ecx,[x2]` reload pair out of the `q != 0` path.
//      Reverting only this piece out of a 67.7% file costs 13.4 points.
//  (C) 67.6 -> 68.9. Two things at once, from a tools/permute.py pass:
//      (C1) `#include <stdio.h>` must be REMOVED (+2.2 on its own). The earlier
//           128-header-set sweep never tried dropping one include at a time from the
//           set that was in the file, which is a different search.
//      (C2) the language test inside the text-width computation is wrapped in
//           `do { ... } while (0)` and the `noText` arms are the other way round from
//           what the file used to say: `if (noText != 0) { w = 0; } else { do { if (0 ==
//           DAT->language) ... else { glyph loop } } while (0); }`. Spelling it as a
//           plain `else if` is 64.7%, the arms the other way round is 64.8%.
//  (D) also from the same pass, load bearing: the text loop's third arm is written with
//      the POSITIVE test first, `else if (0 == (me->flags & 0x100) && me->field_ba
//      == line + me->bc.field_bc && 0 != me->field_c0) { highlight } else {
//      FUN_004c13a0(...) }`. The `else if (!(...))` spelling is 64.7%.
//  (E) 68.9 -> 69.1. The glyph loop's guard `if (c != 0)` has to go through the
//      one-line helper `NonZero(c)` below. Written inline (`if (c != 0)`, `if (c)`,
//      `if (0 != c)`, `if (c > 0)`, or via a named int) it is 68.9%; through the call
//      MSVC 5 materialises the boolean (`xor edx,edx / setne dl / test dl,dl`) and
//      swaps ebp and ebx for the whole text loop, which scores 69.1% but is 4 bytes
//      longer with 12 bytes of trailing nop padding. That is alignment luck, not
//      better code, and no natural spelling reaches it.
//  (F) 69.1 -> 71.8, four more things from a second permuter pass, all of them needed
//      together (each one alone is worth nothing or less):
//      (F1) the include set is just <windows.h>: adding <stdio.h>, <stdlib.h> or
//           <string.h> is neutral, but <memory.h> is not allowed (71.6 with it, and
//           68.8 for the old <windows.h> <memory.h> set). The 128-set sweep is not
//           the right search for this function, see (C1).
//      (F2) the entry test's second operand goes through a one-line helper,
//           `HasText(me)` for `me->text != 0` (+2.8 on its own, 69.0 without it).
//      (F3) the cell loop is spelled `if (1) do { ... } while (1);`, not `while (1)`:
//           69.9 with the plain while.
//      (F4) and the text loop's third arm is NEGATED again, `else if (!(0 == (flags &
//           0x100) && ...)) { FUN_004c13a0(...) } else { highlight }`, the opposite of
//           (D): with the positive form this file is 29.4%.
//  (G) 71.8 -> 73.1, two lines. (G1) the text loop's exit is `if (h < lh) break;`,
//      not the `if (h >= lh) { } else break;` this file used to carry: on the 68.9%
//      version the same rewrite cost 14 points, so loop-exit polarity has to be
//      re-measured after every real change too. (G2) `colPtr = 1 + colPtr;` rather
//      than `colPtr = colPtr + 1;`. Together they also bring the size back to 2160.
//
// STILL DIFFERS: one swap of two PAIRS of callee-saved registers, which then moves
// every stack slot in the function. MSVC 5's free list for the int class is
// [esi, edi, ebx, ebp] (measured on a synthetic, see SHARED.md), so the original
// allocates its four long ranges in the order entries, me, the constant 0, param_1,
// and this source allocates them 0, param_1, entries, me: the two pairs are exchanged.
// With the original's order, `entries` sits at [esp+0x38] and `me` at [esp+0x50];
// here `entries` is at 0x3c and `me` at 0x28. Everything else in the body is a
// consequence: the text loop's `cy` lives in a register instead of being reloaded from
// [esp+0x1c], the cell loop reloads `me` from its own slot, the four colour pushes pick
// different scratch registers.
//
// CONFIRMED INERT for that rotation (all byte-identical or worse, do not retry):
//   - adding or removing uses of param_1, me or entries, including a throwaway extra
//     `if (0 != param_1->holder) { }` and a real `if (me->type == 99) { }`: the order
//     does not move, so it is not reference-count driven
//   - declaring holder/entries/me before the holder store, after it, at the top of the
//     function body, or in the branch; swapping top0/flag/bounds/y/yoff declarations
//   - dropping the constant zero from the entry block entirely (it is not the reason
//     param_1 lands in edi: without it param_1 is still edi)
//   - all 128 header sets via tools/headers.py, and the callee parameter types
//   - the loop tail's statement order (yoff/line/h/y): the file's order is the best of
//     the four permutations tried
//   - `int step = lh + 1; if (me->field_da) step = me->field_da;` versus the ternary,
//     the if/else, and `step = me->field_da ? me->field_da : step` (all 68.9 or worse)
//   - dropping keepW (59.9), making it short/unsigned (68.9), moving yoff's or t's
//     initialiser into the branch (65.5), moving `line = 0` after the b6af0 call (61.8)
//
// Suspected original bugs:
//  - the highlight call at 0x4a1fb6 has both arms dead identical (0x4a1fb8 and
//    0x4a1fcb both push 0x1e and lea the same [esp+0x1c]), so
//    `holder->field_20 == param_2` has no effect on the output.
//  - when none of the flag bits 1, 2 or 4 is set, `xx` and `xw` are never written
//    before 0x4a1e8b reloads their slots (0x58 and 0x5c) and passes them to
//    FUN_004a50e0, so the first line of text is drawn at an uninitialised position.
//    The source here leaves both uninitialised to match, which is what produces the
//    `mov esi,[esp+0x5c] / mov ebx,[esp+0x58]` reload pair.
//
// ---- older passes, kept for the leads they record ----
// PASS (66.5%, from 51.8%). What moved it, in
// order of size:
//  (1) 51.8 -> 60.0. The non-language text-width loop advances `q` itself and
//      keeps the glyph char in a byte local:
//        unsigned char c = *q;
//        while (c != 0) { w += *(unsigned short*)FUN_004b7f30(glyphs, c); c = *(++q); }
//      The original stores the char to a byte slot, reloads it as a dword and masks
//      with 0xff (0x4a1d96..0x4a1da9). That byte slot was the one missing dword: the
//      frame went 0xb8 -> 0xbc, matching the original, and the glyph loop then
//      matched instruction for instruction.
//  (2) 60.0 -> 62.0. Aliasing cell-branch locals onto text-branch locals so the two
//      branches SHARE frame slots, which is what the original does (its cell loop
//      uses the text loop's slots for row/y/cellPtr): `row` onto `h`, `cellPtr`
//      onto `lh`, `y` onto `y`.
//  (3) 62.0 -> 63.2. The cell-branch rowRect STORE ORDER is left, right, top,
//      bottom, not declaration order: the original stores [esp+0x18]=points[0].x,
//      [esp+0x20]=points[1].x, [esp+0x1c]=points[0].y, [esp+0x24]=points[2].y, i.e.
//      both x fields before both y fields (0x4a21cd..0x4a21f0).
//  (4) 63.2 -> 66.5. tools/permute.py hill climb from the 63.2% file. The useful
//      finds, kept here with real names, are the one-line accessors below (routing
//      an expression through an inline helper changes MSVC 5's allocation; inlining
//      them by hand costs 0.1%), the split of `int h = me->h;` into a declaration
//      plus an assignment, `int step = lh + 1;` written before the field_da test,
//      and spelling the w test as a nested `if (q != 0) { ... }`.
//
// Confirmed INERT at 60-63% (all byte-identical or worse, do not retry):
//   - declaration order of top0/flag/bounds/y/yoff, and of yoff vs y
//   - arithmetic operand order in the loop head (`top+yoff+2` spellings)
//   - `short`/`unsigned short`/`int` for keepW; dropping keepW (34.7) or limiting it
//     to one use (34.6); keepW must span the loop
//   - the FUN_004be950 colour parameter type (unsigned char / short / int), and
//     caching colour_8be in any kind of local (45-51%). SUPERSEDED: on the 66.5% base
//     the type really was inert, but from 67.4 on `int` is worth +0.9, see (A) above
//   - headers.py over all 128 include sets; <windows.h> <string.h> is the best
//   - `int LineHeight()` helper for the lh and next computations (59.8)
//   - colPtr/bp/cellPtr as aliases of t/flag/h (36-61%)
// Still differs: one cyclic rotation of the four callee-saved registers. Ours hands
// them out [param_1, zero, entries, me] -> ebx/esi/edi/ebp; the original hands them
// out [zero, entries, me, param_1] -> ebx/esi/edi/ebp, i.e. `param_1` has to be the
// LOWEST-priority value, and `me` has to sit at [esp+0x50] where ours sits at
// [esp+0x2c]. Reducing param_1's use count (using the `holder` local for
// `param_1->holder->field_20`) drops to 50.3, so its use count is already right.
// Suspected original bug: the highlight call at 0x4a1fb6 has both arms dead
// identical (0x4a1fb8 and 0x4a1fcb both push 0x1e and lea the same [esp+0x1c]),
// so `holder->field_20 == param_2` has no effect on the output.
// Old notes below, kept for the leads they record.
// - The cell branch's outline is `if (v & 1) {...} else if (v & 2) {four
//   FUN_004be950}` drawn from the copied rowRect (left+1/bottom-1/...), with
//   param_1->colour_8be re-read for each call; the old y/yy arithmetic was
//   off by one against the original's pushes (33.9 -> 41.6).
// - The cell fetch tests `bp != 0` first (41.9).
// - The two quads are filled by single stores in the original's store order
//   (50.3; the natural point order scores 49.5).
// - Surface selection is `if (surface == 0 && !(holder->field_10 & 0x80))
//   FUN_004b0230(param_1, param_2, surface); else if (surface != 0) ...`,
//   which passes the null surface and keeps the redundant re-test (50.5).
// - top is a separate local initialised to 0 (the original's ebx, which also
//   serves the early `== 0` compares) copied into bounds.top (51.8).
// - me->field_c0 is read directly in the entry test and the highlight test.
// Still differs: callee-saved rotation (original param_1 = ebp, top0 = ebx;
// ours param_1 = ebx, top0 = edx), the frame is 0xb8 against 0xbc, and the
// text loop's exit is `jge end; jmp top` where the original has `jl top`
// followed by an inline epilogue. Replacing keepW with me->w reads (the
// original re-reads the field later) drops to 34.9% by rotating registers.
// PROBE (deepseek-v4.1-flash, issue 4066, best 33.7%, no change): VC5
// rejects `if (param_1->holder != flag)` (C2446, pointer vs int), the
// casted `(Holder_004a1b40*)flag` compare scores 33.6, and swapping the
// `int t;` / `int flag = 0;` declaration order is byte-identical at
// 33.7 / 2102 bytes. Still differs: frame 0xb8 vs 0xbc (one long-lived
// 4-byte local missing), every body slot +4, param_1 homed in esi vs the
// original ebp with the zero in ebx.
//
// PROBE (deepseek-v4.1-flash, issue 3929, best 33.7%, no change): re-measured
// the prologue against the original. Ours now does `sub esp,0xb8` (original
// 0xbc, still 4 short) with `push ebx; push ebp; push esi; mov esi,[esp+0xc8];
// push edi`, while the original is `sub esp,0xbc; push ebx; push ebp; mov
// ebp,[esp+0xc8]; xor ebx,ebx; push esi; push edi` (param_1 homed in ebp,
// esi/edi pushed after the first body instruction). One 4-byte long-lived
// local plus a first-use that forces param_1 into ebp is still missing.
// WIN 4 (deepseek-v4.1-flash, issue 3869): 33.4 -> 33.7 (2103 -> 2102 bytes) by
// widening the lenient field_c0 cache to `int keepC0 = me->field_c0;` (was
// `short keepC0`), i.e. the original caches that short in an int. Re-tested on
// the 33.7 base and rejected (worse/neutral): `unsigned int keepC0`, `int font`
// for the FUN_004c13f0 result (33.4), `int flags` (33.6), `short h` (32.5),
// `unsigned int keepW` (32.3), `short keepW` and `int col` (byte-identical to
// 33.7), moving the keepC0 declaration above flags (byte-identical).
// WIN (deepseek-v4.1-flash, issue 3625): 26.6 -> 30.8 (2105 -> 2123 bytes) by
// building the cell-branch highlight rect as a copy of the dst quad:
//   Rect rowRect; rowRect.left = dst.points[0].x; rowRect.top = dst.points[0].y;
//   rowRect.right = dst.points[1].x; rowRect.bottom = dst.points[2].y;
// The original reloads [esp+0x6c]/[esp+0x70]/[esp+0x74]/[esp+0x80] into
// [esp+0x18..0x24] at 0x4a21cd..0x4a21f0 instead of recomputing x1/yy/right/
// y-1, exactly what the copy spelling produces. Also note (correcting the
// 3325 note): 0x4a207f does `sub esp,0x10` for the by-value Rect of
// FUN_004c6b10 and the callee's `ret 0x10` restores it, so from 0x4a20a6 on
// every [esp+N] is 0x10 higher than earlier in the function; pre-shift
// esp+0x00..0x0f is real (colPtr at post-shift 0x10, 0x4a20be), not reserved.
// WIN 2 (32.2, 2125 bytes): cell-branch x1 is not a fresh local, it overwrites
// (aliases) `left`: the original stores it back to bounds.left's own slot at
// 0x4a20f2 (`mov [esp+0x40], ecx`), so `int& x1 = left; x1 += 2;` is used.
// WIN 3 (33.4, 2103 bytes): the text-branch row rect is the four live ints,
// not a copy. The original's rect slots 0x18/0x1c/0x20/0x24 are exactly
// x1/cy/x2/cy2 and are strength-reduced across the loop, so the source holds
// one Rect and aliases it: int& x1 = rowRect.left; int& cy = rowRect.top;
// int& x2 = rowRect.right; int& cy2 = rowRect.bottom; then assigns each.
// Remaining gap: the register rotation (original param_1=ebp, me=edi,
// zero=ebx; ours param_1=ebx, me=esi, zero=edi) plus ~57 bytes of extra
// spills in the original.

// RETRY (deepseek-v4.1-flash, issue 3552, best 26.6%, no change): four more
// allocator probes on top of the 3552 handout, all byte-identical at 26.6 /
// 2105 bytes (one frame is 0xc0 against the original 0xbc and the whole body
// is the one-step register rotation below). unsigned keepW, int keepC0,
// both together, and a swap/reuse of the two: no effect. The frame-shape
// trick that moved 0x49be60 (growing a scratch array by one element) has no
// analogue here because this function has no array local; the extra 4 bytes
// must come from a long-lived scalar the original keeps that we do not.
// RETRY (deepseek-v4.1-flash, issue 3575, best 26.6%, no change): three more
// allocator probes, all worse or neutral and reverted: the final highlight test
// spelled `holder->field_20` instead of `param_1->holder->field_20` to extend
// holder's live range (19.8), `int keepW = me->w;` hoisted above `int h` with
// the later declaration dropped (23.5), keepC0 declared before flags (26.6,
// byte-identical). So neither holder's live range nor keepW's first use moves
// the one-step rotation (need param_1=ebp, me=edi, zero=ebx, extra esi home).
// STATUS (deepseek-v4.1-flash, issue 3524): still best 26.6%, not MATCH. One
// more allocator attempt, reverted: defining `y` before `q` in the text prologue
// (so q's first use moves after y's) drops to 24.6, 2098 bytes (original 2160),
// so q must be defined first; the rotation still needs one long-lived node that
// this source shape does not create.
// STATUS (deepseek-v4.1-flash, issue 3325): best 26.6%, not MATCH.
// New evidence for the next attempt: every [esp+N] in the original is >= 0x10 and
// a multiple of 4 (checked with objdump over the whole function), i.e. the original
// frame has 16 bytes at esp+0x00..0x0f that no instruction ever touches, and its
// lowest live slot is t/line at 0x10. Ours uses esp+0x00..0x0f (t/line/colPtr,
// xx/row, entries/yy, flag), so our allocator starts 0x10 lower and the extra
// 16 bytes of the original are not the 4-byte frame difference (0xc0 vs 0xbc).
// A likely cause: a by-value struct temporary (the 16-byte Rect for FUN_004c6b10)
// whose home slot MSVC reserved at the bottom of the frame but never used, since
// the real copy is made with `sub esp,0x10; mov eax,esp` at 0x4a207f. Two more
// original details that are not in this file: the window rect is built twice in the
// cell branch (a dead store of x1 into the cell-loop rect at base+0x84 at 0x4a213c,
// and the real rect at base+0x18 later), and the text call's y argument is served
// from q's own slot (0x2c) because cy is copied there at 0x4a1e2e-0x4a1e32, so the
// source's cy/q live ranges really do interleave. Also tried on issue 3325 and
// neutral: an inline LineHeight_004a1b40() helper used for both lh and next
// (byte-identical to this file, 26.6%).
// STATUS (deepseek-v4.1-flash, issue 2934): best 26.6%, not MATCH.
// What moved it: (1) restore the TWO holder loads. The original reads
// [param_1+0x18] at 0x4a1b53 and again at 0x4a1b6c, because the store
// [holder+0x14]=1 may alias holder, so there is no single holder local spanning
// the store. Two-load alone is check.py 17.9% but aligns better (27.1% true LCS
// vs 23.9%). (2) two lenient locals that MSVC may re-load: `int keepW = me->w;`
// and `short keepC0 = me->field_c0;`. The original re-reads me->w (0x4a1bb5 and
// 0x4a1d35) and me->field_c0 (0x4a1c6a and 0x4a1f96); declaring a local for each
// is semantically free and changes the allocator state, 18.6 -> 26.6. keepW alone
// is 25.2 and keepC0 alone is 17.9, so both are load bearing. The three changes
// only work together: the same two locals on the original single holder load score
// 18.8, so the two holder loads are needed as well.
// Suspected original bug: the highlight call at 0x4a1fb6 has both arms dead
// identical (0x4a1fb8 and 0x4a1fcb both push 0x1e and lea the same [esp+0x1c]),
// so `holder->field_20 == param_2` has no effect on the output.
// What still differs: the body is a one-step register rotation. Original has
// param_1=ebp, me=edi, zero=ebx, and an extra esi home; ours has me=esi,
// zero=edi, param_1=ebx. So one more long-lived node ahead of `me` is missing.
// Adding speculative locals for type/flags/da/text/d6/ba/tab did not help. Frame
// is 0xc0 against the original 0xbc. Tried and rejected: keepW used for x2 still
// (17.9, the local must stay live into the loop); more persistent locals.
// Re-tried on issue 3304 (deepseek-v4.1-flash) with the same 26.6% ceiling:
// removing keepC0 (25.2), removing keepW (17.9), removing both (17.9), keepDa
// local (19.4), keepParam2 local (26.6, neutral), keepFlags local (24.9), self
// alias of param_1 (neutral), hoisting x1/x2/cy/cy2 declarations to the top
// (byte-identical to v0, so MSVC assigns slots by use, not declaration order),
// while(1) for the text loop (neutral), unsigned char colour index (neutral),
// headers.py (no header set beats 26.6). The text/cell loop bodies reconstruct
// instruction for instruction, so the remaining gap is pure allocator state,
// not missing code: the 55-byte shortfall is spill count (fewer memory operands
// where the original spills more).
// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by
// claude-opus-5-5. Names are provisional.
//
#include <windows.h>

#pragma pack(push, 1)

struct Entry_004a1b40 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;             // +0x1f
    char unknown_23[0x28 - 0x23];
    char tab;                           // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                    // +0xb6 (entry 0)
        char text_b6[0xb8 - 0xb6];
    } b6;
    short unknown_b8;
    short field_ba;                     // +0xba
    union {
        void* surface;                  // +0xbc (entry 0)
        short field_bc;                 // +0xbc
    } bc;
    short field_c0;                     // +0xc0
    char* text;                         // +0xc2
    int* cells;                         // +0xc6
    char unknown_ca[0xd6 - 0xca];
    void* field_d6;                     // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a1b40 {
    char unknown_00[4];
    Entry_004a1b40* entries;            // +0x04
    char unknown_08[0x10 - 0x08];
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                       // +0x20
    void* surface;                      // +0x24
};

struct Class_004a1b40 {
    char unknown_00[0x18];
    Holder_004a1b40* holder;            // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour;               // +0x8b2
    char unknown_8b3[0x8be - 0x8b3];
    unsigned char colour_8be;           // +0x8be
    char unknown_8bf[0xcd2 - 0x8bf];
    void* fallback;                     // +0xcd2
};

struct Glyph_004a1b40 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
};

struct Language_004a1b40 {
    char unknown_0[0xc];
    unsigned short* glyphs;             // +0xc
};

struct LanguageRoot_004a1b40 {
    int current;                        // +0x0
    char unknown_04[0x14 - 0x04];
    Language_004a1b40* language;        // +0x14
};

struct Rect_004a1b40 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point_004a1b40 { int x; int y; };
struct Quad_004a1b40 { Point_004a1b40 points[4]; };

struct Class_004c6ae0 {
    void FUN_004c6ae0(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void FUN_004c6b10(Rect_004a1b40 rect);
};

#pragma pack(pop)

extern LanguageRoot_004a1b40* DAT_0051fba4;

void __stdcall FUN_004b0230(Class_004a1b40* obj, int index, void* bmp);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_004a1b40* rect, int* pos);
void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
void __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a1b40* rect, int id);
void __stdcall FUN_004c7580(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

// Partial (18.6%), not MATCH. Attribution: started by deepseek-v4.1-flash, then GPT-6,
// edited by deepseek-v4.1 (issue 2379).
// Frame size (0xbc), ret 8, the &entries[param_2] lea chain and the overall branch
// structure are right; the differences are prologue register assignment and stack slots.
// Original prologue: sub esp,0xbc / push ebx / push ebp / mov ebp,[esp+0xc8] /
//   xor ebx,ebx / push esi / push edi, i.e. param_1 is loaded into ebp and ebx holds the
//   zero used for flag=0 and for top=0. Ours loads param_1 into another register
//   (eax/ebx depending on the variant) and zeroes into edi, so every [esp+N] drifts.
// Original slot map (verified from the disassembly, all offsets relative to esp after
// the four pushes): 0x10 line/tab counter (both share one slot, disjoint ranges),
// 0x14 flag, 0x18 x1, 0x1c cy, 0x20 x2, 0x24 cy2, 0x28 y, 0x2c q, 0x30 yoff, 0x34 h,
// 0x38 entries, 0x3c lh, 0x40..0x4c bounds (left,top,right,bottom), 0x50 me, 0x54 step,
// 0x60 font byte, 0x64 col, 0x68 glyph char temp. Ours: entries=0x10, flag=0x1c,
// h=0x2c, bounds=0x38..0x44, me=0x48, so the whole frame is reshuffled, not shifted.
// Original keeps me in edi, q in ebp, step in ebx, x1 in esi, x2 in ecx, step=ebx in the
// loop tail; ours uses different roles, so most of the text loop (0x4a1c5c) differs.
// The loop bottom is 0x4a1ff3: yoff+=step, line++ (slot 0x10), y++, h-=step, h<lh exit.
// Tried and rejected: dropping the `holder` local for fresh param_1->holder derefs
// (18.6% -> 17.9%, but the original does reload [ebp+0x18] twice at 0x4a1b53/0x4a1b6c, so
// a `holder` local is closer over the whole function); hoisting the tab counter `t` to
// the top (neutral); removing <windows.h> (18.6% -> 15.6%, include is needed).
// Open question worth solving next: the highlight call at 0x4a1fbe passes esp+0x1c after
// the 0x1e id was pushed, i.e. rowRect+8, and both of its arms (0x4a1fb8/0x4a1fcb) pass
// the same pointer. Either the original really indexes past rowRect or its highlight rect
// is a second rect whose slot overlaps rowRect (mutually exclusive branches).
//
// An earlier pass needed nine one-line inline accessors here (FontArg, GlyphStart,
// CellWidth, RowRight, RectLeft, RectLeftPlus2, RectLeftPlus2b, HighlightRow,
// EntryFlags) because routing those expressions through a helper changed MSVC 5's
// allocation. On this pass's source all nine can be written inline for the same score,
// so they are gone. The one below is the exception: it is worth +0.2%, and MSVC 5 only
// takes that path when the test is written as a call.
static inline bool NonZero(unsigned char c) { return c != 0; }

static inline bool HasText(Entry_004a1b40* me) { return me->text != 0; }

// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* param_1, int param_2)
{
    unsigned char font;
    int xw;
    unsigned int t;
    Rect_004a1b40 bounds;
    int top0 = 0;
    int flag = 0;
    int& top = bounds.top;
    int& left = bounds.left;
    int& right = bounds.right;
    int xx;
    int lh;
    int& bottom = bounds.bottom;
    if (0 != param_1->holder)
        param_1->holder->field_14 = 1;
    Holder_004a1b40* holder = param_1->holder;
    Entry_004a1b40* entries = holder->entries;
    Entry_004a1b40* me = &entries[param_2];
    int h = me->h;
    if (me->type == 0) left = 0; else {
        left = me->x;
        top0 = me->y;
    }
    top = top0;
    int keepW = me->w;
    right = me->w + left - 1;
    bottom = me->h + top - 1;
    void* surface = holder->surface;
    if (surface == 0)
        surface = param_1->fallback;
    if (surface == 0 && (0x80 & holder->field_10) == 0)
        FUN_004b0230(param_1, param_2, surface);
    else if (((void*)surface) != 0)
        FUN_004c6d20(entries->bc.surface, surface, &bounds,
                     &left);
    if (!DAT_0051fba4->language)
        lh = FUN_004c1450();
    else
        lh = ((Glyph_004a1b40*)FUN_004b7f30(DAT_0051fba4->language->glyphs,
                                            0x49))->height + 2;
    int step = lh + 1;
    if (me->field_da)
        step = me->field_da;
    int yoff = 0;
    int y = 0;
    char* q;
    unsigned int flags;
    flags = (unsigned int)me->flags;
    if (((flags & 0x10) != 0 && HasText(me)) != 0 && me->field_c0 != 0) {
        // ---- text-line renderer ----
        t = 0;
        int i = 1;
        for (; i < entries->b6.count + 1; ) {
            if (7 == entries[i].type) {
                if (t == me->tab) {
                    FUN_004c1420(*((int*)((char*)&entries[i] + 0xd6)));
                    break;
                }
                t = t + 1;
            }
            i = i + 1;
        }
        int ranOff = i == 1 + entries->b6.count;
        int line = 0;
        if (ranOff)
            FUN_004c1420(DAT_0051fba4->current);
        FUN_004c1440();
        font = (unsigned char)FUN_004c13f0();
        q = FUN_004b6af0(me->text, me->bc.field_bc);
        y = me->bc.field_bc;
        Rect_004a1b40 rowRect;
        while (1) {
            int& x1 = rowRect.left;
            int& cy = rowRect.top;
            int& x2 = rowRect.right;
            int& cy2 = rowRect.bottom;
            x1 = left + 2;
            x2 = keepW + (x1) - 2;
            cy = (top + yoff) + 2;
            cy2 = step + cy;
            int w;
            int noText = q == 0;
            if (noText != 0) {
                w = 0;
            } else {
                // do/while(0) is a plain block, but MSVC 5 needs the loop node here:
                // spelled as an else-if or as a bare block this costs 4 points.
                do {
                    if (0 == DAT_0051fba4->language) {
                        w = FUN_004c1480(FUN_004c1440(), q);
                    } else {
                        w = 0;
                        unsigned char c = *q;
                        unsigned short* g = 0;
                        if (NonZero(c)) {
                            do {
                                g = (unsigned short*)FUN_004b7f30(
                                    DAT_0051fba4->language->glyphs, c);
                                c = *(++q);
                                if (0 != g)
                                    w += *g;
                            } while (c != 0);
                        }
                    }
                } while (0);
            }
            int next = 0;
            unsigned int col = (unsigned int)*(me->colours + ((unsigned char*)param_1 + 0x8b2));
            if (me->field_d6) {
                if (1 == *(y + (char*)me->field_d6)) flag = 1;
            } else if (0x26 == *q) {
                if (q[1] == 0x47)
                    flag = 1;
                q += 2;
            }
            flags = (unsigned int)me->flags;
            if ((flags & 1) != 0) {
                xx = x1;
                xw = x2 - x1 + 1;
            } else if (flags & 4) {
                xx = x2 - w;
                xw = w;
            } else if (2 & flags) {
                xx = (x1 + x2 - w) / 2;
                if (xx < x1)
                    xx = x1;
                xw = (x2 - xx) + 1;
            }
            if (0 == DAT_0051fba4->language)
                next = FUN_004c1450();
            else
                next = 2 + ((Glyph_004a1b40*)FUN_004b7f30(
                    DAT_0051fba4->language->glyphs, 0x49))->height;
            if (me->field_da > next + 6)
                FUN_004a51d0(entries->bc.surface, q, xx, cy, xw, bottom - top,
                             0);
            else
                FUN_004a50e0(entries->bc.surface, q, xx, cy, xw, 0);
            q = FUN_004b6af0(q, 1);
            if (flag) {
                flag = 0;
                FUN_004bf4d0(entries->bc.surface, &rowRect, -0x13);
                FUN_004bf4d0(entries->bc.surface, &rowRect, -0x14);
                FUN_004bf4d0(entries->bc.surface, &rowRect, -0x15);
                FUN_004bf4d0(entries->bc.surface, &rowRect, -0x16);
            } else if (!(0 == (me->flags & 0x100) &&
                       me->field_ba == line + me->bc.field_bc &&
                       0 != me->field_c0)) {
                FUN_004c13a0((int)col, (int)(font & 0xff));
            } else {
                // both arms (0x4a1fb8 and 0x4a1fcb) pass the same rect and id,
                // the arm is chosen by holder->field_20 == param_2
                if (param_1->holder->field_20 == param_2)
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
                else
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
            }
            line = line + 1;
            yoff += step;
            y = y + 1;
            h -= step;
            if (h < lh)
                break;
            if (line + me->bc.field_bc >= me->field_c0)
                return;
        }
    } else if ((0xa0 & ((unsigned int)flags)) != 0) {
        // ---- cell-grid renderer ----
        int* colPtr = 0;
        unsigned int bp = (int)((flags >> 7) & 1);
        void* surf = entries->bc.surface;
        Rect_004a1b40 clip;
        ((Class_004c6ae0*)surf)->FUN_004c6ae0(&clip);
        ((Class_004c6b10*)surf)->FUN_004c6b10(bounds);
        h = me->bc.field_bc;
        lh = 0;
        char*& cellPtr = (char*&)lh;
        if (0 == bp)
            colPtr = &me->cells[h];
        else
            cellPtr = (char*)me->cells + h * 0x18;
        left += 2;
        int yy = top + 2;
        y = yy + step;
        if (1) do {
            void* cell;
            if (bp != 0) {
                cell = cellPtr;
                cellPtr += 0x18;
            } else {
                cell = *(void**)(*colPtr + 0x28);
            }
            if (cell != 0) {
                if (*(int*)((char*)cell + 0x10) != 0) {
                    Quad_004a1b40 src;
                    Quad_004a1b40 dst;
                    Rect_004a1b40 rowRect;
                    dst.points[3].x = left;
                    src.points[0].x = 1;
                    src.points[0].y = 1;
                    src.points[3].x = 1;
                    src.points[1].y = 1;
                    dst.points[0].x = left;
                    dst.points[1].x = right;
                    dst.points[2].x = right;
                    dst.points[3].y = y - 1;
                    dst.points[2].y = y - 1;
                    src.points[1].x = *(unsigned short*)cell - 1;
                    src.points[2].x = *(unsigned short*)cell - 1;
                    dst.points[1].y = yy;
                    dst.points[0].y = yy;
                    src.points[2].y = *(1 + (unsigned short*)cell) - 1;
                    src.points[3].y = *(1 + (unsigned short*)cell) - 1;
                    FUN_004c7580(surf, cell, &dst, &src);
                    // The original stores the x pair before the y pair here.
                    rowRect.left = dst.points[0].x;
                    rowRect.right = dst.points[1].x;
                    rowRect.top = dst.points[0].y;
                    rowRect.bottom = dst.points[2].y;
                    unsigned char v = *((unsigned char*)me->field_d6 + h);
                    if (v & 1) {
                        FUN_004bf4d0(surf, &rowRect, -0x14);
                    } else if (v & 2) {
                        FUN_004be950(surf, rowRect.left + 1, rowRect.bottom - 1, rowRect.right - 2, rowRect.top + 1, param_1->colour_8be);
                        FUN_004be950(surf, rowRect.left + 2, rowRect.bottom - 1, rowRect.right - 1, rowRect.top + 1, param_1->colour_8be);
                        FUN_004be950(surf, 1 + rowRect.left, 2 + rowRect.top, rowRect.right - 1, rowRect.bottom - 2, param_1->colour_8be);
                        FUN_004be950(surf, rowRect.left + 2, 2 + rowRect.top, rowRect.right - 2, rowRect.bottom - 2, param_1->colour_8be);
                    }
                }
            }
            if ((me->flags & 0x100) == 0 && me->field_ba == h) {
                    Rect_004a1b40 hl;
                    hl.left = left;
                    hl.top = yy;
                    hl.right = left + *(unsigned short*)cell - 1;
                    hl.bottom = yy + *((unsigned short*)cell + 1) - 1;
                    FUN_004bf4d0(surf, &hl, 0x14);
                }
            h = h + 1;
            if (bp == 0)
                colPtr = 1 + colPtr;
            yy += step;
            y = step + y;
            if (yy >= bottom)
                break;
            if (h >= me->field_c0)
                break;
        } while (1);
        ((Class_004c6b10*)surf)->FUN_004c6b10(clip);
    }
}
