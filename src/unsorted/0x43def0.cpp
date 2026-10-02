// Decompiled by space-bunny-free, improved by Claude Opus 5.5, edited by deepseek-v4.1. Names are provisional.
// claude-opus-5-5 (#4428): unchanged; a 10-minute permuter run (about 500 candidates) found nothing.
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
//
// 30-min checkpoint (space-bunny-free): still 99.2%, this file is unchanged
// and is the best. New this pass, every form scored on a /Fa harness that
// compares only the range block: the 6 statement orders, the 6 chained forms,
// the 6 grouped and two-statement forms, cross-field copies, `((int*)&w)[k]`
// index writes, comma forms, 9 zero expressions that do not look foldable
// (`index - index`, `index & 0`, `~index & 0`, ...), 21 dead-code inserts
// (`int t = 0; if (t) w.f = 1;`, `while (t)`, a dead `for`, a dead local
// array element, a null-pointer test, `bits & 0`) in every position and on
// every field, a `Vec3` class with a 3-int ctor in all 6 body orders and all
// 6 parameter orders, `v = Vec3(0, 0, 0)`, `Vec3 v(0, 0, 0)`,
// `return Vec3(0, 0, 0)`, an inline `Zero3()` helper returned and called
// through a pointer, and a dead `Zero()` call. Every one either keeps x, z, y
// or merges the two zero blocks (87.3). The only form that moves the ecx and
// esi xors is the ctor with a z, y, x body, and it emits all three xors
// before `mov edi, eax`, the wrong shape. A scanned whole-exe search found
// only three blocks of this shape in TotalA.exe (0x424265, matched, which
// 0x424050.cpp writes as `spot->vel = Vec3(0, 0, 0)` into a *member*, and our
// two), so there is no near-copy to lift the spelling from.
//
// Wrap-up (space-bunny-free): 99.2%, the sixth-pass file is still the best and
// is unchanged apart from these notes. Also tried this pass, all on the same
// /Fa harness: sharing one Vec3 between the range block and the null block or
// the end block (tA/tB/tC/tD), a second struct type with the fields declared
// in all 6 orders crossed with the 6 assignment orders (36 forms, fo_*), the
// 6 assignment orders through a union's `int[3]` view (un_*), an `Identity`
// / `Touch` / `Add3` inline helper around one zeroed field and around the whole
// zero (id_z, touch_y, add3), a shared `int z = 0` local read by two fields
// (shared_x/y/z), reading the fields back after zeroing (read_y, readall), a
// loop over the fields in 12 spellings (short/uchar/long index, while, do-while,
// reverse, through `int*`, `&w.x`, of `-0`, of a folded local), a loop copy
// from a separately zeroed local in all 6 zeroing orders (cp_*), five control
// flow shapes around the zeroing (`if (1)`, `do {} while (0)`, `switch (0)`,
// `1 &&`, a ternary), a function-scope `Zero3()`/`Mk3(a,b,c)` helper with the
// zeroing in all 6 orders, five range-test spellings that keep `jl`/`jge`
// (swapped operands, `!(a && b)`, an unsigned compare, two separate tests, a
// `goto`), and N = 1..12 uncalled `static inline` functions at file scope plus
// six single uncalled helpers. Nothing reaches y before z: the only forms that
// reorder the xors at all put all three before `mov edi, eax`, and the only
// ones that keep the right register mapping always emit ecx before esi.
//
// Leads for the next attempt (not tried, out of time):
// - `tools/permute.py 0x43def0 --seed 11/12/13` (the default-seed run of 6
//   minutes on this file found nothing, but a seed sweep fixed another
//   address today at seed 11). The permuter's statement-order and temporaries
//   mutations cannot reach this pair on their own, so give it the range block
//   as the only focus.
// - The original's range block is one basic block with two predecessors; every
//   shape tried here still reaches it from two. A spelling that gives it three
//   predecessors (pd_triple was 3 stores but the two blocks merged) or that
//   keeps `block` live across the branch so ecx cannot go to the first zero
//   was not found; kp_* (a `block->count` local, a redundant second compare,
//   `w.z = block->count - block->count`) is the unfinished half of that idea.
// - The end block only matched as an unrolled loop *copy*, so the range block
//   may want a loop copy too, but from a source MSVC cannot fold to one shared
//   zero register. Every zero source tried (a local, an array, a union, an
//   inline helper) folds, so the source has to be something whose zeros are
//   only known at run time and still cost one `xor` each.
//
// Seventh pass (space-bunny-free): still 99.2%, the sixth-pass body is
// unchanged. Measured this pass, with all scores from check.py:
// - The register mapping and the xor *emission* order are two independent
//   things. The mapping is always x = <1st reg>, y = <3rd reg>, z = <2nd reg>
//   off the source order (the copy-back's stores are always in field order), so
//   the source's field order only picks which field lands on which emitted
//   xor. The emission is always x first, then the other two in source order,
//   and the three registers always come out in the order edx, ecx, esi. The
//   target therefore needs the source order y, x, z (x first, then y, z) *and*
//   the register order edx, esi, ecx, and no source order gives both: y, x, z
//   emits x, y, z but maps y = ecx, z = esi; z, x, y maps y = esi, z = ecx
//   but emits x, z, y. The end block already has the target's map and order
//   (edx, esi, ecx for x, y, z) because it is an unrolled loop copy, so the
//   range block wants the same shape with a zero source MSVC will not fold.
// - New this pass, all scored: `memset(&w, 0, sizeof w)` and two spellings of
//   its size (357 bytes, 93.8%: one shared zero plus `mov edx, ecx`), a loop
//   write `for (i = 0; i < 3; i++) ((int*)&w)[i] = 0;` through `&w` and
//   through an `int*` (357 bytes, same single zero), `Vec3 w = {0}` (357
//   bytes: y and z share ecx), `Vec3 w = {}` and `w = Vec3()` (MSVC 5 rejects
//   the first, the second is 92.6%), float zeros 0.0f and 0.0 (330 bytes, the
//   two blocks merge), duplicate zeroing statements in every order (best
//   99.2%, same diff), folded conditionals on a field (`if (index - index)`,
//   `index - index ? 1 : 0`), a live `if (block)` and `if (block->count)` after
//   the zeroing (91.4 to 91.8%), self-assignments `w.f = w.f` on each field,
//   nine end-block loop spellings crossed with the range block (all 99.2%,
//   the end block's shape does not move the range block's xors), the null
//   block split into two `if`s and a nested range test with two zero blocks
//   (both 99.2%, identical diff), the range test split into two `if`s (83.5%),
//   and the count in a local.
// - `tools/permute.py 0x43def0 --minutes 8 --seed 12` evaluated 3072
//   candidates and found nothing; seed 13 evaluated 3819 and found nothing.
//
// Eighth pass (space-bunny-free): still 99.2%, the body is unchanged. This
// pass pinned the register rule down exactly, so the next attempt can aim at
// it instead of sweeping. For the range block, with the field order the source
// writes (all six scored, 359 bytes each except x, y, z):
//
//   source   xor emission   physical   x, y, z land in
//   x,y,z    x y z          ecx edx esi  ecx edx esi   (330 bytes, blocks merge)
//   x,z,y    x z y          ecx edx esi  ecx esi edx
//   y,x,z    x y z          edx ecx esi  edx ecx esi
//   y,z,x    y z x          ecx edx esi  esi ecx edx
//   z,x,y    x z y          edx ecx esi  edx esi ecx   (99.2%, this file)
//   z,y,x    z y x          ecx edx esi  esi edx ecx
//   target   x y z          edx esi ecx  edx esi ecx
//
// So the emission is the source order except that x is hoisted to the front
// when x is written second, and the two volatile zeros (edx, ecx) always take
// the first two slots with the callee-saved esi always last. The target is the
// only shape that needs esi second, i.e. ecx unavailable at the second
// allocation and free at the third. Nothing below made that happen:
// - zero expressions that keep `block` or the count live across the second
//   zero (`block->count - block->count`, `& 0`, `* 0`, `^ itself`,
//   `-c + c`, `~c & 0`, `c` in a local) on each field, and the same as the
//   range test's condition: all 97.5%, the same block as plain y, x, z.
// - a `Vec3` class with a 3-int constructor (6 body orders x 3 spellings:
//   `Vec3 w(0,0,0)`, `w = Vec3(0,0,0)`, `return Vec3(0,0,0)`) and
//   `unsigned int` or `long` fields with `0u` literals: identical to the plain
//   struct, so the constructor is transparent here.
// - a loop *copy* is the one thing that does follow the source order: copying
//   field by field out of a separately zeroed local through
//   `for (i = 0; i < 3; i++) ((int*)&w)[i] = ((int*)&src)[i];` emits the three
//   xors in the order `src` was zeroed (x y z for an x, y, z source), which is
//   the order the target needs. But the loop always keeps a counter register,
//   so the block grows to 361 bytes with a trailing `mov eax, esi`, in all 13
//   loop spellings tried (`!=`, `unsigned`/`char`/`short` index, while,
//   do-while, pointer walk, reverse, `+ 0`). The end block's copy has no
//   counter because its source is homed in the frame; a zeroed source would
//   cost three extra immediate stores, so that route is closed too.
// - shape changes: `goto` labels for either zero block, the zero block as the
//   fallthrough of `if (in range) goto in_range;` (three predecessors), the
//   zero block inside an `if (in range) { ...main... }`, the two range
//   conditions swapped (MSVC keeps the source order, +2 bytes), a `switch` on
//   the folded condition, `!(index >= 0 && index < count)`, `index - count
//   >= 0`, the null check through a pointer local, and the null block zeroing
//   through a `Vec3*`: 99.2% with the same diff, or worse.
// - `tools/permute.py 0x43def0 --minutes 8 --seed 13`: 3819 candidates, no
//   gain. Seeds 11, 14 and 15 were also started.
// - Tooling note for other passes: score scratch variants with a tag that is
//   unique per batch (hash the variants file's name into it). Reusing
//   r000..rNNN across batches silently overwrote an earlier variant and made
//   z, y, x look identical to z, x, y for one batch's worth of results.
//
// Ninth pass (space-bunny-free): still 99.2%, the body is unchanged. The rule
// above is now closed for the "three field assignments then return" family, so
// the original's range block is spelled some other way:
// - The order that matters is the order of the *copy* into the returned value,
//   not the order of the zeroing statements: a separately zeroed local copied
//   field by field gives the same result for all 36 zero-order x copy-order
//   pairs, and the five that copy z, x, y are all 99.2% while all six that copy
//   x, y, z merge the two zero blocks. MSVC coalesces the copy into the
//   zeroing, so the two orders cannot be decoupled this way.
// - Copy order x, y, z emits the xors as the original does (x, y, z) but
//   allocates the same registers as the null block, which is why the blocks
//   merge. Breaking the merge with dead code in the *null* block (a dead int,
//   a dead array, a dead Vec3, a dead `if`, a repeated statement, in eight
//   placements) changes nothing: the merge and the allocation are unaffected.
//   So the allocation is not sensitive to anything the source can perturb
//   without emitting code.
// - Casts of the zero literals (`(char)`, `(short)`, `(long)`, `(unsigned)`,
//   `(float)`, `(double)`, `'0' - '0'`) on each of the three fields, and
//   `static const int` / `static const Vec3` / `extern const int` zero sources,
//   fold to the same node: 99.2% or 97.5%, the same blocks as plain zeros.
// - `return` spellings that put an assignment inside the return expression
//   (`return (w.y = 0, w);`, `return (w.z = 0, w.x = 0, w.y = 0, w);`, a
//   ternary on a field, a shared zero local): all 99.2%, same block.
// - `tools/headers.py 0x43def0`: 128 header sets, best 99.2% (<windows.h>,
//   <string.h>, <ddraw.h> and pairs), no change.
// - `tools/permute.py 0x43def0 --minutes 8 --seed 13`: 3819 candidates, no
//   gain. Seeds 11, 14 and 15 were also started; seeds 11, 12, 13, 14 and 15
//   together evaluated 12600 candidates and none of them matched.
// - Zeroing through a local struct's member (`struct S { Vec3 v; } s;
//   s.v.z = 0; ... return s.v;`) in all six orders, and the same with a second
//   vector copied in after, give exactly the plain-field numbers, so member
//   access is transparent here too.
// - Self-conditional zero sources, `(w.z ? w.z : w.z)` and `(t ? t : t)` on
//   each of the three components and `w.f = (w.f ? w.f : w.f)` after it, plus
//   `(0 ? 0 : 0)` and `(index ? index : index) - index`: 99.2% with the same
//   diff (the last is 93.0% and 2 bytes longer). The phi these create is
//   folded away before the allocator runs, so the ranking effect that helped
//   elsewhere does not reach this block.
//
// Final state of this pass: 99.2%, 359 of 359 bytes, the body above is
// unchanged from the sixth pass. What still differs is one instruction pair in
// the range block, and the table above says why: the original needs the xor
// emission x, y, z with the registers edx, esi, ecx, and every source order
// that emits x, y, z also allocates edx, ecx, esi (and merges with the null
// block), while every order that allocates edx, esi, ecx emits x, z, y. The
// lead for the next attempt is that the two are coupled only through the
// graph node order of the three zero values, which the field assignment order
// fixes; the one construct found that decouples them is the unrolled loop copy
// (it emits in index order), and it needs a counter register because its
// source is not homed in the frame. So the thing still missing is a way to
// copy three zeros out of something MSVC has to keep in memory without paying
// for three immediate stores: a homed source that is already known to be zero
// at that point in the function. Nothing in the current frame is (the angles
// array is three shorts, `result` is live only on the main path), and giving
// either one a home changes the frame size.

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