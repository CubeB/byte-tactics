// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Retry #2441: GPT-6.1-sol rechecked the saved source (98.7%, no MATCH) and
// ran headers.py across all 128 common header sets; none changed the score.
// GPT-6.1-sol retry #1952: 10 checks kept 98.7%. Header sets, an inline byte-returning helper, and pointer-index spelling tied; int value scored 64.4%, bool state 31.7%.
// Retry #1733: GPT-6.1-sol verified 98.7% (430/430); no MATCH. The remaining loop-head history store and state normalization order is unresolved.
// PARTIAL, 98.7% (430 of 430 bytes, 154 of 158 instructions). One basic
// block differs: four instructions in the head of the main loop are in a
// different order. Everything else matches, including the prologue, the
// stack slot layout, the callee-saved register assignment (ebx = the
// current byte, esi = the run length, ebp = the run start, edi = the output
// pointer), both `switch` dispatches and all three return points.
//
// Compresses one row of an 8-bit sprite (called by FUN_004b9e60, which
// matched). The first loop is only a test for "the whole row is the colour
// key": the body then re-reads the row from its first pixel, so the index
// that loop computed is thrown away. Bytes go into the history array
// DAT_0051fcaf[1..0x80], whose entry 1 is the separate global DAT_0051fcb0,
// and runs are emitted through FUN_004b9ed0 (repeat the current value) and
// FUN_004b9f50 (repeat a value, or emit a count-only run). The return value
// is the global byte counter DAT_0051fdb0, so a null `out` only measures the
// row, which is how FUN_004b9e60 asks for the size before compressing.
//
// What still differs, in one block. The original's loop head is
//   p reload, ++n, load byte, ++p,
//   DAT_0051fcaf[n] = c,          <- history store
//   p = p,                        <- pointer spill
//   state load, value = c, sub eax,0, je
// and this file emits
//   p reload, ++n, load byte, ++p,
//   p = p, state load, sub eax,0, value = c, DAT_0051fcaf[n] = c, je
// So exactly two relative orders are wrong: the history store sits four
// slots too late, and the switch's `sub eax,0` two slots too early. Both
// stores have the right operands, the block is ten instructions long in
// both, and the four callee-saved registers are all spoken for (esi the run
// length, ebp the run start, edi the output pointer, ebx the current byte),
// so the source pointer really does have to live on the stack.
//
// CORRECTION to an earlier note in this file, re-derived from the operand
// bytes: "MSVC 5 sinks a store to the last slot of the block in this
// function, always" is WRONG. It sinks a store that is a TOP LEVEL statement
// of the block. Two counter-examples, both verified by compiling:
//  - the pre-loop block (0x4ba037..0x4ba06c) is emitted in plain source
//    order, with the p store at 0x50, the value store at 0x54 and the
//    `test ecx,ecx` between them. The latch block (0x4ba157) likewise
//    interleaves the `prev` store at 0x15b between the width load and the
//    width decrement. So sinking is not a property of this function.
//  - in the loop head, moving the two stores into a NESTED expression tree
//    makes them come out IN PLACE. A `static __inline` helper whose return
//    value is assigned to p, with the history and value stores inside it,
//    emits the history store at slot 5 and the value store at slot 6, the
//    original's exact positions. What that shape costs is the register
//    allocation: MSVC then picks `ecx` for the pointer and `al` for the byte
//    instead of `eax` and `bl`, and it re-reads through the pointer, so the
//    block grows by two bytes and the score drops. Getting the store
//    positions right and the register allocation right at the same time has
//    not been found.
// So the residual is not a scheduler tie and not an operand-order or a
// block-order problem: it is that the original's loop head is one expression
// tree whose stores MSVC 5 emitted in place, and every flat spelling of the
// same three statements puts them through the delayed-store list.
//
// Things measured, all by compiling a variant and scoring it with
// `check.py --sym` (all 98.7% or worse, so none is worth a real run):
//  - the three statements `c = *p++`, `value = c`, `DAT_0051fcaf[n] = c` in
//    all six relative orders, and with the read split from the increment
//    (`c = *p; p++;`) in four arrangements;
//  - the four chained-assignment orders of the two stores, with and
//    without the read fused in (`n++` must stay on its own line: folding it
//    into the chain's index scores 19.8%);
//  - `value = c = state` style commas, a `switch (value = c, state)`
//    condition, both stores inside a nested block, and a throwaway
//    temporary for the loaded byte;
//  - the pointer advance written as its own statement in four spellings
//    (`++p`, `p = p + 1`, `p = &p[1]`, `(p++, value = c)`), which does not
//    lift the history store past the pointer spill;
//  - declaration order. `value` before `prev`, `state` before `n`, and the
//    byte locals before `n`/`state` all leave the loop head byte for byte
//    identical and only shuffle other blocks (98.7%, 98.1%, 97.5%). The
//    frame is unaffected, so the slot assignment is not what is being
//    probed here;
//  - the history store through a cached base pointer
//    (`histbase[n] = c`), through `DAT_0051fcb0[n - 1]` (which is the same
//    byte, since DAT_0051fcb0 is DAT_0051fcaf[1]), and through a pointer
//    parameter, all 98.7% with the same block;
//  - the two stores reached through pointers taken as `&value` and
//    `DAT_0051fcaf + n`;
//  - loop shapes `while (width) { width--; ... }`, `for (; width; width--)`
//    and the decrement moved into the latch. All three give the same block;
//  - `static __inline` helper shapes, twenty of them, grouped by what makes
//    the difference:
//     * helpers taking the byte BY VALUE (the caller reads, the helper
//       stores) never change the block: the stores go back to the end. The
//       byte has to be read through the pointer INSIDE the helper;
//     * helpers that read through the pointer and return the bumped pointer
//       do emit the stores in place. The best of these put the history
//       store at slot 5 and the pointer store at slot 6 (the original's
//       positions) but cost an extra reload of the byte and switch the
//       pointer to `ecx` and the byte to `al`;
//     * helpers whose read is a separate top-level statement from the call
//       always revert to the sunk form.
//    The register shift is the thing still to solve: the flat form keeps
//    `bl` because `c` is a byte local that MSVC enregisters, and the helper
//    form loses it because the byte is produced by a call whose result
//    feeds an assignment.
//
// Two explanations for the late history store were considered and the
// disassembly rules both out. The first is that `p` might be live for
// longer than it looks, but [esp+0x1c] is read only at the loop head
// (0x4ba076) and never inside the switch, so its live range really does end
// at the store. The second is the induction variable: the width counter is
// spilled to the `src` argument's dead slot [esp+0x10], and one might
// expect its spill to compete with `p`'s. It does not, because the back
// edge is `jne 0x4ba076` at 0x4ba164 and that target skips the
// `mov [esp+0x10], ecx` at 0x4ba072, so the width spill is peeled out of
// the loop and only happens on the first iteration.
//
// Frame, for whoever looks next. `sub esp, 8` is exactly the two dedicated
// locals: the width copy (int) at [esp+0x10] and `value` (byte) at
// [esp+0x14], 4 + 1 rounded to 8. The other three locals are overlaid on
// dead argument slots, which is why no frame space is spent on them:
// `p` reuses `dest` at [esp+0x1c] (edi already holds it by 0x4ba03b),
// `prev` reuses `src` at [esp+0x20] (esi already holds it by 0x4ba00e),
// and `state` reuses `width` at [esp+0x24] (ecx already holds it). So the
// slot a local lands in is decided by its declaration order against the
// argument list, and moving a declaration moves a slot. Note that [esp+0x14]
// is written as a byte and read as a DWORD at 0x4ba17a, so `value` is a
// byte local whose four byte slot is passed whole to FUN_004b9f50.
//
// Two explanations for the late history store were considered and the
// disassembly rules both out. The first is that `p` might be live for
// longer than it looks, but [esp+0x1c] is read only at the loop head
// (0x4ba076) and never inside the switch, so its live range really does end
// at the store. The second is the induction variable: the width counter is
// spilled to the `src` argument's dead slot [esp+0x10], and one might
// expect its spill to compete with `p`'s. It does not, because the back
// edge is `jne 0x4ba076` at 0x4ba164 and that target skips the
// `mov [esp+0x10], ecx` at 0x4ba072, so the width spill is peeled out of
// the loop and only happens on the first iteration.

// Appended by space-bunny-free. Re-verified 98.7% (430 of 430 bytes, 154 of
// 158 instructions) with a real check.py run, and confirmed the residual really
// is only the four-instruction permutation in the loop head. Confirmed from a
// real /Fa listing, which the note above had only inferred: the emitted head of
// the flat form is exactly
//   p reload, ++n, load byte, ++p, p store, state load, sub eax,0, value store,
//   history store, je
// and the two in-place-store helper shapes that were reported to emit the
// stores at slots 5 and 6 really do, but they also move the pointer store
// ahead of the history store and switch the switch-test to a different
// register, so neither block matches. New shapes compiled and scored with
// `check.py --sym`, none better than 98.7%:
//  - `DAT_0051fcaf[++n] = c` with the read split from the increment: 86.7%,
//    worse, because the increment then sits inside the store's index and the
//    loop no longer peels the width spill;
//  - `c = *p; n++;` before the store (the increment after the read), with
//    and without `n++` folded in, and with the store fused into the read:
//    98.1% each, one point worse, so the `++n` really must precede the read;
//  - `p = bump(p, n, c)` with `bump` an inline helper that stores the history
//    and returns the bumped pointer, plus the same helper taking `value` by
//    reference and returning the pointer: 98.7%, but the pointer store lands
//    before the history store, which is the wrong way round;
//  - the same helper taking `c` and `value` by reference and returning the
//    pointer, i.e. the read inside the helper: 60.4%, far worse;
//  - `p = (p + 1, DAT_0051fcaf[n] = c, p)` and `value = DAT_0051fcaf[n] = c`
//    and `DAT_0051fcaf[n] = value = c`: 63.5%, 98.7% and 98.7%. The
//    63.5% one is worth remembering: a comma expression whose left operand
//    is the pointer increment makes MSVC give `p` a register it keeps across
//    the switch, which changes the whole tail.
// So the residual is still unresolved, and it is a single cause: MSVC 5 emits
// the original's loop head only when the read, the pointer advance and the
// history store are one nested expression tree, and every shape that does that
// also loses `bl` for the byte or `eax` for the pointer.
//
// Appended by deepseek-v4.1-flash. Still 98.7%, the same single block. These
// further shapes were compiled and scored: hoisting one or both stores into
// the switch condition as a comma expression
// (`switch (DAT_0051fcaf[n] = c, state)`, `switch (value = c,
// DAT_0051fcaf[n] = c, state)`, `switch (DAT_0051fcaf[n] = c, value = c,
// state)`), and chaining the two stores into one assignment
// (`DAT_0051fcaf[n] = c = *p++`, `value = DAT_0051fcaf[n] = c`,
// `DAT_0051fcaf[n] = value = c`), with the read fused or split. Every one
// emitted a byte-identical block to the flat spelling below, so MSVC 5's
// scheduler is indifferent to store order and to the expression tree here;
// the original history-store and `sub eax,0` positions are not reachable this
// way.
//
// Appended by mimo-v2.6-pro. Still 98.7%, rechecked with one real check.py
// run, and the residual is still only the four-instruction permutation of the
// loop head. New shapes compiled and compared through /Fa listings (all emit
// the identical flat head, so none needed a check.py run): the value store
// nested in the switch condition as `switch (value = c, state)` combined with
// the history store chained into the read (`DAT_0051fcaf[n] = c = *p++`),
// with the read and pointer advance split (`DAT_0051fcaf[n] = c = *p; p++;`),
// with both stores in one chain (`DAT_0051fcaf[n] = value = c = *p++`), and
// with the pointer advance as the right operand of a comma
// (`DAT_0051fcaf[n] = c = *p, p = p + 1;`). So a store that is a comma
// operand of the switch condition is still sunk: the inlined-helper shape
// above remains the only one that emits the stores in place, and its register
// allocation (ecx for the pointer, al for the byte, plus an extra byte
// reload) is the unresolved half of the problem. What still differs is the
// same single block described above: the history store must sit at slot 5
// (before the pointer spill) and `sub eax, 0` at slot 9 (after the value
// store).
//
// Appended by mimo-v2.6-pro (second pass). THE SINK RULE IS NOW KNOWN, and
// the exact target order was reproduced in a science model (build/scratch/
// 0x4ba000/sci/w95.cpp): MSVC 5 delays plain local/global stores to just
// before the block's terminator branch whenever the block ends in a dispatch
// (a switch or an if/else chain). The delay is prevented only by an aliasing
// hazard: a store with a VARIABLE index into an object that also holds other
// accesses pins every store in the block in place (verified two ways: a local
// struct { h[256]; int st; unsigned char val; } with `s.h[n] = c` pins the
// s.st load and s.val store; changing to s.h[0] = c removes the hazard and
// the stores sink again). With the stores pinned, source order
//     c = *p; DAT_0051fcaf[n] = c; p = p + 1; value = c; switch (state)
// emits EXACTLY the original block: load p, n++, load byte, p+1 (hoisted),
// hist store, p store (its statement position), state load (hoisted above
// the value store), value store, sub eax,0, je. The split read (c = *p
// before the hist store, the pointer update after it) is what puts the hist
// store at slot 5 and the p store at slot 6.
// What is still missing is the aliasing hazard for this exact frame: the
// original's hist store is a direct global array store
// (`mov [esi+0x51fcaf], bl`) and state/value are stack slots, and MSVC 5 does
// NOT treat a global store with a variable index as aliasing the stack (the
// science model r1/y100/y101 confirm: same shape, stores sink). So the
// original's source must reach that aliasing some other way (or pin the
// stores some other way) that was not found. Tried and rejected this pass:
// pointer/reference parameters (fold to direct stores, keep the delay),
// local pointers to the slots (folded, delayed), state read through a local
// pointer (folded, delayed), parameter-slot stores (delayed), switch
// expression tricks state + (value = c) * 0 and (value = c) * 0 + state
// (the store still sinks; with the byte in al they only ever reorder the
// flush, never un-sink it), an out-of-bounds declared array size (delayed).
// Also confirmed the compare sub eax,0 is not the lever: it sits after the
// flushed stores in every pinned variant and before them in every sunk one,
// so fixing the sink fixes the compare position too.
//
// Appended by Space Bunny Free. Still 98.7%, 430 of 430 bytes, and the
// residual is still only the four-instruction permutation of the loop head.
// This pass bought measurements instead of a match, and they narrow the
// search:
//  - the emitted loop head does not depend on the source order of the three
//    stores at all. More than twenty spellings of the head (the read fused
//    with the advance, the read split, every relative order of the history
//    store, the pointer advance and the value store, the value store chained
//    into the history store, the history store folded into the pointer
//    advance with a comma, each store alone in a nested block, a
//    `do {} while (0)` block, a `goto` to a label just before the switch, a
//    `;` expression statement, an `if (0) { }` after them) all compile to
//    the identical block `p store, state load, sub eax,0, value store,
//    history store, je`. So the block is decided by the backend, not by
//    statement shape, and the flush order is canonical (value store, then
//    history store): with a third trailing store added as a probe
//    (`DAT_0051fdb0 = 0`, five extra bytes) the flush follows the source
//    order instead, so the fixed order only shows up for the two stores the
//    original block actually has.
//  - the sink is an aliasing effect, confirmed on a fresh pair of micro
//    models in build/scratch/0x4ba000/sci1.cpp. In t1, where the history
//    store is a global array store with a variable index and the state and
//    the value are plain frame locals, the store is delayed past the switch
//    dispatch. In t4, where the same store goes into a local struct that also
//    holds the state and the value, it is not delayed and the state load
//    follows it. Nothing else differs, so what stops the delay is exactly
//    the may-alias relation between the history store and a later access.
//    This function has no such pair: after the history store the only
//    accesses in the block are the pointer store, the state load and the
//    value store, three distinct frame slots, and MSVC 5 does not confuse a
//    global plus a register index with the frame. That is why the original's
//    order looks unreachable from any flat spelling.
//  - three ways of giving a local an escaped address do NOT create the
//    hazard the struct does: taking `&p`, `&value` and `&state` and passing
//    each to a declared function inside `if (0) { }` leaves the block in the
//    sunk form (with `&state` taken the switch test moves into ecx and it
//    still sinks), and handing `&value` to an inline function as a local
//    pointer folds back to the direct store. So the refinement that matters
//    here is not address-taken-ness.
//  - the guide's "a nested store is not sunk" rule does not fire for the
//    shapes available here: an inline helper that takes the byte by value and
//    does both stores, called both as a statement and with its result
//    assigned to p, still emits the identical sunk block. Only the shape the
//    earlier notes describe, where the helper reads through the pointer and
//    returns the bumped pointer, moves the store positions, and it costs the
//    register allocation.
//  - the dispatch has to be a real `switch`: an `if (state == 0) ... else if
//    (state == 1) ...` chain compiles to `test eax,eax; jne; cmp eax,1; jne`
//    (build/scratch/0x4ba000/sci2.cpp, w1 against w2), not to the original's
//    `sub eax,0; je; dec eax; je`.
//  - it is not a toolchain build difference: the unpatched compiler
//    (BT_TOOLCHAIN=msvc5-rtm) emits the same sunk block as msvc5-sp3.
//  - permute.py over 25 minutes with 8 jobs found nothing better than 98.7%.
//
// What I would try next, in order. (1) The pointer store is the one store in
// this block that is never delayed, in any of those spellings, so if the
// original's history store was flushed at the pointer store then that store
// has to be reachable by an address MSVC 5 cannot tie to the frame, which
// the emitted `mov [esp+0x1c], eax` does not show. (2) The value store then
// also has to be unpinned, since the original has it in place between the
// hoisted state load and the dispatch, and it is the only store in the block
// whose slot is a dedicated frame slot rather than a dead parameter slot.
// (3) Breaking the flush group with another memory op is dead: the probe
//    above shows a third global store joins the same group. The only memory
//    ops MSVC 5 cannot delete are global stores, and each one costs bytes,
//    so this route is closed.

extern unsigned char DAT_0051fcaf[];
extern unsigned char DAT_0051fcb0[];
extern int DAT_0051fdb0;

char* __stdcall FUN_004b9ed0(char* out, int count);
char* __stdcall FUN_004b9f50(char* out, int count, unsigned char a, unsigned char b);

// FUNCTION: 0x4ba000
int __stdcall FUN_004ba000(char* dest, char* src, int width, unsigned char key)
{
    int runStart = 0;
    int skip;
    for (skip = 0; skip < width; skip++) {
        if (key != (unsigned char)src[skip]) {
            break;
        }
    }
    if (skip >= width) {
        return 0;
    }

    DAT_0051fdb0 = runStart;
    char* out = dest;
    char* p = src;
    unsigned char c = *p;
    p++;
    width--;
    unsigned char value = c;
    unsigned char prev = c;
    DAT_0051fcb0[0] = c;
    int n = 1;
    int state = (c == key);
    while (width) {
        width--;
        n++;
        c = *p++;
        value = c;
        DAT_0051fcaf[n] = c;
        switch (state) {
        case 0:
            if (c == key) {
                n--;
                out = FUN_004b9ed0(out, n);
                n = 1;
                DAT_0051fcb0[0] = c;
                runStart = 0;
                state = n;
                break;
            }
            if (n > 0x80) {
                n--;
                out = FUN_004b9ed0(out, n);
                DAT_0051fcb0[0] = c;
                n = 1;
                runStart = 0;
                break;
            }
            if (c == prev) {
                if (n - runStart >= 3) {
                    if (runStart > 0) {
                        out = FUN_004b9ed0(out, runStart);
                    }
                    state = 1;
                } else if (runStart == 0) {
                    state = 1;
                }
            } else {
                runStart = n - 1;
                break;
            }
        case 1:
            if (c == prev && n - runStart <= 0x80) {
                break;
            }
            out = FUN_004b9f50(out, n - runStart - 1, prev, key);
            runStart = 0;
            DAT_0051fcb0[0] = c;
            n = 1;
            state = (c == key);
            break;
        }
        prev = c;
    }
    switch (state) {
    case 0:
        FUN_004b9ed0(out, n);
        break;
    case 1:
        FUN_004b9f50(out, n - runStart, value, key);
        return DAT_0051fdb0;
    }
    return DAT_0051fdb0;
}
