// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, GPT-6.1-sol and Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PASS 14 (Space Bunny Free, 2026-10-02): 54.5 -> 58.1 percent (646 bytes, unchanged size), and the
// entry register finally lands on EDX. THE LEVER IS THE NUMBER OF REFERENCES TO THE THIRD PARAMETER,
// not its live range and not the clamp or any statement order: take the address of `order->pos` ONCE,
// as a `char* const pbase`, and reach pos.y, the timestamp and the owner pointer through it
// (`pbase + 4`, `pbase + 0x24`, `pbase - 0x14`). That leaves `order` referenced only at the top of
// the body, and MSVC 5 then gives it EDX and never enregisters it again, exactly as the original
// does at 0x438c3d / 0x438cbd / 0x438d41.
//
// Why this works when the earlier "hoist the owner read" shapes did not: hoisting moves the load to the
// top and keeps a *copy* live, which costs a frame slot. Reaching it through a pointer costs nothing,
// because the pointer is the same `&order->pos` the original already materialises and spills at
// 0x438c3d. Measured on this baseline, all four combinations of {pos.y, timestamp, owner} through the
// pointer: all three 58.1, timestamp+owner only 53.3, owner only 47.3, timestamp only 46.5, none
// 54.5. So pos.y through the pointer is what carries it, and the other two are close to free on top.
// `*out = *(Vec3f*)pbase` instead of `*out = order->pos` is byte-identical (58.1 either way).
//
// Still true from the older passes: the bitfield flag test is worse than the mask at this baseline
// (55.9 against 58.1), and relaying the mid-body projection reads is all 52.8 to 57.0.
//
// WHAT THE PROBE NOW SAYS ABOUT THE ALLOCATOR (build/scratch/0x438c00/filter.py). The next wall,
// after the entry register, is that the original keeps `surface` in a callee-saved register and ours
// reloads it from its argument slot at every one of the eight calls. The counts: the original loads
// surface once into EBX at 0x438da0 and pushes EBX eight times; ours pushes EDX, EAX, ECX or EDI each
// time. The reason is visible one step earlier, in which value EBX holds. The original carries
// ax in EBP and bx in EBX, so EBX is dead after the `mov [esp+0x14], ebx` spill of dx at 0x438d34
// and is free for `surface`. Ours carries ax in EBX and bx in EBP, so EBX is live to the last call.
// MSVC picks EBX for `scroll_y` (0x438c74) and then, after scroll_y dies, for `world.lo.x.whole`.
// The original instead recycles EBP twice in the middle: px is in EBP at 0x438c23, then pz overwrites
// it at 0x438c7d, then `world.lo.x.whole` lands in it at 0x438c83; and ECX twice, from &order->pos at
// 0x438c3a to `view` at 0x438c5e to hi.x.whole at 0x438c94. So the target is not "get surface into a
// register" but "make scroll_y not land in EBX", which is a statement-order question about the seven
// projection statements. All 5040 orders are being re-swept at this baseline (PASS 12 swept them at
// the old 52.7 one, before the entry register was fixed, so the old result does not carry over).
//
// The harness that found all of this is build/scratch/0x438c00/: probe.sh drives it (facts = compile
// only and read the entry register, about 3 s; fast = score a directory in parallel; filter = compile
// only and count structural features; regs = which register each call pushes; cmp = side-by-side with
// the original; wparts = the order the five whole-part reads land in). A compile-only iteration instead
// of a 60 s check.py one is what makes a several-hundred-variant sweep affordable, and the previous
// passes' sweeps were all done at the older, lower baselines.
//
// MEASURED AT 58.1 AND ALL WORSE, so the next pass need not repeat them (this pass is about 90
// variants, every one compiled and scored with check.py's own comparison):
//  * the projection statement order. All 5040 orders re-swept at THIS baseline: the best two are
//    `half, sy, sx, az, bz, ax, bx` (what the file has) and `half, sy, az, sx, bz, ax, bx`, both
//    58.1, third place 57.6. So that axis is closed again and needs the allocator to move first.
//  * compiler state: N = 0..129 dummy file-scope `extern int` declarations, flat at 58.1.
//  * headers: windows.h 54.5, windows.h alone 54.5, string.h 57.2, math.h 57.2. Ruled out.
//  * the bitfield flag test 55.9, so the mask stays.
//  * the box as five plain ints, or as a struct of five ints instead of the Fixed struct: 25.0 both,
//    617 bytes. The whole parts MUST be read back out of memory with movsx, so the box has to be a
//    real Fixed struct in the frame.
//  * a union over the two late reads (the brief's technique 8) to force an invalidation: 43.8
//    through the pointer, 46.5 reading `order` directly. The reloads are not an aliasing effect.
//  * reading `view` before the box is built (57.2), after the lo.z store (56.8) or after the lo.y
//    store (55.0), so the original's mid-box `mov ecx, [esp+0x48]` is a scheduler outcome, not a
//    source order.
//  * scroll_y read after scroll_x (56.8), inlined into the az expression (56.8), subtracted as its own
//    step (56.3), the 0x80/0x20 biases as their own step (56.3), and both orders of that (56.3).
//  * hoisting the timestamp to the top with the owner left late: 53.9. Hoisting both, or hoisting
//    the owner, or moving the whole colour block up with them: 44.5 to 47.6.
//  * the 36-way cross product of {pos.y, timestamp, owner, final copy} each spelled direct or
//    through the pointer (build/scratch/0x438c00/mk3.py, v3/): four members tie at 58.1, namely both
//    spellings that put timestamp and owner through the pointer, and none beats it. So the choice of
//    which reads go through `pbase` is settled: it must be the timestamp and the owner.
//
// PASS 14b (Space Bunny Free, 2026-10-02): 58.1 -> 58.3 (654 bytes). tools/permute.py (seed 1, 15 min,
// --jobs 4) found one real edit among the two it returned: compute `dz` before `dx`. It also added a
// named single-use temporary for `bx` (`int tmp0 = ...; int bx = tmp0 + 0x80;`), which is the
// permuter's own noise and is worth nothing: measured on its own, bx-temp 58.1, ax-temp 58.1,
// az-temp 58.1, dz-first alone 58.3, both 58.3. So the 0.2 is entirely the division order, which is
// also what the original does (the vertical `mov ecx, edi / sub ecx, esi` at 0x438d13 feeds the first
// `imul ecx`, the horizontal `sub ecx, ebp` at 0x438d07 the second).
//
// PASS 13 (claude-sonnet-5-5, 2026-10-02): 52.7 -> 54.5 percent (646 bytes). NEW LEVER: name the two
// vertical-gap sums `ty = az + dz` and `by = bz - dz` as locals declared right after `dz`, and use them in
// calls 3, 4, 7 and 8. Declaring them next to their first use (between the calls) is worth nothing; right
// after dz is 54.5, naming rx = bx - dx right before the calls is 54.0, naming all four is worse (52.0).
// The entry ecx/edx swap and the bx-in-ebp (original keeps surface in ebx, spills bx) wall are unchanged.
// A DrawFrame(g=1 / g=0) inline helper for the eight calls is byte-identical-ish (52.2). Expressing ax/bx/az/bz
// as macros breaks the frame size (0x34).
//
//
// PASS 12 (Space Bunny Free, 2026-10-02): 52.7 percent, up from 52.2. One real
// gain, one real closure, and one solid explanation of the wall. All measurements
// below are in build/scratch/0x438c00/ (probe.py, probe2.py .. probe5.py compile
// small probe functions with /Fa so the entry register is readable; sweep.py runs
// variants through check.py in parallel, one directory per variant so the objects
// never collide).
//
// 1. COMPILER STATE IS NOW CLOSED, not just unswept. The earlier passes only went
//    to N = 39 dummy `extern int` declarations, and the matched siblings show the
//    matching window can sit at N = 92..404 (0x47dfc0) or 43..298 (0x4399f0), so
//    the old sweep could easily have stopped short of it. Swept N = 0..129 step 1
//    and N = 130..616 step 6, 212 variants: flat at 52.2 with dips to 51.8 at
//    N = 2, 3, 10, 11, 58, 67, 91, 184, 220, 256, 292, 328, 364, 400, 514, 538, 586
//    (a mod-8 micro effect), and every variant is the same 647 bytes. So the
//    earlier "flat at 50.2" note was right in substance but had not been tested
//    anywhere near the window its siblings needed.
//
// 2. THE ENTRY REGISTER IS REACHABLE, and the probe pinpoints what decides it.
//    0x419be0 and 0x49c9c0 hit the same family of wall (a parameter read back from
//    its home slot instead of from its register), so the useful question is what
//    moves VC5's parameter ranking. Compiling a batch of minimal stdcall functions
//    through /Fa gives, for this exact signature shape:
//      1 or 2 uses of `order`          -> `mov eax,[esp+0xc]`
//      the full body without the owner -> `mov edx,[esp+0xc]`  (the original)
//      the full body as written here   -> `mov ecx,[esp+0xc]`
//    So the original's EDX is reachable and `order->owner` is the single use that
//    tips `order` from the second to the first choice. Removing it, or hoisting
//    ONLY that read to just after the guard (`int sel = order->owner->flags.bits.b4;`,
//    `Unit* owner = order->owner;`, or hoisting just `order->timestamp`), all give
//    EDX; every other owner shape (a test flag local, an owner local, a flags
//    local, the bitfield, the mask, a static inline) still gives ECX, and so does
//    every hoisting that leaves the raw parameter in the late test.
//
// 3. WHY THE EDX SHAPES SCORE WORSE, and why no shape can fix it. The original's
//    `order` is not a register variable at all: its argument slot [esp+0x4c] is
//    never written before 0x438d45, `order` is rematerialised from it at 0x438cbd
//    and 0x438d41, and only then is the slot reused for `color1` (0x438d67). That
//    is why `level` lands in the view slot in the original and in the order slot
//    here. Hoisting the owner read does buy EDX but forces the hoisted value into
//    local+0x00 (46.7 percent for `int sel`, 47.9 for `Unit* owner`, 48.1 for
//    `owner` with the mask, all worse than 52.2), and it moves the load to the top
//    of the body where the original has `and eax,0xffff / push edi`. So the only
//    shapes that reach the register are the ones that cannot keep the late read,
//    and the only shapes that keep the late read keep the register. Adding an
//    extra reference to `world`, `def` or `view` pushes `order` out to a callee
//    saved register instead (esi, loaded after the pushes), which is a third shape
//    and no better. This is the same wall as 0x419be0's `mov ebp,[esp+0x34]`: no
//    construct tried (local copies, `Order&`, `Order* const`, `&order`, pointer
//    casts, inline helpers by value, pointer or reference) makes VC5 keep a
//    parameter in memory. Treat the entry register as allocator behaviour.
//
// 4. THE GAIN. Reading `half` (the `world.lo.y.whole >> 1`) BEFORE the two scroll
//    reads is worth +0.5. All 5040 orders of the seven projection statements were
//    measured, against both the mask and the bitfield form of the flag test
//    (10080 variants): the two winners are
//      half, sy, sx, az, bz, ax, bx   52.7
//      half, sy, az, sx, bz, ax, bx  52.7
//    and nothing else beats the old 52.2, so the projection order axis is closed.
//    Also measured and no better: all 120 box-store orders (best 52.2), all 6
//    pos-read orders (all 52.2), the colour block before or after the two
//    divisions (49.7 / 49.4), the colour block at the top (does not compile),
//    declaration order of level/dx/dz and colour1/colour2 (all 52.2 or 50.9),
//    a `void* surf = surface;` local for the eight calls (52.2), inlining the two
//    scroll reads (47.9), and six spellings of the clamp beyond the inline-cast
//    form (48.7 to 52.2). The one thing PASS 10 found is still true and still
//    worth its 1.0 point of loss: the flags test must be the mask, not the
//    bitfield, at this baseline (52.2 against 50.9).
//
// 5. WHAT STILL DIFFERS, unchanged from the older notes: the single ecx/edx
//    assignment at 0x438c00, and the whole downstream cascade follows from it.
//    Ours is 647 bytes against the original's 660. The 13 missing bytes are the
//    original's version of the same facts: the `mov [esp+0x2c],ecx` spill of bx,
//    the two `mov edx,[esp+0x4c]` rematerialisations of `order`, the
//    `mov edx,[esp+0x48]` reload of level at the clamp join, the `mov ebx,edx` /
//    `mov ecx,edx` pair in the two division tails, the `mov [esp+0x14],ebx` spill
//    of dx, the `mov [esp+0x5c],ecx` that puts bx + 1 over the dead surface
//    argument, and the extra byte of `shr ecx,4` in the bitfield extract. Ours has
//    the matching extras: seven reloads of `surface` instead of one load into ebx,
//    the `mov edx,ebp` / `sub edx,ebx` pair, and the `order->owner` load hoisted
//    into the middle of the first division.
//
//
// PASS 11 (claude-sonnet-5-5, 2026-10-01): 52.2 unchanged. Measured, none above 52.2:
//  * tools/permute.py 15 min (--jobs 4): 495 candidates, no gain.
//  * `Vec3f* pp = &order->pos` used for the py read and the final copy, with all 120 orders of the
//    five box stores: best 51.0 (the stores in source order). It reproduces `mov edx,[ecx+4]`
//    but spills pp at local+0x0c instead of local+0x10, and still enregisters order in ecx.
//  * sy/sx read first thing, after the type test, after def, or before the box: 44 to 49.
//  * az/bz written as z-sy-half, z-(half+sy), z+0x20-half-sy, ...: byte-identical (the
//    compiler canonicalises the sum); the original's (z-half)-sy order is a scheduler outcome.
//  * a real extra use of `index` late in the body is the only thing that ever gave
//    `mov edx,[esp+0xc]` at entry (index>>20 folded into the first call's x argument), but the
//    index then lives in cx and the score is 41.
//  * making `order` address-exposed does not make MSVC reload it from its slot.
//  Reading of the original: `order` is only register-resident up to the `lea ecx,[edx+0x22]`; the
//  later uses (timestamp, owner) reload [esp+0x4c], i.e. the register file is full (ebp, edi, esi,
//  ebx, ecx=view, eax, edx all live at 0x438cb7) and order is spilled to its home slot there. Ours
//  keeps order in ecx because its order of the sx/sy subtractions frees registers earlier.
//
// PASS 10 (mimo-v2.6-pro, 2026-10-01): 52.2 percent, unchanged. NEW FACT, and
// the biggest single find of this pass: the flags test IS a bitfield extract.
// `if (order->owner->flags.bits.b4)` with a 1-bit field at bit 4 of a dword
// bitfield struct at +0x110 compiles to exactly the original's
// `mov reg, [reg+0x110]; shr reg, 4; test reg8, 1` (measured in
// build/scratch/0x438c00/flagstest.cpp, shapes t3/t4: the same extract as
// `shr reg, 4; and al, 1` when the value is used numerically, and every plain
// `(flags & 0x10)` / `(flags >> 4) & 1` spelling folds to
// `test byte ptr [..], 0x10` instead). This closes the "needs a shape not yet
// found" item from PASS 7. Standalone it still scores WORSE (50.9, see
// v1_bitfield.cpp): the extract lands in edx over the dz division result, so
// colour2's byte store sinks into the dx slot and level spills into local+0x00,
// while the original spills dz into the dead view slot and keeps colour2 at
// local+0x00. v15 (colour assignments reversed, 50.9) and v16 (bitfield with
// the dz division first, 51.0) do not recover it. Whoever fixes the entry
// register swap should keep the bitfield and re-check: the flags bytes then
// match for free.
// Entry-register probes (build/scratch/0x438c00/probe.cpp, five minimal
// __stdcall functions): a multi-use 3rd parameter loads into ECX at entry by
// default (`mov ecx, [esp+0xc]` in P1 to P5), and the index*585 multiply copy
// takes EDX when free and falls back to ECX when EDX is held (P2). So the
// multiply copy register is DERIVED from where `order` sits, not the other way
// round, and the original's entry EDX for `order` is the one unexplained
// choice (74 of 1031 functions in the exe load a stack arg into EDX at entry,
// but only this one loads [esp+0xc] there before a `sub esp`).
// Compiler-state sweep at the 52.2 baseline (N = 0..39 dummy `extern int`,
// step 1, finer than any earlier sweep): flat at 52.2 except dips to 51.8 at
// N = 2, 3, 10, 11, 26, 27 (a mod-8 micro effect). No declaration-count
// window at this baseline either.
// New source shapes measured this pass, none above 52.2: pos reads in order
// px, pz, py (52.2); `Vec3f* pp = &order->pos` with px/pz through order and py
// through pp (51.0); the same with pp formed after the px/pz reads (50.9);
// py read as `*(int*)((char*)&order->pos + 4)` (52.2, byte-identical); all
// three pos reads through pp in px, pz, py order (46.7); the dz division
// written before dx (51.1, 641 bytes); the def computed via an explicit
// `index * 0x249` char* multiply (51.8); `level * (bx - ax)` operand order
// (52.2, byte-identical); a `const Order*` parameter (52.2, byte-identical);
// pos declarations in order pz, px, py (51.3) and py, pz, px (51.3); all
// three pos locals in one declaration statement (52.2).
//
// PASS 8 (deepseek-v4.1-flash, 2026-09-30): 52.2 percent, unchanged. Two more
// pointer shapes for the pos reads both scored WORSE than the current 52.2:
//   `Vec3f* ppos = &order->pos;` used for all three pos reads   46.7 percent
//     (it also lengthens to 653 bytes; original is 660)
//   px and pz read through `order`, py read through that ppos    50.9 percent
// PASS 9 (deepseek-v4.1-flash, 2026-10-01): an `int* p = (int*)&order->pos;`
// alias for the three pos reads (p[0], p[1], p[2], and the py-first spelling)
// scores 46.7 percent / 653 bytes, worse than the direct order->pos reads, so
// that alias shape is closed too.
// Neither changes the top-of-body register rotation: MSVC still spends ECX on
// `order`. So the pos-pointer axis is now closed as well, and the residual is
// confirmed to be the single ecx/edx assignment at 0x438c00 (which then makes
// the original spill `order` and keep `view` in ecx, while ours does the
// opposite). No source spelling tried in eight passes has reached that choice.
// Best: 51.1% (unchanged; the pointer experiment below scored 49.9%). The push order in the original IS edi, esi, ebp, ebx and the pop order IS ebx, ebp, esi, edi, exactly as the build emits, so the residual is NOT a callee-saved rotation. The first difference is a single scratch-register swap at the top: the original loads `order` into EDX and puts the type-index copy in ECX, ours loads `order` into ECX and puts the copy in EDX. Everything after (which register holds level, which the two `imul`s scratch in, whether `surface` stays in ebx or is reloaded from its argument slot, and which argument slot each dead local lands in) follows from that one swap.
//
// SLOT MAP, decoded from the original and worth keeping (the earlier passes got
// this wrong in places). The prologue does `sub esp, 0x30` and THEN pushes
// edi/esi/ebp/ebx, so with B = esp after the four pushes the saved registers are
// at [B, B+0x10) and the 0x30 bytes of locals are at [B+0x10, B+0x40), i.e. a
// displacement D in the body is local D-0x10. That makes every stack slot in the
// original legible, and it confirms the struct shapes already used here:
//   local+0x00  colour2 (a byte store, later read as a dword)
//   local+0x04  dx, then reused for ix2 = bx - dx
//   local+0x08  ix1 = ax + dx
//   local+0x0c  iy1 = az + dz
//   local+0x10  &order->pos (the final `*out = *that` reads it)
//   local+0x14 .. local+0x30  the 28-byte box: lo.x, lo.y, lo.z, pad, hi.x,
//               hi.y, hi.z. The whole-part reads are at +0x16, +0x1e, +0x26,
//               +0x1a, +0x2e, which pins lo as a 16-byte Vec3q (x,y,z,pad) and
//               hi as a 12-byte Vec3f (x,y,z) whose y is never read.
// The dead argument slots are reused: the view slot (E+8) holds `level` and then
// dz, the order slot (E+0xc) holds colour1 and then colour2, and the surface
// slot (E+4) holds bx + 1.
// Draws the on-screen bounding box of the object's unit type. `order` is one of
// the per-unit list objects that 0x439b30 walks (type index at +0x36, 16.16
// position at +0x22, owner at +0xe, timestamp at +0x46). The box corners are the
// object position plus the UnitType bounds at +0x15e (0x249-byte entries in the
// array at g_game+0x1439b), projected to screen with (x - scroll_x + 0x80,
// z - (y >> 1) - scroll_y + 0x20). The box is then shrunk towards its centre by
// level/10, where level counts g_game->ticks up to 10 from the object's
// timestamp. Eight lines are drawn: the four sides of the inner rectangle in the
// owner's colour, then the same rectangle offset one pixel outward in the
// alternate colour. Finally the object's position is copied to `out`.
//
// PARTIAL, 51.1 percent (was 44.7). Three fixes, all confirmed with check.py;
// do not re-sweep any of them:
//  1. The world box is 28 BYTES, not 32. `lo` is a 16-byte Vec3q (x, y, z, pad)
//     and `hi` is a 12-byte Vec3f (x, y, z), not another Vec3q: the frame is
//     0x30 and hi.z is the last dword of the locals, so a 32-byte box does not
//     fit and MSVC gives 0x34. This is worth 3.6 points and it also moves the
//     `level` spill from the arg3 slot to the arg2 slot, as in the original.
//  2. The clamp needs the difference written out INLINE and cast to unsigned:
//     `__min(__max((unsigned)(g_game->ticks - order->timestamp), 0), 10)`.
//     With either half of that missing (a `delta` local, or no cast) MSVC
//     value-numbers the two __max subtrees the __min macro expands to and emits
//     a single evaluation plus a conditional store, and the 16 bytes of the
//     original's second `xor/cmp/sbb/and` in the taken arm disappear. The
//     earlier note here ("this looks like optimizer state too") was wrong: it
//     is source shape. unsigned (not int) is required, since the signed form
//     gives `setle` instead of the original's `sbb`. Worth 1.3 points.
//  3. Projection order `sy, sx, half, az, bz, ax, bx` (not sx, sy, half, ax, az,
//     bx, bz) is worth 1.0, and reading the 16.16 values through
//     `*(int*)&x.frac` with a plain frac/whole struct (the 0x438ea0 idiom)
//     rather than a Fixed union with a `value` member is worth 0.4. Naming
//     ix1 = ax + dx, ix2 = bx - dx, iy1 = az + dz, iy2 = bz - dz before the
//     eight calls is worth a further 0.3.
//
// DEAD LEVERS, ALREADY EXHAUSTED (with the shape count, so they are recorded as
// measurements rather than intuitions):
//  * Compiler state. Unlike the matched sibling 0x4399f0, this function has NO
//    declaration-count window: 15 include sets (stdlib alone, with memory.h,
//    math.h, string.h, windows.h, ctype.h, setjmp.h, limits.h, float.h,
//    time.h, assert.h, stdio.h, the three-header set, windows+memory) crossed
//    with 20 dummy `extern int` counts (0 to 320 in steps of 16) is 300
//    variants, and every one of them compiles to the same 50.2 percent. The
//    earlier note in this file about <memory.h> shifting the declaration
//    counter was measured on the wrong 32-byte box and is void.
//  * The clamp spelling, beyond fix 2. 16 static spellings tried (if/else both
//    ways, ?:, two statements, a named temporary, an int/unsigned level, an
//    `age` local, the expression written into both dx and dz, `__max(0, x)`
//    order, `10u`, an extra `level = level;`): all give a single evaluation
//    except the inline-cast form, and the unsigned level silently turns the
//    division unsigned (0xcccccccd, mul, shr 3), so it is wrong as well.
//  * The order of the five box stores: all 120 permutations measured, the best
//    two (lo.x, lo.y, lo.z, hi.x, hi.z and lo.x, hi.x, lo.y, hi.z, lo.z) tie at
//    the level of fix 3 and the worst is 1.5 points below it.
//  * Where the three pos reads and the index test sit relative to each other
//    and to the `def` computation: 10 orderings, all within 3.5 points, best is
//    the plain "index, test, def, px, py, pz".
//  * Statement-order levers for the register rotation (see below): 4 projection
//    orders, 2 with the view scroll read inline, 7 ways of naming the four draw
//    deltas, 3 with a `&order->pos` pointer local, 3 with age/ownerflags hoisted
//    above the box, a single-exit `if (index != 0) { ... }` block, a local copy
//    of `order`, a local copy of `view`, and two `static inline` projection
//    helpers. 24 shapes, none moved the score by more than 0.5.
//
// WHAT STILL DIFFERS: a single register rotation, and everything else follows
// from it. The original does NOT keep `order` in a register: it reloads it from
// its argument slot at 0x438cbd and 0x438d41. That frees ecx, which it spends
// on `bx` and then spills into the (by then dead) world.lo.z slot at 0x438ce2.
// With ecx free, the two divisions use ecx as the multiply scratch while `level`
// stays in edx, and ebx is free to hold `dx` and then `surface`. Ours keeps
// `order` in ecx for the whole function, so ecx is never a scratch: the
// divisions both run in edx, `level` and `dx` are memory-only, and `surface` is
// reloaded from its argument slot for four of the eight calls. The 11 bytes we
// are short are exactly the instructions that fall out of the original's
// version: the `mov [esp+0x2c], ecx` spill of bx, the `mov edx, [esp+0x48]`
// reload of level at the join, the `mov ebx, edx` / `mov ecx, edx` pair in the
// division tails, the `mov [esp+0x14], ebx` spill of dx, and the
// `mov [esp+0x5c], ecx` that writes bx + 1 over the dead surface argument.
// Ours has matching extras: four `mov reg, [esp+0x54]` reloads of surface
// instead of `push ebx`, the `mov edx, ebp` / `sub edx, ebx` pair, and the
// `order->owner` load hoisted into the middle of the first division.
//
// A SIXTH PASS checked the "callee-saved rotation" wall and found it void: the
// original pushes edi, esi, ebp, ebx and pops ebx, ebp, esi, edi, which is
// exactly what the build emits, so there is no rotation to reproduce. What is
// left is one scratch-register swap at the top of the body, as described
// above. Two experiments, both scored with `check.py --sym`:
//   `Vec3f* pos = &order->pos;` used for the pos.y read and for `*out = *pos`
//     (49.9 percent, WORSE by 1.2) - this does reproduce the original's
//     `lea ecx, [edx+0x22]; mov [esp+0x20], ecx; ...; mov edx, [ecx+4]` shape,
//     but it moves the &order->pos spill to local+0x0c instead of local+0x10
//     and leaves `order` in ecx anyway, so the register swap survives.
//   128 header sets (`tools/headers.py`): flat at 51.1 percent, none better.
//
// The thing still worth trying is whatever stops MSVC spending ECX on `order`
// at the top: it is the parameter's live range crossing the two divisions, and
// since neither the pointer shape nor any spelling of the clamp nor any
// statement order moves it, the lever is probably the ORDER in which the three
// pos components and the index are first read, or the fact that ours reads
// pos.y through `order` (the original reads it through a pointer to pos).
#include <stdlib.h>
#include <memory.h>

#pragma pack(push, 1)
struct Fixed_00438c00 {
    unsigned short frac;
    short whole;
};
struct Vec3f_00438c00 {
    Fixed_00438c00 x, y, z;
};
struct Box_00438c00 {
    Vec3f_00438c00 lo, hi;
};
struct Vec3q_00438c00 {
    Fixed_00438c00 x, y, z, pad;
};
// 28 bytes, not 32: the frame is 0x30 and the original's last field (hi.z) is
// the last dword of the locals, so the `hi` half has no pad dword.
struct Boxq_00438c00 {
    Vec3q_00438c00 lo;
    Vec3f_00438c00 hi;
};
struct UnitType_00438c00 {
    char unknown_0[0x15e];
    Box_00438c00 bounds;               // +0x15e
    char unknown_176[0x249 - 0x176];
};
union Flags_00438c00 {
    unsigned int flags;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int rest : 27;
    } bits;
};
struct Unit_00438c00 {
    char unknown_0[0x110];
    Flags_00438c00 flags;              // +0x110
};
struct Order_00438c00 {
    char unknown_0[0xe];
    Unit_00438c00* owner;              // +0xe
    char unknown_12[0x22 - 0x12];
    Vec3f_00438c00 pos;                // +0x22
    char unknown_2e[0x36 - 0x2e];
    unsigned short type;               // +0x36
    char unknown_38[0x46 - 0x38];
    int timestamp;                     // +0x46
};
struct View_00438c00 {
    char unknown_0[0x2c];
    int scroll_x;                      // +0x2c
    int scroll_y;                      // +0x30
};
struct Game_00438c00 {
    char unknown_0[0xdcc];
    unsigned char color_dcc;           // +0xdcc
    char unknown_dcd[0xdce - 0xdcd];
    unsigned char color_dce;           // +0xdce
    char unknown_dcf[0xdd4 - 0xdcf];
    unsigned char color_dd4;           // +0xdd4
    unsigned char color_dd5;           // +0xdd5
    char unknown_dd6[0x1439b - 0xdd6];
    UnitType_00438c00* types;          // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00438c00* g_game;

void __stdcall FUN_004be950(void* surface, int x0, int y0, int x1, int y1, int color);

// A fifth pass closed the one axis the fourth left open. The suggested lever was
// the 0x4a76b0 one, a `static inline` helper taking fresh memory-based
// arguments so a store inside invalidates the pointer, on the theory that it
// would stop MSVC keeping `order` live in ecx. Five shapes, all measured with
// `check.py --sym` after `rm -rf build/obj`, none better than the 51.0% in the
// file:
//
//   BuildBox helper reading order->pos internally    49.7%
//   Clamp helper taking (ticks, order->timestamp)    50.1%
//   both helpers together                            48.8%
//   clamp moved above the box stores                 40.4%
//   order->pos re-read after the box stores          50.6%
//
// So the `static inline` lever, which is worth a lot elsewhere in this project
// (four distinct mechanisms in 0x4a76b0, 0x458dd0, 0x489280 and 0x451220), does
// not reach this particular register rotation. Combined with the fourth pass's
// 300 variants showing compiler state is flat here (15 include sets by 20 dummy
// `extern int` counts, all byte-identical, so unlike 0x4399f0 there is no
// declaration-count window), the `order` lifetime looks settled rather than
// unexplored. It would need something outside the source.
//
// PASS 7 (deepseek-v4.1, 2026-09-30): 52.2 percent, up from 51.1. The four
// precomputed locals ix1/ix2/iy1/iy2 were WRONG: the original recomputes
// `bx - dx` at each call site (see 0x438daf: mov eax,[esp+0x2c]; mov
// ecx,[esp+0x14]; sub eax,ecx; mov [esp+0x18],eax) and shares one slot for
// the x pair, so the eight FUN_004be950 calls must be written with the
// arithmetic inline (`ax + dx - 1`, `bx - dx + 1`, `az + dz - 1`, ...).
// That is +1.1 points and +1 byte (647 vs 646). Still partial: the first
// divergence is still the top-of-body register swap (original order->edx,
// index->ecx; ours order->ecx, index->edx) which cascades. Also checked and
// neutral (byte-identical to the 51.1 build): `if (order->type == 0)` plus a
// second direct `order->type` read instead of the index local (CSE), and
// `((flags >> 4) & 1)` both inline and through an `unsigned int flags`
// local (MSVC5 folds every spelling to `test byte ptr [..], 0x10`; the
// original's `mov ecx,[eax+0x110]; shr ecx,4; test cl,1` is reached by
// neither, so it needs a shape not yet found).
//
// PASS 9 (deepseek-v4.1-flash, 2026-10-01): 52.2 percent, unchanged. Thirty
// more shapes measured with `check.py --sym`, all 52.2 or below, confirming the
// entry register choice is closed to source shape here:
//   index declared uninitialised at the top (before the type read)  52.2
//   index as an `int`                                                50.4
//   a local `Order* o = order;` used for every order read           52.2
//   `Order& order` reference parameter                              52.2
//   `Order* const order`                                            52.2
//   `&g_game->types[index]` array indexing instead of pointer add    52.2
//   `types` cached in a local first                                 52.2
//   a local `View* v = view;` (top, after test, and at first use)   52.2
//   a local `void* s = surface;` used for all eight calls           52.2
//   `order->owner` and `order->timestamp` hoisted into locals       52.2 / 51.3
//   a `static inline` GetType/GetDef for the top two reads         51.8 (both
//                                                                   52.2 combined)
//   the guard read twice (`if (order->type == 0)` then the index)   52.2 (CSE)
// All produce the same 647-byte body, so the divergence stays the single
// ecx/edx assignment at 0x438c00 described above.
//
// deepseek-v4.1-flash 10-minute pass (2026-10-01): a `register` hint on the
// `order` parameter is ignored by VC5 (52.2% unchanged), and wrapping the whole
// body in a positive `if (order->type != 0) { ... }` block instead of the early
// return is byte-identical at 52.2%, so neither steers the entry ecx/edx choice.
// FUNCTION: 0x438c00
void __stdcall FUN_00438c00(void* surface, View_00438c00* view, Order_00438c00* order,
                            Vec3f_00438c00* out, int unused)
{
    unsigned short index = order->type;
    if (index == 0)
        return;

    UnitType_00438c00* def = g_game->types + index;

    int px = *(int*)&order->pos.x.frac;
    int pz = *(int*)&order->pos.z.frac;
    // The whole lever of this pass. `pbase` is &order->pos, so pos.y is
    // pbase+4, order->timestamp (order+0x46) is pbase+0x24 and order->owner
    // (order+0xe) is pbase-0x14. Reaching all three through it leaves `order`
    // itself referenced only at the top of the body (the type, pos.x, pos.z),
    // which is what the original does: it keeps `order` in EDX for five
    // instructions and never enregisters it again. Ours used to hold it in ECX
    // from the entry to the colour test, and that one register is what the
    // whole downstream cascade came from.
    char* const pbase = (char*)&order->pos;
    int py = *(int*)(pbase + 4);

    Boxq_00438c00 world;
    *(int*)&world.lo.x.frac = px + *(int*)&def->bounds.lo.x.frac;
    *(int*)&world.lo.y.frac = py + *(int*)&def->bounds.lo.y.frac;
    *(int*)&world.lo.z.frac = pz + *(int*)&def->bounds.lo.z.frac;
    *(int*)&world.hi.x.frac = px + *(int*)&def->bounds.hi.x.frac;
    *(int*)&world.hi.z.frac = pz + *(int*)&def->bounds.hi.z.frac;

    int half = world.lo.y.whole >> 1;
    int sy = view->scroll_y;
    int sx = view->scroll_x;
    int az = world.lo.z.whole - half - sy + 0x20;
    int bz = world.hi.z.whole - half - sy + 0x20;
    int ax = world.lo.x.whole - sx + 0x80;
    int bx = world.hi.x.whole - sx + 0x80;

    // The cast and the lack of a `delta` local are both needed: with either
    // one alone MSVC value-numbers the two __max subtrees of the __min macro
    // and emits a single evaluation, with a conditional store instead of the
    // original's recomputation in the taken arm.
    int level = __min(__max((unsigned)(g_game->ticks - *(int*)(pbase + 0x24)), 0), 10);
    // dz before dx, not the other way round: the original runs the vertical
    // division first (0x438d15, off EDI and ESI) and the horizontal one second
    // (0x438d07 is the `sub ecx, ebp`, but the `imul ecx` at 0x438d0e follows
    // the vertical `mov ecx, edi`). Worth 0.2 and it is the only part of
    // tools/permute.py's output worth keeping: it also introduced a named
    // single-use temporary for `bx`, which is worth nothing (58.1 either way).
    int dz = ((bz - az) * level) / 10;
    int dx = ((bx - ax) * level) / 10;
    int ty = az + dz;
    int by = bz - dz;

    unsigned char color1;
    unsigned char color2;
    if (((*(Unit_00438c00**)(pbase - 0x14))->flags.flags & 0x10) != 0) {
        color1 = g_game->color_dce;
        color2 = g_game->color_dd5;
    } else {
        color1 = g_game->color_dcc;
        color2 = g_game->color_dd4;
    }

    FUN_004be950(surface, (ax + dx) - 1, az - 1, (ax + dx) - 1, bz + 1, color1);
    FUN_004be950(surface, (bx - dx) + 1, az - 1, (bx - dx) + 1, bz + 1, color1);
    FUN_004be950(surface, ax - 1, ty - 1, bx + 1, ty - 1, color1);
    FUN_004be950(surface, ax - 1, by + 1, bx + 1, by + 1, color1);
    FUN_004be950(surface, (ax + dx), az, (ax + dx), bz, color2);
    FUN_004be950(surface, (bx - dx), az, (bx - dx), bz, color2);
    FUN_004be950(surface, ax, ty, bx, ty, color2);
    FUN_004be950(surface, ax, by, bx, by, color2);

    *out = order->pos;
}