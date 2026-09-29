// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 39.4% (1134 vs 1304 bytes). Full control flow, structs and slot
// layout are transcribed; what still differs is register allocation, which
// rotates almost every later operand:
//  - the frame is 0x1ac, original 0x1b0. `own` is registerized into edi here
//    (so it has no stack slot and every later local sits 4 bytes lower); the
//    original keeps `own` in the [esp+0x20] slot and edi as a live zero, which
//    is what makes its `cmp eax,edi` (ours `test eax,eax`) and its root-init
//    zero stores use di. Forcing a zero constant did not move it.
//  - dest/src are swapped: original dest=ebx, src=ebp, ours dest=ebp, src=ebx.
//  - the 17-byte fill loop and the literal/length branch tails then differ in
//    which callee-saved register holds count/pos/cur.
// LZSS compressor, sibling of the matched 0x4d1480 (same window lock, same
// 0x1011 byte window, same tree at DAT_00526ff0 and the same error strings).
// See 0x4d0b80/0x4d0c10/0x4d0c50/0x4d0de0 for the tree node layout
// (parent, smaller, larger) and 0x4d0b10 for the one-child contract.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Node_004d0f60 {
    unsigned short parent;   // +0x0
    unsigned short smaller;  // +0x2
    unsigned short larger;   // +0x4
};

struct Tree_004d0f60 {
    Node_004d0f60 nodes[0x1000];
    unsigned short root_parent;   // +0x6000
    unsigned short root_smaller;  // +0x6002
    unsigned short root_larger;   // +0x6004
};

extern HANDLE DAT_0052a4f8;
extern long DAT_0052a4fc;
extern int DAT_0052a4f4;
extern int DAT_00526ff8;
extern int DAT_00526ffc;
extern char DAT_0051ffd0[];
extern char DAT_00520fe8[];
extern char* DAT_00526ff4;
extern Tree_004d0f60* DAT_00526ff0;

int __stdcall FUN_004d0b10(int oldNode, int newNode);
int __stdcall FUN_004d0b80(int oldNode, int newNode);
int __stdcall FUN_004d0c10(int index);
void __stdcall FUN_004d0c50(int t);
int __stdcall FUN_004d0de0(int pos, int* out);

// FUNCTION: 0x4d0f60
int __stdcall FUN_004d0f60(unsigned char* dest, unsigned char* src, int len)
{
    unsigned char flags;
    int cur;
    int pos;
    int count;
    int own;
    unsigned int mask;
    int n;
    int accum;
    unsigned char* end;
    unsigned char* base;
    unsigned char lit[0x81];
    unsigned short lenstack[0x81];
    int tid = (int)GetCurrentThreadId();

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
    end = src + len;
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
    DAT_00526ff0 = (Tree_004d0f60*)calloc(0x1001, 6);
    if (DAT_00526ff0 == 0) {
        printf("Could not alloc compression tree.\n");
        if (own == 0) {
            DAT_0052a4f4 = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    pos = 1;
    if (DAT_00526ff8 != 0 && DAT_00526ffc != 0) {
        memcpy(DAT_00526ff4, DAT_0051ffd0, 0x1011);
        memcpy(DAT_00526ff0, DAT_00520fe8, 0x6006);
    } else {
        DAT_00526ff0->root_parent = 0;
        DAT_00526ff0->root_larger = 1;
        DAT_00526ff0->root_smaller = 0;
        DAT_00526ff0->nodes[1].parent = 0x1000;
        DAT_00526ff0->nodes[1].larger = 0;
        DAT_00526ff0->nodes[1].smaller = 0;
    }
    count = 0;
    while (src < end) {
        DAT_00526ff4[count + 1] = *src++;
        count++;
        if (count >= 0x11)
            break;
    }
    cur = 0;
    accum = 0;
    mask = 1;
    flags = 0;
    while (count > 0) {
        int j;
        if (cur > count)
            cur = count;
        if (cur <= 1) {
            lit[mask] = DAT_00526ff4[pos];
            n = 1;
        } else {
            lenstack[mask] = (unsigned short)(((cur - 2) & 0xf) | (accum << 4));
            flags |= (unsigned char)mask;
            n = cur;
        }
        mask <<= 1;
        if (mask & 0x100) {
            *dest++ = flags;
            for (j = 0; j < 8; j++) {
                if ((flags & 0xff) & (1 << j)) {
                    *dest++ = (unsigned char)lenstack[1 << j];
                    *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
                } else {
                    *dest++ = lit[1 << j];
                }
            }
            mask = 1;
            flags = 0;
        }
        if (n > 0) {
            do {
                int p17 = (pos + 0x11) & 0xfff;
                if (DAT_00526ff0->nodes[p17].parent != 0) {
                    if (DAT_00526ff0->nodes[p17].larger == 0) {
                        FUN_004d0b10(p17, DAT_00526ff0->nodes[p17].smaller);
                    } else if (DAT_00526ff0->nodes[p17].smaller == 0) {
                        FUN_004d0b10(p17, DAT_00526ff0->nodes[p17].larger);
                    } else {
                        int q = FUN_004d0c10(p17);
                        FUN_004d0c50(q);
                        FUN_004d0b80(p17, q);
                    }
                }
                if (src < end)
                    DAT_00526ff4[p17] = *src++;
                else
                    count--;
                pos = (pos + 1) & 0xfff;
                if (count != 0)
                    cur = FUN_004d0de0(pos, &accum);
            } while (--n != 0);
        }
    }
    lenstack[mask] = 0;
    flags |= (unsigned char)mask;
    {
        int j;
        int cnt = 1;
        if (mask != 0) {
            do {
                cnt++;
                mask >>= 1;
            } while (mask != 0);
        }
        *dest++ = flags;
        for (j = 0; j < cnt; j++) {
            if ((flags & 0xff) & (1 << j)) {
                *dest++ = (unsigned char)lenstack[1 << j];
                *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
            } else {
                *dest++ = lit[1 << j];
            }
        }
    }
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff0);
        DAT_00526ff0 = 0;
    }
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    int written = (int)(dest - base);
    if (own == 0) {
        DAT_0052a4f4 = 0;
        InterlockedExchange(&DAT_0052a4fc, 0);
        SetEvent(DAT_0052a4f8);
    }
    return written;
}
