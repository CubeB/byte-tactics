// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
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
