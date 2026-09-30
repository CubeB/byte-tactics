// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by DeepSeek V4.1 Flash. Names are provisional.
// MATCH (1304 bytes). One-child tree updates are inlined in the executable;
// supplying the matched helper body restores those blocks. The byte-copy loop
// must be a plain `while (n0 < 0x11)` with the src check inside (do-while gets
// rotated). What took it from 84.1% to 99.2%:
//  - the encode loop is `while (n > 0) { ...; n--; }`, not `do/while (--n)`;
//    the while form keeps n in ebp and frees esi for the window (the residual
//    was the register-allocation cascade, not the loop body);
//  - in the `cur <= 1` arm `n = 1;` must be written BEFORE the lit store;
//  - the main byte-output loop is `int f = flags & 0xff; if (!(f & (1 << j)))`
//    with the lit arm as the then-block;
//  - the inner window update is `if (src >= end) state.count--; else ...`.
// The last 0.8% was three operand-order-only instructions, size already exact:
//  - preload loop: name the index `int k = n0 + 1; DAT_00526ff4[k] = *src++;`
//    rather than the inline `DAT_00526ff4[n0 + 1]`; materialising the index
//    flips the SIB base back to the global so it emits `[edx+eax+1]`.
//  - main byte-output loop: hoisting `int f = flags & 0xff;` above the `if`
//    and testing `!(f & (1 << j))` emits `test esi,eax` (flags first); the
//    inline `!((1 << j) & (flags & 0xff))` always emits `test eax,esi`.
//  - window store: recompute the index inline,
//    `DAT_00526ff4[(state.pos + 0x11) & 0xfff] = *src++;`, instead of using
//    the `p17` local; that flips `[esi+edx]` to `[edx+esi]`.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Node_004d0f60 {
    unsigned short parent;  // +0x0
    unsigned short smaller; // +0x2
    unsigned short larger;  // +0x4
};

struct Tree_004d0f60 {
    Node_004d0f60 nodes[0x1001];
};

extern HANDLE DAT_0052a4f8;
extern long DAT_0052a4fc;
extern int DAT_0052a4f4;
extern int DAT_00526ff8;
extern int DAT_00526ffc;
extern char DAT_0051ffd0[];
extern char DAT_00520fe8[];
extern char *DAT_00526ff4;
extern Tree_004d0f60 *DAT_00526ff0;

inline void __stdcall FUN_004d0b10(int oldNode, int newNode) {
    DAT_00526ff0->nodes[newNode].parent = DAT_00526ff0->nodes[oldNode].parent;
    if (DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].larger == (unsigned short)oldNode)
        DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].larger = (unsigned short)newNode;
    else
        DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].smaller = (unsigned short)newNode;
    DAT_00526ff0->nodes[oldNode].parent = 0;
}

int __stdcall FUN_004d0b80(int oldNode, int newNode);
int __stdcall FUN_004d0c10(int index);
void __stdcall FUN_004d0c50(int t);
int __stdcall FUN_004d0de0(int pos, int *out);

// FUNCTION: 0x4d0f60
int __stdcall FUN_004d0f60(unsigned char *dest, unsigned char *src, int len) {
    unsigned char flags;
    int n;
    unsigned int mask;
    int accum;
    unsigned char *end;
    unsigned char *base;
    unsigned char lit[0x81];
    unsigned short lenstack[0x81];
    struct {
        int cur;
        int pos;
        int count;
        int own;
    } state;
    int tid = (int)GetCurrentThreadId();

    while (1) {
        int r = InterlockedExchange(&DAT_0052a4fc, tid);
        if (r == 0) {
            DAT_0052a4f4 = tid;
            state.own = 0;
            break;
        }
        if (DAT_0052a4f4 == tid) {
            state.own = r;
            break;
        }
        WaitForSingleObject(DAT_0052a4f8, -1);
    }
    base = dest;
    end = src + len;
    DAT_00526ff4 = (char *)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        if (state.own == 0) {
            DAT_0052a4f4 = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    DAT_00526ff0 = (Tree_004d0f60 *)calloc(0x1001, 6);
    if (DAT_00526ff0 == 0) {
        printf("Could not alloc compression tree.\n");
        if (state.own == 0) {
            DAT_0052a4f4 = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    state.pos = 1;
    if (DAT_00526ff8 != 0 && DAT_00526ffc != 0) {
        memcpy(DAT_00526ff4, DAT_0051ffd0, 0x1011);
        memcpy(DAT_00526ff0, DAT_00520fe8, 0x6006);
    } else {
        DAT_00526ff0->nodes[0x1000].parent = 0;
        DAT_00526ff0->nodes[0x1000].larger = 1;
        DAT_00526ff0->nodes[0x1000].smaller = 0;
        DAT_00526ff0->nodes[1].parent = 0x1000;
        DAT_00526ff0->nodes[1].larger = 0;
        DAT_00526ff0->nodes[1].smaller = 0;
    }
    {
        int n0 = 0;
        while (n0 < 0x11) {
            if (src >= end)
                break;
            { int k = n0 + 1; DAT_00526ff4[k] = *src++; }
            n0++;
        }
        state.count = n0;
    }
    state.cur = 0;
    accum = 0;
    mask = 1;
    flags = 0;
    while (state.count > 0) {
        int j;
        if (state.cur > state.count)
            state.cur = state.count;
        if (state.cur <= 1) {
            n = 1;
            lit[mask] = DAT_00526ff4[state.pos];
        } else {
            lenstack[mask] = (unsigned short)(((state.cur - 2) & 0xf) | (accum << 4));
            flags |= (unsigned char)mask;
            n = state.cur;
        }
        mask <<= 1;
        if (mask & 0x100) {
            *dest++ = flags;
            for (j = 0; j < 8; j++) {
                int f = flags & 0xff;
                if (!(f & (1 << j))) {
                    *dest++ = lit[1 << j];
                } else {
                    *dest++ = (unsigned char)lenstack[1 << j];
                    *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
                }
            }
            mask = 1;
            flags = 0;
        }
        while (n > 0) {
                int p17 = (state.pos + 0x11) & 0xfff;
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
                if (src >= end)
                    state.count--;
                else
                    DAT_00526ff4[(state.pos + 0x11) & 0xfff] = *src++;
                state.pos = (state.pos + 1) & 0xfff;
                if (state.count != 0)
                    state.cur = FUN_004d0de0(state.pos, &accum);
            n--;
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
            if (!((1 << j) & (flags & 0xff))) {
                *dest++ = lit[1 << j];
            } else {
                *dest++ = (unsigned char)lenstack[1 << j];
                *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
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
    if (state.own == 0) {
        DAT_0052a4f4 = 0;
        InterlockedExchange(&DAT_0052a4fc, 0);
        SetEvent(DAT_0052a4f8);
    }
    return written;
}
