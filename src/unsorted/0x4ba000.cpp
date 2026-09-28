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
