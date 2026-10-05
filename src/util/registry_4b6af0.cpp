// Decompiled by Opus. Names are provisional.

// The loop test was an inlined helper with one return per outcome. Its
// multi-block body stops MSVC from rotating the loop (moving the test to the
// bottom) and from merging the two identical "next line" branches; written
// as a plain `while (n != lines)` the loop is rotated and the branches merged.
static inline int NotDone(int wanted, int current)
{
    if (wanted == current) {
        return 0;
    }
    return 1;
}

// Returns a pointer to the start of line `n` of `text`; lines end at '\n' or '\0'.
// FUNCTION: 0x4b6af0
char* __stdcall SkipTextLines(char* text, int n)
{
    int i = 0;
    int lines = 0;
    while (NotDone(n, lines)) {
        if (text[i] == 0) {
            lines++;
            i++;
        } else if (text[i] == '\n') {
            lines++;
            i++;
        } else {
            i++;
        }
    }
    return text + i;
}
