// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// claude-sonnet-5-5 2026-10-02 (#4709 retry): still 55.9%, no MATCH. ~40 more
// whole-function shapes scored with --sym, none above 55.9%. What I measured:
//  - The original's loop (base starting at 0, f reloaded from its dead arg1
//    slot, `add ecx,0x18` then [ecx+esi*2]) is the shape MSVC emits only when
//    f is NOT in a callee-saved register. Whenever f gets edi, MSVC folds the
//    0x18 header offset into the base induction variable (base starts at 12).
//    So the loop mismatch is a consequence of the pre-call allocation, not a
//    separate problem.
//  - f's allocation priority decides it: with one loop store instead of two
//    (value computed in both branches, one `f->cells[...] = v` at the merge)
//    f DOES spill to [esp+0x38] and h gets esi (the original's set), but MSVC
//    then turns the store into a walking pointer (42.8%), so that is not the
//    original either. Using h a second time inside the y loop also gives h esi
//    (f stays edi, count spills to [esp+0x40]); extra uses of pitch/count in
//    the loops make things worse (26-37%).
//  - No effect (byte-identical output to the plain forms): static inline
//    SetCell/FillLens/AllocFrame(ref) helpers, goto/while/do-while spellings of
//    the loops, `(unsigned short*)(f + 1)` instead of f->cells, y*w+x instead
//    of a base accumulator (47%), pitch declared unsigned short (54.5%), scale
//    declared after count/pitch (the fild is scheduled early regardless).
//  - int hw/hh with (short) casts at the uses (the original's stored dwords at
//    [esp+0x40]/[esp+0x10] and movsx word reads) is 35% in this register
//    family, as before.
// Space Bunny Free 2026-10-02: still BEST 55.9% (419/423), no MATCH. Roughly
// 230 further shapes scored with check.py --sym from a generator harness (build/
// scratch/0x4b91b0/gen.py) and every one landed on 55.9%, 54.5%, 53.8%, 53.3%,
// 52.6% or below. tools/permute.py also ran a full 15 minutes (1116 candidates)
// and stayed at 55.9%. What I established beyond the notes above:
//  - The register family is a hard two-way split, and the ONLY shapes that reach
//    the original's family (w->ebp, h->esi, pitch->edi, count->ebx, f spilled
//    to the dead arg1 slot) are `count = pitch * h` with `pitch` a NAMED local
//    declared before it, and they all cost 2-3 bytes and drop to 52.6%. The
//    shapes that keep 55.9% (count = (w*2)*h declared first) fold to
//    `imul esi, ebp` and put count in h's register. Confirmed by dumping the
//    .asm prologues directly (build/scratch/0x4b91b0/prog.py): no variant
//    reaches better than 3/6 prologue markers, and the two that get
//    pitch->edi are the 52.6% family.
//  - `double scale = lens;` must stay the FIRST statement: declared last it
//    costs 8 points (47.2-47.4%), which is the fild/fstp pair moving ahead of
//    the pitch/count multiply.
//  - hw and hh must be `short`, and the `f->half_width`/`f->half_height` stores
//    must repeat `(short)(w / 2)` / `(short)(h / 2)` rather than reuse the
//    locals: int locals with (short) casts score 34-37%, using the locals 54.5%.
//    Confirmed over every hw/hh typing in {short, int, unsigned short} x {store
//    from local, store from expression} x {cast or no cast in the loop}.
//  - The loop must keep `for (y = 0; y < h; y++, base += w)` with the inner
//    `for (x = 0; x < w; x++)`: while/do-while forms, a shared `int idx`
//    (which also flips the pre-call family and then collapses the loop into a
//    walking pointer, 34.7%), `unsigned short* cells = f->cells` in the loop
//    (41.1%), and hoisting dy/dy2/thresh into an explicit `if (w > 0)` block
//    (37.6%) are all worse.
//  - Moving any per-cell work into a `static inline` helper (SetCell, CellValue,
//    Falloff, and a NotDone loop condition helper) costs 12-31 points: the
//    expansion changes the frame. The 0x4c6f80 alloc+init helper is also worse
//    here (52.6% in all three argument shapes I tried), because this function's
//    own frame is already at the 9-dword maximum.
//  - The alloc+init helper form is NOT the missing piece here, unlike at
//    0x4c6f80: with the helper, h loses its register again.
//
// UNTESTED leads for whoever picks this up:
//  - The `add eax, ebx` for `f->end` (the original adds count INTO the data
//    register; ours emitted `add esi, eax`, the other way round) IS fixed by an
//    `unsigned short* p = f->cells;` local feeding the data and end stores, and
//    it is score-neutral (55.9% either way), so the file below now carries it.
//    But the same local used by the LOOP as well costs ~20 points (35-41%): MSVC
//    then rewrites the loop into a walking pointer.
//  - `sub esp, 0x24` matches and both keep 9 dwords of locals, so the frame is
//    right; the residual is purely which value gets which callee-saved register
//    across the alloc call.
//  - The struct's declared shape does not drive the register choice: reordering
//    it into a nested tail, aliasing it through a second struct with the same
//    layout, or writing the pitch field through a `unsigned char*` base are all
//    55.9%. Only making `cells` a separate `unsigned short*` MEMBER (instead of
//    a trailing array) is different (41.6%), and worse.
//  - Adding extra long-lived locals (copies of `w` and `h` used as the loop
//    bounds) does not push MSVC over the edge into the 4-way split either: all
//    such variants stay at 55.9%. So "the original has more values live across
//    the call" is not the lever either, and the choice really is decided by the
//    order the multiply operands are written in.
//
// deepseek-v4.1-flash 2026-10-01 (session 2): BEST 55.9% (419/423), up from
// 54.5%. The win is the outer loop written as a for with TWO induction
// variables in its head, `for (y = 0; y < h; y++, base += w)` (the guide's
// "Loops with several induction variables" item); base += w as a body
// statement scores 54.5 and y/base both in the for-init scores 52.6.
//
// What still differs is the one register rotation the notes below describe,
// and I mapped its mechanism exactly this session:
//  - The pre-call family is decided by whether f (the alloc result) keeps a
//    callee-saved register. Every spelling where the else branch computes the
//    store index ONCE (`int idx = base + x` used both as `f->cells[idx]` and in
//    the value subtraction, or `*(f->cells + idx)`) flips f to a stack spill
//    (the original's shape: `mov [esp+0x38], ecx` right after the call, stores
//    through ecx) and puts h in a callee-saved register; but MSVC then
//    restructures the loop into a walking pointer (`mov [edx], cx; add ecx, 2`)
//    and the score collapses to 35-37%. Every spelling without that shared idx
//    keeps f in edi, leaves h to be reloaded from its arg slot, and folds the
//    0x18 header offset into the base accumulator (base = 12), all as the
//    old notes describe. The original CSEs base+x across the store address and
//    the value subtraction WITHOUT a named idx (its falloff path is
//    `add esi, edi` with esi = base, then `mov esi, [esp+0x18]` to restore
//    base), and no spelling here reproduces that CSE: the compiler splits the
//    subtraction into -x and -base instead.
//  - The prologue's `lea edi,[ebp+ebp]; mov ebx,edi; imul ebx,esi` is
//    count = pitch*h with pitch a variable (no reassociation); the
//    (w*2)*h family that scores best here always reassociates to (w*h)*2
//    (`imul esi,ebp; shl esi,1`) which clobbers h's register. The pitch*h
//    spellings give count=ebx but pitch=esi and h never loaded, and score
//    52.6. `unsigned short pitch` + `f->pitch = pitch / 2` (or
//    `f->pitch = f->pitch / 2`) is what emits the 16-bit `shr di,1`;
//    `f->pitch = (unsigned short)(pitch / 2)` with int pitch gives a 32-bit
//    sar instead.
//  - A `unsigned short* p = f->cells;` local feeding the data/end stores fixes
//    the end-pointer add direction (`add eax, ebx` into the data register, as
//    the original); without it MSVC emits `add esi, eax` the other way round.
//    On this base it is score-neutral (55.9 either way) because the h reload
//    lands between the add and the end store.
//
// Tried this session, all scored free with check.py --sym (none beat 55.9):
// ~70 full-function variants. Structure: for-head base in 3 forms; idx/src
// temps in the else branch (with and without the for-head), idx only in the
// subtraction, cell pointers, `*(f->cells + idx)`, value-into-local, 5 value
// groupings including sequential accumulation and (dx/g)+(hw+term); init:
// p-local with data/end/height in 6 store orders, height first (54.3), the
// 0x4c69f0-style static inline Init helper; halvings in 6 statement orders,
// hw/hh as int with (short) casts (32.3), hw int + hh short and the reverse
// (the target's `sub eax, ebx` for dx = x - hw is an untruncated int hw read
// while `movsx ebx, word [esp+0x40]` is a (short) cast in the value, so hw
// and hh may genuinely be differently typed, but every typing here scores
// worse while the register family is wrong); count = pitch*h / h*pitch /
// (w*2)*h / w*h*2 / w*h+w*h / (w*h)*2 / h*(w*2) / pitch*h via ushort height
// locals; scale declared first/middle/last (first is best: 55.9); thresh in
// the outer body (41.3, much worse); while-loop inner (55.9, no change).
// tools/permute.py: run 1 died (started with plain python3 instead of uv run),
// run 2 (on the old 54.5 base) stayed at 54.5, run 3 (on this 55.9 base,
// 10953 candidates) stayed at 55.9 with only this 2-line statement move as its
// best (the hh declaration moved between the pitch/2 and half_height stores;
// same check.py % but its fine score dropped 3514 -> 3259, so the instruction
// alignment is closer). Its output is in the MAIN checkout's
// build/permute/0x4b91b0/ (permute.py resolves its root from __file__), not
// this worktree's; re-derive from there if useful.
//
// STRUCTURE CORRECTION (the "Cavedog slip" note below is wrong): the field at
// +0x14 is not a one-past-the-end pointer, it is a SECOND buffer pointer.
// count = 2*w*h BYTES is the size of ONE buffer (w*h shorts), the allocation
// is 0x18 + 2 buffers (count*2 + 0x18) and the loop fills exactly the first
// (w*h cells). [ecx+0x10] = f+0x18 and [ecx+0x14] = f+0x18+count are the two
// buffer starts, matching Image_004b95a0 (data +0x10, mask +0x14) and
// Sprite_004b9360 (buffers[2] at +0x10). Nothing is over-allocated, so do
// not report a bug for it.

// mimo-v2.6-pro 2026-10-01 continuation: STILL 54.5%, could not break it. The
// residual is one hard register rotation rooted entirely in the pre-call, and I
// confirmed it is a genuine two-way dichotomy with NO source spelling in
// between. Expressing the size as count=pitch*h (named pitch) gives count=ebx
// (correct) but pitch=esi and h spilled to [esp+0x3c] (imul ebx,[mem]), f=edi.
// Expressing it as count=(w*2)*h / w*h*2 / (w*h)*2 / w*h+w*h / (w*h)<<1 / a
// named wh then *2, ALL fold to imul esi,ebp; shl esi,1 (h loaded to esi in the
// prologue, matching the original's mov esi,[esp+0x38], but the w*h product
// clobbers esi so h is destroyed; count=esi, pitch=ebx, f=edi). The original
// wants lea edi,[ebp+ebp]; mov ebx,edi; imul ebx,esi, i.e. count=pitch*h with
// BOTH pitch(edi) and h(esi) already in callee-saved registers and the product
// landing in ebx. That needs h to win esi ahead of pitch so pitch shifts to edi
// and f spills to the dead arg1 slot. I could not make h win esi.
// Tried this session, all scored via check.py --sym (none beat 54.5): the
// documented static-inline alloc+init helper (0x4c6f80 pattern) in three forms
// (helper taking w,h and computing pitch/count inside = 50.7; helper taking
// pitch,count,h = 51.2; helper taking w,h,pitch,count), all reproduce the same
// dichotomy rather than flipping it; x+base vs base+x in the store and value
// (54.5, no change at all); f->data addressing instead of f->cells (39.6, worse);
// count=h*pitch vs pitch*h and h*(w*2) (51.2, operand order is not the lever);
// scale declared before vs after the count/pitch pair (before is 54.5, after
// drops to 49.8); uninitialized decl order int pitch,count vs count,pitch and a
// Mul(a,b) inline multiply helper (all 51.2 or 54.5); and the guide's
// register-priority trick of an extra folding use of h (int hy=h;(void)hy,
// +0*h in the product, int H=h) which changed nothing (pitch stays in esi).
// The pre-call fix alone would fall through the whole function because every
// later hunk is that same rotation cascaded. Not a proven plateau: the missing
// construct is one that makes h a callee-saved register candidate winning esi
// over pitch while the count product lands in ebx. Closest shapes (count=pitch*h)
// are 51.2 with the product in ebx but pitch in esi and h spilled.
// Older note (mimo-v2.6-pro first pass): BEST 54.5% (419/423 bytes). The threshold is a plain
// signed `w / 4` (the cdq; and edx,3; add; sar idiom), NOT (w+3)/4: earlier
// notes were wrong there and the extra lea cost 3 bytes. What still differs is
// the same register rotation documented below: the original holds w/h/pitch/
// count in ebp/esi/edi/ebx across the alloc call and spills f to the dead arg1
// slot, while this form gives count esi and f edi. Also re-derived the loop
// reads: the movsx-from-word reads of the hw/hh slots mean (short) casts on int
// locals in the dy/dx/value expressions (int locals with casts reproduce the
// movsx word reads and the dword fisubr; plain short locals read back as dwords
// and score lower). Tried this pass, all worse than 54.5: pitch/count/scale
// declared in all 6 orders around `int pitch = w*2; int count = pitch*h;`
// (34.3 each, count then uses h from memory and pitch takes esi), int hw/hh
// with (short) casts on top of that (34.3, and MSVC folds the 0x18 header
// offset into the base accumulator, base starting at 12, because the value
// subtracts the same base+x the addressing uses), and a value expression
// reordered to evaluate the (dy/g+hh)*w term first (the fold variant above).
// Untested levers worth trying next: an inlined alloc-and-init helper as in
// 0x4c6f80 (the stores-before-null-check strongly suggests one, and it is the
// guide's known way to put size arguments in callee-saved registers), and
// spelling the value's index as x+base against an address of base+x to defeat
// the 0x18 fold.
// Retry #2441: BEST 53.1% (420/423 bytes), no MATCH after 5 scored checks and 2 compile-failed edits. Residual is register/frame allocation: target keeps w/h/pitch/count in ebp/esi/edi/ebx across allocation and spills result; tested declaration order, a height alias, and pitch*h without improvement. Return to the prior partial; recheck if continuing.

// Retry #1952: GPT-6.1-sol tested pitch/count declaration order and equivalent
// products. The saved 53.1% (420/423) variant remains best; no MATCH. The
// prologue still puts the 2*w pitch in ebx/esi work registers and clobbers h,
// while the original keeps pitch in edi, count in ebx, and h in esi across the
// allocator call. Loop locals also remain in different registers and stack slots.
// deepseek-v4.1-flash (third pass): declaring double scale = lens; BEFORE the
// count/pitch pair lifts the score from 51.7 to 53.1 percent (best so far). The
// first diff hunk is still the allocator: the original holds w in ebp, h in esi,
// pitch (2*w) in edi and count (2*w*h) in ebx across the alloc call and spills f
// to the arg1 slot; every spelling here instead computes count in esi
// (imul esi,ebp / shl esi,1), takes ebx for pitch and keeps f in edi, reloading h
// from its argument slot. To close it, h must win esi over the count temp: all
// pre-block orderings (scale, count, pitch, f permutations; w*h*2, (w*2)*h,
// h*(w*2)) give 51.7 or 53.1 and never move count out of esi.
// deepseek-v4.1-flash (second pass, same session model): re-confirmed the
// prologue is a genuine plateau, and pinned what does and does not reach the
// allocator here. 24 further spellings of the pitch/count pre-block
// (h*2*w, 2*h*w, h*(2*w), pitch*h, count*=pitch, w*h then shl, += doubling,
// unsigned, separate assignments) all give the identical 420-byte body at
// 51.7 percent; so do 6 loop-head spellings (x/y/base declared at function
// scope, base as unsigned, while loops, x+base vs base+x). The loop spelling
// therefore never reaches the allocator. The only spellings that move count
// into ebx (as the original has it) are the y*w+x index or pitch*h, but each
// then takes pitch into esi and either spills h (reloaded from [esp+0x38]) or
// puts it in ebx, so the four callee-saved slots come out permuted. int hw/hh
// with (short) casts regresses to 35.1 percent, so the short locals are load
// bearing. Defining the real preceding function (0x4b90a0) above this one in
// the scratch file changed nothing (still 51.7), as did <string>, <vector>,
// <map>, <list> and <iostream>.
// deepseek-v4.1-flash: the falloff quotient's SIGN was wrong. The disassembly's
// `fisubr dword [esp+0x40]` computes `half_width - dist`, and `fdiv st,st(1)`
// divides that by `scale`, so the correct expression is
// `g = (hw - dist) / scale` and the value is
// `hw + ((int)(dy / g) + hh) * w + (int)(dx / g) - (base + x)` (hw added
// before the dy term, matching `add ecx,edx`). With the sign fixed the score
// is unchanged at 51.7 percent, which confirms the remaining gap is only the
// register rotation, not the arithmetic. The upstream cause is unchanged: the
// original holds h in esi, pitch (2*w) in edi and count (2*w*h) in ebx across
// the alloc call; every pre-block declaration order tried here either drops h
// into memory (pitch takes esi) or reuses esi for count. Sweeps of the
// pre-block (25 orders), the whole pitch/count/size spelling, and
// tools/headers.py (128 sets, flat 51.7) did not move it.
// PARTIAL, 53.1% (420 of 423 bytes). The 146-instruction
// original is matched instruction for instruction in outline, so what is left
// is register and slot placement, not missing code. About ninety shapes were
// scored with check.py --sym; this is a strong partial and NOT a proven
// plateau, the register-allocation question below is under-explored.
//
// Corrected from the previous attempt's notes, which were wrong in two ways
// worth recording:
//  - hw and hh are ONE halving, `w / 2` and `h / 2`, not `w/2/2`. The
//    disassembly's `cdq; sub eax,edx; ... sar reg,1` is a single signed
//    halving, and reading it as two cost several points. Fixing the semantics
//    alone was 45.6% to 46.3%.
//  - The frame size now MATCHES: both this file and the original are
//    `sub esp, 0x24`, and `w` is in ebp as in the original. The previous
//    attempt's 0x28 frame and its "the value needs one local more than the
//    original's nine" conclusion are both superseded.
//
// What this builds: the "lens" image that 0x420620 asks for with (22, 22, 8).
// A 0x18-byte header followed by a w by h array of 16-bit cells, each either
// 0x7d00 (a hard '}' mark) or a short built from two integer divisions of
// (dist - half_width) by the falloff argument, plus the header's pitch, height
// and half-width/half-height fields.
//
// Derived, with the evidence:
//  - The header field at +0 is written twice, 2*w right after the allocation
//    and w after the null test, and the second store is `shr di, 1` on the
//    16-bit register, so it divides the first value by two rather than
//    reloading the parameter.
//  - The end pointer at +0x14 is `(char*)data + 2*w*h`, byte arithmetic on the
//    doubled width times the height, while the allocation is
//    `2*(2*w*h) + 0x18` bytes. Both come from one shared 2*w*h value, which is
//    why the original computes `lea edi,[ebp+ebp]`, `mov ebx,edi`,
//    `imul ebx,esi` and then `lea eax,[ebx+ebx+0x18]`, and reuses ebx for the
//    end. The loop only writes w*h cells, so the allocation is twice what the
//    cell array needs while the end pointer is exactly right: one of the two
//    is a Cavedog slip.
//  - +0x9, +0xa and +0xb are zeroed as three separate bytes and +0x8 is left
//    alone, and the null test floats up beside the `xor eax,eax` that feeds
//    them, so the source stores the header first and tests afterwards.
//  - hw and hh are `short`: stored truncated to words, sign extended at every
//    use (`movsx ecx,dx` in the preheader, `movsx ebx,word [esp+0x40]` in the
//    loop) but the falloff subtraction is `fisubr dword`, the promoted value
//    in the dead third parameter slot.
//  - The falloff argument becomes a double once at the top and stays on the
//    x87 stack across both loops, popped by the `fstp` at the common exit.
//  - (int)dist keeps the sqrt on the stack (`fld st(0)` before _ftol) so the
//    else branch subtracts half_width from it without reloading.
//  - (w+3)/4 and dy*dy are loop invariant and sink into the `if (w > 0)` body
//    that a for loop lowers to, ahead of the x loop's own preheader; dy2 keeps
//    a copy in ecx that the back edge reloads.
//  - The value is `(a + hh) * w + b + hw - (base + x)`: the compiler forms
//    half_width + (a + half_height) * w first (declaration order), adds b,
//    then subtracts the cell index.
//
// What still differs:
//  - The original computes the doubled width before the height product and
//    keeps it in edi across the call; a named pitch local is the only spelling
//    that stops MSVC folding the doubling into the size, and that declaration
//    also costs the parameters ebp and esi, so here the count is declared
//    first and MSVC emits `imul esi,ebp` / `shl esi,1`.
//  - The original spills the frame pointer to the dead first parameter slot
//    and reloads the height from the second; here the frame stays in a
//    register and the width is re-read from its own parameter slot, so the
//    loop rotates (base in ebx, the x counter in esi), MSVC folds the header
//    offset 0x18 into the index (base starts at 12 with a matching +12 in the
//    value) and every local slot moves.
//  - Frame size 0x28 against the original's 0x24: SUPERSEDED, both are 0x24
//    now. The old conclusion that "the value needs one local more than the
//    original's nine" was wrong; the frame was right all along once the
//    semantics were fixed. What the original is doing instead is holding four
//    values live across the call in the four callee-saved registers (ebp = w,
//    esi = h, edi = pitch, ebx = count) and spilling f to the dead first
//    parameter slot at [esp+0x38]. That is the single largest remaining hunk
//    and the one lever not yet exhausted: make h genuinely live across the
//    call, by touching it in a way that does not merely reload it, and MSVC
//    should be forced into esi. Getting four registers live would in turn make
//    f spill, which is what the loop's base addressing is waiting for.
//  - The loop body differs in register rotation and indexing. The original
//    uses esi as the running base and edi as x, addressing the cell as
//    `(f + 0x18) + index*2` with base starting at 0; this file folds the 0x18
//    into base (starting at 12, with a matching +12 in the value) and uses a
//    countdown `inc esi; dec ecx; cmp esi,edi` where the original has a plain
//    `inc edi; cmp edi,ebp`. Routing through a `cells` pointer local or
//    through `f->data` both scored far worse (38% to 40%), so the folding is
//    MSVC's own choice for the current source shape, not a spelling error.
//  - The `f->half_width` and `f->half_height` stores repeat the expressions
//    `(short)(w / 2)` and `(short)(h / 2)` rather than using the `hw` and `hh`
//    locals. That duplication is load-bearing: using the locals scores 50.3%
//    against 51.7%. Two other spellings score the same 51.7%, so the
//    duplication is required but its exact form is not pinned down, which
//    suggests the original's shape is something else again that frees the
//    same register.
//
// Tried, with no better result (each scored on check.py from a scratch copy):
// the size as count*2, count+count, (w*2)*h*2, 2*((w*2)*h) and with the
// constant at 0x30; the count initialised as (w*2)*h, w+(w)*h, h*(w*2),
// h*pitch and pitch*h; count declared before and after pitch, both as one
// `int a, b;` declarator and as two assignments; pitch as an int, an unsigned
// and an unsigned short; the end pointer through data, through cells, and as
// short-pointer arithmetic (which folds to f + size and is wrong for the
// original's byte offset); hw and hh as int with (short) casts and as short
// locals; the value as one expression, with a named int, with a named short,
// with a named index, with no index subtraction and with -base - x; a cells
// pointer local; the height store moved before the data and end stores; and
// the threshold inlined in the comparison. Those spellings were a plateau at
// 45.6% and are no longer one: the semantics fix plus the value expression
// above reached 51.7%, which shows the plateau was an artefact of a wrong
// reading rather than a wall. Treat any future plateau here with the same
// suspicion and re-derive from the disassembly before believing it.
#include <math.h>

struct LensFrame_4b91b0 {
    unsigned short pitch;      // +0, the row stride in bytes, then the width
    unsigned short height;     // +2
    short half_width;          // +4
    short half_height;         // +6
    char field_8;              // +8, left alone
    char field_9;              // +9
    char field_a;              // +a
    char field_b;              // +b
    char unknown_c[4];         // +c
    unsigned short* data;      // +0x10
    char* end;                 // +0x14, one past the last cell, in bytes
    unsigned short cells[1];   // +0x18
};

extern void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// FUNCTION: 0x4b91b0
unsigned char* __stdcall FUN_004b91b0(int w, int h, int lens)
{
    double scale = lens;
    int count = (w * 2) * h;
    int pitch = w * 2;
    LensFrame_4b91b0* f = (LensFrame_4b91b0*)FUN_004d83b0("LensFrame", count * 2 + 0x18);
    unsigned short* p = f->cells;
    f->pitch = (unsigned short)pitch;
    f->data = p;
    f->end = (char*)p + count;
    f->height = (unsigned short)h;
    f->half_width = 0;
    f->half_height = 0;
    f->field_9 = 0;
    f->field_a = 0;
    f->field_b = 0;
    if (!f)
        return 0;
    short hw = (short)(w / 2);
    f->pitch = (unsigned short)pitch / 2;
    f->half_width = (short)(w / 2);
    short hh = (short)(h / 2);
    f->half_height = (short)(h / 2);
    int base = 0;
    int y;
    for (y = 0; y < h; y++, base += w) {
        int x;
        for (x = 0; x < w; x++) {
            int dy = y - hh;
            int dy2 = dy * dy;
            int thresh = w / 4;
            int dx = x - hw;
            int d2 = dx * dx + dy2;
            double dist = sqrt((double)d2);
            if ((int)dist >= thresh) {
                f->cells[base + x] = 0x7d00;
            } else {
                double g = (hw - dist) / scale;
                f->cells[base + x] = (unsigned short)(hw + (((int)((double)dy / g)) + hh) * w + ((int)((double)dx / g)) - (base + x));
            }
        }
    }
    return (unsigned char*)f;
}