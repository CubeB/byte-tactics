// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// RETRY deepseek-v4.1-flash (2nd pass, timeboxed): 80.6 -> 80.8 (1521 vs 1519).
// THE ONE WIN THIS PASS: `inner = 0;` written as its own statement before
// `accum = 0;` (instead of leaving the zero to the for-init clause, which put
// accum=0 first). That fixed the loop-head store order to the original's
// `mov [esp+0x18],esi` (inner) then `mov [esp+0x1c],esi` (accum). The jle still
// sits after both stores here, the original has it between them.
// Also confirmed by reading the loop body: [esp+0x10] outer, [esp+0x14] &grid1
// spill, [esp+0x18] inner (`imul eax,[esp+0x18]`), [esp+0x1c] accum
// (`mov edx,[esp+0x1c]; sub edx,cellval>>1`), [esp+0x20] t20, [esp+0x24] t24;
// p1 is ESI and p2 is EDI inside the big loop (both registers, no slots).
// Declaring h1/w1/grid1 in the order h1, w1, grid1 shrinks the function to 1518
// bytes but recolors grid1 into edi and scores 78.4; h1, grid1, w1 is 80.8
// (byte-identical to this file); grid1, w1, h1 is 80.6.
// The duplicated `xor esi,esi` is a C2 block-placement choice, not a source
// shape: the shared zero at the grid1 ternary merge is rematerialised at the
// END of both arms (killing the old esi = count1 value there), while the
// original kills it once at the merge. Three new spellings this pass:
//   shared local + if/else (`unsigned char* c1 = 0; if (count1 != 0) c1 =
//   new ...; grid1->cells = c1;`): 1520 bytes, 79.1. It suppresses the second
//   `xor esi,esi`, but only because the already-live zero in edi (p1's later
//   0) is reused instead, so the guard becomes `cmp ecx,edi` and esi is gone.
//   `grid1->cells = 0; if (count1 != 0) grid1->cells = new ...;`: 1520, 78.8.
//   named local holding the ternary result (`unsigned char* c1 = count1 != 0
//   ? new ... : 0; grid1->cells = c1;`): 1521, 80.6, byte-identical to the
//   current best.
// New evidence from the disassembly: the original's merge sequence is
// `mov ecx,[ebx+0xc]` (reload of field_c, must precede the store because the
// store may alias ebx+0xc) / `xor esi,esi` / `mov [ebx],eax` / `xor eax,eax` /
// `cmp ecx,esi`, and esi == 0 then stays LIVE all the way through the big
// loop's head (`cmp [ebp+0x14233],esi`, `cmp eax,esi`, the two p1/p2 stores),
// which is why the original never rematerialises it. In ours the zero dies at
// the fill loop's guard and is born again after the outer-loop jle
// (`jmp` + `xor esi,esi`), so MSVC treats it as rematerialisable and sinks the
// definition into both arms. Fixing the esi liveness (or the earlier
// prologue's h1/w1 = ebp/esi split, original h1 = esi / w1 = edi) is what the
// remaining hunks reduce to; every hunk past the grid1 block is only the
// 2-byte jump-target shift.
// RETRY deepseek-v4.1-flash (timeboxed pass, no new variants run): 79.5 holds.
// Diff analysis for the next attempt: (1) the duplicated xor esi,esi is p1's
// zero sunk into BOTH arms of the grid1 alloc ternary; the original
// materialises it once at the merge (between mov ecx,[ebx+0xc] and
// mov [ebx],eax), so the source needs the first use of that zero strictly
// after the merge. (2) The fourth border loop in the original keeps the index
// in eax and computes the next index in ecx at the body top
// (lea ecx,[eax+1], used as the row multiplier, then mov eax,ecx at the
// latch); spell it as `for (i = 0; i < h; ) { k = i + 1; ...cells[k*w - 1]...;
// i = k; }` to get that shape. TRIED by deepseek-v4.1-flash: that exact
// spelling scores 77.2 (1516 bytes, 5 bytes SHORTER than the original), so the
// latch form is wrong too even though it moves the index into eax; the
// `lb4 + 1 < height` guard form stays. (3) Both max/min fill loops in the original
// share one shape: prev copied to bl just before the second cell load clobbers
// its register (row: prev in cl, m reuses cl; col: prev in dl, m reuses dl),
// with res selected into a third register; a plain `prev = m` carry keeps the
// cross-loop coalescing that puts m in prev's own register. (4) grid2 setup
// register rotation: original keeps b (field_14227) in edi and a (field_14223)
// in eax, with the delete-arg load of grid2->cells interleaved between the two
// field loads.
// deepseek-v4.1-flash timeboxed retry: 79.5 -> 80.6 (1521 bytes, still +2 vs
// the original). The win is the big accum loop's latch order: writing
// `accum += 0x10;` as the second for-increment
// (`for (inner = 0; inner < g_game->height; inner++, accum += 0x10)`, body
// unchanged) makes VC5 emit `inc eax; add edx,0x10; cmp; mov store/mov store;
// jl`, the original's order. What still differs: the duplicated `xor esi,esi`
// rematerialised in the grid2 allocation ternary's arm (2 bytes, which also
// shifts every later jump target by 2), plus the usual slot/register pair
// swaps. Swept this session, all neutral or worse: `outer = 0` as a statement
// before its loop (80.6), `int outer = 0;` at its declaration (80.6), swapping
// the grid2 ternary arms with `count2 == 0 ? 0 : new` (80.6, same size), a
// fresh `outer2` for the big loop (76.4), `grid2->cells = 0; if (count2) ...`
// (77.9), and declaring `outer` in the for-init (C2374, MSVC5 for-scope).
// RETRY deepseek-v4.1-flash: 76.8 -> 79.5. The 0x10/0x14/0x18/0x1c stack
// permutation IS source-reachable after all: use ONE `int outer;` for BOTH the
// column-max loop counter and the big loop's outer counter (column pass:
// `for (outer = 0; (unsigned int)outer < (unsigned int)grid2->width; outer++)`)
// and declare it FIRST, before `unsigned char* cells2`. The counter then lands
// in [esp+0x10] and cells2 in [esp+0x14], exactly the original; function-scope
// `int inner; int accum;` after cells2 take 0x18/0x1c. Only residual slot
// issue: ours gives accum=0x18 and inner=0x1c, the original the reverse;
// neither declaration order, assignment order, nor a do/while rebuilt from
// Ghidra flips it. The 2 extra bytes vs the original come from a duplicated
// `xor esi,esi` around the grid1 allocation ternary merge.
// PARTIAL: 79.5% (1521 vs 1519 bytes). Layout follows the matched 0x41d920:
// grid1 at +0x1428f and grid2 at +0x1429f (the +0x10/+0x14 words are the
// separate g_game fields 0x142af/0x142b3). The function allocates both
// passability grids, marks their borders, propagates row and column maxima
// twice, then fills the smoothed values.
// 76.6% -> 76.8% (deepseek-v4.1-flash retry): the scan loop's pointer walk
// must be in the FOR-INCREMENT clause, `for (x = 0; x < width; x++, cellp +=
// 0xd)`, not a separate `cellp += 0xd;` statement at the end of the body. The
// original emits `inc ecx` (x++) BEFORE `add ebx, 0xd` (cellp++), which only a
// comma in the increment clause produces; the body statement gives the reverse
// order. That one fix removed the whole passability-pass diff block.
// 75.5% -> 76.6%: the FIRST row-max loop must NOT go through a separate
// `res` local. `if (prev < m) prev = m; p[1] = prev; prev = m;` (updating prev
// in place and storing it) is worth 3.1 points. The SAME rewrite applied to
// the second, column-max loop scores 73.7, so the two sites genuinely differ
// and only the first one takes it.
// 73.2% -> 73.5%: `grid1->cells = count1 ? new(count1*2) : 0;` as one
// ternary statement, and the fourth border loop written `lb4 + 1 < height`
// instead of `lb4 < height`.
// 64.1% -> 73.2%: the passability pass re-applies q to the freshly assigned
// p1/p2 cells inside the valid-range branch, not just to the cells carried
// over from the previous row. Adding `p1[0] = max(p1[0], q); p1[1] =
// min(p1[1], q);` (and the p2 twin) after each pointer assignment restored
// the original's second max/min pair. That also released the register
// pressure that had spilled `cellval` to the stack, so the frame is now the
// original 0x18 and `idiv` uses ebp.
// What still differs (all confirmed with check.py --sym, 1521 vs 1519 bytes):
//  - accum vs inner slot: original [0x18] = inner (the big loop counter) and
//    [0x1c] = accum; ours is accum=[0x18], inner=[0x1c] (see the retry note).
//    This is the one slot pair I could not move.
//  - the outer loop carries an extra `jmp` + `xor esi,esi` at its head where
//    the original zeroes p1 (esi) at the inner loop's latch. Writing an
//    explicit `p1 = 0;` at the end of the inner body reproduces the latch
//    position but collapses the whole loop (76.6 -> 54.1), so the original
//    does not have that statement in any spelling I found.
//  - `accum` is memory resident in the original ([esp+0x1c], loaded and
//    stored each iteration); in ours it round-trips through ebx. The original
//    has all eight registers live in the inner body (g_game=ebp, p1=esi,
//    p2=edi, cellval=ebx, q=eax, block=ecx, v=edx, plus accum), so it had no
//    register to keep it in. Adding a live reference to force that spill is
//    the obvious next lever.
//  - the two max/min fill loops keep the same instruction sequence but with
//    `prev` in dl where the original has it in cl with a bl copy of it.
//  - the grid2 setup swaps which of field_14227 / field_14223 lands in edi
//    versus eax, and the grid1 setup loads &grid1 into ecx where the original
//    uses ebx.
// Tried and rejected (all scored worse, do not repeat):
//  - hoisting `p1`/`p2` out of the outer loop to function scope: 73.2 -> 53.4.
//  - `p1 = 0;` at the end of the inner body: 76.6 -> 54.1.
//  - applying the no-`res`-local rewrite to the SECOND (column) max loop as
//    well as the first: 76.6 -> 73.7.
//  - reading h2/w2 back out of grid2->field_14/field_10 instead of keeping
//    the locals: 76.6 -> 70.7.
//  - declaring `cells2` inside the block that uses it: 76.6 -> 69.5.
//  - a named local for the `operator delete` argument: neutral.
//  - reordering the big loop's declarations (p1/p2/accum before t20/t24):
//    neutral at 76.6.
//  - `prev <= m` instead of `prev < m` in the first max loop: neutral.
// deepseek-v4.1-flash retry, also tried and rejected (do not repeat):
//  - headers.py over all 128 header sets: none beats 76.8; the source shape, not
//    compiler state, is what differs.
//  - the windows `max(a,b)` macro form for either max loop: 76.6 -> 73.7 (row)
//    and 67.3 (both). The `if (m <= t) m = t; if (prev < m) prev = m;` shape is
//    right, only the cl/bl/dl rotation differs.
//  - grid1 alloc as an `if (count1) ... else grid1->cells = 0;` instead of the
//    ternary: 76.3. The 2 extra bytes are the duplicated `xor esi,esi` that the
//    ternary emits in both arms; the original materialises the zero once after
//    the merge.
//  - swapping the grid2 `a`/`b` declaration order: 76.6, 1524 bytes.
//  - swapping the grid1 `h1`/`w1` declaration order: 76.6.
//  - hoisting all six big-loop locals (`d`, `cells2`, `inner`, `accum`, `t20`,
//    `t24`) to function scope in the order that reproduces the original's
//    ascending slots: the permutation does NOT move, still 76.8. MSVC's stack
//    allocator is not following declaration, first-store or first-use order
//    here, so the 0x10/0x14/0x1c three-cycle is not source-reachable this way.
//  - the fourth border loop as `i < height` with `(i+1)` (76.4), with a
//    separate `unsigned k = i + 1;` (76.6), or as `k = 1; k <= height` (76.4).
//    The original keeps the loop index in eax and reuses `ecx = i + 1` as the
//    next index; all three spellings leave the index in ecx.
#include <new.h>
#include <windows.h>

#pragma pack(push, 1)

struct Grid_482c20 {
    unsigned char* cells;               // +0x0
    int width;                          // +0x4
    int height;                         // +0x8
    int field_c;                        // +0xc
};

struct Grid2_482c20 {
    unsigned char* cells;               // +0x0
    int width;                          // +0x4
    int height;                         // +0x8
    int field_c;                        // +0xc
    int field_10;                       // +0x10
    int field_14;                       // +0x14
};

struct Rec_482c20 {
    Rec_482c20() { a = 0; b = 0; flags = 0; field_6 = 0; }
    unsigned char a;                    // +0x0
    unsigned char b;                    // +0x1
    int flags;                          // +0x2
    int field_6;                        // +0x6
};

struct Game_482c20 {
    char unknown_0[0x14223];
    int field_14223;                    // +0x14223
    int field_14227;                    // +0x14227
    char unknown_1422b[0x14233 - 0x1422b];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char field_1427f;          // +0x1427f
    char unknown_14280[0x14287 - 0x14280];
    unsigned char* cells_14287;         // +0x14287
    char unknown_1428b[0x1428f - 0x1428b];
    Grid_482c20 grid1;                  // +0x1428f
    Grid2_482c20 grid2;                 // +0x1429f
    Rec_482c20* field_142b7;            // +0x142b7
};

#pragma pack(pop)

extern Game_482c20* g_game;

// FUNCTION: 0x482c20
void FUN_00482c20(void)
{
    Rec_482c20* rec = new Rec_482c20;
    g_game->field_142b7 = rec;
    g_game->field_142b7->flags = 0x1f;

    Grid2_482c20* grid2 = &g_game->grid2;
    int b = g_game->field_14227 * 0x10000;
    int a = g_game->field_14223 * 0x10000;
    grid2->field_14 = b;
    grid2->field_10 = a;
    int h2 = (b + 0x7fffff) >> 0x17;
    int w2 = (a + 0x7fffff) >> 0x17;
    grid2->width = w2;
    grid2->height = h2;
    operator delete(grid2->cells);
    int count2 = (h2 * w2 + 7) & 0xfffffff8;
    grid2->field_c = count2;

    grid2->cells = count2 != 0 ? (unsigned char*)new Rec_482c20[count2] : 0;
    int outer;
    unsigned char* cells2 = grid2->cells;
    unsigned char* cellp = g_game->cells_14287;
    int inner;
    int accum;

    for (unsigned int lb1 = 0; lb1 < (unsigned int)grid2->width; lb1++)
        ((Rec_482c20*)grid2->cells)[lb1].flags |= 1;
    for (unsigned int lb2 = 0; lb2 < (unsigned int)grid2->width; lb2++)
        ((Rec_482c20*)grid2->cells)[(grid2->height - 1) * grid2->width + lb2].flags |= 2;
    for (unsigned int lb3 = 0; lb3 < (unsigned int)grid2->height; lb3++)
        ((Rec_482c20*)grid2->cells)[lb3 * grid2->width].flags |= 4;
    for (unsigned int lb4 = 0; lb4 + 1 < (unsigned int)grid2->height; lb4++)
        ((Rec_482c20*)grid2->cells)[(lb4 + 1) * grid2->width - 1].flags |= 8;

    for (int d = 0; d < grid2->field_c; d++)
        grid2->cells[d * 10] = g_game->field_1427f;

    {
        for (int y = 0; y < g_game->height; y++) {
            unsigned char* recp = cells2;
            for (int x = 0; x < g_game->width; x++, cellp += 0xd) {
                if (cellp[5] > recp[0])
                    recp[0] = cellp[5];
                if ((x & 7) == 7)
                    recp += 10;
            }
            if ((y & 7) == 7)
                cells2 += grid2->width * 10;
        }
    }

    for (unsigned int r = 0; r < (unsigned int)grid2->height; r++) {
        unsigned char* p = (unsigned char*)grid2->cells + r * grid2->width * 10;
        unsigned char prev = 0;
        for (unsigned int fr = 1; fr < (unsigned int)grid2->width; fr++) {
            unsigned char t = p[10];
            unsigned char m = p[0];
            if (m <= t)
                m = t;
            if (prev < m)
                prev = m;
            p[1] = prev;
            prev = m;
            p += 10;
        }
        p[1] = prev;
    }

    for (outer = 0; (unsigned int)outer < (unsigned int)grid2->width; outer++) {
        unsigned char* p = (unsigned char*)grid2->cells + outer * 10;
        unsigned char prev = 0;
        for (unsigned int gc = 1; gc < (unsigned int)grid2->height; gc++) {
            unsigned char t = p[grid2->width * 10 + 1];
            unsigned char m = p[1];
            if (m <= t)
                m = t;
            unsigned char res = prev;
            if (prev <= m)
                res = m;
            p[1] = res;
            prev = m;
            p += grid2->width * 10;
        }
        p[1] = prev;
    }

    Grid_482c20* grid1 = &g_game->grid1;
    int h1 = g_game->height / 2;
    int w1 = g_game->width / 2;
    grid1->width = w1;
    grid1->height = h1;
    operator delete(grid1->cells);
    int count1 = (h1 * w1 + 7) & 0xfffffff8;
    grid1->field_c = count1;
    grid1->cells = count1 != 0 ? (unsigned char*)operator new(count1 * 2) : 0;
    for (int hf = 0; hf < grid1->field_c; hf++) {
        grid1->cells[hf * 2] = 0;
        grid1->cells[hf * 2 + 1] = 0xff;
    }

    for (outer = 0; outer < g_game->width; outer++) {
        int t20 = (outer - 1) >> 1;
        int t24 = outer >> 1;
        unsigned char* p1 = 0;
        unsigned char* p2 = 0;
        inner = 0;
        accum = 0;
        for (; inner < g_game->height; inner++, accum += 0x10) {
            int idx = g_game->width * inner + outer;
            int cellval = g_game->cells_14287[idx * 0xd + 4];
            int v = accum - (cellval >> 1);
            int block = v >> 5;
            if (block > -1) {
                int q = ((block * 32 + 31) * cellval) / (v + 31);
                if (p1) {
                    p1[0] = max(p1[0], q);
                    p1[1] = min(p1[1], q);
                }
                if (p2) {
                    p2[0] = max(p2[0], q);
                    p2[1] = min(p2[1], q);
                }
                if ((unsigned int)t20 < (unsigned int)grid1->width
                        && (unsigned int)block < (unsigned int)grid1->height) {
                    p1 = grid1->cells + (block * grid1->width + t20) * 2;
                    p1[0] = max(p1[0], q);
                    p1[1] = min(p1[1], q);
                } else {
                    p1 = 0;
                }
                if (t20 != t24
                        && (unsigned int)t24 < (unsigned int)grid1->width
                        && (unsigned int)block < (unsigned int)grid1->height) {
                    p2 = grid1->cells + (block * grid1->width + t24) * 2;
                    p2[0] = max(p2[0], q);
                    p2[1] = min(p2[1], q);
                } else {
                    p2 = 0;
                }
            }
            if (p1) {
                p1[0] = max(p1[0], cellval);
                p1[1] = min(p1[1], cellval);
            }
            if (p2) {
                p2[0] = max(p2[0], cellval);
                p2[1] = min(p2[1], cellval);
            }
            ;
        }
    }

    int def = g_game->field_1427f;
    for (int jf = 0; jf < grid1->field_c; jf++) {
        unsigned char* p = grid1->cells + jf * 2;
        int aa = p[0];
        int bb = p[1];
        int q1 = (aa * 2 + bb) / 3;
        int q2 = (aa + bb * 2) / 3;
        if (q1 <= def)
            q1 = def;
        p[0] = q1;
        if (q2 <= def)
            q2 = def;
        p[1] = q2;
    }
}
