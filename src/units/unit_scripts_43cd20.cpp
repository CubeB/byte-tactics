// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro,
// finished by space-bunny-free, finished by DeepSeek V4.1 Flash, finished by
// Claude Opus 5.5. Names are provisional.
//
// Claude Opus 5.5, 2026-10-03: the bytes now match (943 bytes, up from
// 94.8%). check.py still prints "bytes match, but a reference is wrong"
// because it reads the two 0x500000 immediates (`gap1 > 0x500000`, `gap1 -
// 0x500000`: 80.0 in 16.16 fixed point) as hard-coded addresses; they are
// plain constants, and no spelling can give them a relocation. Three changes:
//  1. The real preceding function, UnitMotion::FUN_0043cc20 (0x43cc20,
//     matched in its own file), is defined above this one without its
//     annotation (the guide's preceding-function rule; it still MATCHes from
//     this file with `--sym FUN_0043cc20`). With it in the file the two
//     final calls cross-jump as in the original (one shared `mov
//     ecx,[esp+0x10]; push eax; push edi; call`, the then arm ending in a
//     `jmp`): 94.8 -> 96.7. Its Unit and UnitDef declarations are merged with
//     this file's; +0x70 (the whole part of pos.y that 0x43cc20 reads) is a
//     union view over pos.
//  2. The `imul ecx`: VC5 only narrows a 64-bit multiply to a one-operand
//     imul when neither operand's sign extension is shared with another
//     multiply. `(__int64)field_20 * field_20` below used to CSE the same
//     `(__int64)field_20`, which forced `_allmul` (87.7). Reading field_20
//     into a local (`spd`) for that square, and writing the turned product as
//     `(__int64)(adiff & 0xffff) * (__int64)field_20`, gives the original's
//     `mov eax,esi; and eax,0xffff; ... imul ecx` (96.7 -> 98.1). The
//     `(unsigned short)adiff` spelling swaps the operands' registers (95.8).
//  3. The hasPath==0 arm binds its amount to a const reference,
//     `const int& amount = -unit->type->field_19a;` (found by permute.py as an
//     address-taken copy, then reduced to this). The bound temporary is what
//     makes VC5 load unit into ecx before the `mov [esi+0x24],ax` store and
//     keep the rate in eax (98.1 -> bytes match). A plain int, a named rate,
//     `turn = hasPath`, type locals, local unit copies, inline Brake helpers,
//     an out-parameter helper and every shared-call spelling leave unit in eax
//     (or edi) there.
// The turn block also compiles byte-identically as an inlined call of 0x43cbb0
// (Class_0043cbb0::FUN_0043cbb0(unit, diff), the same clamp, defined above).
//
// The older notes below predate these changes; their tail, imul and arm
// findings were measured without the preceding function and no longer hold.
//
// DeepSeek V4.1 Flash, 2026-10-02 (fresh continuation worker, 94.8% kept,
// no new best). Re-ran check.py on the file as it stands: 94.8% (943 original,
// 964 ours), the same three hunks (arm rotation, imul ecx, tail call split).
// stackcmp shows the frame fully aligned (0x44 + 0x10 saved), so only
// registers and instruction order remain. Measurements this pass, all on the
// 94.8 base and all byte-identical to it unless noted:
//  - arm: `turn = 0` (any spelling) still stores ax and keeps the store first;
//    keeping hasPath live through the argument (`-r2 + hasPath`,
//    `-(r2 - hasPath)`, `*hp`) folds to the immediate 0 and drops to 92.9
//    (rate-first) or stays 94.8; per-arm `Unit* u` copies, `&unit`, `unit + 0`
//    and a `Unit** up` all scalarise and sink the load past the store. The
//    original's unit-in-ecx only appears when the rate load is written first,
//    which then folds the store to `mov [esi+0x24], 0`; the two requirements
//    (store ax first, unit loaded before it) remain antagonistic.
//  - tail: EVERY value-select spelling duplicates the call in both arms
//    (if/else amount, ternary as argument or assigned, nested ternary, goto,
//    switch, do/while, block-local amount in each arm, trailing return,
//    inverted condition with bodies swapped). Only the pointer select joins
//    (84.1, 949 bytes) and its join adds a `lea`/load and pushes in edx, not
//    the original's `mov eax,[esp+0x14]; neg eax` phi. All 16 tail rewrites
//    this pass scored <= 94.8 (the two-call base and the inverted-condition
//    variant are byte-identical). Cross-jumping the two identical call tails
//    is what the original shows, but VC5 will not do it from here.
//  - imul: the turned block differs by only `imul ecx` vs `imul eax,ecx; cdq`.
//    The 64-bit form `(__int64)(unsigned short)adiff * field_20` does narrow
//    to `imul ecx` in an isolated function but not inside this one: every
//    in-function spelling (adiff temp of every width, field_20 temp, cast
//    order, operand order, separate __int64 product temp, `prod *= field_20`,
//    an __inline helper, abs respelled as a ternary) raises the score drop to
//    87.7 (972 bytes, extra _allmul). The abs()-derived adiff is the likely
//    trigger: a small standalone with abs() also widens to _allmul, while a
//    plain parameter does not.
//  - tools/permute.py 3 min, 2087 candidates, 2 jobs: no gain (94.8 -> 94.8).
//    tools/headers.py, 256 sets: closest is the current <math.h> at 94.8,
//    no set matches. So this is not compiler state.
// What still differs: the three hunks above. Best left as is.
//
// space-bunny-free, 2026-10-02 (about 15 check/compile rounds, best still
// 94.8%, file unchanged). New measurements, all on the 94.8% base:
//  - arm, the fold can be stopped. Taking the address of the v5 result keeps
//    the `mov [esi+0x24],ax` store: `int* hp = &hasPath; turn = *hp;` compiles
//    the store from ax (VC5's conditional propagation cannot see through the
//    pointer, and the back end still coalesces the load), whether `hp` is
//    declared before the `if` or inside the arm, and also with a local
//    `Unit* u = unit` next to it. So the store form is no longer the blocker.
//    What is still missing is the position: the unit load sinks past the store
//    in every combination (hp alone, u alone, both, and rate-first), and
//    rate-first plus the pointer store is 92.9 (966 bytes), worse than the
//    94.8 store-first version. The two requirements really are antagonistic:
//    store first keeps the rotation (unit=eax, type=ecx, rate=edx), rate
//    first keeps the rotation the original wants (unit=ecx, type=edx,
//    rate=eax) but moves the store after all three loads instead of after the
//    first one, and folds it.
//  - tail, new positive result: the original's diamond (one shared call, the
//    then arm ending in `jmp` over the else arm) IS reachable. Selecting a
//    POINTER rather than a value leaves the join alone:
//      int negrate = -rate;
//      int amount = *(d1 > lim && d2 > r ? &unit->type->field_19e : &negrate);
//    compiles to `mov eax,[edi+0x92]; add eax,0x19e; jmp $L; $L: lea
//    eax,_negrate; $L: mov edx,[eax]; mov ecx,_this; push edx; push edi;
//    call`. Every value phi with two non-empty arms is duplicated into both
//    arms by VC5 instead (if/else assign, ternary assigned or as the argument,
//    braces, else first, goto, switch, and an inline helper returning the
//    amount: all 986 bytes, 82.6%). The join also survives when the else arm
//    is EMPTY, because the CFG is then not a diamond:
//      int amount = -rate; if (d1 > lim && d2 > r) amount = unit->type->field_19e;
//    keeps one call, but hoists `neg` above the tests, lands the phi in esi
//    (callee-saved) and needs no `jmp`. Note the criterion is the empty arm,
//    not a value before the branch: `int amount = rate; if (...) amount =
//    field_19e; else amount = -amount;` still duplicates. So for the original
//    the remaining lead is the duplication threshold: the pointer join is 11
//    instructions against the value join's 10, so if a select can be spelled
//    with one more instruction in the join (or one less in each arm) the
//    shared call may survive.
//  - imul: the one-operand `imul ecx` is reachable from the 64-bit spelling,
//    but not inside this function. Standalone, `(__int64)(unsigned short)a *
//    s->f / s->m` (a, s->f, s->m all different pointers, member field_20
//    through this too) compiles to `and eax,0xffff; imul DWORD PTR [ecx];
//    mov esi,edx; ...; cdq; push edx; push eax; push esi; push ecx; call
//    _alldiv`, exactly the original's shape. Inside 0x43cd20 every spelling
//    drops to `_allmul` with an early `cdq` (87.7%, 972 bytes): the member or
//    a local copy of field_20, either operand order, and an `unsigned short`
//    temp for the other operand all behave the same. So this is register
//    pressure where the multiply sits, not the expression, and it is worth
//    revisiting only together with a fix for the tail (the two interact: the
//    64-bit form moves the _alldiv arguments and the whole tail block with
//    them).
//
// mimo-v2.6-pro, 2026-10-01 third retry (fresh continuation worker): re-ran
// check.py on the file as it stands: 94.8% (original 943, ours 964), kept.
// New measurements this pass (all scored on scratch copies):
//  - tail: every one-call (phi) spelling still sinks the call into both arms
//    AND shifts the whole allocation (lazy callee-saved pushes at the branch
//    target, this at [esp+4], d1 in ebx instead of ebp): nested ternary
//    `d1 > lim ? (d2 > r ? A : B) : B` 81.7, `goto callit` before one call
//    82.6 (same as the if/else select and the ternary argument). New two-call
//    spellings (braced arms + goto to a shared label after (94.8), explicit
//    `return` after the last call (94.8), else arm reloading
//    `-unit->type->field_19a` so both arms read unit->type (94.8), a named
//    amount local in each arm (94.8)) are all byte-identical to this file.
//    Guide research: 0x4034a0's cross-jump merges a call tail AFTER the
//    differing argument's push (RTL push order puts the last parameter's push
//    in the arm: `push x; jmp L` / `L: mov ecx, this; push common; call`), so
//    even a successful two-call merge would push the amount in the arms and
//    share only `push edi; mov ecx, this; call`, which is NOT the original
//    (`mov ecx,[esp+0x10]; push eax; push edi; call` with both pushes in the
//    join). So the original's join shape can only come from a phi feeding one
//    call, and every phi spelling found duplicates that call into the arms.
//  - arm, the key finding: the rotation IS reachable. Writing the rate
//    statement BEFORE the turn store (`int r2 = unit->type->field_19a; turn =
//    hasPath; call(unit, -r2);`) produces the original's rotation exactly:
//    `mov ecx,[esp+0x58]; mov edx,[ecx+0x92]; mov eax,[edx+0x19a]; ...
//    neg eax; push eax; push ecx; mov ecx,esi; call` (92.9). It fails only
//    because the store folds to `mov word ptr [esi+0x24], 0` instead of the
//    original's `mov [esi+0x24], ax`: once the arm's first statement is past,
//    VC5 has propagated `hasPath == 0` from the branch test and constant-folds
//    the assignment. With `turn = hasPath` written FIRST (this file) the store
//    stays `mov [esi+0x24], ax` but the unit load then sinks past it and the
//    rotation goes one step off (unit=eax). Stopping the fold with `short* tp
//    = &turn; *tp = hasPath;` (still `mov [esi+0x24], 0`) and with an inline
//    `SetTurn((short)hasPath)` member (still folds) both stay 92.9. The rule
//    is statement position, not the store's form: inside an inline helper
//    body, `*turnSlot = t` through a pointer parameter keeps the ax store
//    only when it is the body's FIRST statement (receiver load still sinks
//    past it); moved after another statement it folds to the immediate 0
//    again. So the whole arm gap is: keep the store as the FIRST statement
//    (for the ax store) yet make the compiler evaluate `unit` before it and
//    hold it in a register (for the ecx rotation). Likely candidates not yet
//    tried: a value for the store that is in ax but not the branch-zero name
//    (some alias of the v5 result the optimizer cannot see through), or a
//    receiver/argument expression for an inlined helper whose `unit` load
//    cannot sink because it is computed rather than reloaded from the
//    parameter slot.
//  - arm: the store `turn = hasPath` always hoists to the front of its
//    statement no matter where the comma puts it (arg1 comma 94.8, arg2 comma
//    94.8, double comma 94.8), and every copy of `unit` is scalarised with its
//    load sunk past the store: block-scoped `Unit* u = unit;` (94.8), `u =
//    unit + 0` (94.8), rate named before the store (92.9), an inline member
//    helper `SlowStep(unit, hasPath)` whose parameter bind loads unit before
//    the body's store (94.8, still sunk), a free static inline helper with
//    this passed explicitly (91.2). The load-store-rotate sequence
//    (`mov ecx,[esp+0x58]; mov [esi+0x24],ax; mov edx,[ecx+0x92];
//    mov eax,[edx+0x19a]`) is still unmatched.
//  - imul: `((__int64)((unsigned short)adiff) * field_20)` (64-bit product)
//    matches `imul ecx` but sign-extends field_20 early (cdq + spill, _alldiv
//    args reordered) and drags the turned block with it: 87.7 (972 bytes),
//    confirming the earlier combined measurement. Syntax gotcha: VC5 rejects
//    `(__int64)(unsigned short)adiff * field_20` with C2059; it needs
//    `(__int64)((unsigned short)adiff)`.
//
// mimo-v2.6-pro, 2026-10-01 second retry: 94.8% (original 943 bytes, ours
// 964). Two spellings lifted the 81.7% base (the old negative measurements
// below were all made on the 74.8% base and no longer hold):
//  1) the tail is TWO call statements, one in each arm of
//     `if (d1 > lim && d2 > r) call(unit, unit->type->field_19e); else
//     call(unit, -rate);`. Every select spelling (if/else amount plus one
//     call, ternary as the argument or assigned, braced or not) makes MSVC
//     sink the call into both arms with two epilogues (988 bytes, 83.5);
//     with the call written in each arm the whole register allocation falls
//     into place at once: the ebx<->ebp swap, the prologue register saves
//     and the v3 call setup all match the original (81.7 -> 92.9).
//  2) the hasPath arm is `turn = hasPath; int r2 = unit->type->field_19a;
//     call(unit, -r2);` (store first, rate named after it): that stores ax
//     as in the original instead of an immediate 0 (92.9 -> 94.8).
// Still differs (three things, all small):
//  - the arm's load order: the original loads unit into ecx BEFORE the turn
//    store (eax is still busy with the v5 result, so the scratch rotation
//    runs unit=ecx, type=edx, rate=eax with `neg eax; push eax; push ecx;
//    mov ecx,esi`); ours stores first and the rotation is one step off
//    (unit=eax, type=ecx, rate=edx). Forcing the unit load above the store
//    failed: `Unit* u = unit;` is scalarised and its load sinks past the
//    store (94.8 same), a type-pointer copy before the store (87.1), the
//    comma forms `(turn = hasPath, unit)` and `(turn = hasPath,
//    -unit->type->field_19a)` (identical to store-first), an inline rate
//    chain (identical), `turn = 0` with the literal-reuse trick (94.8
//    same), a doubled `turn = hasPath; turn = hasPath;` pair (dead-store
//    folded, identical) and the fold-away `u += 1; u -= 1` pointer pair
//    (91.1; it does not fold and keeps two adds).
//  - `imul ecx` (one-operand 64-bit multiply) against our `imul eax,ecx;
//    cdq`: the `(__int64)(unsigned short)adiff * field_20` spelling matches
//    those two bytes on its own (81.7 -> 84.2) but breaks the tail's
//    register allocation in every combination with the new arm and tail
//    (92.9 -> 85.8, 94.8 -> 87.7), so the int-product spelling stays.
//  - the tail arm layout: the original merges the two arms before ONE call
//    (`mov eax,[ebx+0x19e]; jmp join; mov eax,[esp+0x14]; neg eax; join:
//    mov ecx,[esp+0x10]; push eax; push edi; call`); the two-call spelling
//    leaves the else arm's copy of the whole call tail out of line after
//    `ret 4` (about 27 diff lines, most of the remaining gap). Also tried:
//    switch on the condition (85.8), a goto label before one shared call
//    (82.6), explicit returns in both arms and else-first (both 94.8
//    same), reverse default-first (77.7) and two `if` assignments of
//    -rate (77.9).
// Earlier attempts (deepseek-v4.1-flash et al, 74.8% base) are kept below for
// the history; their measured negatives still hold where re-measured (the
// 64-bit `(__int64)(unsigned short)adiff * field_20` imul spelling: 71.5
// alone, 67.2 with the merged tail; if/else and ternary selects: 62-66; t1
// with the neg before the tests: 73.4; recompute through ppos: 46.5; ppos
// before the v3 call: 69.6; reversed operator- operands: 71.6).
//
// ---------------------------------------------------------------------------
// Earlier notes (deepseek-v4.1-flash et al), kept for the history:
//
// Decompiled by Space Bunny Free, finished by space-bunny-free, edited by
// deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash, 2026-10-01, second probe: `Vec3* const ppos` is
// byte-identical (74.8%, 964 bytes); moving the hasPath==0 FUN_0043cc20 call
// before `turn = hasPath` regresses to 71.2 (966 bytes); hoisting `int rate`
// above the diff block regresses to 71.8 (978 bytes). Best stays the version
// below.
//

// deepseek-v4.1-flash retry, 2026-10-01. State: 74.8% (original 943 bytes,
// ours 964); kept, no improvement. Also tried hoisting `UnitDef* type =
// unit->type;` and using it in the hasPath==0 arm: 70.4% / 959 bytes, so the
// extra dword of frame is not a cached type pointer. The original tail really
// is ONE
// FUN_0043cc20 call shared by both arms (jmp 0x43d0ba with eax preloaded and
// `mov eax,[esp+0x14]; neg eax` as the else arm), but spelling it as an
// if/else amount plus a single call scores 62.4 (986 bytes), so the duplicated
// call below stays. Same for the ternary spelling. Everything else is the
// known unit-in-eax-vs-edi allocator gap.
//
// select tail (`int amount = (d1 > lim && d2 > r) ? unit->type->field_19e :
// -rate;` plus one call) scores 62.4 (986 bytes) and rotates unit from ebx to
// ebp; the duplicated cold epilogue after ret 4 persists even with a single
// call statement, so the tail split comes from the conditional expansion, not
// from the two call statements. Best remains the 74.8 version below.
// Eighth pass (deepseek-v4.1-flash, retry #2896, 74.8%, 0 counting runs, all
// scored with check.py --sym on scratch copies). Still the same two open
// items: frame 0x40 against 0x44 and the ebx<->edi rename (ours unit=ebx /
// ppos=edi, original unit=edi / ppos=ebx). Tried and rejected this pass, all
// <= 74.8:
//   - ppos declared before the v5 call (71.0), before the v3 call (70.9),
//     between the ax and az lines (74.8), forward-declared at the top with
//     the assignment left in place (74.8), declared with ax/az/gap1 up front
//     (74.8): the declaration point does not move the allocator, which
//     follows first definition in the flow graph.
//   - `Vec3& ppos = unit->pos;` with `.` access: compile error (the call
//     site needs a `Vec3*`).
//   - `static inline Vec3* PosOf(Unit*)` and `(Vec3*)((char*)unit + 0x6a)`:
//     byte-identical to the file (74.8).
//   - first delta through ppos for x and z (62.0) or only z (62.0): unit
//     leaves the callee-saved set.
//   - recompute deltas through ppos (46.2), ppos removed entirely with every
//     access spelled unit->pos (63.5).
//   - single select + one tail call, `int amount; if..else`: 62.4, and the
//     disassembly shows unit in ebp (lea edi,[ebp+0x6a]), confirming the
//     earlier note.
//   - `(int)(((__int64)(unsigned short)adiff * field_20) / max_turn)` (the
//     one-operand `imul ecx` the original has): 70.3, so the byte-count
//     optimum still prefers the int product spelled here even though the
//     original multiplies in 64 bits.
//   - naming the hasPath==0 amount in a local before the turn store: 72.7.
// Conclusion unchanged: with this source shape the ebx<->edi swap is an
// allocator tie-break no source rewrite has moved, and every rewrite that
// moves it loses the frame or the ppos=ebx assignment.
//
// Seventh pass (deepseek-v4.1-flash, retry, ~15 scratch/check runs, 74.8%,
// 964 bytes). Two changes raised the score:
//  1) computing the recompute delta as az first then ax
//     (`az = p[1].z - unit->pos.z; ax = p[1].x - unit->pos.x;`)
//     and swapping the order of the bx/bz statements: 72.0 to 74.8.
//  2) with (1) in place the whole main path is now a pure ebx<->edi rename of
//     the original (ours unit=ebx/ppos=edi, original unit=edi/ppos=ebx).
// Measured and rejected:
//   - the tail really is ONE call in the original (0x43d0c0, amount selected
//     into eax by the two `jle` arms). Both `int amount; if..else..` and the
//     same select as a ternary argument give 984-986 bytes, 61.4-62.4%, and
//     move unit to ebp.
//   - ppos introduced before the first delta (first reference to ppos earlier
//     than to unit) gives 62.0-62.4%, so the allocator's choice follows
//     reference order, not a source declaration order we can flip.
//   - a local `Unit* u = unit;` copy is scalarised (byte-identical).
//   - a Vec3 first-delta temp (with dead y) and int[3] with dead index 1 are
//     scalarised too: same 964 bytes.
//   - tools/headers.py: 128 sets, best 74.8% (<math.h> etc), and a sweep of
//     N unused extern declarations from 0 to 200 in steps of 8 is flat at
//     74.8%, so this is not compiler state.
// Still differs: frame 0x40 against 0x44 (the array sits 4 bytes lower, and
// every stack slot is off by 4), and the ebx/edi rename. The hasPath==0 arm
// loads unit after the turn store here, before it in the original. First
// delta args use one slot pair in ours, 8-bytes-apart in the original.
//
// Sixth pass (deepseek-v4.1, 4 more runs, 72.0%, 962 bytes): naming the
// GetHeadingBetween result first is the big lever,
//   short ang = (short)GetHeadingBetween(ppos, &p[1]);
//   short diff = ang - unit->heading;
// lifts 61.1 to 72.0 and rotates the callee-saved assignment from
// unit=ebp/ppos=ebx to unit=EBX/ppos=EDI (original unit=edi/ppos=ebx), so the
// remaining register diff is a straight ebx<->edi swap. Still open: the frame
// is 0x40 against 0x44, and the tail else branch is laid out after the
// epilogue (two epilogues, +19 bytes) instead of inline before the shared
// call. `(__int64)(unsigned short)adiff * field_20` (64-bit imul before the
// divide, instead of the int product cast up) scores 67.5, so the int-product
// spelling stays. All other variants measured this pass were byte-identical.
//
// Fifth pass (deepseek-v4.1, 8 further check runs, 61.1%): the q numerator
// must be spelled with a named int temp and a 64-bit shift, not a multiply:
//   int t = (int)(((__int64)field_20 * field_20) >> 16);
//   int q = (int)(((__int64)t << 16) / (2 * rate));
// `* 0x10000` makes VC5 emit _allmul with a constant (939 bytes, 60.9) where
// the original does cdq + _allshl 16 after the _allshr; the temp form lifts
// this to 61.1 and 936 bytes. Register allocation is unchanged (unit ebp,
// original edi; frame 0x40 against 0x44):
//   - A first-delta Vec3 temp (v6/v9: `Vec3 d; d.x=..; d.z=..;` and
//     `int d[3]` with members 0 and 2) compiles byte-identical to the plain
//     int ax/az form: VC5 coalesces both temps into one slot and the frame
//     stays 0x40, so the missing 4 bytes are NOT reclaimable this way.
//   - A local copy `Unit* u = unit;` for the whole main path, a
//     `Vec3& ppos` reference and moving ppos between ax and az all compile
//     byte-identical to this file (60.9 at the time).
//   - Replacing the two tail calls with a common `amount` select (if/else or
//     ternary) collapses the duplicated epilogue but drops to 52.2 (915
//     bytes): the original really lays out the two calls separately.
//
// Fourth pass notes (deepseek-v4.1, 14 further check runs). Still 60.9%; the
// frame stays 0x40 against the original 0x44 and `unit` stays in ebp against
// the original edi. New measurements:
//   - The original's 17-dword frame has a HOLE at [esp+0x28]: the first
//     _hypot's two int args live at [esp+0x24] and [esp+0x2c], 8 bytes apart
//     with the middle dword never written anywhere in the function, i.e. the
//     shape of a 12-byte Vec3 temp whose y member is dead (a Vec3 delta).
//     Ours coalesces both args into the parameter slot (free once `unit` is
//     enregistered), which is exactly the missing 4 bytes.
//   - The original's d1 deltas are plain scalars (z into the reused parameter
//     slot, x in a register), so the Vec3 temp is only the first _hypot's
//     argument pair, and the pull-back recompute is genuinely separate.
//   - Tried and measured (all <= 60.9, most byte-identical to the file):
//     a local `Vec3* ppos` declared before the ax/az pair and used for it
//     (unit moves to eax, 52.6); `Vec3 d = p[1] - *ppos;` with x,y,z and
//     x,0,z operator- bodies and with `p[1] - unit->pos` (unit eax, 52.6);
//     removing ppos entirely (unit lands in EDI, frame 0x3c, 50.7, so the
//     allocator can choose edi but never with ppos also live); `<< 16` for
//     the `* 0x10000` in q (emits _allshl like the original but shrinks the
//     frame to 0x3c, 57.4); `rate + rate` for `2 * rate` (identical);
//     unsigned short temp for the turned product (identical); ax/az declared
//     before the v3 call, swap of the ax/az statements, ppos declared and
//     assigned on separate lines, `if (obj->v5() == 0)` with `turn = 0`,
//     ppos->x used for different operands (all byte-identical, 60.9).
//   - Conclusion: at this source shape the ebp/edi swap is an allocator
//     tie-break (same pattern as 0x4a6ae0 / 0x4866d0 on the shared board);
//     every rewrite that changes it also loses the ppos=ebx assignment or the
//     frame. What is still needed is a source shape that keeps unit in edi
//     AND ppos in ebx AND the 0x44 frame at the same time.
//
// NOT MATCHED (53.5%, 940 bytes against 943). Semantically correct. The frame
// is 0x3c here, the original's is 0x44 (the earlier 45.2% note's 0x38 is stale).
//
// Third pass notes (space-bunny-free, 1 check run). Frame slot map, measured
// from the original, everything relative to the parameter slot (which is where
// the original keeps several dead locals):
//   -0x04 .. -0x08  the three dwords above the array: the array ends 4 bytes
//                   below the parameter, ours ends 8 bytes below it
//   -0x28           &p[0]  (ours: -0x2c)
//   -0x2c, -0x30    the two temps the first _hypot's int args are spilled to
//                   before the two filds (ours reuses ONE slot, -0x34)
//   -0x34           len (gap1 sits at -0x3c)
//   -0x38           a hole, which is where a nested block would end
//   -0x3c           gap1
//   -0x44           dz, then ndz
//   -0x48           the spill of `this` (the top-of-function
//                   `mov [esp+0x10], esi`); ours spills it at -0x44
//   the parameter slot itself (0x00) holds, in turn, dx, ndx, p[1].z - pos.z
//   and the GetHeadingBetween result
// So the original needs 8 more bytes than ours, and it gets them from two more
// live slots below the array plus one dword more above it.
//
// The root cause of nearly every difference is that ours keeps re-reading
// `unit` from its parameter slot (see _unit$[esp+72] three times in the tail)
// while the original parks it in edi with &unit->pos in ebx from just after the
// v3 call to the end. Because the parameter stays live, the compiler never
// frees its slot, so the original's `dx` in the parameter slot (0x43cdd1) has
// nowhere to go in ours. Getting unit into a callee-saved register is the
// thing to chase; rewriting the expressions will not move it.
// The `hasPath == 0` arm wants the parameter load first
// (`mov ecx, [esp+0x58]` then `mov word [esi+0x24], ax`); ours stores the
// turn first whatever the spelling of the two statements was.
// Also tried: naming both _hypot arguments as locals and swapping their order
// (byte-identical output to the inline form, still one temp slot, still 0x3c).
//
// What this function does: the path object (vtable 0x4fd458, see
// 0x44f010.cpp; slot 3 is 0x44f150, slot 5 is 0x44f290) hands over the next
// three waypoints. Waypoint 1 is pulled back along the first segment when the
// unit is farther than 0x500000 from it, the turn toward it is applied and
// clamped to the type's max_turn, and then the distance this frame may travel
// is either the type's +0x19e or the negated rate at +0x19a.
//
// The slow-path amount is -type->field_19a, not -dz. At 0x43d03a
// `mov [esp+0x24],esi` runs after four pushes, so it writes [esp+0x14],
// overwriting the dz slot, and 0x43d0b4 `mov eax,[esp+0x14]; neg eax` reads
// that value back; dz is dead after 0x43ce8c. q's numerator is
// ((field_20*field_20) >> 16) << 16, not >> 32 << 16. lim is
// (turned*turned >> 32) * 4 where turned is the earlier 64-bit quotient
// (stored at 0x43d027, reloaded at 0x43d087); r is q*q >> 32.
//
// What helped: a local `Vec3* ppos = &unit->pos` used for every pos access
// (lever 6) reproduced the original's ebx = &unit->pos and, with the
// operator-/Square member functions added to Vec3 (the idiom the matched
// siblings 0x404730/0x414a80 use), the frame grew from 0x3c to the original
// 0x44 and the score rose from 50.7 to 58.6 (that variant reused the
// pre-branch ax/az for d1, which is wrong because the pull-back modifies
// p[1]; recomputing them is correct but scores 53.5).
//
// What still differs (first hunks): in the hasPath == 0 branch the original
// loads unit into ecx before storing turn, ours stores turn first; and after
// the v3 call the original computes ax = p[1].x - pos.x before az = p[1].z -
// pos.z (slots B+0x14, B+0x1c) while ours computes az first (slots B+0x10,
// B+0x04). unit ends in eax (original edi); d1 ends in ebp in both.
//
// Suspected original bug: none beyond the dead dz store and the reused
// argument home; the store of turned at 0x43d027 is live (0x43d087 reloads it).
//
// Also tried: inline (non-local) recomputation for d1 (49.0), Vec3 delta
// temporaries, and the header sets headers.py covers.

#include <math.h>
#include <stdlib.h>

struct Vec3 {
    int x, y, z;
    Vec3 operator-(const Vec3& other) const {
        Vec3 r; r.x = x - other.x; r.y = y - other.y; r.z = z - other.z; return r;
    }
    int Square() const {
        __int64 a = x, b = z;
        return (int)((a*a) >> 32) + (int)((b*b) >> 32);
    }
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;           // +0x1427f
};

struct UnitType_0043cd20 {
    char unknown_0[0x192];
    int field_192;                    // +0x192
    char unknown_196[0x19a - 0x196];
    int field_19a;                    // +0x19a, the rate
    int field_19e;                    // +0x19e, the long-step distance
    char unknown_1a2[0x1ba - 0x1a2];
    unsigned short max_turn;          // +0x1ba
    char unknown_1bc[0x241 - 0x1bc];
    int field_241;                    // +0x241
};

struct Unit {
    char unknown_0[0x66];
    short heading;                    // +0x66
    short field_68;                   // +0x68, in 2048ths of a circle
    union {
        Vec3 pos;                     // +0x6a
        struct {
            char unknown_6a[0x70 - 0x6a];
            short field_70;           // +0x70, whole part of pos.y
        };
    };
    char unknown_76[0x92 - 0x76];
    UnitType_0043cd20* type;          // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags_0 : 16;        // +0x110
    unsigned int moved : 1;           // +0x110 bit 16
    unsigned int flags_17 : 15;
};
#pragma pack(pop)

struct Vec3_0043cc20 {
    int x;
    int y;
    int z;
};

extern Game* g_game;
extern signed char DAT_00505205[];

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// Hand-written fixed-point atan2 in the gap at 0x4b70a0.
int __stdcall GetHeadingBetween(Vec3* from, Vec3* to);

// The path object: slot 5 (vtable +0x14) says whether a path is active, slot 3
// (vtable +0xc) copies `count` points out starting at `first`.
class Iface_0043dd20 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3(Vec3* out, int first, int count);
    virtual void v4();
    virtual int v5();
};

static inline void ClampToZero(int& value)
{
    if (value < 0)
        value = 0;
}

class UnitMotion {
public:
    Iface_0043dd20* obj;               // +0x0
    char unknown_4[0x8 - 0x4];
    Vec3_0043cc20 pos;                 // +0x8
    char unknown_14[0x20 - 0x14];
    int field_20;                      // +0x20
    short turn;                        // +0x24

    void FUN_0043cc20(Unit* unit, int amount);
    void SteerGroundUnit(Unit* unit);
};

// The preceding function in the original object file (0x43cc20, matched in
// its own file), defined here without its annotation: see the note above.
void UnitMotion::FUN_0043cc20(Unit* unit, int amount)
{
    field_20 = field_20 + amount;
    ClampToZero(field_20);

    int idx = unit->field_68 >> 11;
    if (idx < -5)
        idx = -5;
    if (idx > 5)
        idx = 5;

    int range = (int)(((__int64)(DAT_00505205[idx] << 16) * unit->type->field_192) >> 16);
    range = (int)(((__int64)range << 16) / 0x640000);
    if (unit->field_70 < g_game->seaLevel && !(unit->type->field_241 & 0x81000))
        range = (int)(((__int64)range * 0x8000) >> 16);
    if (field_20 > range)
        field_20 = range;

    int dist = field_20;
    unsigned short angle = unit->heading;
    Vec3_0043cc20 v;
    v.x = -FUN_004b70ef(angle, dist);
    v.y = 0;
    v.z = -FUN_004b7123(angle, dist);
    pos = v;
}

// FUNCTION: 0x43cd20
void UnitMotion::SteerGroundUnit(Unit* unit)
{
    if (obj->v5() == 0) {
        turn = 0;
        const int& amount = -unit->type->field_19a;
        FUN_0043cc20(unit, amount);
        return;
    }

    Vec3 p[3];
    obj->v3(p, 0, 3);

    Vec3* ppos = &unit->pos;
    Vec3 d = p[1] - *ppos;
    int gap1 = (int)_hypot(d.x, d.z);

    int dz;
    if (gap1 > 0x500000) {
        int dx = p[1].x - p[0].x;
        dz = p[1].z - p[0].z;
        int len = (int)_hypot(dx, dz);
        if (len >= 0x10000) {
            int ndx = (int)(((__int64)dx << 16) / len);
            int ndz = (int)(((__int64)dz << 16) / len);
            int back = gap1 - 0x500000;
            if (back > len)
                back = len;
            p[1].x -= (int)(((__int64)ndx * back) >> 16);
            p[1].z -= (int)(((__int64)ndz * back) >> 16);
        }
    }

    int az = p[1].z - unit->pos.z;
    int ax = p[1].x - unit->pos.x;
    int d1 = (int)(((__int64)ax * ax) >> 32) + (int)(((__int64)az * az) >> 32);

    short ang = (short)GetHeadingBetween(ppos, &p[1]);
    short diff = ang - unit->heading;
    int sdiff = diff;
    int adiff = abs(sdiff);

    int bz = p[2].z - unit->pos.z;
    int bx = p[2].x - unit->pos.x;
    int d2 = (int)(((__int64)bx * bx) >> 32) + (int)(((__int64)bz * bz) >> 32);

    if (diff != 0) {
        unsigned short max = unit->type->max_turn;
        if (sdiff >= max)
            turn = max;
        else if (sdiff <= -max)
            turn = -max;
        else
            turn = diff;
        unit->heading += turn;
        unit->moved = 1;
    } else {
        turn = 0;
    }

    int turned = (int)((((__int64)(adiff & 0xffff) * (__int64)field_20)
                        / unit->type->max_turn));
    int rate = unit->type->field_19a;
    int spd = field_20;
    int t = (int)(((__int64)spd * spd) >> 16);
    int q = (int)(((__int64)t << 16) / (2 * rate));
    int r = (int)(((__int64)q * q) >> 32);
    int lim = (int)(((__int64)turned * turned) >> 32) * 4;

    if (d1 > lim && d2 > r)
        FUN_0043cc20(unit, unit->type->field_19e);
    else
        FUN_0043cc20(unit, -rate);
}

