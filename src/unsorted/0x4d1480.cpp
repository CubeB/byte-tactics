// Decompiled by space-bunny-free. Names are provisional.
// LZ decompressor. It first takes the window lock: a spinlock on DAT_0052a4fc
// holding the owning thread id, with DAT_0052a4f4 naming the owner and
// DAT_0052a4f8 the event that releases the waiters. Then it allocates the
// 0x1011 byte sliding window, and reloads it from the template at DAT_0051ffd0
// unless a re-entrant call is in progress (DAT_00526ffc) or the template was
// never set up (DAT_00526ff8). The bit stream that follows is read through a
// mask that counts 1, 2, 4 ... 0x80 and then reloads a fresh flag byte: a clear
// bit is a literal byte, a set bit is a 16 bit word whose high 12 bits are the
// distance back into the window and whose low four bits are the length minus
// two, and a distance of zero ends the stream. Every output byte also goes
// into the window at the current position, which starts at 1 and wraps at
// 0xfff. The window is freed at the end, the lock released if this thread took
// it, and the number of bytes written returned.
// The last byte, the operand order of the loop head's flags & mask test, comes
// from the header block, not from the source: which of the two byte loads MSVC
// emits first depends on how many declarations the file has seen. With
// <windows.h>, <stdio.h>, <stdlib.h> and <string.h> this file was one
// register pair out (98.7%, 493 bytes on both sides, mask loaded first instead
// of flags). Adding <math.h>, which the function does not use, fixes it and
// nothing else in the source had to change.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern HANDLE DAT_0052a4f8;
extern long DAT_0052a4fc;
extern int DAT_0052a4f4;
extern int DAT_00526ff8;
extern int DAT_00526ffc;
extern char DAT_0051ffd0[];
extern char* DAT_00526ff4;

// FUNCTION: 0x4d1480
int __stdcall FUN_004d1480(unsigned char* dest, unsigned char* src)
{
    unsigned char* base;
    int own;
    int tid = (int)GetCurrentThreadId();
    int i;
    int pos;
    unsigned mask;
    unsigned char flags;
    while (1) {
        int r = InterlockedExchange(&DAT_0052a4fc, tid);
        if (r == 0) {
            DAT_0052a4f4 = tid;
            own = 0;
            break;
        }
        if (DAT_0052a4f4 == tid) {
            own = r;
            break;
        }
        WaitForSingleObject(DAT_0052a4f8, -1);
    }
    base = dest;
    DAT_00526ff4 = (char*)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        if (own == 0) {
            DAT_0052a4f4 = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    if (DAT_00526ff8 && DAT_00526ffc)
        memcpy(DAT_00526ff4, DAT_0051ffd0, 0x1011);
    pos = 1;
    flags = *src++;
    mask = 1;
    for (;;) {
        if (!(flags & mask)) {
            unsigned char c = *src;
            *dest++ = c;
            src++;
            DAT_00526ff4[pos] = c;
            pos = (pos + 1) & 0xfff;
        } else {
            int w = *(unsigned short*)src;
            src += 2;
            unsigned dist = (unsigned)w >> 4;
            int len = (w & 0xf) + 2;
            if (dist == 0)
                break;
            for (i = 0; i < len; i++) {
                unsigned char c = DAT_00526ff4[(dist + i) & 0xfff];
                *dest++ = c;
                DAT_00526ff4[pos] = c;
                pos = (pos + 1) & 0xfff;
            }
        }
        mask <<= 1;
        if (mask & 0x100) {
            mask = 1;
            flags = *src++;
        }
    }
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    int n = (int)(dest - base);
    if (own == 0) {
        DAT_0052a4f4 = 0;
        InterlockedExchange(&DAT_0052a4fc, 0);
        SetEvent(DAT_0052a4f8);
    }
    return n;
}
