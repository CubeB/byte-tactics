// Decompiled by deepseek-v4.1. Names are provisional.
// LZSS compressor start-up: allocate the window and tree, seed the root and one
// node, copy the first len bytes into the window, insert each of them in the
// tree, then publish the state in the static buffers. Sibling of the already
// matched 0x4d0a10 (same root/node store order) and 0x4d0de0 (InsertNode).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entry_004d1670 {
    short a;   // +0x0
    short b;   // +0x2
    short c;   // +0x4
};

struct Tree_004d1670 {
    Entry_004d1670 entries[0x1000];
    short field_6000;   // +0x6000
    short field_6002;   // +0x6002
    short field_6004;   // +0x6004
};

extern char* DAT_00526ff4;
extern Tree_004d1670* DAT_00526ff0;
extern int g_lzssPresetReady;
extern char g_lzssPresetWindow[];
extern char g_lzssPresetTree[];

int __stdcall LzssAddString(int pos, int* out);

// FUNCTION: 0x4d1670
int __stdcall LzssSetPreset(unsigned char* src, int len)
{
    int out;
    int n;
    int i;

    DAT_00526ff4 = (char*)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        return -1;
    }
    DAT_00526ff0 = (Tree_004d1670*)calloc(0x1001, 6);
    if (DAT_00526ff0 == 0) {
        printf("Could not alloc compression tree.\n");
        return -1;
    }
    n = len;
    if (n > 4000)
        n = 4000;
    DAT_00526ff0->field_6000 = 0;
    DAT_00526ff0->field_6004 = 0x12;
    DAT_00526ff0->field_6002 = 0;
    DAT_00526ff0->entries[0x12].a = 0x1000;
    DAT_00526ff0->entries[0x12].c = 0;
    DAT_00526ff0->entries[0x12].b = 0;
    memcpy(DAT_00526ff4 + 0x13, src, n);
    memcpy(g_lzssPresetWindow, DAT_00526ff4, 0x1011);
    for (i = 0; i < n; i++)
        LzssAddString(i + 0x13, &out);
    memcpy(g_lzssPresetTree, DAT_00526ff0, 0x6006);
    g_lzssPresetReady = 1;
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
        return 0;
    }
    free(DAT_00526ff0);
    DAT_00526ff0 = 0;
    return 0;
}

