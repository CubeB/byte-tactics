// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
//
// SOLVED (DeepSeek V4.1 Flash, #4833): 93.5% -> MATCH, 858 -> 857 bytes. Two
// changes in the sea-level block, and they only work together:
//
//   * `PlayerRec_0048a490* o = u->owner;` and then `q = o->sight` and
//     `n = g_game->frame - o->age`. The local keeps the owner pointer live
//     across the four 64-bit helper calls, so the allocator holds it in ebp
//     (the original's single `mov ebp, [edi]` reused as `mov esi, [ebp+0x2a]`
//     after the divide). With `u->owner` read at each use the front end
//     rematerialises `[edi]` after the calls and ebp goes to p instead.
//   * The 60-frame clamp has to be inline in the multiply as
//     `(n < 60 ? n : 60)` rather than a separate `if (n >= 60) n = 60;`. The
//     if form schedules the frame/age loads after mm, so mm lands in esi, p
//     goes to ebp and the function comes out 847 bytes at 89.7%. The ternary
//     keeps the age load hoisted before mm, so mm lands in ecx, p spills to
//     [esp+0x28] and eax holds the clamp result, which is the original.
//     `min(n, 60u)` gives the same allocation but selects into edx (95.7%).
//     Every if-shaped spelling was measured and all regress to 847/89.7.
//
// Space Bunny Free (#4163): 77.8% -> 92.2% -> 93.5% (858 bytes). All the gains
// are in the loop body, and all of them are the register allocator agreeing with
// the original after a change of statement shape. Four changes did it, the first
// three by hand and the fourth from tools/permute.py with seed 7:
//
//   * `pts[k].x = t.x = vx;` as a chained assignment, where the previous text
//     had `t.x = vx; pts[k].x = vx;`. Worth 7.6 points (77.8% -> 85.4%). The
//     chained form puts the two stores in the original's order AND frees the
//     register the allocator wanted for the corner-array pointer: with it the
//     map pointer goes back to edx, the pointer induction variable to ecx and
//     u->aim back to `mov cx`, which is 20 diff lines at once. Splitting them
//     the other way round (`t.x = pts[k].x = vx`) scores 77.8% again, so the
//     store order alone is not the lever; it is the register choice the chain
//     makes.
//   * `wx` before `hz`, and `fx` before `fz`. Worth 4.3 points (85.4% ->
//     89.7%). The original computes wx first and copies it straight into esi
//     (fx) and edx (gx) before it computes hz at all, which is only reachable
//     if the two halves are written in that order. All 560 legal orders of the
//     six declarations (wx, hz, fx, fz, gx, gz) with `gridW` inserted at each
//     of the seven positions were swept; seven tie at 92.2% and this is one.
//   * Writing the interpolated height through the array before reading it
//     back (`hs[k].h = H0 + ((H1 - H0) * fz) / 16; int H = hs[k].h;` instead of
//     `int H = ...; hs[k].h = H;`) is worth 2.5 points (89.7% -> 92.2%). It
//     puts H in ecx, which is the register the original's sea-level max select
//     wants, and that fixes the whole max hunk. `H > sea ? H : sea`, an
//     unsigned char sea and the reversed sum `((H1 - H0) * fz) / 16 + H0` are
//     all byte-flat here, so the max itself was never the problem.
//   * `* 2048` for `<< 11` in p (which keeps the original's `and eax, 0x1f`;
//     see the micro probe note below), `short` for FUN_004b7123's first
//     parameter (which turns `movsx eax, word ptr [esp + 0x28]` into the
//     original's plain `mov eax, dword ptr [esp + 0x28]`) and reading hs[1]
//     before hs[0] in the tail together give 92.4% at 858 bytes.
//   * The permuter then found the last 1.1 points (92.4% -> 93.5%): declaring
//     `unsigned gz` before `int fz`, and giving `gw` and `c0` their own
//     declaration apart from `t`, `pts`, `hs` and `k`. Both are declaration
//     moves and neither is obvious: on the 92.4% version `gz` before `fz` was
//     byte-flat (92.4% either way), and it only becomes worth 1.1 points once
//     the other two declarations move as well, so the pair is the lever, not
//     either alone. (Whether `gw` and `c0` sit outside the block or inside it
//     is byte-flat too; what matters is that they are declared separately.)
//
// Compiler rules measured while getting p right (the micro probe is
// build/scratch/0x48a490/micro.cpp, which compiles to micro.lst):
//
//   * `int` results add the addend 32-bit, with an explicit zero-extension first
//     (`and edx, 65535; ...; add eax, edx`), while `short` and `unsigned short`
//     results give the 16-bit `add ax, word ptr [u->fix_lo]` the original has.
//     So p has to be a 16-bit object even though it is pushed as a dword, which
//     is why declaring FUN_004b7123's first parameter `short` turns the reload
//     into the original's plain `mov eax, dword ptr [esp + 0x28]`.
//   * The original keeps `and eax, 0x1f` on the random value, and `<< 11` throws
//     that mask away here (93.3% without it) while `* 2048` for the same product
//     keeps it and still gives `add ax`. The micro probe says which side of the
//     line it is: with one expression and `<< 11` the mask survives
//     (`and eax, 31; ...; shl eax, 11; add eax, ecx`) but the add is 32-bit, and
//     it is the split form `short p = (short)(X << 11); p = (short)(p + g);` that
//     loses the mask entirely. Whichever way it goes, `<< 11` is not what the
//     original was written with, so the source uses `* 2048`. That costs 3 bytes
//     and the `short` callee parameter gives 1 byte back.
//
// What still differs (18 diff lines, 8 of them only jump targets):
//
//   * The sea-level block (10 lines). The original holds `u->owner` in ebp from
//     before the p expression to after the 64-bit divide, loads age into esi and
//     frame into eax (`mov eax, [g_game]`, the 5-byte A1 form), and clamps in
//     eax. We hold the owner in ecx, hoist n into ebp and clamp in ebp, so
//     `g_game` goes through `mov edx, [g_game]` (6 bytes). That one byte is the
//     whole reason the function is 858 instead of 857: with it the loop's
//     `je`/`jne` targets and the two `jae` in the head all sit one byte past
//     the original's, and taking that byte out is worth the eight target-only
//     lines on its own (93.5% to about 96.4%). The frame load is the only
//     instruction in the whole block whose encoding is one byte shorter in
//     MSVC 5's A1 form, and A1 is only ever used for eax, so it comes down to
//     making the allocator pick eax. Nothing reaches it: the late-n schedule
//     that would give the original's `mov eax, [g_game]` compiles and matches
//     the owner load, but it moves mm from ecx to esi and the pts induction
//     variable from ecx to edx and scores 89.7%. Caching the owner pointer, an
//     owner load before p, `frame` or `age` into a local, `n` split over two
//     statements or into `unsigned`, `n` moved before q or s, a `ty` local for
//     u->type, and all 45 legal orders of the block's six statements were
//     measured; every one of them is byte-flat or worse.
//
// Everything else that was tried is byte-flat or worse: int/unsigned/short for
// p, `unsigned short` against `short` for fix_lo, a by-value helper around the
// min, the head pair split into `row = m->rows; row += m->count;`, a named maps
// array, a named map index, all 24 orders of t/pts/hs/k, all 24 orders of the
// four hs[] reads in the tail and both operand orders of both sums, /2/2 instead
// of (h0+h1)/2, `u->type->sight / 2` against abs(), and (t.x + u->posx) against
// (u->posx + t.x).
// deepseek-v4.1-flash (#3932) retry: swapping the second tail sum to (h3 + h2)
// and swapping both a/b tail sums is byte-flat at 77.8% / 857 bytes, so the tail
// hunk (load order plus lea [ebp + ebx]) is not source-operand-order reachable.
// The residual hunks stand: m edx/ecx in the prologue, the loop carry reloads,
// and the max-select register roles (H in eax here against ecx in the original).
//
// What still differed at 77.8%, all of it a register choice MSVC 5 makes for
// itself (this section is the 92.2% pass, kept for the record):
//
//   1. hz lands in eax, the original's in ecx (2 diff lines).
//   2. The sea-level block (11 diff lines). The original keeps `u->owner` in
//      ebp from before the p expression to after the 64-bit divide and only
//      then computes n = g_game->frame - u->owner->age into eax; we hoist n
//      into ebp before the call and reload the owner late. Both schedules
//      compile, and neither dominates: hoisting n fixes the owner load but
//      moves mm from ecx to esi and the pts induction variable from ecx to edx,
//      which loses more than it gains. Three sub-levers were measured
//      separately and each is real but none fits: `* 2048` instead of `<< 11`
//      keeps the original's `and eax, 0x1f` (3 bytes, 860 total, 91.3%),
//      declaring FUN_004b7123's first parameter short gives the original's
//      plain `mov eax, [esp + 0x28]` instead of `movsx` (1 byte shorter, 856,
//      91.5%), and together with the late n they land at 848 bytes and 87.8%.
//      857 bytes only fits the version that has none of them.
//   3. The tail reads h1 before h0 and sums them as `lea eax, [ebp + ebx]`
//      (3 diff lines). All 24 orders of the four hs[] reads and both operand
//      orders of both sums were swept: 24 tie at 92.2%, and reading b's
//      operands straight from the array scores 91.9% with a better shape
//      (95.1%), so this is a plateau, not a missed permutation.
//
//   * `pts[k].x = t.x = vx;` as a chained assignment, where the previous text
//     had `t.x = vx; pts[k].x = vx;`. Worth 7.6 points (77.8% to 85.4%). The
//     chained form puts the two stores in the original's order AND frees the
//     register the allocator wanted for the corner-array pointer: with it the
//     map pointer goes back to edx, the pointer induction variable to ecx and
//     u->aim back to `mov cx`, which is 20 diff lines at once. Splitting them
//     the other way round (`t.x = pts[k].x = vx`) scores 77.8% again, so the
//     store order alone is not the lever; it is the register choice the chain
//     makes.
//   * `wx` before `hz`, and `fx` before `fz`. Worth 4.3 points (85.4% to
//     89.7%). The original computes wx first and copies it straight into esi
//     (fx) and edx (gx) before it computes hz at all, which is only reachable
//     if the two halves are written in that order. All 560 legal orders of the
//     six declarations (wx, hz, fx, fz, gx, gz) with `gridW` inserted at each
//     of the seven positions were swept; seven tie at 92.2% and this is one.
//   * Writing the interpolated height through the array before reading it
//     back (`hs[k].h = H0 + ((H1 - H0) * fz) / 16; int H = hs[k].h;` instead of
//     `int H = ...; hs[k].h = H;`) is worth 2.5 points (89.7% to 92.2%). It
//     puts H in ecx, which is the register the original's sea-level max select
//     wants, and that fixes the whole max hunk. `H > sea ? H : sea`, an
//     unsigned char sea and the reversed sum `((H1 - H0) * fz) / 16 + H0` are
//     all byte-flat here, so the max itself was never the problem.
//
// What still differs, all of it a register choice MSVC 5 makes for itself:
//
//   1. hz lands in eax, the original's in ecx (2 diff lines).
//   2. The sea-level block (11 diff lines). The original keeps `u->owner` in
//      ebp from before the p expression to after the 64-bit divide and only
//      then computes n = g_game->frame - u->owner->age into eax; we hoist n
//      into ebp before the call and reload the owner late. Both schedules
//      compile, and neither dominates: hoisting n fixes the owner load but
//      moves mm from ecx to esi and the pts induction variable from ecx to edx,
//      which loses more than it gains. Three sub-levers were measured
//      separately and each is real but none fits: `* 2048` instead of `<< 11`
//      keeps the original's `and eax, 0x1f` (3 bytes, 860 total, 91.3%),
//      declaring FUN_004b7123's first parameter short gives the original's
//      plain `mov eax, [esp + 0x28]` instead of `movsx` (1 byte shorter, 856,
//      91.5%), and together with the late n they land at 848 bytes and 87.8%.
//      857 bytes only fits the version that has none of them.
//   3. The tail reads h1 before h0 and sums them as `lea eax, [ebp + ebx]`
//      (3 diff lines). All 24 orders of the four hs[] reads and both operand
//      orders of both sums were swept: 24 tie at 92.2%, and reading b's
//      operands straight from the array scores 91.9% with a better shape
//      (95.1%), so this is a plateau, not a missed permutation.
//
// Nothing else moved the score: int/unsigned/short for p, unsigned short for
// fix_lo, a by-value helper around the min, a cached owner pointer, the head
// pair split into `row = m->rows; row += m->count;`, a named maps array, a
// named map index, all seven orders of t/pts/hs/k, /2/2 instead of (h0+h1)/2,
// `u->type->sight / 2` against abs(), and (t.x + u->posx) against
// (u->posx + t.x) are all byte-flat or worse.
// deepseek-v4.1-flash (#3891) retry: caching the owner pointer as a named
// local (`PlayerRec_0048a490* o = u->owner;` used for both `o->sight` and
// `o->age`, matching the original's single `mov ebp,[edi]` held across the
// sight compare and the 0x3c clamp) regresses 77.8 to 76.7 percent / 856
// bytes: the front end keeps the fused `g_game->frame - o->age` in ebp as
// before and the extra named local only costs a byte, so the owner register
// is not reachable from a local of that shape.
// deepseek-v4.1-flash (#3754) retry: hoisting the m declaration above row
// (m = g_game->maps[u->map]; row = m->rows + m->count;) is byte-flat at 77.8% / 857 bytes,
// so the edx/ecx head swap is not declaration order; the remaining hunks stand as below.
// Samples the ground under a unit at its four surrounding terrain
// vertices and stores the resulting pitch (0x68) and roll (0x70) on the
// unit, plus a heading (0x64) from the two side vertices. The 0x11/0x04
// bytes of a heightmap tile are the two half heights of its edge pair,
// and the corner heights are bilinearly interpolated with a plain / 16
// (MSVC 5 spells that cdq/and 0xf/add/sar 4; there is no second shift).
//
// PARTIAL (77.8%, 857 bytes against 857; Sonnet 5.5 retry #1091 took it from 70.8% with a scripted
// statement-order hill climb, see the end of this comment). Older notes below said 65.5%.
// deepseek-v4.1-flash (#3453) retry: 77.8% unchanged (857 bytes). Swapping the
// tail add to `int a = (h1 + h0) / 2;` to chase the original's
// `mov ebx,[esp+0x60] / mov ebp,[esp+0x6c] / lea eax,[ebx+ebp]` load/add order
// is byte-neutral: the front end canonicalises the commutative add back to the
// h0-first form, so the tail hunk keeps the reversed loads and `lea eax,[ebp+ebx]`.
// Spelling the tail sums directly from the array (`(hs[0].h + hs[1].h) / 2`)
// drops the h0..h3 locals and scores 77.4% / 856 bytes, so the named locals stay.
// GPT-6.1-sol retry (#2922): map/row/count declaration variants stayed at 77.8%; tools/headers.py also found no better header set (128 tried). Still differs in prologue register roles, loop carry/reloads, terrain interpolation spills, and random-mask/reload code; see the detailed notes below.
//
//  1. The bilinear block. The original keeps the first tile's low half in
//     ebx and spills the second tile pointer (esp+0x28) and b1 (esp+0x2c);
//     we keep the second tile pointer in ebx and spill b0 and b1 instead.
//     Both spill two values, so the instruction counts agree, only the
//     choice of which value the allocator drops differs. The loads
//     themselves are in the original's order (b0, b1, c0, c1) and every
//     spelling of the two H0/H1 expressions I tried gave the same code.
//  2. The loop back edge. The original carries the corner-array pointer in
//     ecx and the height-record pointer in ebx across the back edge, so
//     its reload block is only the row and map pointers (0x48a4ed,
//     0x48a4f1). We reload all three, so we emit one `mov edx, [esp+0x18]`
//     the original does not. An explicit `int*` induction variable does not
//     change this: MSVC 5 folds it back into the frame slot either way.
//  3. Small register swaps that follow from the two above: the map pointer
//     in the prologue (edx against ecx), the height max (eax against ecx),
//     and `mov esi,2 / sub esi,eax` against `mov ecx,2 / sub ecx,eax`.
//
// What did work, in order of size, and worth keeping:
//
//  * The loop has to be a `for (k = 0; k < 4; k++)`. As a `do { } while
//    (++k < 4)` MSVC 5 rotates it the other way, duplicates the first block
//    of the body into the preheader and the function comes out 174 bytes
//    too long at 39%. That single change is worth 14 points.
//  * The roll and pitch arguments are `(h0+h1)/2` and `(h2+h3)/2`, not
//    `/2/2` twice. The original has three cdq/sub pairs and three sars for
//    the whole tail; `/2/2` on each half makes MSVC 5 emit six.
//  * The 64 bit part has to be one expression. Written as three statements
//    on an `__int64 l`, MSVC 5 keeps the variable live and stores its high
//    dword (an extra `mov [esp+..], edx`); folded into
//    `2 - (int)((((__int64)q << 16) / s) * 2 >> 16)` the value is dead
//    after the low half is taken. 58.6% to 62.3%.
//  * `p` is a `short`, not an int. `short p = (short)(... + u->fix_lo)` is
//    the only spelling that gives the original's 16 bit `add ax, [u+0xaa]`
//    followed by a plain `mov [esp+0x28], eax`: the int versions either
//    sign-extend the addend first or emit a `movsx` after it. 62.3% to
//    63.5%.
//  * The second tile is `tb + g_game->gridW * 13` rather than `tb + gw * 13`
//    (gw is a local), and hz is computed before wx. Together these are what
//    bring the function to within one byte of the original's 857.
//  * `u->type->sight / 2`, not `abs(sight) / 2`. The cdq/sub pair at
//    0x48a695 is MSVC 5's signed /2 with its truncation fixup, deferred
//    past the `sar esi,1` at 0x48a69d; abs() would be cdq/xor/sub.
//  * The heading's second argument is `abs(pts[0].x - pts[1].x) >> 16`
//    and the pitch's is `abs(pts[0].z - pts[3].z) >> 16`, both with no /2,
//    and both `abs()` (cdq/xor/sub, guide's abs note) rather than a
//    hand-written test.
//
// The frame model, read off the disassembly and confirmed by the offsets
// our code now uses: locals run from esp+0x10 to esp+0x8b, with k at
// +0x10, fz (reused as the max() temporary) at +0x14, the corner-array
// pointer at +0x18, the height-record pointer at +0x1c, the spilled row
// and map-info pointers at +0x20 and +0x24, two scratch dwords at +0x28
// and +0x2c, hz at +0x30, the {x,z} pair handed to 0x4b7173 at +0x34, the
// four 8-byte corner positions at +0x3c and the four 12-byte height
// records at +0x5c. The height record is {wx, h, spare} and its .spare is
// hz, not junk: +0x30 is written at 0x48a54e (with the two call arguments
// still on the stack, so that instruction's esp+0x38 is esp+0x30) and read
// back at 0x48a736. The epilogue reads .h at +0x60, +0x6c, +0x78, +0x84.

//
// deepseek-v4.1 (1208): re-checked from the 77.8% partial; no source shape found that fixes the
// remaining hunks. What still differs, and what was tried: (1) the prologue register roles (m in edx
// and count in ecx in the original, m in ecx and count in edx here) and everything downstream of
// them (the two lea/pointer temps, the pts pointer ecx-vs-edx, H vs sea in the max block, the
// add eax,ecx operand order) cascade from that one allocation choice; retried with maps held in a
// named local, with row computed via m->rows[m->count], and with the pair declared in both orders,
// all scored the same or lower. (2) The p computation: the original keeps `and eax,0x1f` on the
// random value and pushes [esp+0x28] with a plain 32 bit load; declaring p as short with
// FUN_004b7123's first parameter short drops the movsx (856 bytes, 77.0%) but the mask stays dead,
// and int p (75.1%), a separate rand temp (66.4%) and a split p = (short)(p + fix_lo) statement
// (75.8%) all score lower than the present 77.8%. (3) Moving the n = frame - age statement back
// after the 64 bit division, with and without a cached owner pointer, drops to 73.9% even though the
// original schedules it there.
//
// deepseek-v4.1 (1208), second pass: re-scored the whole diff and the mask/movsx hunk. The original's
// p pair (`and eax,0x1f`, then a 16 bit `add ax, [u+0xaa]`, then a dword store and a plain dword
// reload) cannot be produced by aliasing p: `union { int i; unsigned short s; } p; p.i = ...;
// p.s += u->fix_lo;` and `*(unsigned short*)&p += u->fix_lo;` both give 867 bytes at 71.9 (taking p's
// address forces extra spills), so the mask stays dropped here. wx-before-hz scores 76.3 alone and
// 72.5 with n moved after the 64 bit block, so the present order (hz then wx, n before q, which keeps
// q = owner->sight and n = frame - age in the same blocks the original schedules them) stays best.
// Still differs: the m/count ecx/edx swap in the prologue and everything downstream of it, the
// dropped mask, and the p reload spelled movsx instead of a plain dword load.
//
// Sonnet 5.5 retry (#1091), 70.8% to 77.8%: a hill climb over statement positions (moving one
// statement of the loop body or of the sea-level block at a time, with every statement that
// touches the same local kept in order and no statement crossing the call it depends on) found two
// moves worth 3 points each: `int gw = g_game->gridW;` goes before the two bounds checks, and
// `unsigned n = g_game->frame - u->owner->age;` goes up next to `p`. What still differs, in
// order of the diff: (1) m and count swap ecx/edx in the prologue (original: m in edx, count in
// ecx; int n / row-first / return-first spellings did not flip it); (2) the original computes wx
// before hz (edx = t.x + posx, ebp = wx, then hz in ecx) but every wx-first order scores lower
// overall; (3) the original keeps `and eax, 0x1f` on the random value and passes p to
// FUN_004b7123 with a plain `mov eax, [esp+0x28]; push eax`, where this build drops the mask (the
// short cast makes it dead) and emits `movsx eax, word ptr [esp+0x28]`; short, unsigned short and
// int spellings of p and of that callee's first parameter all fail to give the pair;
// (4) u->owner is cached in ebp after the call in the original.
// deepseek-v4.1-flash retry (10 min timebox): confirmed 77.8% is the wall. Tested and all tied or lost:
// (a) index variable (ushort and uint) before the map lookup, maps array in a named local, address-of
// form, reference form, m declared first, count kept in a local int: m stays in ecx and count in edx,
// 77.8% or lower. The m-in-edx assignment is a pure allocator tie-break that nothing in the source
// shape flipped; every downstream hunk (pts pointer, wx/hz order, H vs sea registers, operand order)
// cascades from it. (b) p as two statements (short p = ...; p += u->fix_lo), p as int with a
// truncating cast, p as unsigned short, and the callee first parameter as short: 75.8/75.1/67.5/77.0.
// The original's kept `and eax,0x1f` plus a 16-bit `add ax,[u+0xaa]` plus a plain dword reload can
// only come from a 32-bit variable whose low half is added in place, which needs an address-taken
// union and that spills (71.9%, tried before). (c) Moving n after the 64-bit division to match the
// original schedule drops to 73.9/73.6 even with the owner cached in a local. What still differs:
// items 1 and 3 of the first comment (prologue m/count ecx/edx swap and its cascade, the dropped
// mask and the movsx reload of p).
// deepseek-v4.1-flash (#4087, 10 min timebox): byte-flat at 77.8% / 857 bytes.
// Splitting the head pair into `MapRow* row = m->rows; row += m->count;` does not
// move the m-in-ecx / count-in-edx allocator tie-break or any downstream hunk.
#include <stdlib.h>

#pragma pack(push, 1)

struct MapVertex_0048a490 {
    int x;                              // +0x0
    int y;                              // +0x4
    int z;                              // +0x8
};

struct MapRow_0048a490 {
    char unknown_0[0xc];
    unsigned short* ids;                // +0xc
    char unknown_10[0x20 - 0x10];
};

struct MapInfo_0048a490 {
    char unknown_0[0xc];
    int count;                          // +0xc
    char unknown_10[0x24 - 0x10];
    MapVertex_0048a490* verts;           // +0x24
    MapRow_0048a490* rows;               // +0x28
};

struct UnitType_0048a490 {
    char unknown_0[0x192];
    int sight;                          // +0x192
    char unknown_196[0x241 - 0x196];
    unsigned int flags;                 // +0x241
};

struct PlayerRec_0048a490 {
    char unknown_0[0x20];
    int sight;                          // +0x20
    char unknown_24[0x2a - 0x24];
    int age;                            // +0x2a
};

struct Pos2_0048a490 {
    int x;
    int z;
};

struct Hs_0048a490 {
    int wx;                             // +0x0
    int h;                              // +0x4
    int spare;                          // +0x8
};

struct Unit {
    PlayerRec_0048a490* owner;           // +0x0
    char unknown_4[0x64 - 4];
    unsigned short hdg;                 // +0x64
    unsigned short aim;                 // +0x66
    unsigned short pitch;               // +0x68
    int posx;                           // +0x6a
    unsigned short posy_lo;             // +0x6e
    unsigned short roll;                // +0x70
    int posz;                           // +0x72
    char unknown_76[0x92 - 0x76];
    UnitType_0048a490* type;            // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short map;                 // +0xa6
    char unknown_a8[0xaa - 0xa8];
    unsigned short fix_lo;              // +0xaa
    char unknown_ac[0x110 - 0xac];
    unsigned int flags;                 // +0x110
};

struct Game {
    char unknown_0[0x14233];
    int gridW;                          // +0x14233
    int gridH;                          // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* hmaps;               // +0x14287
    char unknown_1428b[0x14377 - 0x1428b];
    MapInfo_0048a490** maps;            // +0x14377
    char unknown_1437b[0x38a47 - 0x1437b];
    int frame;                          // +0x38a47
};

#pragma pack(pop)

extern Game* g_game;

unsigned int FUN_004b6340();
int __cdecl FUN_004b7123(short a, int b);
int __cdecl FUN_004b715a(int x, int y);
void __cdecl FUN_004b7173(unsigned short deg, Pos2_0048a490* p);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// Samples the ground under a unit at its four surrounding terrain vertices and
// stores the resulting pitch (0x68) and roll (0x70) on the unit, plus a heading
// (0x64) from the two side vertices. The 0x11/0x04 bytes of a heightmap tile are
// the two half heights of its edge pair, and the corner heights are bilinearly
// interpolated with a plain / 16 (MSVC 5 spells that cdq/and 0xf/add/sar 4;
// there is no second shift). The sea-level block then adds a jitter that fades
// out over 60 frames.
// FUNCTION: 0x48a490
void __stdcall FUN_0048a490(Unit* u)
{
    MapInfo_0048a490* m = g_game->maps[u->map];
    MapRow_0048a490* row = m->rows + m->count;
    if (m->count < 0)
        return;
    {
        int gw, c0;
        Pos2_0048a490 t;
        Pos2_0048a490 pts[4];
        Hs_0048a490 hs[4];
        int k;
        for (k = 0; k < 4; k++) {
            MapVertex_0048a490* v = m->verts + row->ids[k];
            int vx = v->x;
            pts[k].x = t.x = vx;
            int vz = v->z;
            t.z = vz;
            pts[k].z = vz;
            FUN_004b7173(u->aim, &t);
            int wx = (short)((t.x + u->posx) >> 16);
            int hz = (short)((u->posz - t.z) >> 16);
            int fx = wx & 0xf;
            unsigned gz = (unsigned)hz >> 4;
            int fz = hz & 0xf;
            gw = g_game->gridW;
            unsigned gx = (unsigned)wx >> 4;
            if (gx >= gw - 1)
                return;
            if (gz >= g_game->gridH - 1)
                return;
            unsigned char* tb = g_game->hmaps + (gz * gw + gx) * 13;
            int b0 = tb[4];
            unsigned char* tb1 = tb + g_game->gridW * 13;
            int b1 = tb1[4];
            c0 = tb[0x11];
            int c1 = tb1[0x11];
            int H0 = b0 + ((c0 - b0) * fx) / 16;
            int H1 = b1 + ((c1 - b1) * fx) / 16;
            hs[k].wx = wx;
            if ((u->type->flags & 0x1000) && (u->flags & 0x10000000)
                && !(u->flags & 0x4000)) {
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
                int H = hs[k].h;
                int sea = g_game->seaLevel;
                hs[k].h = max(H, sea);
                short p = (short)(((FUN_004b6340() & 0x1f) + k * 8) * 2048 + u->fix_lo);
                int s = u->type->sight / 2;
                PlayerRec_0048a490* o = u->owner;
                int q = o->sight;
                if (q >= s)
                    q = s;
                int w = (int)((((__int64)q << 16) / s));
                int mm = 2 - (int)((((__int64)w * 2) >> 16));
                unsigned int n = g_game->frame - o->age;
                mm -= (unsigned int)(mm * (n < 60 ? n : 60)) / 60;
                hs[k].h = FUN_004b7123(p, mm) + hs[k].h;
            } else {
                hs[k].h = H0 + ((H1 - H0) * fz) / 16;
            }
            hs[k].spare = hz;
        }
        int h1 = hs[1].h;
        int h0 = hs[0].h;
        int h2 = hs[2].h;
        int h3 = hs[3].h;
        int a = (h0 + h1) / 2;
        int b = (h2 + h3) / 2;
        u->roll = (a + b) / 2;
        u->pitch = FUN_004b715a(b - a, (short)(abs(pts[0].z - pts[3].z) >> 16));
        u->hdg = FUN_004b715a(h0 - h1, (short)(abs(pts[0].x - pts[1].x) >> 16));
    }
}
