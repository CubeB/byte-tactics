// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-sonnet-5-5. Names are provisional.
// PARTIAL (83.0%), best of everything tried. Space Bunny Free, issue 4354,
// 77.6 -> 83.0%. Four sources of gain, each confirmed by check.py:
// (1) In the kind-7 outer loop the initialisation order is `pt = *start;
//     sp = *start;`, NOT `sp` first: pt's fields load first, which flips the
//     whole loop body by one register. The reverse order costs 1.3%.
// (2) headers.py: `<stdio.h> <stdlib.h> <string.h> <math.h> <memory.h>` scores
//     79.2 against 78.9 for `<math.h> <stdlib.h>` alone. No header set matches.
// (3) tools/permute.py, 79.2 -> 82.9: take
//     `build/permute/0x49be60/best_ratio.cpp` (NOT best.cpp, and NOT the log,
//     whose percentage scale is its own internal score). Its temporary names
//     were then all renamed to real locals with the score unchanged, and its
//     goto and no-op-cast leftovers tidied out. `scrollXk6` MUST stay a
//     function-scope declaration: removing it drops 82.9 -> 73.6, so it is
//     load-bearing for the frame and only its name could be changed.
// (4) 82.9 -> 83.0: in the kind-1 rect, `rect[2]` is written BEFORE
//     `rect[1] += 0x8000`, with `short* rp` declared before the dword copy.
//     The original's instruction order is not the source order that matches.
// Two more permuter passes over ~13,000 candidates found nothing past 82.9, and
// all 24 orderings of the int x1/y1/x2/y2 declarations plus all 720 orderings
// of the six function-scope declarations compile byte-identically.
// What still differs, about 115 of 714 instructions:
//   - kind 0 (orig insn 84-102): identical instruction SEQUENCE, but the
//     original finishes x2 into ebp before y2 into esi (add ebp,0x80 then
//     add esi,0x20) while we finish y2 into ebp and x2 into esi. All ten
//     orderings of x1/y1/x2/y2 were compiled and none puts x2 in ebp, so this
//     is not source order. The original also spills (pos.y>>16)>>1 to
//     [esp+0x28] and keeps the start.y half in ecx; we spill scrollY instead.
//   - kind 7 (orig insn 494-512, 550-613): same sequence, different slots. The
//     original keeps dx/dy/dz at frame+0x50/0x54/0x58, reloads the steps from
//     0x64/0x68 and stores `prev` at 0x6c/0x70/0x74; ours uses the low slots.
//     It reads the segment count as `word [esp + 0x12]`, the high word of a
//     dword at frame+0x00 shared with kind 0's colour1, where we read frame
//     +0x1e. Declaring dx/dy/dz at function scope or in a struct, adding a
//     fourth kind-7 Vec3, adding an __int64 to the nSeg union, or reordering
//     the union members move none of these. The original also enters the outer
//     loop with `jmp` past a `mov esi,[esp+0x28]` that sits on the back edge
//     (two entry points); we reload inline. The kind-7 colour is at frame
//     +0x1c there and frame+0x24 here.
//   - the kind-1 rect sits at frame+0x28 here and frame+0x20 there, and the
//     original's kind-3 argument is a SEPARATE 4-short array at frame+0x28
//     while ours passes `clip` (12 bytes) from higher up.
//   - the trailing jump-table bytes, which follow from the above.
// `clip` is DEAD in this file: short clip[4|5|6|8|12] do NOT all compile
// identically any more (clip[6] is best at 83.0), but it is still only ever
// passed, never written. It is kept because it pads the frame to the original's
// 0x68; dropping it gives 0x5c and costs about 1.6%.
// Tried and inert (byte-identical) or much worse, do not retry: all ten
// orderings of x1/y1/x2/y2 and all 24 of their declarations; all 720 orderings
// of the function-scope declarations; abs() vs a local Abs_ helper vs double;
// three swap-temp spellings; named temps for the halved y; real field access
// (pos->z>>16) instead of (char*)pos+N casts; colour as int/unsigned/short/
// char; nseg split into int + __int64; reloading `start` at the top or bottom
// of the kind-7 outer loop. Splitting `x2 = a - b + c` into two statements drops
// to 38-47%: never break these expressions apart.
// Verified identical already: the prologue, the whole LOS visibility test
// (insn 24-68, the ByteMap/MapSize inline members and both visible arms), the
// entry to every kind 1-6 branch, the kind-4 switch, and kinds 5 and 6
// (only jump-target addresses differ there, a byte-shift artifact).
// Everything below is the previous sessions' notes, still accurate about the
// field offsets and the kind-7 semantics.
//
// PARTIAL (67.3%). claude-sonnet-5-5 pass (#4143), 43.3 -> 67.3 (stopped by the
// lead before it wrote notes): the LOS test goes through small inline members
// (ByteMap::Get, MapSize::Contains) with an explicit visible = 1 / else 0, the
// line-drawing branch tests `field_10e != 0` first with abs() and the swap
// arms nested inside, x1/y1/x2/y2 are assigned in the original's order, and
// scrollX/scrollY are read as (short) of 4-byte fields. Now 2312 bytes
// against 2272; the remaining diff was not characterised.
// PROBE (deepseek-v4.1-flash, issue 4066, best 43.3%, no change): the
// if/else visible shape (exactly 2272 bytes) with `unsigned int color1;`
// hoisted above `int time` is byte-identical to that 43.1 form, so the
// visible-spill shape (2292 bytes) stays best. Still differs: low-block
// slots +4 (first spill [esp+0x24] vs the original [esp+0x20]), frame
// 0x64 vs 0x68, and p/pos in ebp/esi swapped against the original esi/ebp.
// rect[5] (2312 bytes) regresses to 38.5.
//
// PROBE (deepseek-v4.1-flash, issue 3929, best 43.3%, no change): hoisting
// `int visible;` from the loop body to function scope (declared right after
// `int index = 0;`) is byte-identical at 43.3 / 2292 bytes, so the visible
// spill slot at frame+0x00 is allocated by first use, not by declaration
// order. The file below declares it at function scope; the old loop-scope
// spelling scored the same.
//
// RETRY (deepseek-v4.1-flash, issue 3869, best 43.3%, no change): the los arm
// as `if (cond) visible = 1; else visible = 0;` (the shape that removes the
// early [esp+0x10] stores and lands on the original 2272 bytes exactly) scores
// 43.1; `int visible = 0;` with an unzeroed los arm scores 43.0 / 2288; casting
// pb inline instead of the `pi` local is byte-identical at 43.3, so the spill
// form stays best. Note the original does spill visible too: at the `if
// (visible)` test it runs `test eax,eax` then `mov [esp+0x10], eax` before the
// je, so only the store *placement* differs there, not the existence of the
// slot. What is left: the whole low block is one slot high (first store
// [esp+0x24] vs the original's [esp+0x20]) while the frame is 4 short (0x64 vs
// 0x68), plus the swapped p/pos registers and the loop-tail jmp/reloads.
// PASS 10 (deepseek-v4.1-flash, issue 3625, best 43.3%, no change): mapped our
// slots against the original's to localise the +4. Ours: frame0 +0x04 (same),
// visible spill +0x00, color1 +0x08, time +0x0c, offset +0x10, color2 +0x14,
// index +0x18. Original: color1/gaf/nSeg +0x00, frame0 +0x04, time +0x08,
// offset +0x0c, color2/sx +0x10, index +0x14. So the whole low block is exactly
// ours minus the 4-byte `visible` spill at +0x00, and the original's extra
// dword lives at the top (frame 0x68 vs 0x64). One probe: moving the
// `color2 = palette[field_10e]` read after the four coordinate computations
// (which is the original's schedule) makes the frame 0x78 and drops 43.3 ->
// 33.2, so the low-block colouring is worth far more than the schedule.
// PASS 9 (deepseek-v4.1-flash, issue 3552, best 43.3%, 2292 bytes): the frame
// LAYOUT, not the code, was the lever nobody had pulled. Growing the second
// short[4] scratch array by one element (`short clip[6];` instead of
// `short clip[4];`, with `short rect[4];` untouched) raises 39.2 -> 43.3.
// Sizes swept: clip[5] 40.9, clip[6] 43.3, clip[7]/clip[8]/int clip[4..8] 42.6,
// clip[12] 42.6, rect[6] 42.2, rect[8] 41.5, rect[6]+clip[6] 38.1, an extra
// used int/holder local (home9/type/pb spellings) byte-neutral at 43.3, and
// removing `index` does not compile. So the original's 0x68 frame wants the
// low local block +4 (ours is still 4 high: first spill [esp+0x24] vs the
// original's [esp+0x20]) and one more 4-byte slot at the bottom that no
// declaration shape here creates. Still unfixed: loop pointers p/pos are
// swapped (original p in esi, pos in ebp via `lea ebp,[esi+4]`; ours the
// reverse) and our loop tail has an extra jmp plus slot reloads.
//
// RETRY (deepseek-v4.1-flash, issue 3575, best 43.3%, no change): three more
// probes, all byte-neutral or worse and reverted: pos declared before the
// player/pb pair (43.3, byte-neutral), frame0 declared before time (42.6),
// clip[6] declared before rect[4] (43.3, byte-neutral). The p/pos register
// swap (original p in esi with `lea ebp,[esi+4]`; ours p in ebp) is not moved
// by the declaration order of pos, nor by the time/frame0 or rect/clip order.
//
// PASS 8 (deepseek-v4.1-flash, issue 3524, best 39.2%, 2284 bytes): two more
// allocator attempts, both reverted. (a) An explicit `else visible = 0;` on top of
// the current `visible = 0; if (...) visible = 1;` gives 39.0, 2268 bytes, exactly
// the pass-7 if/else score, so the else arm does not merge the pb-in-eax and
// visible-in-eax colourings either. (b) Passing `&p->pos` to FUN_00408090 instead
// of the `pos` pointer is byte-identical (39.2, 2284), so the call argument spelling
// is not the lever that swaps p/pos (ebp/esi).
// PASS 7 (deepseek-v4.1-flash): tested the hypothesis that splitting the
// `visible` if/else (`if (cond) visible = 1; else visible = 0;` instead of
// `visible = 0; if (cond) visible = 1;`) removes the spill. It does: the
// variant emits `mov eax,1` / `xor eax,eax` exactly like the original, has NO
// store to [esp+0x10] and shrinks to 2268 bytes (original 2272). BUT it scores
// only 39.0, because the allocator then hands the player-info base to ebx
// (original: eax, `push eax` at the call) and the viewFlags byte to al
// (original: cl). The memory-spilling form keeps pb in eax and viewFlags in cl,
// which is worth more matched instructions than the removed spill, so the file
// stays on the 39.2 form. Both forms are saved in
// build/scratch/0x49be60/v1_reg_visible.cpp (2268 bytes, 39.0) and here.
// Merging them (pb in eax AND visible in eax) is one allocator decision this
// source shape does not reach; declaration-order perturbations all regressed.
// (started by deepseek-v4.1-flash, retried by GPT-6, retried by deepseek-v4.1,
//  retried again by deepseek-v4.1-flash)
//
// PASS 6 (deepseek-v4.1-flash, best 39.2%, 2284 bytes vs original 2272).
// One real fix: the kind 1 rect fill is a combined dword copy plus folded add,
// exactly as pass 3 decoded: `*(int*)&rect[0] = *(int*)&p->field_34;` then
// `rect[1] += 0x8000;` (gives `mov [esp+0x30],edx` / `add word [esp+0x32],0x8000`)
// and only rect[2] gets a separate `mov ax,[p+0x38]` / `add ax,0x8000` store.
// That alone raised 38.0 -> 39.2.
// Tried and rejected here (all lower than 39.2):
//   - visible as `if (...) visible = 1; else visible = 0;` on its own: 37.6;
//     on top of the rect fix: 39.0. The current `visible = 0; if (...) = 1;`
//     spills visible to the stack slot [esp+0x10] where the original keeps it
//     in eax (mov eax,1 / xor eax,eax), but the spill shape scores better here,
//     presumably because it colours the low frame slots like the original.
//   - row computed before col in the los test: 38.0 / 39.2-neutral (byte count
//     unchanged), so the col/row register roles (ours ebx=col edx=row, original
//     edx=col ecx=row) are not moved by declaration order.
// What still differs (unchanged from pass 5): frame 0x68 vs ours 0x64, every
// loop-body slot +4 (ours time frame+0x0c vs original +0x08), loop pointers
// swapped (original p in esi / pos in ebp via `lea ebp,[esi+4]`, ours the
// reverse), and the loop tail/re-entry uses an extra jmp and slot reloads in
// ours. These look allocator-only; no declaration-order trick moved them.
// PASS 5 (deepseek-v4.1, best 38.0%, 2296 bytes vs original 2272). Three real
// fixes landed on top of the 29.1% handoff:
//   1. kind 7 jitter is 64-bit: `(int)(((__int64)rand() * 11) / 0x8000) - 5`,
//      applied IN PLACE to the high shorts of the point (P) and a `prev` copy
//      (D) is kept as the segment start. The old file jittered in registers
//      with 32-bit math, so it neither called _allmul nor matched any shape.
//   2. the los visibility test must fall out of `if (...) visible = 1; else
//      visible = 0;` (the original has `mov eax,1` / `xor eax,eax`), NOT
//      `visible = los[..] != 0` (which gives setne) and NOT `visible = 0;`
//      followed by one combined `if` (worse, 37.6%).
//   3. `Vec3_0049be60* pos = &p->pos;` (used for every pos access and passed
//      to FUN_00408090) plus reusing the function-scope `sp` Vec3 as kind 7's
//      `cur`, with `pt`/`prev` hoisted to function scope: frame dropped
//      0x78 -> 0x64 (original 0x68) and score 33.1 -> 36.9. The pos pointer
//      alone (before the sp reuse) gave 38.0%.
// What still differs: every loop-body stack slot is +4 vs the original (ours
// time [esp+0x24] frame+0x0c, original [esp+0x20] frame+0x08; index/offset the
// same +4), the frame is 0x64 vs 0x68, and the two loop pointers are swapped:
// ours p in ebp / pos in esi, original p in esi / pos in ebp (`lea ebp,
// [esi+4]`). The original also reaches the loop body by fall-through from the
// count check, ours enters through an extra `jmp`/slot reload, and the
// whole-body register allocation is still permuted. Declaration order changes
// (pos before p, pos inside the counter test, pt/prev order) are byte-identical
// at 38.0%, so the residue is allocator-only.
// Partial: kind-7 lightning interpolation and stack/register layout still differ.
//
// PARTIAL. 0x49be60 (2272 bytes) is the projectile render pass: it walks the
// 300-entry projectile array (count g_game+0x141f3, base g_game+0x141f7,
// stride 0x6b) and draws every live projectile (p+0x60 counter == 0) that is
// visible to the local player, dispatching on the shot-type byte type+0x10c
// (0..7). The visibility test mirrors 0x481930/0x482c20: with viewFlags bit 2
// set it indexes the per-player line-of-sight grid (PlayerInfo+0x7c ptr,
// +0x80 width, +0x84 height), otherwise it calls FUN_00408090.
//
// This first pass transcribes the full control flow, the branch structure and
// every field offset used. What is NOT right yet and is the next step:
//   - register allocation everywhere (the original keeps g_game in edi and
//     frame0 in [esp+0x14]; MSVC gives different roles here);
//   - the exact local layout: the original's frame is 0x68 with cur/prev
//     Vec3s at +0x48/+0x54, random offsets at +0x60/+0x64/+0x68 and the
//     kind-1 rect at +0x30; the sp Vec3 of kinds 1/3/4/6 sits at +0x48;
//   - kind 7 (the multi-segment trail) is approximated: its inner loop draws
//     a segment from the previous point to the current point plus a per-axis
//     rand()*11/0x8000-5 jitter (rand() returns 0..0x7fff), (short)(nSeg>>16)
//     segments, two outer passes.
// RETRY NOTE: sharing the sp Vec3 and the two rect slots across the disjoint
// kind branches (declared once at function scope) dropped the frame from 0x90
// to 0x6c, still 4 bytes over the original's 0x68, and raised the score from
// 13.1% to 13.4%. The remaining gap is the whole-function register allocation:
// the original keeps g_game in edi (reloaded after calls), p in esi (advanced
// by adding an offset local, not scaled indexing), pos in ebp, frame0 at
// [esp+4], time at [esp+8], the projectile offset at [esp+0xc], the index at
// [esp+0x14]; MSVC here puts g_game in ebp and uses scaled indexing. No
// instruction run aligns with the original, so no local patch closes it.
// Every offset and callee argument order below is believed correct.
//
// UPDATE (deepseek-v4.1-flash, pass 2, best 28.9%): two real fixes landed.
//   1. Caching `Game* g = g_game;` in a local was WRONG. The original source
//      surely uses the global `g_game->...` at every use; MSVC then reloads the
//      pointer out of its own register/rematerializes it around calls. Removing
//      the local and spelling out g_game everywhere raised 13.4% -> 27.9%.
//   2. Game_0049be60 had a 2-byte hole at +0x14321: scrollX is at +0x1431f and
//      scrollY at +0x14323, so with pack(1) every field from +0x14321 on was
//      emitted 2 bytes low (gaf_1480f at +0x1480d, time at +0x38a45). Adding
//   What still differs: `sub esp, 0x6c` vs 0x68 (one extra spilled dword;
//   frame0 gets both ebp and a stack slot here, the original keeps it only at
//   [esp+0x14]), the loop is entered through an extra `jmp`, and the whole-body
//   register allocation is still swapped (original: edi = g_game, esi = p,
//   ebp = pos; here ebp = g_game/scratch). The relocated call set and every
//   struct offset now match, so the remaining gap is instruction selection.
//
// RETRY NOTE (deepseek-v4.1, pass 4): three shapes tried, all 29.1% or worse.
//   (a) `Vec3* pos = pos;` used everywhere: 27.8% but it MOVED frame0 to
//       the original's slot [esp+0x14] (= frame+0x04) and made
//       `lea ebp,[esi+4]` appear, proof that the original really holds a
//       pos pointer in ebp; every other slot then shifted +4 (time at
//       frame+0x14 instead of +0x10), so the win did not carry.
//   (b) same pointer only inside the visibility test and the FUN_00408090
//       call: 29.1%, 2208 bytes (4 bytes worse than baseline), no layout change.
//   (c) hoisting `int visible;` to function scope: byte-for-byte the baseline.
//   So the whole gap is one allocator decision: in the baseline frame0 lives in
//   BOTH ebp and [esp+0x14], pushing index to frame+0x18 and the frame to 0x6c
//   (every stack offset +4); the original keeps frame0 memory-only at
//   frame+0x04. In the original the four callee-saved registers are all held
//   (edi g_game, esi p, ebp pos, ebx type), which is what forces frame0
//   into a slot; our baseline leaves ebp free for frame0 because the pos
//   addresses are folded into esi-relative operands.
//
// RETRY NOTE (deepseek-v4.1, pass 3, best 29.1%): decoded the exact frame map
// and one real extra store. The prologue is `sub esp,0x68` then push
// ebx,ebp,esi,edi, so the post-prologue esp is frame-0x10: a local seen as
// [esp+N] is at frame offset N-0x10, [esp+0x78] is the return address and
// [esp+0x7c] is the `surface` argument. The 0x68-byte frame is one shared
// scratch region, every kind branch reusing the same slots:
//   +0x00 color1 (kind 0) / gaf (kind 4 switch) / nSeg (kind 7, int)
//   +0x04 frame0, +0x08 time, +0x0c byte offset, +0x14 index
//   +0x10 color2 (kind 0) / sx (kind 5); +0x18 (field_36>>1) temp (kind 0) /
//        &p->start (kind 7); +0x1c palette color (kind 7) / sy (kind 5)
//   +0x20..0x27 kind 1 rect (4 shorts; rect[3] is NEVER written, the original
//        copies field_34+field_36 as one dword then `add word [esp+0x32],0x8000`)
//   +0x28..0x2f kind 3 rect (never written anywhere: passed uninitialised)
//   +0x30/+0x34 nSeg as __int64 (kind 7 divides by the full 64-bit value)
//   +0x38/+0x3c/+0x40 sp Vec3 (kinds 1,3,4,6) and cur Vec3 (kind 7)
//   +0x44/+0x48/+0x4c kind 7 point P, +0x50/0x54/0x58 dx,dy,dz then stepY,stepZ
//        (dx keeps +0x50, stepX stays in ebx), +0x5c/+0x60/+0x64 a second copy
//        of P (called D below) used for the line start
// Removed `rect[3] = 0;`: the original has no store to [esp+0x36] at all, so
// that was pure extra code (our file shrank 2212 -> 2204 bytes, score steady).
// Kind 4's switch is a real jump table at 0x49c72c, indexed field_10d with
// `cmp ecx,4 / ja` default, case bodies in source order 0,1,2,3,4.
// Kind 4 frame divisor: `(time - p->field_42) % *(unsigned short*)gaf`
// (cdq/idiv, so the `%` is signed).
// Still differs (nothing lines up, every instruction run is permuted):
//   - register roles: original edi=g_game (reloaded from [0x511de8] after every
//     call, including the tiny `mov edi,[0x511de8]` at 0x49c052 before the loop
//     tail), esi=p, ebp=pos (lea ebp,[esi+4], so the source really does hold
//     a pos pointer local, not `pos->x` expressions), ebx=type (mov ebx,[esi]).
//     Ours puts g_game in ebp and indexes the projectile array scaled.
//   - frame 0x6c vs 0x68 (one extra spilled dword).
//   - 2204 bytes vs 2272: about 68 bytes of code are missing, none of it
//     identified; the kind 7 loop below is semantically close but the original
//     adds the rand() jitter in place to the high shorts of P (the dword at
//     +0x44/+0x48/+0x4c) and copies P to +0x5c/+0x60/+0x64 before advancing cur.
//   - the loop is entered through an extra `jmp` in ours.
// Recomputed but left as-is: the abs() of the kind 0 diagonal really is the
// inlined `(v ^ (v>>31)) - (v>>31)` form, ours inlines too.
//
// Layout facts:
//   Projectile stride 0x6b. type at +0x0; pos Vec3 (16.16) at +0x4; start
//   Vec3 at +0x10; short field_34/+0x36/+0x38 (the kind 1/6 sprite rect);
//   int field_42 (spawn time) +0x42; int field_46 +0x46; short field_5e +0x5e
//   (sprite half height); short counter +0x60; short field_64 +0x64;
//   ushort flags +0x69.
//   Type: ptr field_74 +0x74 (sprite object, its +0x30 is a second frame);
//   ushort field_e6 +0xe6 (kind 5 animation divisor); byte field_10c +0x10c
//   shot kind; byte field_10d +0x10d palette index; byte field_10e +0x10e
//   second palette index; dword flags +0x111 (bit 21 tested in kind 1).
//   Game: palette bytes +0xdcb; projectiles +0x141f3/+0x141f7; viewFlags byte
//   +0x14281 bit 2; shorts scrollX +0x1431f, scrollY +0x14323; gaf pointers
//   +0x147bb/+0x147bf/+0x147c3/+0x147c7/+0x147cb (kind 4), +0x147f3 (kind 5),
//   +0x1480f (the shared sprite, frame 0); ptr +0x1ab9b (kind 2);
//   localPlayer byte +0x2a43; player array base +0x1b63 stride 0x14b;
//   region +0x37e27 (kind 2); time int +0x38a47.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>

#pragma pack(push, 1)
struct Vec3_0049be60 {
    int x;
    int y;
    int z;
};

struct Type_0049be60 {
    char unknown_0[0x74];
    void* field_74;                    // +0x74
    char unknown_78[0xe6 - 0x78];
    unsigned short field_e6;           // +0xe6
    char unknown_e8[0x10c - 0xe8];
    unsigned char field_10c;           // +0x10c
    unsigned char field_10d;           // +0x10d
    unsigned char field_10e;           // +0x10e
    char unknown_10f[0x111 - 0x10f];
    unsigned int flags_0 : 21;         // +0x111
    unsigned int flag_21 : 1;
    unsigned int flags_2 : 10;
};

struct Sprite_0049be60 {
    char unknown_0[0x30];
    void* field_30;                    // +0x30
};

struct Proj_0049be60 {
    Type_0049be60* type;               // +0x0
    Vec3_0049be60 pos;                 // +0x4
    Vec3_0049be60 start;               // +0x10
    char unknown_1c[0x34 - 0x1c];
    short field_34;                    // +0x34
    short field_36;                    // +0x36
    short field_38;                    // +0x38
    char unknown_3a[0x42 - 0x3a];
    int field_42;                      // +0x42
    int field_46;                      // +0x46
    char unknown_4a[0x5e - 0x4a];
    short field_5e;                    // +0x5e
    short counter;                     // +0x60
    char unknown_62[0x64 - 0x62];
    short field_64;                    // +0x64
    char unknown_66[0x69 - 0x66];
    unsigned short flags;              // +0x69
};

struct MapSize_0049be60 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_0049be60 {
    unsigned char* data;               // +0x0
    MapSize_0049be60 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_0049be60 {
    char unknown_0[0x7c];
    ByteMap_0049be60 explored;         // +0x7c
};

struct Game_0049be60 {
    char unknown_0[0xdcb];
    unsigned char palette[0x2a];       // +0xdcb
    char unknown_df5[0x2a43 - 0xdf5];
    unsigned char localPlayer;         // +0x2a43
    char unknown_2a44[0x141f3 - 0x2a44];
    int projectileCount;               // +0x141f3
    Proj_0049be60* projectiles;        // +0x141f7
    char unknown_141fb[0x14281 - 0x141fb];
    unsigned char viewFlags;           // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x147bb - 0x14327];
    void* gaf_147bb;                   // +0x147bb
    void* gaf_147bf;                   // +0x147bf
    void* gaf_147c3;                   // +0x147c3
    void* gaf_147c7;                   // +0x147c7
    void* gaf_147cb;                   // +0x147cb
    char unknown_147cf[0x147f3 - 0x147cf];
    void* gaf_147f3;                   // +0x147f3
    char unknown_147f7[0x1480f - 0x147f7];
    void* gaf_1480f;                   // +0x1480f
    char unknown_14813[0x1ab9b - 0x14813];
    void* field_1ab9b;                 // +0x1ab9b
    char unknown_1ab9f[0x37e27 - 0x1ab9f];
    char field_37e27[0x38a47 - 0x37e27];
    int time;                          // +0x38a47
};
#pragma pack(pop)

extern Game_0049be60* g_game;

void* __stdcall FUN_004b7f30(void* gaf, int frame);
void __stdcall FUN_004b7f90(void* dest, void* src, int x, int y);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);
void __stdcall FUN_0046bae0(void* dest, Vec3_0049be60* pos, void* sprite, void* rect);
int __stdcall FUN_004b6720(void* region, int x, int y);
void __stdcall FUN_004b9360(void* dest, void* src, int x, int y);
void __stdcall FUN_004be950(void* dest, int x1, int y1, int x2, int y2, unsigned int color);
int __stdcall FUN_004b7f60(void* gaf);
int __stdcall FUN_00408090(PlayerInfo_0049be60* pi, Vec3_0049be60* pos);

static int Abs_0049be60(int v)
{
    return (v ^ (v >> 31)) - (v >> 31);
}

// FUNCTION: 0x49be60
void __stdcall FUN_0049be60(void* surface)
{
    
    int scrollXk6;
    Type_0049be60* type;
    int fr;
    Vec3_0049be60 sp, prev;
    Vec3_0049be60 pt;
    short rect[4];
    short clip[6];
    int time = g_game->time;
    void* frame0 = FUN_004b7f30(g_game->gaf_1480f, 0);
    int index = 0;
    int visible;
    if (g_game->projectileCount <= 0)
        return;
    int offset = 0;
    while (1) {
        Proj_0049be60* p = (Proj_0049be60*)((char*)g_game->projectiles + offset);
        if (p->counter == 0) {
            unsigned char player = g_game->localPlayer;
            char* pb = (char*)g_game + 0x1b63 + 0x14b * player;
            Vec3_0049be60* pos = &p->pos;
            if ((g_game->viewFlags & 2) == 2) {
                int col = (int)*(short*)((char*)pos + 2) >> 5;
                PlayerInfo_0049be60* pi = (PlayerInfo_0049be60*)pb;
                int row = ((int)*(short*)((char*)pos + 10)
                           - ((int)*(short*)((char*)pos + 6) >> 1)) >> 5;
                if (pi->explored.size.Contains(col, row) && pi->explored.Get(col, row))
                    visible = 1;
                else
                    visible = 0;
            } else {
                visible = FUN_00408090((PlayerInfo_0049be60*)pb, pos);
            }
            if (visible) {
                type = p->type;
                if (type->field_10c == 0) {
                    unsigned int color1 = g_game->palette[type->field_10d];
                    unsigned int color2 = g_game->palette[type->field_10e];
                    int x1;
                    int y1;
                    int x2;
                    int y2;
                    x1 = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    y1 = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    x2 = (int)*(short*)((char*)&p->start + 2) - (short)g_game->scrollX + 0x80;
                    y2 = ((int)*(short*)((char*)&p->start + 10)
                              - ((int)*(short*)((char*)&p->start + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (type->field_10e != 0) {
                        if (abs(x1 - x2) > abs(y1 - y2)) {
                            if (x1 > x2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            FUN_004be950(surface, x1, y1 - 1, x2, y2 - 1, color2);
                            FUN_004be950(surface, x1, y1, x2, y2, color1);
                        } else {
                            if (y1 > y2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            FUN_004be950(surface, x1 - 1, y1, x2 + 1, y2, color2);
                            FUN_004be950(surface, x1, y1, x2, y2, color1);
                        }
                    } else {
                        FUN_004be950(surface, x1, y1, x2, y2, color1);
                    }
                } else if (type->field_10c == 1) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = 0x80 + (int)*(short*)((char*)&sp + 2);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    short* rp = &p->field_34;
                    *(int*)&rect[0] = *(int*)&p->field_34;
                    rect[2] = (short)(0x8000 + rp[2]);
                    rect[1] = rect[1] + 0x8000;
                    FUN_0046bae0(surface, &sp, type->field_74, rect);
                    Sprite_0049be60* s = (Sprite_0049be60*)type->field_74;
                    if (0 != s->field_30 && p->field_46 > time) {
                        if (type->flag_21) {
                            rect[0] = p->field_64;
                            FUN_0046bae0(surface, &sp, s->field_30, rect);
                        } else {
                            FUN_0046bae0(surface, &sp, s->field_30, rect);
                        }
                    }
                } else if (type->field_10c == 2) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (FUN_004b6720((void*)g_game->field_37e27, sx, sy) == 0)
                        return;
                    FUN_004b9360(surface, g_game->field_1ab9b, sx, sy);
                } else if (type->field_10c == 3) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    FUN_004b8500(surface, frame0, sx, sy);
                    FUN_0046bae0(surface, &sp, type->field_74, clip);
                } else if (type->field_10c == 4) {
                    if (type->field_10d < 0xff) {
                        void* gaf = 0;
                        sp.x = pos->x - (g_game->scrollX << 16);
                        sp.y = p->pos.y;
                        int scrollYs = g_game->scrollY;
                        sp.z = p->pos.z - (scrollYs << 16);
                        FUN_004b8500(surface, frame0,
                                     (int)*(short*)((2 + (char*)&sp)) + 0x80,
                                     (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20);
                        int sy = ((int)*(short*)((char*)&sp + 10)
                                  - ((int)*(short*)((char*)&sp + 6) >> 1)) + 0x20;
                        char* spp = (char*)&sp;
                        int sx = (int)*(short*)(2 + spp) + 0x80;
                        switch (type->field_10d) {
                        case 0: gaf = g_game->gaf_147bb; break;
                        case 1: gaf = g_game->gaf_147bf; break;
                        case 2: gaf = g_game->gaf_147c3; break;
                        case 3: gaf = g_game->gaf_147c7; break;
                        case 4: gaf = g_game->gaf_147cb; break;
                        }
                        if (gaf) {
                            int n = *(unsigned short*)gaf;
                            FUN_004b7f90(surface, FUN_004b7f30(gaf, (time - p->field_42) % n), sx, sy);
                        }
                    }
                } else if (type->field_10c == 5) {
                    int sx = 0x80 + ((int)*(short*)(((char*)pos + 2)) - (short)g_game->scrollX);
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    void* gaf = g_game->gaf_147f3;
                    int n = FUN_004b7f60(gaf);
                    fr = n - ((p->field_46 - time) * n) / (int)type->field_e6;
                    if (fr >= 0 && fr < n) {
                        FUN_004b8500(surface, FUN_004b7f30(gaf, fr), sx, sy);
                    }
                } else if (type->field_10c == 6) {
                    scrollXk6 = g_game->scrollX;
                    sp.x = pos->x - (scrollXk6 << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->field_5e >> 1) + 0x20;
                    int sx = ((int)*(short*)((char*)&sp + 2) + 0x80);
                    FUN_004b8500(surface, frame0, sx, sy);
                    FUN_0046bae0(surface, &sp, type->field_74, &p->field_34);
                } else if (type->field_10c == 7) {
                    Vec3_0049be60* start = &p->start;
                    unsigned int color = g_game->palette[type->field_10d];
                    int dx = pos->x - start->x;
                    int dy = pos->y - start->y;
                    int dz = pos->z - start->z;
                    union { int i; short s[2]; } nSeg;
                    nSeg.i = (int)(((__int64)((int)sqrt(dx * (double)dx + (double)dy * dy + (double)dz * dz)) << 16) / 0x50000);
                    if (0 != nSeg.i) {
                        int stepX = (int)(((__int64)dx << 16) / (__int64)nSeg.i);
                        int stepY = (int)(((__int64)dy << 16) / (__int64)nSeg.i);
                        int stepZ = (int)(((__int64)dz << 16) / (__int64)nSeg.i);
                        int outer = 2;
                        for (; 1; ) {
                            pt = *start;
                            sp = *start;
                            {
                                short n = nSeg.s[1];
                                if (n > 0) {
                                    int i = n;
                                    do {
                                        prev = pt;
                                        sp.x += stepX;
                                        sp.y = stepY + sp.y;
                                        sp.z += stepZ;
                                        pt = sp;
                                        *(short*)((char*)&pt + 2) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 6) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 10) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        FUN_004be950(surface,
                                            (int)*(short*)(2 + (char*)&prev) - (short)g_game->scrollX + 0x80,
                                            ((int)*(short*)(10 + (char*)&prev)
                                             - ((int)*(short*)((char*)&prev + 6) >> 1))
                                            - (short)g_game->scrollY + 0x20,
                                            (int)*(short*)((char*)&pt + 2) - (short)g_game->scrollX + 0x80,
                                            0x20 + (((int)*(short*)((char*)&pt + 10)
                                             - ((int)*(short*)(6 + (char*)&pt) >> 1))
                                            - (short)g_game->scrollY),
                                            color);
                                        i = i - 1;
                                    } while (i != 0);
                                }
                            }
                            outer = outer - 1;
                            if (!outer)
                                break;
                        }
                    }
                }
            }
        }
        index = index + 1;
        offset += 0x6b;
        if (index >= g_game->projectileCount)
            break;
    }
}