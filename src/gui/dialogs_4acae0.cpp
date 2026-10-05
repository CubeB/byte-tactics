// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Interpolates the 256-entry RGB palette between entry `lo` and entry `hi`.
// Note: the two 0x400-byte copies run on a 768-byte stack buffer (see the bug
// note in the pull request); the original really does emit them, so they stay.
#include <string.h>

// FUNCTION: 0x4acae0
void __stdcall FUN_004acae0(unsigned char* data, int a, int b)
{
    unsigned char buf[768];
    memcpy(buf, data, 0x400);

    int lo = a < b ? a : b;
    int hi = a > b ? a : b;

    int base[3];
    int delta[3];
    int den = hi - lo;
    int i0 = lo * 3;
    base[0] = buf[i0 + 0];
    base[1] = buf[i0 + 1];
    base[2] = buf[i0 + 2];
    unsigned char* dst = &buf[i0];
    int i1 = hi * 3;
    delta[0] = buf[i1 + 0] - base[0];
    delta[1] = buf[i1 + 1] - base[1];
    delta[2] = buf[i1 + 2] - base[2];

    for (int i = lo; i < hi; i++) {
        for (int j = 0; j < 3; j++) {
            *dst = (unsigned char)(base[j] + delta[j] * (i - lo) / den);
            dst++;
        }
    }

    memcpy(data, buf, 0x400);
}
