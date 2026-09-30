// Decompiled by space-bunny-free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// PARTIAL: 66.2%, 358 of 366 bytes (was 64.5% and 354 bytes).
// space-bunny-free pass (#1805): the one change that helped is the third
// distance, `d3 = Dist(c, b)` (66.2% against 64.5% for `Dist(b, c)`), so
// FUN_00480570's operand order (b minus c) is what the original wrote even
// though the abs makes it invisible in the values. Swapping the other two the
// same way does not help: `Dist(c, a)` for d2 gives 64.7% and `Dist(b, a)` for
// d1 gives 65.2%. The unnamed `Dist(*p, b)` / `Dist(*p, c)` source keeps the
// right slot use (local0 = first `a` copy, then d1 in the same slot, local1 =
// second copy, local2 = a spill) but only reaches 62.0%, with or without
// `Dist(c, b)`; declaring d1/d2/d3 up front and assigning them later is 62.0%
// too, so the declaration placement does not move it.
// What still differs, in the first two distance blocks only: the original keeps
// b as two member values for the whole body (b.x in ebx, b.y in ebp) and c as
// two member values too (c.x in ecx, c.y in edx, spilled to [esp+0x18] for the
// third block), while ours keeps b's members in the volatile ecx/edx (spilling
// b.y to [esp+0x18]) and copies c as one dword into ebx. That single choice
// cascades into every later block, including the two epilogues, which in the
// original re-read the arg slots and in ours compare against bx. Nothing in the
// source tried here makes MSVC 5 split c; the reverse-angle idea from the
// packet (computing the distances in the other order, so a return slot is
// reused) was tried as d2 before d1 (49.5%, 46.9%) and d3 first (39.9%).
// deepseek-v4.1-flash pass: tried the unnamed `Dist(*p, b)` form (62.0%, and it
// reorders the two distances: it loads c as a whole dword into ebp and computes
// a.x - c.x first), a mixed `Dist(a, b)`/`Dist(*p, c)` form (62.0%), the exact
// FUN_004805570 body for Dist (`Diff d = Sub(a, b); abs...`) both named and
// unnamed (52.3%, but exactly 366 bytes), `Point a; a = *p;` (64.5%), a `short`
// Toward step (64.5%) and dropping `inline` (64.5%). Best stays this file. The
// one remaining structural gap: the original's first Dist temp at local0 is
// overwritten by d1 (so local1 is the second Dist's `*p` temp), while with a
// named `Point a` the compiler keeps `a` in local0 and puts d1 in local1;
// everything else is the resulting register-allocation cascade in the first two
// distance blocks.
// Claude Sonnet 5.5 pass (#554): the original calls nothing, because it inlines
// FUN_00480570 (max of the two absolute differences, Points passed by value) three
// times and inlines a "move one point towards another" helper twice. That is what
// the source is now:
//  - `Dist(Point a, Point b)` with the ternary body `return dx > dy ? dx : dy;`
//    (the if-form scored 44 to 50%, the ternary 60%); the `a` copy in stack slot
//    [esp+0x10] and its reuse for d1 come from the by-value parameter temp.
//  - `Toward(Point from, Point to, int d)` modifies its by-value `from` in place
//    and returns it, `*p = Toward(b, c, d1)`: the original tail (`mov eax,[esp+0x24];
//    mov edx,[esp+0x28]; cmp dx,ax; mov [esp+0x24],eax` then in-place word stores)
//    is reproduced instruction for instruction in shape, because the inline
//    parameter reuses the caller's dead argument slot.
//  - min is written `if (d1 > d3) d1 = d3;` (62.0 against 60.5 for `d3 < d1`).
// What still differs is only register allocation in the first two distance blocks:
// the original loads b.x into ebx and b.y into ebp with two `movsx` from the
// argument slots before anything else (member-wise), keeps c.x in ecx and spills
// c.y to [esp+0x18]; ours loads b and c as whole dwords (c: `mov ebx,[esp+0x28]`,
// then uses bx) and spills b.x. A named local `Point a = *p;` scores 63 to 64.5%
// but adds a local at [esp+0xc]; the unnamed `Dist(*p, b)` form (62.0%) has the
// right slots ([esp+0x10] and [esp+0x14] for the two `a` temps) but hoists the whole
// dword load of b. Tried without effect: Dist or Toward taking the second Point by
// const reference (44 to 62%), comparison direction in Toward (all the same), Dist
// with if-forms or swapped operands, d3 first (39.9%), d2 before d1 (49.5%), a
// separate result temp in the tail. The declaration-count probe (0 to 400 unused
// externs) is flat for the old file, and headers.py gives 64.5% for every set that
// compiles, so this is source shape, not compiler state.
// edited by deepseek-v4.1 pass (#2100): re-ran the ground truth. The real 0x480570
// body compiles to `cmp ecx,eax; jle; mov eax,ecx` (result in eax), while the three
// inlined blocks here store |dx| to the result slot first and then conditionally
// overwrite it (`jg skip; mov [slot],eax`), so the inlined source is the ternary
// form kept below; blocks 1 and 2 are instruction for instruction identical apart
// from the register names, so the entire gap is one allocator decision: the
// original promotes b.x/b.y member-wise into the callee-saved ebx/ebp in the
// prologue (before the `mov edi,[eax]` deref) and c.x/c.y into ecx/edx at block 2
// (c.y spilled to [esp+0x18], reloaded from [esp+0x28] at the tail), while every
// source spelled here makes MSVC load each Point as one dword (b as a dword into
// ecx/edx with b.y spilled here, or into ebp in the unnamed `Dist(*p, b)` form)
// and keep c whole in ebx for the tail. New variants tried: `const Point&` second
// Dist parameter named and unnamed (46.3, 46.7, 45.9, 47.1%), `(short)` cast on the
// Toward step (45.7%), min written `if (d3 < d1) d1 = d3;` plus `Dist(b, c)`
// (63.0%). Best stays the named-`a` ternary file below at 66.2%.
// deepseek-v4.1 pass (#2385): re-ran the board's unnamed-form idea, since it is
// the one shape that gives d1 the first `*p` temp's stack slot the way the
// original does. `int d1 = Dist(*p, b); int d2 = Dist(*p, c); int d3 =
// Dist(c, b);` scores 61.5% (353 bytes): MSVC copies the by-value b as one
// whole dword into ebp (`mov ebp, dword ptr [esp + 0x1c]`) before the deref,
// the opposite of the original's member-wise ebx/ebp homes, so the slot it
// gets right does not pay for the two registers it loses. `Dist(b, c)` for d3
// is 62.0%, `if (d3 < d1) d1 = d3;` for the min is 60.8%, and an unnamed first
// call followed by a named second copy (`Point a = *p;` after the d1 line) is
// 61.5%. Every shape tried by the three earlier passes plus these still leaves
// the same gap: b member-wise in whatever the allocator has free (ecx/edx plus
// spills here, ebx/ebp in the original) and c as a whole dword (ebx here,
// ecx/edx in the original). Best stays this named-`a` file at 66.2%.
#include <stdlib.h>

struct Point_004805b0 {
    short x;
    short y;
};

static inline int Dist(Point_004805b0 a, Point_004805b0 b)
{
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    return dx > dy ? dx : dy;
}

static inline Point_004805b0 Toward(Point_004805b0 from, Point_004805b0 to, int d)
{
    if (to.x < from.x) from.x -= d;
    else if (from.x < to.x) from.x += d;
    if (to.y < from.y) from.y -= d;
    else if (from.y < to.y) from.y += d;
    return from;
}

// FUNCTION: 0x4805b0
void __stdcall FUN_004805b0(Point_004805b0* p, Point_004805b0 b, Point_004805b0 c)
{
    Point_004805b0 a = *p;
    int d1 = Dist(a, b);
    int d2 = Dist(a, c);
    int d3 = Dist(c, b);
    if (d1 > d2) {
        if (d1 > d3) d1 = d3;
        *p = Toward(b, c, d1);
    } else {
        if (d2 > d3) d2 = d3;
        *p = Toward(c, b, d2);
    }
}

