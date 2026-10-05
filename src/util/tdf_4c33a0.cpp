// Decompiled by Opus. Names are provisional.
// Blanks out C and C++ style comments in a text buffer, in place. A method
// that ignores `this`: its callers (0x4c2f60, 0x4c3120) pass their own
// `this` through in ecx.

// Handles the text at p (a // comment, a /* */ comment, or one ordinary
// character) and returns where to continue.
static inline char* SkipComment(char* p)
{
    if (p[0] == '/' && p[1] == '/') {
        while (1) {
            if (*p == '\n')
                break;
            *p = ' ';
            p++;
            if (*p == 0)
                break;
        }
    } else if (p[0] == '/' && p[1] == '*') {
        p[1] = ' ';
        p[0] = ' ';
        p += 2;
        while (*p) {
            if (p[-1] == '*' && p[0] == '/') {
                p[0] = ' ';
                p[-1] = ' ';
                p++;
                break;
            }
            p[-1] = ' ';
            p++;
        }
    } else {
        p++;
    }
    return p;
}

class Class_004c33a0 {
public:
    void StripComments(char* p);
};

// FUNCTION: 0x4c33a0
void Class_004c33a0::StripComments(char* p)
{
    while (*p)
        p = SkipComment(p);
}
