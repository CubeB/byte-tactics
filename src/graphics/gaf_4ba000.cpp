// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
//
// Compresses one row of an 8-bit sprite (called by CompressFrame). The first
// loop only tests whether the whole row is the colour key; the body then
// re-reads the row from its first pixel. Bytes go into the history array
// DAT_0051fcb0[0..0x7f], and runs are emitted through EmitCopyRun (copy the
// buffered bytes) and EmitRepeatRun (a repeated value, or a count-only run of
// the key). The return value is the global byte counter DAT_0051fdb0, so a
// null `out` only measures the row, which is how CompressFrame asks for the
// size before compressing.
//
// What made it match (claude-opus-5-5, #4733): the loop head is ONE
// statement, `DAT_0051fcb0[n++] = c = *p++;`. The earlier 98.7% versions
// wrote the history store as `DAT_0051fcaf[n] = c` after a separate `n++`
// (the exe's operand is 0x51fcaf because MSVC folds the post-increment's -1
// into the address), and every spelling with a separate increment left the
// history store at the end of the block instead of right after the byte
// load. Writing the store with the post-increment index puts it in place.
extern unsigned char DAT_0051fcb0[];
extern int DAT_0051fdb0;

char* __stdcall EmitCopyRun(char* out, int count);
char* __stdcall EmitRepeatRun(char* out, int count, unsigned char a, unsigned char b);

// FUNCTION: 0x4ba000
int __stdcall CompressRow(char* dest, char* src, int width, unsigned char key)
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
        DAT_0051fcb0[n++] = c = *p++;
        value = c;
        switch (state) {
        case 0:
            if (c == key) {
                n--;
                out = EmitCopyRun(out, n);
                n = 1;
                DAT_0051fcb0[0] = c;
                runStart = 0;
                state = n;
                break;
            }
            if (n > 0x80) {
                n--;
                out = EmitCopyRun(out, n);
                DAT_0051fcb0[0] = c;
                n = 1;
                runStart = 0;
                break;
            }
            if (c == prev) {
                if (n - runStart >= 3) {
                    if (runStart > 0) {
                        out = EmitCopyRun(out, runStart);
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
            out = EmitRepeatRun(out, n - runStart - 1, prev, key);
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
        EmitCopyRun(out, n);
        break;
    case 1:
        EmitRepeatRun(out, n - runStart, value, key);
        return DAT_0051fdb0;
    }
    return DAT_0051fdb0;
}
