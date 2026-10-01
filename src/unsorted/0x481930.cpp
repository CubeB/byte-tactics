// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, re-tried by
// deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited
// by deepseek-v4.1, edited by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// Space Bunny Free. Names are provisional.
// MATCH, from 83.3%. Four changes did it, and the last one is the interesting
// one: the whole fix is about WHERE the locals sit in the function's single
// allocation table, not about their order relative to the code that uses them.
//  1. THE INNER LOOP IS THE MATCHED SIBLING 0x481d50'S LOOP, VERBATIM.  That
//     function MATCHes and its loop is the guarded do-while with `int j1;`
//     DECLARED BEFORE `int bestDiff = -1;` and `j1 = 1;` ASSIGNED inside the
//     guard.  The earlier notes here tried the do-while with `int j1 = 1;`
//     after bestDiff (79.1%) and concluded the `bestDiff * j1` imul operand
//     order was not worth chasing.  It is: 0x481d50's declaration order
//     (bestIdx, j1, bestDiff, j) fixes that operand order too, 79.1 -> 79.7.
//     The rule its header records holds here: MSVC takes the imul REGISTER
//     operand from declaration order, so the variable declared first is the one
//     loaded into eax.  Neither half helps alone.
//  2. THE ELSE BRANCH'S LOCALS MUST BE DECLARED AT THE TOP OF THE FUNCTION AND
//     ASSIGNED IN THE BRANCH, not initialised in place.  That took the size
//     from 1055 back to 1052.  So the 3 bytes the do-while appeared to cost are
//     not a cost of the loop: they are paid only while the else branch's
//     locals are initialised in place.
//  3. x AND y MUST BE IN THAT TOP BLOCK TOO.  This is the big one and it had
//     never been tried: adding `int x, y;` to the same declaration block and
//     assigning them after bit/halfW/halfH took the score from 82.1 to 92.3 and
//     126 diff lines to 53.  Putting x and y in the function's single local
//     table is what finally gives the allocator the register pressure shape the
//     original has, and with it BOTH long-standing problems disappear at once:
//     the first visibility cell's in-place `imul edi, [esp+0x10]` (the product
//     lands in halfW's own register) with the 5-byte `mov eax, [g_game]`, and
//     every jump target after it.  This is the "add or remove one local from
//     the allocator's table" lead the earlier notes kept circling; the real form
//     of it is "put x and y in the table too".
//  4. THE LAST 7.7 POINTS WERE `changed` MERGED INTO `int x, y, changed = 0;`
//     AND `frame` MOVED DOWN PAST `bit`.  Both are pure declaration moves and
//     both are needed: the exact position of `frame` in the table is what puts
//     it in grid's slot (esp+0x24) with the LOS frame pointer in ecx instead of
//     its own slot with the pointer in edx.  Before this the two branches'
//     locals shared slots in the wrong pairing (`grid+limitX`, `frame+?`) and
//     every reload in the else branch moved with it; with it they share the
//     original's six pairs.  Found by tools/permute.py as two `move_decl`
//     mutations off the 92.3% base, then written up here by hand.
// The order inside the top block is otherwise free, but `x, y` must come
// before it, and `frame` must come after `bit`.  Dropping x or y from the block
// drops the score to 82.1; `changed` may sit before or after `int x, y;`.
// One more thing is load bearing and looks wrong: in the row loop the
// `dst` declaration must come BEFORE the `src` one, the reverse of the order
// the values are used in.  Swapping the two back costs 7.7 points, because the
// visibility-mask pointer is loaded into the register the frame pointer wants
// and MSVC has to keep both live one instruction longer.  This is the same
// `dst++; src++;` order the earlier notes recorded for the two pointer bumps,
// and it is the same reason: the statement order is what decides which
// register is free when.
// <windows.h> is required.
#include <windows.h>
#include <windows.h>

#pragma pack(push, 1)

class Class_00433500 {
public:
    void* FUN_00433500(int n);
};

class Class_00433520 {
public:
    short FUN_00433520();
};

class Class_004335c0 {
public:
    short FUN_004335c0();
};

class Class_4335e0 {
public:
    void* FUN_004335e0(short i);
};

class Class_004339c0 {
public:
    short FUN_004339c0();
};

class Class_004339e0 {
public:
    void FUN_004339e0(short i, unsigned short* a, unsigned short* b);
};

extern char DAT_0051e6a0[];

struct Grid_00481930 {
    unsigned char* cells;              // +0x00
    unsigned int width;                // +0x04
    unsigned int height;               // +0x08
    int field_c;                       // +0x0c
};

struct Player_00481930 {
    char unknown_0[0x7c];
    Grid_00481930 grid;                // +0x7c
    char unknown_8c[0x146 - 0x8c];
    unsigned char field_146;           // +0x146
};

struct Params_00481930 {
    Player_00481930* field_0;          // +0x00
    short* field_4;                    // +0x04
    short field_8;                     // +0x08
    unsigned char field_a;             // +0x0a
    char unknown_b;                    // +0x0b
    unsigned char* field_c;            // +0x0c
    char unknown_10[0xc];              // +0x10
};

struct LosTable_00481930 {
    unsigned short count;              // +0x00
};

struct Frame_00481930 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    char unknown_4[4];                 // +0x04
    unsigned char mask;                // +0x08
    char unknown_9[0x10 - 9];
    unsigned char* data;               // +0x10
};

struct Game_00481930 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x14273 - 0x1423b];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short bit0 : 1;           // +0x14281
    unsigned short bit1 : 1;
    unsigned short flag2 : 1;
    unsigned short flag3 : 1;
    unsigned short rest : 12;
    char unknown_14283[0x1428f - 0x14283];
    Grid_00481930 grid1;               // +0x1428f
    char unknown_1429f[0x142f1 - 0x1429f];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2[0x1485b - 0x142f2];
    LosTable_00481930* losTable;       // +0x1485b
};

#pragma pack(pop)

extern Game_00481930* g_game;

Frame_00481930* __stdcall FUN_004b7f30(LosTable_00481930* table, int index);

inline int LodRaw_00481930(Params_00481930* params)
{
    return params->field_8 / 32;
}

inline int Lod_00481930(Params_00481930* params)
{
    int v = LodRaw_00481930(params);
    return v < 0 ? 0 : v;
}

// FUNCTION: 0x481930
void __stdcall FUN_00481930(Params_00481930* params)
{
    int changed = 0;
    int x, y;
    int limitX, limitY, nx, ny;
    int i, stride, off;
    unsigned int bit = 1 << params->field_0->field_146;
    Frame_00481930* frame;
    int halfW = g_game->width / 2;
    int halfH = g_game->height / 2;
    x = params->field_4[0];
    y = params->field_4[1];
    if (g_game->flag2 == 1) {
        Grid_00481930* grid = &g_game->grid1;
        if ((unsigned)x < grid->width && (unsigned)y < grid->height) {
            void* table =
                ((Class_00433500*)DAT_0051e6a0)
                    ->FUN_00433500(
                        (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32) <
                                ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1
                            ? (params->field_8 / 32 < 0 ? 0 : params->field_8 / 32)
                            : ((Class_00433520*)DAT_0051e6a0)->FUN_00433520() - 1);
            short count = ((Class_004335c0*)table)->FUN_004335c0();
            unsigned short* cell = &g_game->visibilityMask[halfW * y + x];
            if ((unsigned short)(bit & *cell) == 0) {
                *cell ^= bit;
                changed = 1;
            }
            int ref = *params->field_c;
            for (short i = 0; (short)i < count; i++) {
                void* line = ((Class_4335e0*)table)->FUN_004335e0(i);
                short num = ((Class_004339c0*)line)->FUN_004339c0();
                int bestIdx = 0;
                int j1;
                int bestDiff = -1;
                short j = 0;
                if ((short)num > 0) {
                    j1 = 1;
                    do {
                        int y2, x2;
                        ((Class_004339e0*)line)->FUN_004339e0((short)j, (unsigned short*)&x2, (unsigned short*)&y2);
                        x2 += x;
                        y2 += y;
                        if ((unsigned)(short)x2 < grid->width &&
                            (unsigned)(short)y2 < grid->height) {
                            unsigned char* c =
                                grid->cells + ((short)y2 * grid->width + (short)x2) * 2;
                            int d1 = c[1] - ref;
                            int d0 = c[0] - ref;
                            if (d0 * bestIdx > bestDiff * j1) {
                                unsigned short* q = &g_game->visibilityMask[
                                    halfW * (short)y2 + (short)x2];
                                if ((unsigned short)(bit & *q) == 0) {
                                    *q ^= bit;
                                    changed = 1;
                                }
                                if (d1 * bestIdx > bestDiff * j1) {
                                    bestIdx = j1;
                                    bestDiff = d1;
                                }
                            }
                        }
                        j++;
                        j1++;
                    } while ((short)j < (short)num);
                }
            }
        }
    } else {
        int lod = LodRaw_00481930(params) - 5;
        if (lod < 0)
            lod = 0;
        else if (lod >= g_game->losTable->count)
            lod = g_game->losTable->count - 1;
        frame = FUN_004b7f30(g_game->losTable, lod);
        limitX = (x + frame->width < halfW) ? frame->width : halfW - x;
        limitY = (y + frame->height >= halfH) ? halfH - y : frame->height;
        nx = x < 0 ? -x : 0;
        ny = y < 0 ? -y : 0;
        changed = 0;
        i = ny;
        if (i < limitY) {
            stride = halfW * 2;
            off = ((y + ny) * halfW + nx + x) * 2;
            do {
                unsigned short* dst =
                    (unsigned short*)((unsigned char*)g_game->visibilityMask + off);
                unsigned char* src = frame->data + i * frame->width + nx;
                if (nx < limitX) {
                    int n = limitX - nx;
                    do {
                        if (*src != frame->mask && (unsigned short)(bit & *dst) == 0) {
                            changed = 1;
                            *dst ^= bit;
                        }
                        dst++;
                        src++;
                    } while (--n);
                }
                i++;
                off += stride;
            } while (i < limitY);
        }
    }
    if (changed && params->field_0->field_146 == g_game->playerIndex) {
        g_game->flag3 = 0;
        g_game->flags_142f1 |= 4;
    }
}
// NOTES for the next attempt (all of these were tried and did NOT help, so do
// not repeat them):
//  * Naming the clamped lod in a local (`int lod = Lod(params); if (lod < ...)`)
//    does not force the clamp before the call. MSVC 5 still sinks the mask
//    sequence past the call and keeps only the RAW value live in ebp.
//  * The clamp helper spelled `if (v < 0) v = 0; return v;` instead of
//    `v < 0 ? 0 : v` is worse by 3.1: the original really is the ternary, which
//    if-converts to sets/dec/and.
//  * Nesting the two lod helpers (a clamped one calling an unclamped one) makes
//    no difference at all, as the guide's "an inlined boundary is not a CSE
//    boundary" note predicts. Only the caller's spelling matters.
//  * `int j, j1` instead of `short j` for the inner counters: worse by 10.
//  * `int count` / `int num` instead of `short`: worse by 0.3.
//  * Reordering the `bestIdx` / `bestDiff` initialisers: catastrophic, 28.9%.
//  * Hoisting a shared `int rhs = bestDiff * j1;` for the two comparisons
//    changes nothing: the original already reuses the product in eax.
//  * Moving `j1++` to the end of the loop body instead of the top: no change.
//  * Swapping the declaration order of bestIdx / j1 (bestIdx first) and moving
//    j1++ from the body top to the body bottom together: 71.0%, same score but
//    1051 bytes instead of 1047. MSVC5 still gives ebx to j1 and spills bestIdx
//    (`mov ebx, 1` before the num guard, `imul edx, [esp+0x20]`), so the
//    original's ebx-held bestIdx is not reachable by declaration order.
//  * `for (j = 0; j < num; j++, j1++)` (both increments in the for-increment
//    clause, bestIdx/bestDiff/j1 order): 70.7%, worse.
//  The remaining diff is the register allocator's choice of loop-carried
//  candidate for ebx (bestIdx in the original, j1 here) plus the knock-on
//  scheduling of the lod clamp before vs after the FUN_00433520 call.
//  * Wrapping the loop in a bare brace block instead of `if (num > 0)`: needed
//    for fix (2) above; the block itself is otherwise free.
// Tried again on the 81.5% base, all neutral or worse (the score column is
// check.py's, and 81.2 was the base before the pointer-bump fix):
//  * the lod clamp as a named local compared against a fresh helper call in the
//    arms (81.2, no change), both arms using the local (71.8, and the size
//    collapses to 1026 because the second FUN_00433520 call disappears), a
//    separate statement before the `if` (64.4), the count side named too
//    (70.8, size 1010). So no spelling of a local gets the clamp above the
//    call: only dropping the helper altogether does.
//  * the whole min-with-call as its own inline helper (62.8 for the two-armed
//    if/else form, 70.7 for the one with a named local): duplicating the call
//    in both arms of a real `if` is much worse than the ternary here, the
//    opposite of the guide's item 27 for this shape.
//  * `y * halfW + x` and a named `int idx`, `row` or `hw` for the first
//    visibility cell: all exactly neutral, MSVC 5 picks that multiply's
//    destination from register pressure, not from the source's operand order.
//  * the inner loop: `for (j = 0; j < num; j++, j1++)`, `j1++` at the top of
//    the body, one combined declaration, `unsigned int` for the counters,
//    hoisting either side of the comparison into a local, and the reversed
//    comparison `bestDiff * j1 < d0 * bestIdx`: all 81.2, i.e. none of them
//    touch the ebx choice. Declaring j1 before bestIdx is 74.5, moving
//    bestDiff into the middle 80.9, `int j` instead of `short j` 73.8, and
//    `short j1` 76.x: types and declaration order all steer it, none of them
//    onto the original's split.
//  * the else branch: naming fw/fh (59.4), if/else instead of the ternary
//    (75.2), declaring limitY before limitX (79.0), an accessor local for
//    &frame->width, a second `Frame*` alias, `(int)(unsigned short)frame->width`
//    in the clamp, and every parenthesisation of `frame->data + i*w + nx` with
//    a data or width or frame-pointer local: all neutral or worse. The frame
//    pointer keeps landing in edx rather than ecx, and `frame->data` keeps
//    being materialised into a register instead of folded into the final
//    `add ecx, [edi+0x10]`.
//  * writing the two bumps as `*src++ = *src; *dst++ = *dst;`: neutral. The
//    order of the two STATEMENTS is what counts, not the form.
// Re-tried on the 81.5% base by space-bunny-free, ALL neutral (81.5, 122 diff
// lines, exactly as the base), so none of these is the missing construct:
//  * hoisting `int rhs = bestDiff * j1;` for the two comparisons. This was the
//    most promising idea left, because the allocator's choice of ebx looks like
//    a reference-count priority: j1 has 4 refs (2 imul reads, the `mov ebx,
//    [esp+0x1c]` read, and the increment) against bestIdx's 3 (2 imul reads and
//    the write), so j1 wins the callee-saved slot. Cutting j1 down to 2 refs
//    does NOT flip it, which kills the reference-count theory.
//  * `j1++` as the first statement of the inner body (the original's init at
//    0x481abc is below the `jle`, which is what the rotated-loop peel looks
//    like, so the top-of-body spelling seemed worth a test).
//  * `g_game->visibilityMask + halfW * y + x` instead of the indexed form, and
//    `g_game->width` through a local: the first cell's `imul` destination is
//    not reachable from the source's shape at all.
//  * `halfW > x + frame->width` and `halfH > y + frame->height` (swapping the
//    `<` for a `>`, per the guide's item 16, to flip which ternary arm is the
//    fall-through in the limitY clamp): 81.2 each, so the limitY `jl` polarity
//    is not under source control either.
//  * a second `Frame_00481930*` alias used in the inner loop, and
//    `if (limitY > ny)` for the row guard.
// Worse, for the record: declaring j1 before bestIdx (74.8), `changed = 0` at
// the top of the else branch rather than just before the row guard (75.5), one
// combined `int bestIdx = 0, bestDiff = -1, j1 = 1;` (81.2), the reversed
// comparison `bestDiff * j1 < d0 * bestIdx` (81.2), and nx/ny written as
// `if (nx < 0) nx = 0;` after a bare `-x` (57.1).
// What the diff still looks like, for the next attempt: the three regions are
// (a) the first cell's `imul` destination, (b) which of j1/bestIdx inherits
// ebx from `bit`, (c) the else branch's frame pointer, which the original keeps
// in ecx and slot 0x24 and we keep in edx and slot 0x38. (b) and (c) are the
// same slot-allocation story: swapping which variable holds ebx also swaps
// 0x1c and 0x20 for j1 and y2, and swapping the else branch's frame slot
// 0x24/0x38 moves every reload. A construct that moves ALL THREE at once is
// wanted, not three local fixes.
// deepseek-v4.1 swept 37 more shapes against this base. 22 came out
// byte-identical (81.5) and the other 15 were worse, so none of these is the
// missing construct. Neutral: adding
// <stdio.h>, <stdlib.h>, <string.h>, <stddef.h> (any order), `x + halfW * y`
// and `(short)x2 + halfW * (short)y2` in the two visibility cells,
// `x + halfW * y` in both at once, `g_game->visibilityMask + (halfW * y + x)`,
// reversed conditions on both loops, assignment instead of initialisation for
// `frame` and for `j1`, ctor-style `int bestIdx(0)` / `int j1(1)`, j1 declared
// inside the bare block, `int changed` declared last, `unsigned int changed`,
// and a detached `Frame_00481930* frame;` declaration. Worse: `j1 = 1` moved
// inside an `if ((short)num > 0)` wrapper (76.1, adds a second guard test),
// `short j = 0` hoisted above bestIdx's declaration (74.5), x/y declared
// before halfW/halfH (78.3), the cell line moved above the count line (69.7,
// 1046 bytes), `ref` loaded before the cell (73.6), x/y as `short` (67.9,
// 1085), bit declared last (58.4), x/y last (59.7), <math.h> (78.7),
// WIN32_LEAN_AND_MEAN (78.7), and dropping <windows.h> altogether (36.4: the
// file needs it). Conclusion: (a), (b) and (c) are not reachable from the
// source shape of this inner block at all; the next attempt should look for a
// different construct, most likely one that makes bestIdx and j1 be born in
// different extended basic blocks (the original's `j1 = 1` store sits in the
// preheader below the `jle`, ours sits in the guard block with bestIdx), or a
// different source for the outer loop that moves ebx's first free point.
// Re-tried by space-bunny-free on the 83.3% base. Free scratch scoring
// (check.py --sym on build/scratch/0x481930/v*.cpp) makes each of these half a
// second, so the list below is long; every one of them is 83.3%, i.e. exactly
// neutral, so none of them is the missing construct:
//  * the frame pointer: detached at function scope, `const`, a separate
//    declaration line, `frame->data + (i*w + nx)`, a named `frameData` local
//    (75.8, 1066 bytes), `char*` for `src`, `char*` for the destination cast,
//    `visibilityMask + off/2` for the destination (66.0, 1064), and
//    `limitX`/`limitY` as `unsigned int` (82.1). The frame pointer keeps
//    landing in edx and slot 0x38 and limitX in 0x24.
//  * the row loop: `int stride` declared before `int i` (82.4),
//    `halfW << 1` and `2 * halfW` for the stride, `off` as
//    `(y+ny)*(halfW*2) + (nx+x)*2`, `int n = limitX; n -= nx;`, the inner loop
//    as `while (n) { ... n--; }` (66.6, 1062), `int i` for the OUTER counter,
//    `int i; i = ny;` instead of `int i = ny;`, `changed = 0` after the `i`
//    initialisation (83.0), and `!(y + frame->height < halfH)` for the
//    limitY condition.
//  * nx/ny hoisted above the two limit clamps (65.2): the original's order
//    really is limits first, then nx, then ny.
//  * the inner loop: both increments in the for-increment clause
//    (`j++, j1++`) compiles to the original's latch ORDER (`inc ecx` then
//    `inc edx`) where the `j1++` at the body end does not, and is still
//    83.3%, so the latch order is not worth chasing on its own. Also neutral:
//    hoisting `bestDiff * j1` into a local, `bestIdx * d0 > j1 * bestDiff`,
//    `bestDiff * j1 < d0 * bestIdx`, `unsigned int` for j1 / bestIdx / both,
//    a separate `short j` declared before the for, and a `while` form of the
//    inner loop. Worse: `j1` declared before `bestIdx` (75.4),
//    `bestDiff` between them (82.4), `short j1` (63.5), `short bestIdx` (45.6).
//  * the first visibility cell: reading `*cell` into a local first (81.5),
//    `if (!(bit & *cell))` with an explicit store (74.8, 1055 bytes), an
//    `(int)(short)` cast on the index (73.7, 1054), and
//    `g_game->visibilityMask + halfW * y + x` instead of the indexed form.
//    The `imul` destination is still edi in the original and eax here in
//    every spelling tried, including `y * halfW + x` and `x + halfW * y`.
// Two things the earlier notes got wrong, corrected here: (i) the limitY
// ternary IS under source control, it just needs its arms swapped rather
// than its comparison flipped (fix 3 at the top), and (ii) `int i = ny;`
// before the guard IS the original's placement, not a dead store to be sunk
// (fix 4). Both were listed as "not under source control" before.
// Where the next attempt should look: the flag2 branch and the else branch
// share six frame slots (0x20, 0x24, 0x30, 0x34, 0x38, 0x50) and MSVC 5
// ranks ALL of the function's locals in one table, so the two swaps that are
// left (j1/bestIdx over 0x1c/0x20, and frame/limitX over 0x24/0x38) are
// probably one ordering decision, not two. What would be worth trying next
// is a construct that adds or removes one local from the table, or changes a
// live range's length, anywhere in the function, and see whether BOTH swaps
// move together.
// Retried by deepseek-v4.1-flash. Nothing beat 83.3, and the two root swaps
// (frame/limitX over 0x24/0x38, bestIdx/j1 over ebx/0x1c/0x20) did not move,
// so the "add or remove one local" theory is not confirmed by any of these:
// limitX/limitY assignment instead of initialisation, detached limitX/limitY
// or frame at function scope, limitX forward-declared before frame, a hoisted
// `Grid_00481930*` shared by both branches and cast to Frame in the else
// (79.3), j1 replaced by `j + 1` (79.7), the bestIdx/bestDiff/j1 declaration
// order (83.0), j1 initialised to 0 and incremented at the top of the body
// (76.6), `unsigned int bestIdx` (83.3), splitting the first cell index as
// `int idx = y * halfW; idx += x;` (83.3), swapping nx/ny (71.5), and `int i`
// for the flag2 line counter (71.6). All of them leave the frame pointer in
// edx/slot 0x38 and j1 in ebx, so the two swaps really are one allocator
// decision that no source shape tried so far reaches.
// One NEW data point (deepseek-v4.1-flash, scratch v1): the missing construct
// IS the inner loop's shape. Rewriting it as an explicit guard plus a
// do-while,
//     short j = 0;
//     if ((short)num > 0) {
//         int j1 = 1;
//         do { ...body...; j++; j1++; } while ((short)j < (short)num);
//     }
// moves BOTH target swaps at once and exactly as wanted: ebx now holds
// bestIdx (`xor ebx,ebx` before the guard, `imul edx, ebx`) and j1 becomes the
// memory-resident one at 0x1c, with its `mov [esp+0x1c], 1` sitting BELOW the
// `jle`, in the preheader, exactly like the original. So the allocator
// question was really "which variable's initialisation is inside the guarded
// block", not declaration order. That variant scores 79.1 (1055 bytes): the
// loop costs 3 extra bytes somewhere in the flag2 branch and flips the
// operand order of `bestDiff * j1` (`mov eax,[esp+0x1c] / imul eax,[esp+0x30]`
// instead of the original's `mov eax,[esp+0x30] / imul eax,[esp+0x1c]`;
// writing `j1 * bestDiff` does not change that, it compiles identically).
// The +3 bytes are the thing to hunt: a loop shape that keeps the do-while's
// guarded j1 initialisation but is byte-for-byte the for-loop's rotation.
// Then that operand order and the remaining first-cell `imul` (edi vs eax)
// are all that would be left.
// Re-checked by deepseek-v4.1-flash (issue 4120): the guarded do-while inner
// loop's +3 bytes live ENTIRELY in the first-cell block, not in the loop. The
// loop preheader and latch really are byte-identical to the original's. The
// first-cell block is 30 bytes against the original's 27, all from the product
// accumulator choice: `imul edi,[esp+0x10]` is 5 bytes where `mov eax,[esp+0x10]
// / imul eax,edi` is 7 (+2), and because the index then occupies eax, g_game
// loads into ecx (`mov ecx,[abs]` = 6) instead of the A1 form `mov eax,[abs]`
// (5, +1). So one register choice (product in eax vs edi) accounts for the
// whole surplus AND drags `mov edi,ebx` above the add. If (a) is ever fixed,
// re-score the do-while variant: it should reach full size with (b) fixed too.
// Re-checked by deepseek-v4.1-flash (issue 3740). The guarded do-while inner
// loop (see the v1 note above) was rebuilt and disassembled byte by byte:
//  * it does fix BOTH swaps as recorded (bestIdx in ebx, j1 at 0x1c with its
//    store below the jle), and every instruction of the inner loop then matches
//    the original's text and encoding except one: the original computes
//    `mov eax,[esp+0x30] / imul eax,[esp+0x1c]` (bestDiff loaded first) where
//    the do-while form computes `mov eax,[esp+0x1c] / imul eax,[esp+0x30]`.
//  * the 3 extra bytes are NOT in the loop: the obj shows the loop preheader
//    (`xor ebx,ebx / xor ecx,ecx / test ax,ax / mov [esp+0x4c],eax /
//    mov [esp+0x30],-1 / mov [esp+0x34],ecx / jle near / mov [esp+0x1c],1 /
//    jmp / mov edi,[esp+0x40]`) byte-for-byte identical to the original's,
//    with `xor ebx,ebx` landing at function offset 0x172 against the
//    original's 0x16f, so the surplus sits above the loop, in the first-cell
//    block, and it also trades the bestDiff/j1 operand order. This file
//    (83.3%) stays the best version.