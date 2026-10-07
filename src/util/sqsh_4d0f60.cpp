// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
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
extern int g_lzssPresetReady;
extern int g_lzssUsePreset;
extern char g_lzssPresetWindow[];
extern char g_lzssPresetTree[];
extern char *DAT_00526ff4;
extern Tree_004d0f60 *DAT_00526ff0;

inline void __stdcall LzssContractNode(int oldNode, int newNode) {
    DAT_00526ff0->nodes[newNode].parent = DAT_00526ff0->nodes[oldNode].parent;
    if (DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].larger == (unsigned short)oldNode)
        DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].larger = (unsigned short)newNode;
    else
        DAT_00526ff0->nodes[DAT_00526ff0->nodes[oldNode].parent].smaller = (unsigned short)newNode;
    DAT_00526ff0->nodes[oldNode].parent = 0;
}

int __stdcall LzssReplaceNode(int oldNode, int newNode);
int __stdcall LzssFindNextNode(int index);
void __stdcall LzssDeleteString(int t);
int __stdcall LzssAddString(int pos, int *out);

// FUNCTION: 0x4d0f60
int __stdcall LzssCompress(unsigned char *dest, unsigned char *src, int len) {
    unsigned char flags;
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
    if (g_lzssPresetReady != 0 && g_lzssUsePreset != 0) {
        memcpy(DAT_00526ff4, g_lzssPresetWindow, 0x1011);
        memcpy(DAT_00526ff0, g_lzssPresetTree, 0x6006);
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
        // Plain while with the src check inside: a do-while gets rotated.
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
        // n is assigned inside each branch below, not before the if.
        int n;
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
                        LzssContractNode(p17, DAT_00526ff0->nodes[p17].smaller);
                    } else if (DAT_00526ff0->nodes[p17].smaller == 0) {
                        LzssContractNode(p17, DAT_00526ff0->nodes[p17].larger);
                    } else {
                        int q = LzssFindNextNode(p17);
                        LzssDeleteString(q);
                        LzssReplaceNode(p17, q);
                    }
                }
                if (src >= end)
                    state.count--;
                else
                    DAT_00526ff4[(state.pos + 0x11) & 0xfff] = *src++;
                state.pos = (state.pos + 1) & 0xfff;
                if (state.count != 0)
                    state.cur = LzssAddString(state.pos, &accum);
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
