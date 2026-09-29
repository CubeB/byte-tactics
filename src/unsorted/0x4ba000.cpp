// Decompiled by space-bunny-free. Names are provisional.
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
// Every shape of these three statements that anyone has tried compiles to
// the same ten instructions with the two stores sunk to the end of the
// block, immediately before the `je`. That is not a tie-break that a
// different spelling can win: with the history store as the ONLY store in
// the block (drop `value = c` from the loop, 87.6% overall but the same
// block shape) the single store is still emitted dead last. MSVC 5 sinks a
// store to the last slot of the block in this function, always, and the
// original has one store it did not sink. The two stores cannot swap with
// each other in the emitted code either: with the three statements in any
// order, or in any of the four chained-assignment orders, the [esp+0x14]
// store is emitted before the [esi+0x51fcaf] store, and only the chain
// whose innermost target is the history array
// (`value = c = DAT_0051fcaf[n] = *p++;`) reverses that pair, and it still
// leaves the pointer spill at slot 5 and the `sub eax,0` at slot 7.
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
//  - five `static __inline` helper shapes, including ones taking fresh
//    pointer arguments and returning the bumped pointer (the technique that
//    fixed 0x4a76b0 and 0x443ff0). The helper can put the two stores in
//    the original's relative order but never lifts the history store past
//    the pointer spill, which says the spill is scheduled by the loop
//    rather than by the statements that feed it;
//  - invisible perturbations that leave the code otherwise identical, since
//    the scheduler is what is being probed: `for (; width; width--)` and
//    `while (width--)` loop shapes, a sized versus incomplete array
//    declaration, an `(unsigned)` index cast, `*(DAT_0051fcaf + n)`, the
//    value local as a one byte struct, as a one byte array element, as a
//    zero-initialised byte assigned before the loop, as `int` and as
//    `char`. A reference-to-array declaration (`unsigned char (&)[0x100]`)
//    costs two bytes (76.3%), so the global must stay a plain array.
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
