// Decompiled by space-bunny-free, improved by Claude Opus 5.5, edited by deepseek-v4.1. Names are provisional.
//
// Returns the world position of animation piece `index` of `obj` (with z
// negated, as the callers add it to obj->pos at +0x6a):
//
//   obj->recs (+0x9e) points to a table whose count is at +0x00 and whose
//   pieces start at +0x22, each 0x36 bytes. A piece has an offset pointer at
//   +0x00, x/y/z at +0x04/+0x08/+0x0c, its three angles at
//   +0x10/+0x12/+0x14 and a `next` at +0x32. `obj` has three base angles at
//   +0x64/+0x66/+0x68.
//
//   The base piece's offset (p->+0x10/+0x14/+0x18 plus x/y/z) seeds the
//   result; each node of the `next` chain rotates the result by its angles
//   (FUN_004b6cc0), adding the object's base angles on the last node, then
//   adds its own offset.
//
// Still differs from the original: 94.2%, 359 bytes like the original.
//
// The source shape of the body is right. The old 64% notes blamed the base
// piece's x load (the original folds it as [ecx+eax*2+0x26] before the item
// pointer exists); that, the folded `add reg, mem` loads and the
// FUN_004b6cc0 argument registers were all compiler state. With N unused
// `extern int` declarations in front, N = 42..295 (and every window of about
// 250 in each period of about 525 up to N = 6000) gives 94.2%, and
// `<string.h>` alone, `<windows.h>` alone or `<ddraw.h>` alone reach the
// same state. No N and no header set (tools/headers.py) goes past 94.2%.
//
// What still differs, both in the return blocks:
// - the final copy: the original loads x and y into edx and esi, copies the
//   return pointer into edi, negates z (ecx) and stores all three; ours
//   copies the pointer into edx and loads x and y through esi one after the
//   other. Every spelling of the final return scores the same: `result.z =
//   -result.z; return result;`, a separate `Vec3 out` in any field order,
//   `Vec3 out = result;` (56%), an inline MakeVec3(x, y, -z) or Flip(const
//   Vec3&) / Flip(Vec3) helper, a Vec3 constructor, a pointer to the result,
//   `int x = ...` locals, and `int result[3]`.
// - the index-out-of-range zero block: the original zeroes x, y, z (edx,
//   esi, ecx); ours zeroes x, z, y (edx, ecx, esi). Zeroing it x, y, z
//   (like the null block) gives the null block's registers, and MSVC then
//   merges the two blocks (87.3%); every other order, chained `a = b = c =
//   0`, memset, ZeroVec()/SetZero()/MakeVec3(0, 0, 0) helpers in either
//   block, and zeroing `result` itself are 87.3 to 94.2%.
// Both blocks in the original use the same registers (x = edx, y = esi,
// z = ecx, pointer = edi), one step on from the null block's (x = ecx,
// y = edx, z = esi); throwaway global stores before or inside either zero
// block did not rotate them. Also tried: one shared `result` returned from both the range block
// and the end (78 to 87%), nested `if (obj && obj->recs) { if (in range)
// {...} }` (47 to 55%, all zero blocks merge), and the range check plus
// computation in an inline helper (41%).
//
// Follow-up (deepseek-v4.1-flash): all six assignment orders of the range
// block's `w` were scored. x,y,z merges the two zero blocks (330 bytes,
// 87.3%); z,x,y is the best (94.2) and already has the original's
// (x=edx, y=esi, z=ecx) allocation, differing only in the xor emission order
// (ours ecx before esi, the original esi before ecx). Initializer forms
// (`Vec3 w = {0,0,0}`, `= {0}`, `= {}`), a chained `w.x = w.y = w.z = 0`, a
// `memset`, indexing through an `int*`, declaring `w` at function scope, and
// a dummy `if` after `block` are all 87.3 to 92.6. The two blocks in the
// original both use dest=edi, x=edx, y=esi, z=ecx; our range block already
// matches that allocation, our end block does not (dest=edx, x=esi). End
// forms tried and all 94.2 (identical diff): an inline FlipZ helper (55.7),
// `Vec3 out` declared at function scope, a pointer to `result`, `int z`/`int
// x`/`int y` locals in every order, `out.z` first, and reading z back from
// memory after the negate.

// Sweep (deepseek-v4.1): the 6 orders of the null block crossed with the 6 of
// the range block were all scored. The range block keeps the original's
// (x = edx, y = esi, z = ecx) allocation only when z is assigned first:
// z,x,y = 94.2 and z,y,x = 91.7. Every other order pushes x into ecx and drops
// to 79.5-92.6, and z,x,y in both blocks merges the two zero tails into one
// (330 bytes, 87.3) just like x,y,z in both. So z,x,y is the unique order for
// the range block, and its only remaining difference is that its two xors come
// out x,z,y where the original emits x,y,z. Chained (w.x = w.y = w.z = 0) and
// comma (w.z = 0, w.x = 0, w.y = 0) spellings are 92.6, not better. The end
// block is stuck the same way: `Vec3 out` assigned field by field, `result.z =
// -result.z` then `Vec3 out = result`, and an inline MakeVec3(x, y, -z) helper
// all give this same 94.2 diff (dest in edx instead of edi, esi reloaded for
// y), while `Vec3 out = result; out.z = -out.z;` alone is 55.7.

// Third pass (deepseek-v4.1): the neighbouring matched file 0x43d210.cpp shows
// the original Vec3 carries a ctor (Vec3 zero(0, 0, 0) uses zeroed registers),
// so Vec3 was retried as a class: `Vec3 v(0, 0, 0); return v;` in the null
// block, `Vec3 w(0, 0, 0); return w;` in the range block and `return
// Vec3(result.x, result.y, -result.z);` at the end are byte-identical to the
// POD field-assignment forms (all four combinations, 94.2, both hunks
// unchanged, 359 bytes); declaring one `result` at function scope and zeroing
// it from both blocks is also 94.2 with the identical diff. Only x,y,z in the
// range block moves bytes: it merges the two zero tails into one (330 bytes,
// 87.3). So both hunks are allocator state: the end block wants dest = edi,
// x = edx, y = esi (the same tuple the object-init blocks use) and the range
// block wants its two xors emitted y before z.

// Fourth pass (deepseek-v4.1-flash): the two return hunks are a front-end
// independent tie. The prefix through the loop (0x43def0 to 0x43dffb) is
// byte-identical, so only the final statements can differ, yet `return
// result;`, field-copying into a fresh `Vec3 out` in all 6 field orders,
// forcing `int x/y/z` locals first (MSVC coalesces them away), `return
// Vec3(x, y, z)`, `0 - result.z`, `return *p` for `Vec3* p = &result`, and
// declaring `out` at function scope all compile to the same 94.2% bytes. An
// explicit out-parameter signature `void f(Vec3* out, ...)` moves obj out of
// edi and scores 52.1%; giving Vec3 a copy constructor scores 50%. The range
// block's xor emission order (ours edx, ecx, esi vs edx, esi, ecx) is likewise
// fixed before the block, whose instructions are identical. 24 variants
// scored, none above 94.2%.
//
// Fifth pass (deepseek-v4.1-flash, ~160 check.py runs, no new best): the tail
// is a *copy decomposition* difference, not a source-shape one. The target end
// block is a three-value copy like the two zero blocks (values edx, esi, ecx,
// dest edi): the compiler materialised x, y and the negated z into separate
// registers before the first store. Ours is a two-register block copy (dest
// edx, one temp esi reused for x and y). The front end normalises every
// end-block spelling tried to the same IR: all 6 field-copy orders, whole
// struct copies (`Vec3 out = result;`), ctor forms (`return Vec3(x, y, -z)`
// with 6 different ctor body orders, all 92.1), inline helpers taking 3 ints,
// by value or by pointer (FlipZ, Copy), comma expressions, `int x/y/z`
// temporaries in every order, `memcpy`, casts, references, a union, dead
// statements, a conditional `result.z ? -result.z : 0`, and the negate spelled
// `-x`, `0 - x` and `x * -1`. A genuine struct copy into a local with a home
// (`Vec3 out = result; out.z = -out.z; return out;`) does produce the
// three-value pattern (x = ecx, y = edx, z = eax, dest = edi, plus one store
// to out's slot) but it also rotates the whole function (55.7%); the same for
// every other form that gives out a home. So the original's tail had the
// three values live at once without giving the destination a home.
// Range block: only source order z, x, y gives the original's mapping
// (x = edx, y = esi, z = ecx); for the other five orders the xor emission
// order equals the source order, but z, x, y emits x, z, y where the original
// emits x, y, z (the original's two zero blocks both emit in store order).
// Chained, comma, grouped and two-statement spellings of z, x, y all give the
// same 94.2 bytes; x, y, z merges the two zero tails (330 bytes, 87.3).
//
// Sixth pass (deepseek-v4.1-flash): the end block is fixed, 99.2%. The
// original's end block is a three-value copy; MSVC only makes one when it
// unrolls a *loop* over the fields, so write the return as
//
//     Vec3 out;
//     for (int i = 0; i < 3; i++)
//         ((int*)&out)[i] = ((int*)&result)[i];
//     return out;
//
// which the compiler unrolls into three independent values and hoists both
// loads above the destination, exactly like the original. Every other copy
// form (struct copy, field copy in any order, ctor, memcpy, cast, reference,
// helper) emits the two-register block copy. The loop spelling is also what
// keeps the range block's register mapping (x = edx, y = esi, z = ecx).
//
// What still differs, one instruction pair in the range block: the original
// emits the two zero xors in store order (x = edx, y = esi, z = ecx), ours
// emits x, z, y (the z xor before the y xor, both before the stores). The
// mapping already matches; only the order of the ecx and esi xors differs.
// Tried for that pair: all 6 statement orders (z, x, y is the only one with
// the original's mapping, and it is also the only order whose emission is not
// the source order), chains, comma expressions, two-statement groupings,
// aggregate initialisers, ctor and default-ctor forms, helpers zeroing by
// pointer and by reference, `int*` index writes in all orders, forward and
// reverse loops, `while` loops, independent `Zero()` calls per field,
// cross-field assignments (`w.y = w.z`), the same variable declared at
// function scope, `(void)block` / `block;` / `block = block;` to keep ecx
// live, and a sweep of unused `extern int` declarations before the function
// (N = 0..30 all give this same 4-line diff; N >= 31 degrades).

#include <string.h>

struct Vec3 {
    int x;
    int y;
    int z;
};

void __stdcall FUN_004b6cc0(Vec3* in, Vec3* out, short* angles);

struct Ptr_0043def0 {
    char unknown_0[0x10];
    int f10;                           // +0x10
    int f14;                           // +0x14
    int f18;                           // +0x18
};

#pragma pack(push, 2)
struct Item_0043def0 {
    Ptr_0043def0* p;                   // +0x00
    int x;                             // +0x04
    int y;                             // +0x08
    int z;                             // +0x0c
    short f10;                         // +0x10
    short f12;                         // +0x12
    short f14;                         // +0x14
    char unknown_16[0x32 - 0x16];
    Item_0043def0* next;               // +0x32
};

struct Block_0043def0 {
    int count;                         // +0x00
    char unknown_4[0x22 - 4];
    Item_0043def0 items[1];            // +0x22
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Object_0043def0 {
    char unknown_0[0x64];
    short f64;                         // +0x64
    short f66;                         // +0x66
    short f68;                         // +0x68
    char unknown_6a[0x9e - 0x6a];
    Block_0043def0* recs;              // +0x9e
};
#pragma pack(pop)

// FUNCTION: 0x43def0
Vec3 __stdcall FUN_0043def0(Object_0043def0* obj, int index)
{
    if (obj == 0 || obj->recs == 0) {
        Vec3 v;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    Block_0043def0* block = obj->recs;
    if (index < 0 || index >= block->count) {
        Vec3 w;
        w.z = 0;
        w.x = 0;
        w.y = 0;
        return w;
    }
    Item_0043def0* item = &block->items[index];
    Vec3 result;
    result.x = item->p->f10 + item->x;
    result.y = item->p->f14 + item->y;
    result.z = item->p->f18 + item->z;
    for (Item_0043def0* n = item->next; n != 0; n = n->next) {
        short angles[3];
        angles[0] = n->f14;
        angles[2] = n->f10;
        angles[1] = n->f12;
        if (n->next == 0) {
            angles[0] = angles[0] + obj->f64;
            angles[2] = angles[2] + obj->f68;
            angles[1] = angles[1] + obj->f66;
        }
        FUN_004b6cc0(&result, &result, angles);
        result.x += n->p->f10 + n->x;
        result.y += n->p->f14 + n->y;
        result.z += n->p->f18 + n->z;
    }
    result.z = -result.z;
    Vec3 out;
    for (int i = 0; i < 3; i++)
        ((int*)&out)[i] = ((int*)&result)[i];
    return out;
}
