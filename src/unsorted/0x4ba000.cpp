// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 98.7% (430 of 430 bytes; 154 of 158 instructions in place). One
// basic block still differs: four instructions at the head of the main loop
// are in a different order. Everything else matches, including the prologue,
// the stack slot layout, the callee-saved register assignment (ebx = the
// current byte, esi = the run length, ebp = the run start, edi = the output
// pointer), both `switch` dispatches and all three return points.
//
// Compresses one row of an 8-bit sprite (called by FUN_004b9e60, which matched).
// The first loop is only a test for "the whole row is the colour key": the body
// then re-reads the row from its first pixel, so the index that loop computed
// is thrown away. Bytes go into the history array DAT_0051fcaf[1..0x80], whose
// entry 1 is the separate global DAT_0051fcb0, and runs are emitted through
// FUN_004b9ed0 (repeat the current value) and FUN_004b9f50 (repeat a value, or
// emit a count-only run). The return value is the global byte counter
// DAT_0051fdb0, so a null `out` only measures the row, which is how
// FUN_004b9e60 asks for the size before compressing.
//
// What still differs from the original:
//  1. The head of the main loop, four instructions reordered inside one basic
//     block. The original emits the history store (`mov byte [esi+0x51fcaf],
//     bl`) immediately after the byte load, then the source-pointer spill,
//     then the `state` load, then the store of the current byte to its own
//     stack slot, and only then the `switch` dispatch. This version emits the
//     pointer spill, the `state` load, the dispatch, and the two stores
//     together after it. Reordering `value = c;`, `DAT_0051fcaf[n] = c;` and
//     `p++` in the source, writing them as chained assignments
//     (`value = c = DAT_0051fcaf[n] = *p++;`, `DAT_0051fcaf[n] = c = *p++;`,
//     `DAT_0051fcaf[n] = value = c;`), splitting the read from the increment,
//     and merging `c` and `value` into a single variable all change the order
//     of the two stores relative to each other but never lift them ahead of
//     the pointer spill: MSVC 5's scheduler sinks the pair as one group here,
//     where the original splits them. Note that the chained form
//     `value = c = DAT_0051fcaf[n] = *p++;` is what puts the history store
//     before the stack store, so the original most likely wrote something of
//     that shape; the remaining gap is the pointer spill, which MSVC hoists
//     in every arrangement tried.
//  2. Nothing else. Every other difference in the raw object diff is only an
//     unresolved address (the calls and the globals), which the checker
//     resolves and confirms against data/symbols.csv.
//
// A second pass narrowed this to a single inversion and then exhausted it.
// Reading the two emissions side by side, the only difference is where the
// history store lands: the original puts `mov byte [esi+0x51fcaf], bl` directly
// after the load and the `inc`, ahead of the source-pointer spill, while this
// version puts it last, after the `switch` test has been computed. The store of
// the current byte to its own stack slot and the `state` load are already in
// the original's order in the file as written, so those are not the problem.
//
// Stating it as two orderings, original is
//   history store < pointer spill < state load < stack store < sub eax,0
// and this file is
//   pointer spill < state load < sub eax,0 < stack store < history store
// so there is exactly one inversion to remove, the history store against
// everything else. That is a tie-break, not a structural error: the block is
// ten instructions in both, all with the right operands, and the four pushed
// callee-saved registers are all already spoken for (esi the run length, ebp the
// run start, edi the output pointer, ebx the current byte), so the source
// pointer genuinely has to live on the stack and its spill genuinely has to
// precede the first call in the switch body. Both positions satisfy that, and
// the original picks the later one.
//
// Twenty-three shapes were measured against it, all by compiling a variant and
// scoring it with `check.py --sym`, and none beat 98.7%:
//  - the three statements `c = *p++`, `value = c`, `DAT_0051fcaf[n] = c` in all
//    six relative orders, including the two orders with `n++` moved to its own
//    statement before and after the load (the latter two are much worse, 86.7%
//    and 19.8%, which is itself informative: `n++` must stay on its own line);
//  - the two stores written as chained assignments in all four directions:
//    `value = c = DAT_0051fcaf[n] = *p++`, `DAT_0051fcaf[n] = c = *p++`,
//    `value = c = *p++` with the history store after, and
//    `DAT_0051fcaf[n] = value = c`;
//  - the read split from the increment (`c = *p; p++;`) in four arrangements;
//  - the load via a fresh temporary (`unsigned char t = *p++;` then
//    `DAT_0051fcaf[n] = c = t;`), and the store through `*(DAT_0051fcaf + n)`;
//  - and five shapes that pull the whole group into a `static inline` helper
//    taking fresh pointer arguments and returning the bumped pointer, which is
//    the technique that fixed 0x4a76b0 and 0x443ff0. All five scored 98.7% or
//    worse (the best two are `helper_fresh` and `helper_vfirst`, both still
//    98.7%; dropping either the history store or the stack store from the
//    helper drops to 87.6%, and bumping the pointer before the load drops to
//    61.0%). So the helper can produce the original's *relative* order of the
//    two stores but still cannot lift the history store past the pointer
//    spill, which confirms the spill is scheduled by the loop, not by the
//    statements that feed it.
//
// I would call this exhausted for the single-file shape.
//
// Two explanations for a late spill were considered and the disassembly rules
// both out. The first is that `p` might be live for longer than it looks, but
// `[esp+0x1c]` is read only at the loop head (0x4ba076) and never inside the
// switch, so its live range really does end at the store. The second is the
// induction variable: the width counter is spilled to the `src` argument's dead
// slot, and one might expect its spill to compete with `p`'s. It does not,
// because the loop back edge is `jne 0x4ba076` at 0x4ba164, and that target
// skips the `mov [esp+0x10], ecx` at 0x4ba072. So the width spill is peeled
// out of the loop and only happens on the first iteration, and it is not what
// the scheduler balances against the `p` spill. Both loop heads are ten
// instructions with the same operands; the only question is the one order.

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
