// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// Sibling of 0x4736e0 and 0x4742c0: the same base call with the third
// argument, the same two 24-byte copies and the same trailing virtual call.
// This one keeps each 24-byte block as a {point, far point} pair, moves the
// point 4/11 of the way along it, and re-aims the far point 3/11 further on,
// so the block ends up holding {point at 4/11, that point's own delta}.
//
// Both divisions are the signed /11 magic 0x2e8ba2e9, the 7/11 one written as
// (d << 3) - d, so they have to be `d * 4 / 11` and `d * 7 / 11` on the same
// difference d. The old start point is kept in a local: the compiler holds it
// in a register (esi) across both stores, as the original does, but the two
// results have to be temporaries assigned to the fields, not written straight
// into them, or the 4/11 point is kept in a register for the subtraction
// below instead of being stored and re-read.
//
// The z component is the odd one out in the original too: its delta is the
// difference of the two lerps computed in registers, with the 4/11 point
// living in edi from before the x and y deltas are re-read from memory until
// the subtraction. `e.z = bz - az` with az and bz locals is what produces that;
// writing the subtraction against the field, or leaving one of the two out of
// a temporary, does not.
//
// Still differs (88.0%, 497 of 499 bytes), all of it scheduling inside the two
// copies of the helper:
//   - the 7/11 point is stored just before the next component's loads, where
//     the original sinks the store past them (x and y, in both blocks);
//   - the z 4/11 point is formed as `add edx, esi; mov edi, edx` instead of
//     `lea edi, [edx + esi]`, and its store lands before the x delta instead of
//     between the x delta's subtract and its store;
//   - the z delta is emitted as `sub edx, edi; add edx, esi` where the
//     original adds sz first and subtracts afterwards: MSVC reassociates
//     `bz - az` no matter how the two lerps are spelled;
//   - in the second block the base register ebp (= this + 0x1c) is also used
//     for start.y and start.z, where the original drops back to [ebx+0x20]
//     and [ebx+0x24], and the x delta is subtracted into eax where the
//     original reuses ebp (freeing the base register).
// I could not move any of those from the source: every spelling of the two
// divisions, of the two temporaries and of the three deltas lands on the same
// code, and no header set changes it either.
//
// Retry (deepseek-v4.1-flash): the store sinking is a real aliasing effect.
// The two Vec3 refs may overlap, so MSVC must keep `e.x = bx` before `s.y` is
// loaded. One plausible source shape is a single Segment pointer (start/end
// fields of one object, so MSVC knows they do not overlap and does sink the
// store). It does sink it, but every single-pointer spelling (Seg&, Seg*,
// int*, two pointers into one object, direct this->seg_34 fields, with or
// without per-component inline helpers) makes MSVC 5 hoist start.y above the
// start.x store and spill it, 497 bytes turns into 513 to 564. A middle shape
// (Segment* for end, Vec3* for start) also spills. The two-ref helper is the
// only shape that stays in registers, so 87.5% was the ceiling from source
// alone as far as that retry could tell.
//
// Retry (LongCat 2.5 Preview Free): 88.0%, 497 of 499 bytes. One real gain:
// the 4/11 x delta has to be a *temporary assigned in its own statement*
// (`int ddx = e.x - s.x; e.x = ddx;`) placed between the 7/11 z lerp and the
// `s.z = az` store, not `e.x = e.x - s.x;` after it. That alone is worth half
// a percent, and it is the only source change in the file. What is left:
//   - `e.x = bx` and `e.y = by` are still stored before the next component's
//     loads where the original sinks them past (the aliasing effect above);
//   - the original's `mov edx, esi; sub ecx, edx` for the z difference, i.e.
//     the start point copied into a second register rather than subtracted in
//     place, and `lea edi, [edx + esi]` for the 4/11 z point rather than
//     `add edx, esi; mov edi, edx`;
//   - the z delta is `add edx, esi` (7/11 point) then `sub edx, edi` (the
//     4/11 point), where ours reassociates to `sub edx, edi; add edx, esi`;
//   - in the second block the original keeps ebp (= this + 0x1c) live only for
//     start.x and drops back to [ebx+0x20] and [ebx+0x24] for y and z, and
//     reuses ebp for the x delta; ours holds ebp for all three components.
// Every one of these is register pressure that follows from the store order,
// so none of them moves on its own. Measured and rejected (all 497 bytes and
// 88.0% unless noted, i.e. identical code): the z difference and the deltas
// through a `static inline int Sub(a, b)`; the same through a `Z(v)` getter
// and a `Delta(b, a)`; the two lerps through `Lerp4(s,d)`/`Lerp7(s,d)`;
// `int q4 = dz*4/11` as its own local with the sum written both ways; the 7/11
// point accumulated with `+=`; `e.z = -az + bz`; a no-op `(int)` cast on
// either operand; computing bz first, or az last, or ddy next to ddx (that
// last one is 493 bytes and 74.7%); `e.z` assigned before `s.z` (78.8%).
// Shapes that keep the values in registers but not in the original's registers,
// all worse: one Vec3& with the end taken as `(&s)[1]` (503 bytes, 39.9%),
// one int* with the end at `p+3` (503, 39.9%), Seg* or Seg& (564, 23.6%),
// a member function on Seg (76 bytes, the body went out of line), Vec3* and
// Vec3& parameter pairs (497 bytes but the argument order changes the code,
// 66.3%), swapping the two calls (496 bytes, 49.6%) or the two struct copies
// (496, 48.0%), and staging the two segments through local Seg temporaries
// (635, 33.7%). `tools/headers.py` over all 128 header sets also stops at
// 88.0%, so this is a codegen difference, not a header one. 88.0% looks like
// this function's ceiling from source alone.

//
// Retry (Sonnet 5.5, #1135), no gain, 0 official runs: the base registers of the
// original say how the helper was fed. In block two the original uses [ebp]
// only for start.x and [ebx+0x20], [ebx+0x24] for start.y and start.z, where
// the Vec3& helper below gives [ebp+4], [ebp+8]. Passing the six components as
// separate `int&` parameters (Split(seg.start.x, seg.start.y, ..., seg.end.z)),
// which makes each address its own this+K node, reproduces every base register
// of the original (497 bytes, 85.9%: lower than this file only because the
// y and z loads are then ordered after the e.x store, and the original hoists
// e.y and s.y above it). Direct `this->seg_34.start.x` member expressions (a
// macro) hoist the loads too but also forward the stored values and spill
// (567 bytes, 29%; a statement hill climb of that shape reached 49%).
// Hill climbs over all single statement moves of both helper shapes are flat
// (88.0 and 85.9), and 125 spellings of the three per-axis delta/load forms
// give 85.9 each. So the missing piece is a source shape with the addressing
// of separate component references and the scheduling freedom of one base
// pointer. Also tried: a method on a Seg subclass that takes six int* locals
// from its own fields (503 bytes, 39.9%).
// Retry (deepseek-v4.1-flash, #1163): no gain, 88.0% stays the ceiling from
// source. All of this was measured with free `check.py --sym` scratch scores:
//   - one straight-line body with direct member expressions (no helper) is
//     31.8%: it keeps all six lerp results live and spills them;
//   - the six-`int&` helper (Split(seg.start.x, ..., seg.end.z)) reproduces
//     the original's base registers but reorders the loads (85.9%);
//   - a single `Seg&` / `Seg*` parameter, and a `Seg::Split()` method, all
//     change the prologue (`this` moves to ebp, an extra stack slot) and fall
//     to 39.9%;
//   - `<windows.h>`, `<string>`, `<iostream>`, `<vector>`, `<map>`, `<list>`,
//     `<stdio.h>`, `<stdlib.h>`, `<math.h>` all give 87.5 to 88.0%, so no
//     header is the lever;
//   - a sweep of 0 to 516 unused `extern int dummyN;` declarations in front
//     never exceeds 88.0%, so it is not compiler state either;
//   - `(&s.x)[1]` / `(&s.x)[2]` compile identically to `s.y` / `s.z`, and
//     dropping the `ddx` local also compiles identically (all 88.0%).
// The remaining diff is instruction scheduling inside the inlined helper:
// MSVC sinks the original's `e.x`/`e.y` stores one slot later (past the next
// component's loads) and keeps az in edi; ours stores earlier, fuses the z
// subtraction, and in the second block keeps ebp for `s.y`/`s.z` where the
// original goes back to `[ebx+0x20]`/`[ebx+0x24]`. No statement order that
// preserves the 88.0% load order moves any of them.
// Retry (deepseek-v4.1-flash, #1380): re-confirmed 88.0%. Moving the y loads
// before the `e.x = bx` store (the first diff hunk) makes MSVC 5 spill s.y to
// [esp+0x1c] (513 bytes, 66.1%): every source order that delays the store has
// to keep bx live across both loads, and the allocator spills instead of
// sinking the store the original's compiler sank. Adding an explicit
// `int ez = e.z;` local before the z delta compiles byte-identically to this
// file, so the z copy is not reachable that way either. 88.0% stands.
class Class_00471d70 {
public:
    void FUN_00471d70(int param_1);
};

struct Vec3_00473b50 {
    int x;
    int y;
    int z;
};

struct Seg_00473b50 {
    Vec3_00473b50 start;
    Vec3_00473b50 end;
};

static inline void Split_00473b50(Vec3_00473b50& s, Vec3_00473b50& e)
{
    int sx = s.x;
    int dx = e.x - sx;
    int ax = sx + dx * 4 / 11;
    int bx = sx + dx * 7 / 11;
    s.x = ax;
    e.x = bx;
    int sy = s.y;
    int dy = e.y - sy;
    int ay = sy + dy * 4 / 11;
    int by = sy + dy * 7 / 11;
    s.y = ay;
    e.y = by;
    int sz = s.z;
    int dz = e.z - sz;
    int az = sz + dz * 4 / 11;
    int bz = sz + dz * 7 / 11;
    // The x delta needs its own temporary, computed here, between the z lerps
    // and the s.z store: that is what puts the store where the original has it.
    int ddx = e.x - s.x;
    e.x = ddx;
    s.z = az;
    e.y = e.y - s.y;
    e.z = bz - az;
}

class Class_00473b50 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    char unknown_4[0x1c - 4];
    Seg_00473b50 seg_1c;
    Seg_00473b50 seg_34;
    void FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c);
};

// FUNCTION: 0x473b50
void Class_00473b50::FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c)
{
    ((Class_00471d70*)this)->FUN_00471d70(c);
    seg_1c = *a;
    seg_34 = *b;
    Split_00473b50(seg_34.start, seg_34.end);
    Split_00473b50(seg_1c.start, seg_1c.end);
    v4();
}
