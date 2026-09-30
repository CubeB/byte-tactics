// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Pass deepseek-v4.1-flash (issue 2953 retry, 10 min box): kept 86.6% (911/919),
// no score movement. Scored on the free `--sym` path. New this pass: the
// semantics-correct break/goto form (68.4 / 69.5%) has an otherwise EXACT frame
// (base 0x10, n 0x14, i 0x18, q 0x1c, p 0x20, j 0x2c, c 0x30), and its single
// regression is that `c` homes to ebp while `base` is displaced to 0x30. So the
// entire remaining gap is "force c out of ebp into memory". Tried and failed to
// force it: `int n[1]` / `int j[1]` arrays (scalarised), an `Entry** pcc = &c`
// alias used across the call and its results (scalarised), a second alias with
// an interposed `unsigned int` temp (scalarised), hoisting `e` / `q` to outer
// scopes, and a `pad` local to raise pressure (all byte-identical). Also tried:
// do/while + break (68.4%), `while (c)` + `goto` to the merge (69.5%),
// while+break (69.5%). All the correct forms are 913 to 918 bytes. The kept
// (semantically wrong) form stays because it is 17 points higher and no correct
// spelling reached the original's c-in-memory / ebp-for-`e` split.
// Pass deepseek-v4.1-flash (issue 2795 retry, 600s box): kept 86.6% (911/919).
// Re-confirmed on the free --sym path that the correct break form is 69.5%
// (913 bytes) and NOT close to the kept build. Six spellings of the correct
// semantics were staged (plain break; if/else break; j inside the if with
// `else break` and c advanced after it; c declared at the walk scope; do/while;
// the guard written as the else arm): all 69.5% / 913 bytes except the ones
// noted in the log below. The reason is visible in the object: in every break
// spelling MSVC keeps the new c in EAX across the back edge, so the back-edge
// block is `cmp eax,edi / jne guard` with NO c/q reload, whereas the original
// has a two-entry reload block at 0x4621d8. The kept (semantically wrong)
// form spills c to [esp+0x30], which is the allocator state the reload block
// needs; no correct-semantics source spelling reached it. Left the 86.6%
// version in place because the correct form is 17 points worse, not near.
// Pass deepseek-v4.1-flash (issue 2475 retry, 10 min box): no score movement,
// kept 86.6% (911/919). Two new facts this pass.
//
// 1. The kept build is SEMANTICALLY WRONG, not just byte-different. Look at the
//    inner loop as written here: `while (c != 0) { if (c->field_c == p) { ...
//    j++; c = c->field_1c; } }`. The `j++`/`c = c->field_1c` latch is INSIDE
//    the if body, so when an entry belongs to another packet the body is
//    skipped and `c` is never advanced: infinite loop. The original breaks out
//    instead: at 0x4621e5 `jne 0x4622e4` jumps STRAIGHT to the merge
//    (`p->count = j`), skipping both the latch and the `c != 0` test, and the
//    latch at 0x4622ca is only reached through the taken body. So the original
//    is `if (c->field_c != (int)p) break;` at the top of the while body, with
//    `j++`/`c = c->field_1c` at the end of that same body. This file cannot be
//    made byte-identical without that break; the 86.6% score is a local
//    optimum of an incorrect spelling. The correct break form scores 69.5%
//    (c becomes loop-carried and MSVC homes IT to ebp, pushing base to memory,
//    the exact opposite of the original's base-in-ebp / c-in-0x30 split).
//
// 2. Frame-slot experiments confirmed again that slots are allocator state.
//    Declaring `int n;` early (after `Entry* base;`) and zeroing it at the old
//    site produced BYTE-IDENTICAL output to the kept build, so `n` still lives
//    in the dead growbufs argument slot 0x2c. Likewise the first-store order
//    (totalentries, base, i, j, p, q) does not predict the slot order
//    (j 0x10, base 0x14, i 0x18, p 0x1c, q 0x20, totalentries 0x24).
//
// The remaining byte diffs are unchanged from the notes below: ours keeps
// `n` in the dead arg slot 0x2c and `j` in frame 0x10 (original n 0x14, j 0x2c),
// our inner loop head is a single block (original has the two-entry reload
// block at 0x4621d8 because its `c` is memory-resident across the back edge),
// and the delete[] argument / tail registers are swapped.
// THIS PASS (deepseek-v4.1, 10 min box): nine check.py runs, no score
// movement, kept 86.6% (911/919); all variants I tried were worse:
//  * do/while + break with the c != 0 branch hoisted (63.9%): the outer zero
//    register flips from edi to ebp and every cmp/je target moves.
//  * `if (c->field_c != p) break;` inside the existing while (69.5%): the
//    shared break/exit block rotates the whole loop layout.
//  * `int j = 0;` moved into the if/else branch (76.9%) or into the
//    `p->count > 0` block (68.7%): also flips the zero register to ebp.
//  * `c = c->field_1c;` moved out of the `if (c->field_c == p)` body to the
//    while's statement level (85.1%, 917 bytes).
//  * tail `count = totalentries;` before `field_1c = n - 1;` (86.3%).
//  * `Entry* base;` declared before `int totalentries` and `Entry* q` before
//    `Packet* p`: byte-identical 911-byte output, no home moves.
// What still differs is listed in the notes below: the home permutation,
// the out-of-line c == 0 handler at 0x4622f9, the e->field_c store slot in
// the copy block, and the edx/ecx swap in the tail.
// THIS PASS (deepseek-v4.1, 10 min box): no movement, kept 86.6% (911/919).
// Re-ran the baseline, then the two n++ placements on the free `--sym` path,
// both again 84.0% / 903 bytes: `e = q++; n++; *e = *c;` and
// `n++; e = q++; *e = *c;` are BYTE-IDENTICAL to each other, so the n++
// statement position is not the lever; the earlier `e = q++; *e = *c; n++;`
// stays. What still differs (all in the walk band plus the tail):
//   * frame slots: ours base 0x14, n 0x2c, p 0x1c, q 0x20, j 0x10 versus the
//     original base 0x10, n 0x14, q 0x1c, p 0x20, j 0x2c. Ours spills n to the
//     dead growbufs slot and homes j in the frame; the original does the
//     reverse. Nothing tried in three passes moves a home, so this is
//     allocator state, not declaration order.
//   * n++ is a register copy here (`mov esi,[esp+0x2c] ... inc esi ...
//     mov [esp+0x2c],esi` after the rep movsd) where the original does the
//     load/inc/store on 0x14 before the copy, and its loop head at 0x4621d8
//     reloads c and q (two entries) where ours reloads one.
//   * the c == 0 handler is inline here and out of line at 0x4622f9 in the
//     original (inverting the test costs 9 points); the tail loads the
//     delete[] argument into edx and the total into ecx, the original the
//     reverse, and stores count before field_1c where ours stores field_1c
//     first.
// THIS PASS: no movement, kept 86.6% (911 of 919 bytes). Scratch runs are free
// (`check.py <addr> <file> --sym FUN_00461fd0`), all three below were scored that
// way. Two corrections to the notes above, and one new confirmed dead end.
//
// CORRECTION 1: the "stale n in ESI" bug claim further down is WRONG, for a
// reason that also fixes the frame map. Every slot this function touches was
// re-derived from the prologue by hand: `sub esp,0x18` plus four pushes puts the
// steady esp at entry_esp-0x28, so the six local dwords are at [esp+0x10] to
// [esp+0x24], the RETURN ADDRESS is at [esp+0x28], the two arguments at
// [esp+0x2c] and [esp+0x30]. That makes the original's map exact:
//   0x10 base, 0x14 n, 0x18 i, 0x1c q, 0x20 p, 0x24 totalentries,
//   0x28 UNUSED, 0x2c arg1 = total, reused by j, 0x30 arg2 = c.
// It is legal C1 behaviour: `total` is the last use of growbufs (0x46202f) and
// growbufs is never read again, so the dead argument slot is recycled for
// `total` and then for `j`; `ret 8` pops the arguments, so nothing is broken.
// The "0x28 total" in the notes above does not exist: both builds store `total`
// at 0x2c.
// CORRECTION 2: since the original DOES reload n at the top of the next packet,
// the register copy esi is in sync and the n increments are ordinary. There is
// also no stale-edi bug: the exit block at 0x4622e4 is reached with edi = 0
// from either the `xor edi,edi` at 0x4622e2 or the `xor edi,edi` at 0x4621e0 in
// the loop head, and the 0x4622f9 path keeps edi = 0 from 0x46210f, so the
// packet-loop top really does compare against a hard zero. The `p->count = j`
// block reloads n only to restore esi as n's register copy for the next
// packet, which is why the original needs 4 reloads there and we emit 3.
// The two "suspected original bug" notes above should be deleted; nothing in
// this function is wrong.
//
// This pass, all worse than the kept 86.6%, scored on scratch:
//  * `e = q++; n++; *e = *c;` (n's increment before the 32-byte copy, where the
//    original puts it, 0x4621f1 before `rep movsd` at 0x462206), with every
//    walk local hoisted to the `if (base)` scope as uninitialised declarations
//    in the original's slot order (n, i, q, p, j, c) and assigned at the same
//    statements as before: 84.0%, 903 bytes. This DOES give base 0x10 and j
//    0x2c, the two slots the kept build has transposed, but it deletes the
//    original's `n = 0` store (0x462183) and lands i 0x14, p 0x18, n 0x1c,
//    q 0x20. So the increment order is right for n's REGISTER and wrong for
//    n's home: our n stays in esi and gets `inc esi`, the original's is a
//    memory read-modify-write (`mov edi,[esp+0x14] / inc edi / mov [esp+0x14],edi`).
//  * that variant plus declaring `q` before `p`: 84.0% and byte-identical, which
//    re-confirms in a NEW allocator state (n home in the frame rather than in a
//    dead arg slot) that p/q declaration order does not move the homes.
//  * that variant plus moving `j = 0;` into the `c != 0` arm after
//    `p->start = n;` (the original's store at 0x4621c9): 75.3%, 905 bytes. The
//    two levers are anti-correlated: with n++ first the j store must stay at the
//    top of the packet body, with n++ last it must move into the else arm.
// Grows both pools of a NetBuffer: a new packet-pointer array of `growbufs` more
// packets and a new entry array of `growpackets` more entries, then moves every
// entry that still belongs to a packet into the new entry array.
//
// Not byte identical yet: 86.6%, 911 of 919 bytes. This pass is +0.7 points over
// the 85.9% it inherited. Both changes are in the notes below; do not undo them.
//
// Change A (earlier pass, kept): the `operator new[]` result is a short-lived
// temporary whose live range ENDS at the merge, and the merged variable is a
// separate name the walk reads:
//     Entry_00461fd0* ne = (Entry_00461fd0*)operator new[](totalentries * 32);
//     Entry_00461fd0* base;
//     if (ne) { <init loop> base = ne; } else { base = 0; }
//     if (base) { <the packet walk, and `q = base + n`> }
//     entries = base;
// The walk must read `base` AND the final `entries =` must read `base`, so that
// `ne` has no use at all after the merge.
//
// Change B (this pass, +0.4): inside the entry loop the latch is
//     j++;
//     c = (Entry_00461fd0*)c->field_1c;
// and NOT the other way round. The original's latch block at 0x4622ca loads j
// (esi) and then reads `c->field_1c` using eax, which is only possible if the j
// work was already done. 86.3% alone.
//
// Change C (this pass, +0.3): the tail store order is
//     entries = base;
//     field_1c = n - 1;
//     count = totalentries;
// i.e. `field_1c` BEFORE `count`, the opposite of the original's store order.
// What it actually buys is the register: it moves `totalentries` out of eax
// (85.9%) into ecx, one step from the original's edx. 86.2% alone, 86.6% with B.
//
// Established pieces from the earlier passes, do not undo:
//  1. The inner entry loop's field_c guard is an IF WITH A BODY, not an early
//     exit: `if (c->field_c == (int)p) { ... }`.
//  2. `n++` comes AFTER `Entry *e = q++; *e = *c;`.
//  3. `int j = 0;` is declared in the `for` body (just before `p`). +6.7 points,
//     pure slot allocation. Re-confirmed this pass: every other spelling of `j`
//     is worse by a lot, see the tried-and-failed list.
//  4. the queue rotate uses OUT-OF-LINE pop/push calls exactly as in 0x462ae0.
//  5. the final push of `e` is the INLINED Queue::Push exactly as in 0x461f90.
//  6. the entry-init loop bound is `i <= totalentries - 1` (the 0x461db0
//     countdown spelling of that same loop now scores identically, see below).
//  7. `totalentries` must stay a named local; recomputing it at all three sites
//     flips MSVC's cached-zero register from edi to ebp and drops to ~48%.
//
// What still differs. The original's frame is
//     0x10 base, 0x14 n, 0x18 i, 0x1c q, 0x20 p, (0x24 UNUSED),
//     0x28 totalentries, 0x2c total AND j (shared), 0x30 c
// and ours is
//     0x10 j, 0x14 base, 0x18 i, 0x1c p, 0x20 q, 0x24 totalentries,
//     0x28 total, 0x2c n, 0x30 c
// Two of the nine locals are on the wrong side: the original keeps the OUTER `n`
// in the low band and lets the INNER `j` share `total`'s slot, while we keep the
// inner `j` low and let `n` share `total`'s. The low band also has p and q the
// other way round. One consequence is visible in the walk: the original's
// `p->count = j` block reloads four slots (j, base, n, i) because n is not in a
// register, ours reloads three (j, base, i), so we are 8 bytes short overall.
// Moving `int n = 0;` one scope in or out does not flip it (79.1% / 79.2%), and
// declaring `q` before `p` does not move q's slot (byte-identical output), so the
// band assignment is not reachable by declaration order.
//
// The entry loop's shape is the other difference. The original has a real
// loop-top block that only the back edge reaches:
//     0x4621d8: mov eax,[esp+0x30]   ; c      <- reload
//     0x4621dc: mov ecx,[esp+0x1c]   ; q      <- reload
//     0x4621e0: xor edi,edi
//     0x4621e2: cmp [eax+0xc], edx
// reached by `jne 0x4621d8` from the latch, while the first iteration jumps
// straight past it (`jmp 0x4621e2` at 0x4621d6). We put the q reload in the
// LATCH instead, so our loop top is just the compare and our test is
// `xor edi,edi / cmp eax,edi / jne` where the original's is `test eax,eax`.
// Because q's register copy has to be rematerialised either way, this is a block
// PLACEMENT difference, not a shape difference, and none of the loop rewrites
// below move it. That same q liveness also costs the original's
// `shl esi,5 / lea ecx,[esi+ebp]` where we emit
// `mov ecx,esi / shl ecx,5 / add ecx,ebp`: the original's esi dies at the shift,
// ours has to preserve n for `p->start`.
// The original also lays the `c == 0` handler out of line (0x4622f9, a `je` to
// it) while ours falls into it inline (`jne` past it).
//
// Tried this pass and did NOT work (do not repeat), all scored with the free
// `check.py --sym` path on build/scratch/0x461fd0:
//  * the field_c guard as an early exit (`if (c->field_c != (int)p) break;`):
//    69.5%. As `if (cond) {...} else break;` the same. The original really does
//    need the if-with-a-body form, because its `jne` at 0x4621e5 lands on the
//    SHARED `p->count = j` block, which is where a `break` would land.
//  * every other spelling of `j`: declared in the `c != 0` arm (76.9%, and the
//    original really does store `j = 0` at 0x4621c9), declared uninitialised in
//    the `for` body with `j = 0;` after `p->start = n` (76.9%) or after
//    `p->field_10 = q` (76.9%), declared at the `field_0 >= 0` level (86.6% but
//    byte-identical to the kept build), declared in the `p->count > 0` arm
//    (86.6%, byte-identical).
//  * loop rewrites: `while (1) { if (c == 0) break; ... }` (83.6% both ways j is
//    ordered), `for (; c != 0; c = c->field_1c)` with the latch in the increment
//    (85.1%), the same with `c = c->field_1c, j++` in the increment (68.9%).
//  * `q = base + n` written as `(Entry*)((char*)base + n * 32)`,
//    `(Entry*)((int)base + n * 32)`, or via a named `int off = n << 5;`: all
//    byte-identical to `base + n`, so the extra `mov ecx,esi` is allocation and
//    not expression shape. Moving the increment: `e = q; q = q + 1;`,
//    `*q = *c; e = q; q++;` and `e = q; *e = *c; n++; q++` are all 68.3%.
//  * `n++` before `*e = *c` (83.6%).
//  * declaration positions: `c` before the `p->count > 0` test, `q` hoisted to
//    the `for` body or to the `c` block, `p` before `j`, `q` before `p`, `i`
//    declared in the `field_0 >= 0` block, `n` inside the `field_0 >= 0` block
//    (79.1%), `j` at the `field_0 >= 0` level, `unsigned int total`,
//    `unsigned int totalentries` (81.6%), an `int last = n - 1;` local for both
//    the field store and the printf argument: every one of these is either
//    byte-identical to the kept build or worse.
//  * hoisting `Queue_00461fd0* qp = &queue;` out of the `if (c->field_10 >= 0)`
//    block to the walk level: 76.9%. Reordering `field_34 = e;` before the
//    `if (field_34) field_34->field_1c = e;`: 75.5%. Swapping the two
//    `field_20 -= / +=` statements: 80.7%. Swapping the `field_34 = 0;` and
//    `field_30 = 0;` initialisers: 86.3%. Reordering `e->field_1c = 0;` before
//    `e->field_18`: 86.3%.
//  * `entries = base;` last in the tail group: 86.3%.
//  * `if (!c)`, `while (c)`, `if (!(c->field_c != (int)p))`,
//    `if (!field_30)`, `p->start = n;` after the `q` computation,
//    `e = q; e = q++;`: all byte-identical to the kept build.
//  * the 0x461db0 countdown spelling of the entry-init loop
//    (`int k = totalentries - 1; ... while (k >= 0) { ...; k--; }`): 86.6% and
//    byte-identical to the kept build, which confirms again that the loop SHAPE
//    is not the lever. With `unsigned int k` and `while (k != 0)`: 82.0%.
//  * hand-rolling the `memset(np, 0, total * 4)` as a byte loop: 79.0%, so the
//    builtin really is what the original used.
//
// Last pass (space-bunny-free) re-derived the frame layout from `objdump`, which
// pins the remaining gap to a n/j slot SWAP, and re-tested the two orderings that
// would fix it. Frame slots, original: base 0x10, n 0x14, i 0x18, q 0x1c, p 0x20,
// totalentries 0x24, then j in the dead growbufs slot 0x2c and c in the dead
// growpackets slot 0x30. Ours: j 0x10, base 0x14, i 0x18, p 0x1c, q 0x20,
// totalentries 0x24, n in the dead slot 0x2c, c 0x30. So `n` and `j` are exactly
// transposed, and once `j` leaves the frame `base`, `i` and `totalentries` land on
// the original's slots. Both builds have six frame slots and two dead-arg slots;
// which variable spills to a dead arg is what differs. Ours keeps `n` in ESI and
// spills it, the original increments `n` in memory (0x4621eb) and keeps no live
// register copy. Making `n` a real frame local therefore has to be forced, and
// every spelling tried either moved the increment to the wrong side of the copy
// (84.0%, 903 bytes) or demoted the whole walk (76.9% / 75.3%).
//
// Tried this pass, free scratch scores, all worse than the kept 86.6%:
//  * `e = q++; n++; *e = *c;` (n's increment BEFORE the 32-byte copy, which is
//    where the original puts it, 0x4621f1 before `rep movsd` at 0x462206):
//    84.0%, 903 bytes. This is the closest variant by SIZE. It does move `n` into
//    the frame (base 0x10, i 0x14, p 0x18, n 0x1c, q 0x20, j 0x2c, c 0x30) and
//    it deletes the original's `n = 0` store, so it is the right direction, but
//    MSVC then keeps n in ESI and emits `inc esi / mov [esp+0x1c], esi` where the
//    original has `mov edi,[esp+0x14] / inc edi / mov [esp+0x14], edi`, and it
//    loses the loop-head reload block. Do not retry this alone.
//  * `int j;` declared uninitialised in the `for` body with `j = 0;` written
//    between `p->start = n;` and the `q = base + n` computation, so the store
//    lands at 0x4621c9 where the original has it: 76.9%, 915 bytes.
//  * that same `j` change plus the `n++` reordering: 75.3%, 905 bytes.
//  * inverting the `c == 0` test to `c != 0` so the walk is the fallthrough: this
//    is the layout the original has (`je` to a handler at 0x4622f9, walk falls
//    through), but it is the single worst edit available, -9 points.
// Remaining known differences are the `c == 0` handler's block layout, the
// missing `mov [esp+0x30],c / mov [esp+0x1c],q` reload block at the inner loop
// head (0x4621d8, the original has a two-entry loop head, ours has one), and
// the `delete[](entries)` argument, which the original loads into ECX before the
// call and the total into EDX after, where ours does the reverse.
//
// Suspected original bug, second and much clearer one: `n` is stale in ESI at the
// packet-walk top. `p->start = n` and `p->field_10 = base + n * 32` read ESI at
// 0x4621c6 and 0x4621cd, but ESI is only ever zeroed, at 0x46217f, before the
// packet loop. Inside the walk ESI is overwritten three times, as the source
// pointer for the `rep movsd` (0x462202), as the queue pointer `lea esi,[ebx+0x38]`
// (0x462223) and as `j` (0x4622ca), and the loop-back edge at 0x4621a4 reloads
// `c` and `q` from 0x30 and 0x1c but never reloads `n` from 0x14. So for the
// SECOND and later packets with entries, `p->start` is set to the previous
// packet's entry count and `p->field_10` points into the middle of the new array.
// Cavedog's own build shipped this, so either the source read a register the
// compiler had not kept up, or the original's author relied on n living in a
// register. A correct build reloads ESI, and that reload is the single byte
// standing between this file and the original: this build emits it at 0x301
// (`mov esi, [esp+0x2c]`), the original at 0x4621a4 does not.
//
// Suspected original bug: at the packet-walk top, `p->count` is compared with
// edi (`cmp [edx+8],edi; jle`) and the freshly loaded `c` with edi
// (`cmp eax,edi; je`), but edi holds 0 only on walk entry. After any packet
// with entries is processed, edi keeps elector residue (n+1, e, or the
// queue-rotate residue), so a later packet's `count > 0` and `c == 0` tests run
// against garbage. Latent: needs at least two non-empty packets on a grow path.

// Pass deepseek-v4.1-flash, 900s limit: no score movement, kept 86.6%. Every
// variant below was scored on the free `--sym` path. The wall is the frame band:
// ours puts the walk's `n` in the dead growbufs arg slot (0x2c) and `j` in the
// low band at 0x10, the original has `n` at 0x14 and `j` at 0x2c, which also
// transposes base/j and p/q. None of these beat 86.6:
//  * `static inline void Bump(int& x) { x++; }` for `n++` (the reference
//    parameter did NOT force `n` to memory; byte-identical).
//  * `static inline void Touch(int*) {}` with `Touch(&n);` (address-take
//    scalarised away; byte-identical).
//  * `unsigned int n` (byte-identical).
//  * all walk locals hoisted to the `if (base)` scope in the original's slot
//    order (base, n, i, q, p, j, c): byte-identical.
//  * `j` declared before `n`; `p` and `q` declared at the top of the `for` body
//    with `q` first; `i` hoisted above `n`: all byte-identical.
// Not kept, all worse:
//  * tail store order `count` then `field_1c`: 86.3%.
//  * walk as the `if` then-branch so the `c == 0` handler is out of line (the
//    original's layout): 84.4%, 912 bytes.
//  * `n++` before `*e = *c`: 84.0%, 903 bytes. This one DOES move `n` into the
//    frame and gives base 0x10 and `j` 0x2c, but lands i 0x14, p 0x18, n 0x1c,
//    q 0x20, so only the base/j half comes right and it loses the loop head.
//  * `while (c != 0 && c->field_c == (int)p)`: 69.5%.
//  * the same with the terms reversed: 68.8%.
//  * `for (; c != 0 && c->field_c == (int)p; c = c->field_1c)`: 69.5%.
//  * `while (1)` with a `break` on each condition: 83.6%.
//  * `int n` declared at function scope: 76.4%.
// The one untried lever I could see is a source construct that makes `n` a
// memory increment without moving the statement order, which no spelling of a
// helper, a reference or an address-take achieved here.
//
// Pass deepseek-v4.1-flash, 900s limit: kept 86.6%, no movement. Scored on the
// free `--sym` path. Confirmed the swing of the whole diff is the inner loop
// SHAPE, not the slot spellings: the two semantically correct forms both
// compile the original's exit (`jne` to the shared `p->count = j` merge) but
// collapse to ~69%, while the kept infinite form scores 86.6%.
//  * `if (c->field_c != (int)p) { break; }` at the top of the loop body, with
//    `j++` and `c = c->field_1c` at the while level: 69.5%. The original's
//    block layout exactly (a `jmp` preheader, a separate back-edge reload
//    block at 0x4621d8, `jne 0x4622e4`), but `break` makes `c` the loop-carried
//    value and MSVC gives IT `ebp`, moving `base` to memory. The original has
//    the opposite split (base in ebp, c in memory at 0x30), which is the whole
//    17-point gap: the reload block wants c spilled, and a `break` keeps it
//    live in a register.
//  * `if (c->field_c == (int)p) { <body> }` with `j++`/`c = c->field_1c` at
//    the while level (same semantics as the kept form, latch outside the if):
//    69.6%. Same c-in-ebp problem.
//  * `for (; c != 0; c = c->field_1c) { if (c->field_c != (int)p) break; ... }`:
//    69.5%.
//  * explicit `do { ... } while (c != 0);` around the kept if-with-body:
//    86.6% and byte-identical to the kept build, so MSVC had already rotated
//    the kept `while`; the explicit spelling changes nothing.
//  * `j` declared at the `if (base)` scope (outside the for): 86.3%; inside the
//    `else` arm (the c != 0 branch): 76.9%; swapping the latch to
//    `c = c->field_1c; j++;`: 86.2%.
//  * `Entry* q = base + n;` written before `p->start = n;`: 86.6% and
//    byte-identical.
//  * `e = q++; n++; *e = *c;` (increment before the copy, as the original's
//    0x4621f1 is): 84.0%, reconfirmed. It fixes the n increment register and
//    the base/j slots but elides the `n = 0` store and loses the loop head.
//  * `e = q++; n++; *e = *c;` examined in the listing: MSVC keeps `n` in ESI
//    for the first iteration (so no `mov [slot],0` for n), while the original
//    forces the store at 0x462183 because the increment lands in EDI while
//    ESI holds the `rep movsd` source. That ESI/EDI split, and the fact that
//    the original homes the long-lived `n` to a real frame slot while the
//    short-lived `j` takes the dead growbufs argument slot, is the allocator
//    state that no source spelling here reproduced.
// Net: the remaining diffs are one allocator state (which of `base`/`c` takes
// ebp, and the resulting n/j, p/q, base/j homes). No source-level lever found;
// the loop shape that fixes the exit is exactly the one that flips ebp to `c`.

// Pass deepseek-v4.1-flash (issue 1230, short box): no score movement, kept
// 86.6% at 911 bytes. Scored on scratch, all byte-identical to the kept build:
// an uninitialised `n` zeroed by a later statement, an uninitialised `total`
// assigned on the next line, and hoisting `base` to function scope. Frame slot
// assignment here is allocator state, not declaration order.
//
// Pass deepseek-v4.1-flash (issue 1561, second pass): no movement, kept 86.6%.
// Ran tools/headers.py first: no header set matches, best is still <string.h>
// at 86.6% (128 sets, 8 compile failures). Tried forcing `n` to a real frame
// local through a C++ reference, `int nstore; int& n = nstore; n = 0;`, which
// is a construct the earlier `Bump(int&)` helper never used: still byte
// identical (911 bytes), MSVC scalarises the reference and keeps the same
// slots. This confirms again that the n/j home swap and the missing two-entry
// loop head are register-allocator state not reachable from declaration order.

#include <string.h>

void* __cdecl operator new[](unsigned int size);
void __cdecl operator delete[](void* ptr);

unsigned int FUN_004b6340();
void __cdecl FUN_00461170(const char* fmt, ...);

class Class_00461fd0;

struct Entry_00461fd0 {
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    int field_8;                    // +0x08, the size
    int field_c;                    // +0x0c, the packet that owns it
    int field_10;                   // +0x10, next in the packet chain
    int field_14;                   // +0x14
    int field_18;                   // +0x18, previous in the global list
    int field_1c;                   // +0x1c, next in the global list
};

struct Packet_00461fd0 {
    Class_00461fd0* pool;           // +0x00
    int start;                      // +0x04
    int count;                      // +0x08
    int field_c;                    // +0x0c
    int field_10;                   // +0x10
    char unknown_14[0x43e - 0x14];
};

// Read side of the ring buffer at +0x38 (out of line, cf. 0x462ae0).
class Class_004623b0 {
public:
    int count;                      // +0x00
    int index;                      // +0x04
    int unused;                     // +0x08
    Entry_00461fd0* buffer[0x400];  // +0x0c

    Entry_00461fd0* FUN_004623b0();
};

// Write side of the ring buffer at +0x38 (out of line, cf. 0x462ae0).
class Class_00462370 {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int FUN_00462370(Entry_00461fd0* value);
};

// The same ring buffer with the push inlined here (cf. 0x461f90).
class Queue_00461fd0 {
public:
    int count;                      // +0x00
    char unknown_4[4];
    int writeIdx;                   // +0x08
    Entry_00461fd0* buf[0x400];     // +0x0c

    int Push(Entry_00461fd0* value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400) {
                writeIdx = 0;
            }
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class Class_00461fd0 {
public:
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    Packet_00461fd0** packets;      // +0x08
    unsigned int field_c;           // +0x0c, the packet pool count
    char unknown_10[4];
    int field_14;                   // +0x14
    char unknown_18[4];
    int field_1c;                   // +0x1c
    int field_20;                   // +0x20
    char unknown_24[4];
    Entry_00461fd0* entries;        // +0x28
    unsigned int count;             // +0x2c, the entry pool count
    Entry_00461fd0* field_30;       // +0x30
    Entry_00461fd0* field_34;       // +0x34
    Queue_00461fd0 queue;           // +0x38

    int FUN_00461fd0(int growbufs, int growpackets);
};


// FUNCTION: 0x461fd0
int Class_00461fd0::FUN_00461fd0(int growbufs, int growpackets)
{
    FUN_00461170("current buffer pool count: %d, current packet pool count: %d\n",
                 field_c, count);
    FUN_00461170("bufs to grow by: %d, packets to grow by: %d\n", growbufs, growpackets);
    FUN_00461170("current buffer pool ix: %d, current packet pool ix: %d\n",
                 field_0, field_1c);
    if (field_c <= 0 || count <= 0) {
        return 1;
    }

    int total = field_c + growbufs;
    Packet_00461fd0** np = (Packet_00461fd0**)operator new[](total * 4);
    if (np) {
        memset(np, 0, total * 4);
        if (field_0 >= 0) {
            int i = 0;
            int ix = field_0;
            while (i < field_c) {
                ix = ix + 1;
                if (ix >= field_c) {
                    ix = 0;
                }
                np[i] = packets[ix];
                i = i + 1;
            }
        } else {
            memcpy(np, packets, field_c * 4);
        }
        field_0 = field_c - 1;
        int ok = 1;
        while (1) {
            if (field_c >= (unsigned int)total) {
                break;
            }
            Packet_00461fd0* q = (Packet_00461fd0*)operator new[](0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->field_c = 0;
                q->field_10 = 0;
            } else {
                q = 0;
            }
            np[field_c] = q;
            ok = (np[field_c] != 0);
            field_c = field_c + 1;
            if (!ok) {
                break;
            }
        }
        operator delete[](packets);
        packets = np;
        if (ok) {
        int totalentries = growpackets + count;
        Entry_00461fd0* ne = (Entry_00461fd0*)operator new[](totalentries * 32);
        Entry_00461fd0* base;
        if (ne) {
            Entry_00461fd0* q = ne;
            for (int i = 0; i <= totalentries - 1; i++) {
                q->field_4 = 0;
                q->field_8 = 0;
                q->field_c = 0;
                q->field_10 = -1;
                q->field_14 = 0;
                q->field_18 = 0;
                q->field_1c = 0;
                q++;
            }
            base = ne;
        } else {
            base = 0;
        }
        if (base) {
        int n = 0;
        if (field_0 >= 0) {
            field_34 = 0;
            field_30 = 0;
            for (int i = 0; i < field_c; i++) {
                int j = 0;
                Packet_00461fd0* p = packets[i];
                if (p->count > 0) {
                    Entry_00461fd0* c = (Entry_00461fd0*)p->field_10;
                    if (c == 0) {
                        p->start = -1;
                        p->count = 0;
                    } else {
                        p->start = n;
                        Entry_00461fd0* q = base + n;
                        p->field_10 = (int)q;
                        while (c != 0) {
                            if (c->field_c == (int)p) {
                            Entry_00461fd0* e = q++;
                            *e = *c;
                            n++;
                            if (c->field_10 >= 0) {
                                c->field_10 = -1;
                                c->field_14 = FUN_004b6340();
                                Queue_00461fd0* qp = &queue;
                                int nq = qp->count;
                                while (nq-- > 0) {
                                    Entry_00461fd0* x =
                                        ((Class_004623b0*)qp)->FUN_004623b0();
                                    if (x == c) {
                                        break;
                                    }
                                    ((Class_00462370*)qp)->FUN_00462370(x);
                                }
                                field_20 -= c->field_8;
                                qp->Push(e);
                                e->field_10 = 0;
                                field_20 += e->field_8;
                            }
                            e->field_c = (int)p;
                            e->field_18 = (int)field_34;
                            e->field_1c = 0;
                            if (field_34) {
                                field_34->field_1c = (int)e;
                            }
                            field_34 = e;
                            if (field_30 == 0) {
                                field_30 = e;
                            }
                            j++;
                            c = (Entry_00461fd0*)c->field_1c;
                            }
                        }
                        p->count = j;
                    }
                }
            }
        }
        operator delete[](entries);
        entries = base;
        field_1c = n - 1;
        count = totalentries;
        FUN_00461170("current packet pool index set to: %ld\n", n - 1);
        return 1;
        }
        }
    }
    return 0;
}
