// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by fledge-alpha-free, finished by Opus. Names are provisional.
// Rebuilt from the disassembly (pass 13, Opus): 43.3% -> 75.7%; pass 15 (Opus): 77.6%;
// pass 16 (Opus, #5358): 81.4%; pass 17 (Opus, #5515): 83.0%; pass 18 (Opus, #5559):
// 98.6%; pass 19 (Opus, #5583): 98.9%; passes 20 to 26 (Opus, #5585, #5587, #5610, #5625,
// #5650, #5666, #5675): no change.
// The class name is data/symbols.csv's
// Class_00462f30 (the caller 0x4534e0 uses it); Find (0x462d90) is called through
// Class_00462d30, its own file's class, and returns Entry_00462d90*.
//
// Pass 26 (#5675): 98.9%, no change. The tie order cannot be moved, so tick needs a 4th count.
//  * Pass 25's count hook on this file: tick 1 (def), then flag 1, 2, 3 (setg store,
//    `flag = 0`, swap test), then tick 2, 3 (the 0x463790 push, the Take).
//  * C2's block list at the count is a topological order of the flow graph: a block is
//    counted after every forward predecessor, wherever its source sits, and a loop's exit
//    arms come straight after its latch. Tests: a then-arm also reached by a later `goto`
//    is counted after that goto, inside a loop or not; the whole tail written in the first
//    loop's copy-out arm behind `goto queue;` and the `flag = 0` block at the end of the
//    function behind `goto clear;` / `goto swap;` both give the same code (98.9%) and the
//    same walk. The only move after the count is a loop exit arm going to the end of the
//    function (the first loop's copy-out; the receive loop's out-of-memory arm stays after
//    its latch), and the tail cannot be one: it is entered from four places after the loops.
//  * All three flag references lie on every path into the tail (the `if (entry == 0)`
//    join), so with the emitted code fixed flag always reaches 3 before tick's push and
//    Take are counted. The original's tick must have a 4th counted reference, and since
//    hidden reloads are ruled out (passes 24 and 25) it must be a copy that C2 merges after
//    the count. The only merge point holding a tick reference is pass 24's 0x463790 call in
//    both arms of `if (entry == 0)`. Its best form (the Take through entry->tail) is still
//    92.9%, with or without /Gi.
//
// Pass 25 (#5666): 98.9%, no change. Symbol ids do not decide the tick/flag swap.
//  * Unused `extern int`s swept at five places: after <string.h> (every count from 0 to
//    12184, then every 5th to 69995), before the FUNCTION line (every count to 4095, then
//    every 5th to 69996), at the top of the body before the locals (every 5th to 48845 and
//    every 64th to 59520, where C1 hits its heap limit), after g_game and at the end of the
//    file (every 64th to 66560). The function's bytes are the same at every point, and so
//    is the frame. ta_types.h (in a namespace, after its system headers) changes nothing
//    either. A 15-minute permuter run (seed 66, --stack tick,flag, 10185 candidates) found
//    nothing.
//  * Why, read out of C2.EXE: FUN_0042ba3f walks the blocks in list order and, in each
//    tuple whose byte +9 has bit 0 set, counts the sources and then the destinations that
//    name a symbol of kind 3, 4 or 5. FUN_004367f0 keeps the list sorted by size, then
//    count: a counted local moves ahead of same-size locals with a strictly smaller count,
//    and a new one goes to the end of its size group. FUN_0043f93b then tries each local in
//    list order on the dead parameter slots, and FUN_00440cbd on the others. None of these
//    reads a symbol id, so ids could only act through the code, and the code never moves.
//  * A hook on FUN_0042ba3f's entry dumps the count walk: the blocks are in the IL's
//    order (only `ok:` is chained after the copy-out), the first loop's copy-out is counted
//    inside the loop, and every memory operand of tick (417 def store, 609 push, 617 Take)
//    and of flag (518, 546, 556) is a visible instruction. So the original's tick must
//    have a 4th count. Neither of its tick loads can hide one: a merged reload (0x420f30's
//    pCount has 3 reload tuples at the ends of 3 predecessors and one load at the join
//    0x4210ac) needs the load at the start of a join, and 0x463484 sits in a join of only
//    two predecessors (one split point at the block start, by pass 24's reading), while
//    0x4634e1 is in a block with one predecessor.
//  * Pass 24's duplicated call, 63 more forms (the tail pointer per arm, at the join, as a
//    reference or at function scope; the call through entry or the pointer; either arm
//    first; `if ((entry = Find()) == 0)`; with or without a `queued` local; the Peeks and
//    the Take through entry or the pointer in all 8 mixes): only a Take through
//    `entry->tail` merges the copies (92.9%); any pointer for the Take gives 1695 or 1698
//    bytes. In the merged forms the Take's `buffer` folds to [edi+0x28] while the Peeks use
//    the call's `this` in esi, so Pop cannot reuse the Peek's load; reading `buffer` into a
//    local at the Take's top folds the same way (90.8%), and a Peek that hands out the ring
//    is worse (62 to 68%). The merge needs entry live in a register after the call, which
//    the original never uses (it reloads entry from [esp+0x10] for the copy-out), so the
//    duplicated call is unlikely to be the original's shape.
//  * The second-return Take, an early-return Take and a result-local Take in the tail
//    only: the same code and frame (98.9%), 96.0% and 95.9%.
//
// Pass 24 (#5650): 98.9%, no change, but a new lead: the 0x463790 call written in both
// arms of `if (entry == 0)` fixes the frame, and C2 merges the two copies.
//  * Pass 23's lead is closed. No Take/Pop spelling lets the Pop result win the tie
//    (35 shapes: f == 0 or not-due returns first, Pop with an early return or a result
//    local, a named Pop result): the Take result's last write is always in the join after
//    Pop's returns (+0x40 68 or 69 against 65 or 67). Lowering the Take result to 76 in a
//    scratch copy (no net stores on the loop's found path) gives src eax and tick 3 counts
//    again, so the second-return Take's 4th count is only the back-edge reload.
//  * C2's joint split (FUN_00438f79, called from FUN_0041ba2b when one colouring leaves 3
//    or more neighbours without a register) walks the blocks in order. A block's free set
//    is every register minus those of coloured candidates live in it (+0x4c); it joins the
//    region of its first predecessor (else first already-placed successor) whose own free
//    set (every register minus the union of its blocks' +0x4c) still meets it, or opens a
//    new region; a block with nothing free gets none. Split points (0x1a tuples) go at
//    both ends of region-less blocks, at the end of predecessors in another region (at the
//    block's start when no predecessor shares its region), and the same for successors.
//    So tick's def and first loop can only be split if the def block already uses ebp:
//    the pre-header reload theory (an elided `mov ebp, [tick]`) cannot happen. tick dies
//    in the Take's first block, so no elided reload is possible there either.
//  * Cross-jumping runs after the frame count, so a duplicated tail that C2 merges counts
//    once per copy (the Take and copy-out written in both Peek arms: entry 14 counts,
//    tick 4). C2 merges only straight-line tails that end in a jump or return into the
//    block the other copy falls into: the copy-out and NOMSG exits merge, the Take copies
//    (which end in conditional jumps) never do (91.9%).
//  * The call in both arms fits the original's layout exactly (the Find arm falls into
//    0x463470, the other arm's jne lands on it) and gives tick its 4th count (two pushes):
//        int queued;
//        if (entry == 0) {
//            entry = Find...; if (entry == 0) { length = 0; return NOMSG; }
//            queued = entry->tail.FUN_00463790(buffer, length, tick, ...);
//        } else {
//            queued = entry->tail.FUN_00463790(buffer, length, tick, ...);
//        }
//    then `if (queued)` with the two Peeks and the Take. With the Peeks and the Take
//    spelled `entry->tail.Peek()` / `entry->tail.Take(...)`, entry stays in edi across the
//    call in both arms, the copies are identical and C2 merges them: everything up to the
//    call and the whole frame then match (92.9%, 1656 bytes). Only the Peek/Take part
//    differs: the Pop re-reads entry->tail.buffer through edi instead of reusing the
//    Peek's load, so the Peek's buffer takes esi and f eax, and tick's Take piece is ecx.
//    With the base's `tail` pointer for the Peeks and the Take (any placement: after the
//    join, in each arm, before or after the call, a reference), entry is no longer used in
//    a register after the call, the Find arm's entry piece takes eax (the return value)
//    instead of edi, tick's Take piece takes edi across the first copy's call, and the
//    copies differ (92.7%). Arms in either order, `long queued`, a `found` local for the
//    Find result and an inline Queue() helper change nothing. What is left: keep entry in
//    edi across the call in the Find arm while the Peeks and the Take go through `tail`.
//    The merge needs entry used in a register after the call, which the original's code
//    never does (its found path reloads entry from [esp+0x10]), so the Find arm's edi must
//    come from something else, or the duplicated call is the wrong lead. Permuter runs
//    (12 minutes from this file with --stack tick,flag; 18 from the merged form) found
//    nothing.
//
// Pass 23 (#5625): 98.9%, no change. The second-return Take cannot fix the frame.
//  * With that Take, tick's 4th count is the visible reload at the loop's back edge (a
//    count hook on FUN_004367f0 lists it as a load on the def line, counted inside the
//    loop). It exists only because src takes esi and evicts tick there. Keeping that
//    Take but forcing src into eax (a scratch hack that replaces the loop's memcpy)
//    leaves tick as one ebp piece for the def and the loop plus the Take's esi piece,
//    with 3 counts, and the swap returns (as pass 22's after-wrap Pop showed). So that
//    Take with src in eax gives this file's frame, not the original's.
//  * Why src takes esi with that Take: the Take result temp ([-0x60]) and the Pop result
//    temp ([-0x5c]) tie at 80, and the Take result has the larger +0x40 key (69 against
//    65), so it is coloured first, with costs esi -1 (the memcpy) and eax 0. Here it is
//    64, coloured after the Pop result took eax, so eax -1 ties esi -1 and eax wins.
//  * A 288-way sweep (tick declared first or last or assigned after the declarations;
//    for or while; e at function scope or taken after the -1 test; GetFrame, Take(Peek())
//    or a tail pointer; Take or the second-return Take; `goto ok` or `return 0` after the
//    loop's copy; src block-scoped) gives only three results: 98.9% (tick 3 counts),
//    97.0% (3 counts, every `return 0` after the loop's copy) and 82.6% (4 counts, every
//    second-return Take). No change either: a store-back local for src (`void* s = ...;
//    src = s;`), `(char*)src`, `*size = len` before the memcpy, the net stores after it.
//  * Not the pre-header's crowding: three extra constants in the loop (scratch only)
//    raise the pre-header block's K from 5 to 11 and leave tick's pieces as they are.
//  * One copy-out block shared by both finds (`goto copy` into the tail's memcpy) gives
//    tick 4 counts, again through a visible in-loop reload (73.0%). tick assigned at the
//    top of the loop body is not hoisted (82.4%). A permuter run (12 minutes, seed 23,
//    --stack tick,flag, 9409 candidates) found nothing.
//
// Pass 22 (#5610): 98.9%, no change. What the early-return Take does, and more forms
// that leave the code and the counts as they are.
//  * The counting walk (FUN_0042ba3f) only visits tuples whose byte +9 has bit 0 set,
//    and per tuple counts the source operands (+0x18) and then the destinations
//    (+0x1c) whose symbol is of kind 3, 4 or 5.
//  * A Take with a second `return 0` for the not-due case (`if (tick != 0 && d > 0 &&
//    d <= 0x1e) return 0;`, or `return 0;` after the due block inside `if (f != 0)`),
//    used only by the first loop's GetFrame, gives the original frame (tick 4 counts:
//    its def and loop are separate pieces, both esi), but at 82.6%. The cause is the
//    loop's src: the extra return-0 block raises the Take result temp from 64 to 80,
//    level with the Pop result temp, and with the larger +0x40 key (69 against 65) it
//    is coloured first, before the Pop result has eax, so the memcpy's esi preference
//    wins. src in esi evicts tick inside the loop, which is what splits it (reload at
//    the back edge, none on the entry edge). Any change that puts src back in eax
//    (a Pop that indexes the frame after the wrap, 85.7%) also merges tick's pieces
//    and brings the frame swap back. The same Take only in the tail: 96.0%; in both:
//    81.9%; with a single `void* r` result: 95.9% (tail) or 92.2% (loop).
//  * Same code and counts (98.9%): Take as if/else with returns in both arms, with an
//    empty then-arm, or with `goto due`; g_game as `char*`; the whole body after the
//    declarations, the part after the first loop, or the first loop alone inside
//    `for (;;)`; the tail pointer at function scope; the 0x463790 result in a local;
//    the Peek arms (or the arms and the Take) as an inline member taking tick.
//  * Worse: the first loop as an inline helper returning the entry, its tail or src
//    (81 to 82%), or doing the copy-out and returning 1 (97.0%); Pop with an early
//    return (96.8%); GetFrame with `size = 0` after the Peek (80.9%).
//
// Pass 21 (#5587): 98.9%, no change. A scan of matched code for where invisible counts
// come from, and two corrections to the earlier notes.
//  * The scan: every matched function's file compiled with /Fa (which names each
//    [esp+N] local) and run under the counting hook of pass 20, then C2's count per
//    local compared with the listing's visible accesses: 284 locals in 665 functions
//    have invisible counts. The common sources are (a) a split piece's reload on each
//    edge into a join, which codegen tail-merges into one load (0x46d970 `this` 5
//    counts for 3 loads, 0x4cf570 `this`, 0x420f30 pCount); (b) a reload whose value
//    is still in a register: on a loop's bypass edge it emits nothing, on the exit edge
//    a `mov ecx, edx` (0x420b00 effects); (c) a loop guard reading the init value
//    again (0x47e750 cx1: `mov edi, [cx1]; mov eax, edi`).
//  * tick's exact shape (stored from a callee-saved register before a loop, read from
//    that register in the loop, reloaded after it) occurs in 15 matched functions
//    (0x407560 best, 0x40eb70 total, 0x42be30 b, 0x436c30 list, 0x4373a0 name,
//    0x44da00 this, 0x453d40 sender, 0x47a0e0 icons, 0x487bf0 selected, 0x4a51d0 len
//    and others), and in every one C2's count equals the visible accesses: no matched
//    function has an invisible pre-header reload. The pre-header theory below is
//    unsupported; tick's 4th count may be somewhere else, or flag may have one fewer.
//  * Correction to pass 19: the frame pass does not walk the final layout. In a test
//    with `for (...) if (arr[i] == key) { y = arr[7]; g(&y); arr[2] = y; return; }`
//    followed by the same three statements on x, the then-arm is emitted after the
//    function's main return but counted in its place inside the loop, so y (3 counts)
//    is ahead of x (3 counts) in the frame. So a block that the last layout pass moves
//    is counted where the earlier block order had it. Other moves happen before the
//    count: the tail written as the then-arm of an early `if (length != 0) { tail: ...
//    }` (with `goto tail;` after the length == 0 body) gives the same code (98.9%) and
//    is still counted after region A, and the swap block written at the end of the
//    function behind a goto is still counted before the tail (82.2%).
//  * A dead arm is removed before counting: `len = tick;` in the always-false arm of the
//    second `if (n == 0)` (before `error:`) adds no count.
//  * The 0x463790 call written in both arms of `if (entry == 0)` (with or without a
//    tail pointer, either arm first) gives 92.7%: the two copies differ (entry stays in
//    eax after the Find, and tick's Take piece moves its reload before the call), so
//    C2 never tail-merges them. Tail-merging needs byte-identical copies (a 4-line test
//    with the same `h(x, 1); return;` in two arms kept both, ecx against edx).
//
// Pass 20 (#5585): only the frame swap is left, and none of these moved it.
//  * What a hidden count is. A scratch copy of tools/c2prio.py with a hook on
//    FUN_004367f0 (ecx the symbol, ebx the tuple, its line at +0x10) lists every count.
//    entry's 13 include two with no memory access in the code: line `delete
//    entry->field_14` is the `mov esi, ecx` at 0x46322c and `} else {` after
//    `entry->field_8 = ...` is the `mov edi, eax` at 0x4633cb. Both are reloads of a
//    split piece whose value codegen finds in a register, so it emits a register move.
//    tick's fourth count must be such a reload with the value already in the target
//    register: the only place that leaves no code is the first loop's pre-header (def
//    and loop as two pieces both in ebp, or the def in memory with its load temp
//    hinted to ebp). The early-return GetFrame does add a count on the def line, but
//    its reload is a real `mov esi, [esp+0x14]` inside the loop.
//  * tick's pieces (c2prio --ids): #7 is split at step 37 by FUN_00437e67's piece
//    makers (not FUN_00439385, so --trace shows no split line for it) into #77, which
//    step 64 splits into #2 (def and loop, ebp) and #52 (the Take, esi).
//  * Tried, with the same code and the same counts (3 and 3): tick as unsigned, long
//    or unsigned long (local, field, Take's, GetFrame's or 0x463790's parameter);
//    `register` or `const` tick; `do {} while (0);` after the def, at the top of the
//    loop body, after the GetFrame call or before `entry = 0`; GetFrame through a
//    Peek local; GetFrame on a `Class_00463730*` local; the loop as `len = 0;
//    Take(Peek(), ...)`; tick through `static int Due(const int&)`; Take and GetFrame
//    taking `const int*` or `const int&`; e declared outside the loop; tick declared
//    late and assigned just before the loop; a const reference or a named copy for
//    the end (the optimizer propagates both into tick). An `e++` pointer loop gives
//    82.4% (i loses its slot).
//
// Pass 19 (#5583): the receive loop's latch now matches; only the frame swap is left.
//  * The loop is a plain `while (rc != 0)` and the code after it is not guarded by any
//    test of rc. The got code is `if (n == 0) { ... } else { error: ... }`, repeating
//    the enclosing `if (n == 0)`: MSVC drops a test its own dominating test already
//    decided (also `if (!entry)`, entry still 0 there, or a constant), so nothing is
//    emitted, and the error block, being an else arm, stays after B as in the original.
//    Why the old forms failed: C2 threads only conditional jumps into a test of the same
//    value. `if (rc == 0) break;` and do/while latches get threaded, which turns the
//    latch into `je got; jmp head` (do/while and goto loops then also copy the NOMSG
//    compare into the latch); the while loop's exit is a fall-through, so a real
//    `if (rc == 0)` after it is never threaded. Any goto-only error block, wherever its
//    label is (end of function, or between B and the route behind a `goto`), is placed
//    straight after the loop.
//  * The frame (tick and flag swapped) is untouched. C2 counts the frame references in
//    FUN_0042ba3f (called from the frame pass at 0x42b7b6), walking the tuples in final
//    layout order; FUN_004367f0 adds each one and moves the local ahead of same-size
//    locals whose count is strictly smaller, and a new local joins the end of its size
//    group. A write watchpoint on the counts (a scratch copy of tools/c2prio.py) gives
//    tick 1 (def), flag 1, 2, 3 (setg store, `flag = 0`, swap test), tick 2, 3 (push,
//    Take): flag reaches 3 first, so tick needs a fourth counted reference. The
//    original's visible accesses are the same 3 and 3, so its tick has one invisible
//    one: a split piece's reload that codegen turns into a register move or drops (here
//    entry has 13 counts for 11 visible accesses, and 0x4cac40's cur is the same thing).
//    The likely spot is the first loop's pre-header, if tick's def and loop were
//    separate pieces both in ebp (`mov ebp, ebp` dropped): a GetFrame with an early
//    `if (f == 0) return 0;` does split them (tick 4 counts) but changes the loop.
//    Tried with the same code and no change to the counts: tick read in the for init,
//    as a statement before the loop, through an inline getter, or copied to a local for
//    the tail; Take/GetFrame taking tick by const reference or copying it; the first
//    loop as P2/while/for-init/assignment-in-test forms; the first loop as an inline
//    method SendQueued(tick, net, data, size) returning 1 (same code, same counts);
//    block-scoped src/len; flag declared ahead or at the top of region A; 20
//    declaration orders; unused labels before the loop; `tick = tick;`; 256 header sets
//    (tools/headers.py); /Gi. Two full Take copies, the 0x463790 call written in both
//    arms of `if (entry == 0)`, or GetFrame in both arms give tick 4 counts and the
//    right frame, but MSVC keeps both copies (92-93%). A Take written with early
//    returns also splits tick's def from the loop (4 counts) but moves src into esi
//    (81.9%). A 20-minute permuter run from this file (25442 candidates, --stack
//    tick,flag) found nothing.
//
// Pass 18 (#5559), what moved it:
//  * Every DPERR_NOMESSAGES exit is its own `length = 0; return DPERR_NOMESSAGES;`
//    (the receive loop's, the route's and the out-of-order save's), instead of
//    `goto none` to one shared block. MSVC cross-jumps the copies back into the one
//    block the original has, but the shared `none:` label made C2's first split of
//    `entry` leave the receive loop out of its register piece (hence the old reload
//    of entry after the loop) and keep region A in it. With the loop's exit on its own
//    block the first split gives region A its own piece and one long piece from
//    `entry = 0` through the loop to the route (83.0 -> 84.2), but C2 skipped that
//    piece ("spill cost not positive") and net took edi. With the route's or the save
//    path's exit on its own block too, the long piece gets edi, as in the original
//    (84.2 -> 98.4). (tools/c2prio.py --blocks only shows the first pass; a scratch
//    copy that keeps the block hooks on for every re-sort showed the pieces.)
//  * After the 0x463790 call: `Class_00463730* tail = &entry->tail;` for the call,
//    both Peeks and one Take after the join, which is the original's code exactly
//    (the Take's Pop reuses the Peek's buffer register). One GetFrame after the join
//    instead loses the duplicated Peek (94.2%).
//  * Region A's `d = prev - cur` is written in both arms of the first clamp. MSVC
//    merges the two copies after the join and puts the `sub` before the flag's setg,
//    as the original does (98.4 -> 98.6); written once after the join it is scheduled
//    after the setg.
//
// Still different (pass 18's notes, the latch part now fixed):
//  * The frame: tick is in the slot at +0x18 and flag at +0x14; the original has tick
//    at +0x14. Both have 3 memory references (tools/c2prio.py --frame); flag reaches
//    3 first in code order, so it sorts first. Declaring flag at function scope, the
//    Take condition spelled in steps or with `!(...)`, and a NoMessages() inline helper
//    for the exits change nothing.
//
// Earlier passes, still true:
//  * The tail of each entry (+0x18) is Class_00463730, the class of 0x463730/0x463790,
//    and its 0x180c-byte ring is read through small inline methods (Peek, Pop, Take,
//    GetFrame). Pop is the same code 0x463790 inlines.
//  * The first loop walks `Entry* e = &entries[i]`: with plain `entries[i].` indexing
//    the found path recomputed the address from i instead of using the loop pointer.
//  * Both `return 0` copy-outs end in `goto ok;` with one `ok: return 0;` at the end,
//    which MSVC cross-jumps exactly as the original does (pass 17).
//  * After the 4b5 test the original keeps both arms (A, then its shared E_FAIL return,
//    then B), and the HAPINET error block comes after B. Only an else arm keeps it
//    there (see pass 19).
//  * The clamp helpers are `int r = n -+ 1; if (r >= -1) r = -2; return r;`, and the
//    first clamp loads cur in both arms, as the original does.
//  * `delete buffer; ... buffer = new char[capacity];` in the receive loop and
//    `delete entry->field_14; entry->field_14 = new char[length];` in the out-of-order
//    save, instead of operator delete/new: the operators shift the temporary rotation
//    by one.
//
// Receives the next frame for the local player: first any frame queued in a
// player's ring whose tick is due, otherwise a saved out-of-order frame or a new
// packet from HAPINET_receivepacket (growing the buffer on DPERR_BUFFERTOOSMALL).
// Sequenced packets (first dword not -1) are checked against the sender's last
// sequence number: an out-of-order frame is saved in the entry (and
// DPERR_NOMESSAGES returned), a gap is reported through the empty 0x4568b0, and a
// saved frame is swapped in when it can be used. The frame is then queued in the
// player's ring by 0x463790 and the next due frame is returned.
#include <string.h>

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame_00462f30 {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// 0x180c-byte ring of 0x200 frames.
struct Ring_00462f30 {
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame_00462f30 frames[0x200];      // +0xc

    Frame_00462f30* Pop()
    {
        if (count > 0) {
            count--;
            Frame_00462f30* f = &frames[head];
            if (++head >= 0x200)
                head = 0;
            return f;
        }
        return 0;
    }
};

// The tail of a PlayerFrameInfo entry (see 0x463730 and 0x463790).
class Class_00463730 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    Ring_00462f30* buffer;             // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int FUN_00463790(char* src, unsigned int size, int tick, int a4, int a5, int a6);

    Frame_00462f30* Peek()
    {
        if (buffer == 0 || buffer->count <= 0)
            return 0;
        return &buffer->frames[buffer->head];
    }

    // Pops the frame at the head if it is due (or if there is no tick).
    void* Take(Frame_00462f30* f, int tick, int& size)
    {
        if (f != 0) {
            int d = f->tick - tick;
            if (tick == 0 || d <= 0 || d > 0x1e) {
                f = buffer->Pop();
                size = f->size;
                return f->data;
            }
        }
        return 0;
    }

    void* GetFrame(int tick, int& size)
    {
        size = 0;
        return Take(Peek(), tick, size);
    }
};

struct Entry_00462d90 {
    int field_0;                       // +0x00 the id
    int field_4;                       // +0x04
    int field_8;                       // +0x08 last sequence number, -1 for none
    int field_c;                       // +0x0c saved frame length
    int field_10;                      // +0x10 saved frame capacity
    char* field_14;                    // +0x14 saved frame
    Class_00463730 tail;               // +0x18
};

#pragma pack(push, 1)
struct Game_00462f30 {
    char unknown_0[0x38a47];
    int tick;                          // +0x38a47
};
#pragma pack(pop)

extern Game_00462f30* g_game;

class Class_0044f9c0 {
public:
    int FUN_0044f9c0(void* net, char* data, int* size);
};
extern Class_0044f9c0 DAT_005129f8;

void __stdcall FUN_004568b0(int a, int b, int c);
void __cdecl FUN_00461170(const char* fmt, ...);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
char* __stdcall FUN_004c9530(int error);

// The sequence numbers next to n, held below -1 (-1 or more becomes -2).
static int Prev_00462f30(int n)
{
    int r = n - 1;
    if (r >= -1)
        r = -2;
    return r;
}

static int Next_00462f30(int n)
{
    int r = n + 1;
    if (r >= -1)
        r = -2;
    return r;
}

class Class_00462d30 {
public:
    Entry_00462d90* FUN_00462d90(long id);
};

class Class_00462f30 {
public:
    void* vtable;                      // +0x00
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c current frame's sender
    int field_10;                      // +0x10
    Entry_00462d90* field_14;          // +0x14 entry whose saved frame is in use
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    Entry_00462d90 entries[10];        // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int field_230;                     // +0x230 spare buffer's length
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    int FUN_00462f30(void* net, unsigned char* data, int* size);
};

// FUNCTION: 0x462f30
int Class_00462f30::FUN_00462f30(void* net, unsigned char* data, int* size)
{
    int tick = g_game->tick;
    Entry_00462d90* entry;
    unsigned int i;
    void* src;
    int len;
    int rc;

    for (i = 0; i < 10; i++) {
        Entry_00462d90* e = &entries[i];
        if (e->field_0 == -1)
            break;
        src = e->tail.GetFrame(tick, len);
        if (src != 0) {
            *(int*)((char*)net + 0x4b5) = e->tail.field_14;
            *(int*)((char*)net + 0x4b9) = e->tail.field_18;
            memcpy(data, src, len);
            *size = len;
            goto ok;
        }
    }

    entry = 0;
    if (length == 0) {
        int n = 0;
        if (field_14 != 0) {
            if (spare != 0) {
                buffer = spare;
                if (field_230 > 0) {
                    length = field_230;
                    field_c = field_234;
                    field_10 = field_238;
                    n = length - 4;
                    if (n > 0)
                        field_14->field_8 = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                field_14->field_c = 0;
                field_14 = 0;
            } else if (field_14->field_c > 0) {
                spare = buffer;
                field_234 = field_c;
                field_230 = 0;
                field_238 = field_10;
                buffer = field_14->field_14;
                field_14->field_8 = *(int*)buffer;
                length = field_14->field_c;
                field_c = field_14->field_0;
                field_10 = field_14->field_4;
                n = length - 4;
                field_14->field_c = 0;
            } else {
                field_14 = 0;
            }
        }
        if (n == 0) {
            length = capacity;
            rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
            while (rc != 0) {
                if (rc == (int)0x887700be) {    // DPERR_NOMESSAGES
                    length = 0;
                    return (int)0x887700be;
                }
                if (rc != (int)0x8877001e)      // DPERR_BUFFERTOOSMALL
                    goto error;
                delete buffer;
                capacity = length;
                length = 0;
                buffer = new char[capacity];
                if (buffer == 0) {
                    capacity = 0;
                    return (int)0x8007000e;
                }
                length = capacity;
                rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
            }
            // Always true here (the enclosing test), so MSVC emits no test; the error
            // block stays after B only as this if's else arm.
            if (n == 0) {
                field_c = *(int*)((char*)net + 0x4b5);
                field_10 = *(int*)((char*)net + 0x4b9);
                if (*(int*)((char*)net + 0x4b5) != 0) {
                    if ((unsigned int)length >= sizeof(int)) {
                        if (length == sizeof(int))
                            return (int)0x80004005;
                        if (*(int*)buffer != -1) {
                            entry = ((Class_00462d30*)this)->FUN_00462d90(field_c);
                            if (entry != 0) {
                                if (entry->field_8 != -1) {
                                    int prev = entry->field_8 - 1;
                                    int cur;
                                    int d;
                                    if (prev >= -1) {
                                        prev = -2;
                                        cur = *(int*)buffer;
                                        d = prev - cur;
                                    } else {
                                        cur = *(int*)buffer;
                                        d = prev - cur;
                                    }
                                    int flag = entry->field_c > 0;
                                    if (d > 0) {
                                        if (entry->field_c <= 0) {
                                            // Out of order: save it in the entry.
                                            if (length > entry->field_10) {
                                                delete entry->field_14;
                                                entry->field_14 = new char[length];
                                                if (entry->field_14 == 0) {
                                                    FUN_00461170("no memory for allocating saved receive frame\n");
                                                    entry->field_10 = -1;
                                                    return (int)0x8007000e;
                                                }
                                                entry->field_10 = length;
                                            }
                                            memcpy(entry->field_14, buffer, length);
                                            entry->field_4 = field_10;
                                            entry->field_0 = field_c;
                                            entry->field_c = length;
                                            length = 0;
                                            return (int)0x887700be;
                                        }
                                        prev = Prev_00462f30(entry->field_8);
                                        if (*(int*)entry->field_14 <= cur) {
                                            if (prev != cur)
                                                FUN_004568b0(field_c, prev, Next_00462f30(cur));
                                            int p2 = Prev_00462f30(*(int*)buffer);
                                            if (p2 != *(int*)entry->field_14)
                                                FUN_004568b0(field_c, p2, Next_00462f30(*(int*)entry->field_14));
                                            flag = 0;
                                            field_14 = entry;
                                        } else {
                                            if (prev != *(int*)entry->field_14)
                                                FUN_004568b0(field_c, prev, Next_00462f30(*(int*)entry->field_14));
                                            int p2 = Prev_00462f30(*(int*)entry->field_14);
                                            if (p2 != *(int*)buffer)
                                                FUN_004568b0(field_c, p2, Next_00462f30(*(int*)buffer));
                                        }
                                    }
                                    if (flag && entry->field_c > 0) {
                                        // Swap the saved frame in, keep this one as the spare.
                                        spare = buffer;
                                        field_230 = length;
                                        field_234 = field_c;
                                        field_238 = field_10;
                                        buffer = entry->field_14;
                                        length = entry->field_c;
                                        field_c = entry->field_0;
                                        field_10 = entry->field_4;
                                        field_14 = entry;
                                        entry->field_c = 0;
                                    }
                                }
                                entry->field_8 = *(int*)buffer;
                            } else {
                                return (int)0x80004005;
                            }
                        }
                    } else {
                        return (int)0x80004005;
                    }
                } else {
                    // From sender 0 (a system message): hand it out directly.
                    if (*size >= length) {
                        memcpy(data, buffer, length);
                        *size = length;
                        length = 0;
                        return 0;
                    } else {
                        *size = length;
                        return (int)0x8877001e;
                    }
                }
            } else {
            error:
                FUN_00461170("HAPINET_receivepacket failed (%s)\n", FUN_004c9530(rc));
                length = 0;
                return rc;
            }
        }
    }

    if (entry == 0) {
        entry = ((Class_00462d30*)this)->FUN_00462d90(field_c);
        if (entry == 0) {
            length = 0;
            return (int)0x887700be;
        }
    }
    {
        Class_00463730* tail = &entry->tail;
        Frame_00462f30* f;
        if (tail->FUN_00463790(buffer, length, tick, field_c, field_10, field_14 == 0)) {
            length = 0;
            len = 0;
            f = tail->Peek();
        } else {
            len = 0;
            f = tail->Peek();
        }
        src = tail->Take(f, tick, len);
    }
    if (src != 0) {
        *(int*)((char*)net + 0x4b5) = entry->tail.field_14;
        *(int*)((char*)net + 0x4b9) = entry->tail.field_18;
        memcpy(data, src, len);
        *size = len;
        goto ok;
    }

    length = 0;
    return (int)0x887700be;
ok:
    return 0;
}
