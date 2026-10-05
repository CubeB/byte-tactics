// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, re-checked by space-bunny-free. Names are provisional.
// LZSS tree walk (see 0x4d0b80.cpp and 0x4d0c50.cpp for the same node layout):
// walks the 6 byte node array of DAT_00526ff0 (parent, smaller, larger) from the
// root in root_larger, comparing 17 window bytes at cur + n against pos + n and
// keeping the longest run, then inserts or replaces a node. The window is the
// 0x1011 byte buffer DAT_00526ff4 and every index is masked with 0xfff.
// The inner loop index must be (cur + n), not (cur - pos) + j: referencing cur
// inside the loop is what makes MSVC give cur esi, pos ecx, best edi and the
// differing byte edx, which put the frame and every tree-walk address in place
// (57% to 93%, and the file shrank from 362 to the original's 373 bytes).
// The tree addressing modes ([edx + edi] rather than [edi + edx]) depend on the
// compiler state: <stdio.h> (or <windows.h>) at the top makes them match the
// original (tools/headers.py reports 126 sets that do).
#include <stdio.h>

struct Node_004d0de0 {
    unsigned short parent;   // +0x0
    unsigned short smaller;  // +0x2
    unsigned short larger;   // +0x4
};

struct Tree_004d0de0 {
    Node_004d0de0 nodes[0x1000];
    unsigned short root_parent;   // +0x6000
    unsigned short root_smaller;  // +0x6002
    unsigned short root_larger;   // +0x6004
};

extern Tree_004d0de0* DAT_00526ff0;
extern char* DAT_00526ff4;

// FUNCTION: 0x4d0de0
int __stdcall FUN_004d0de0(int pos, int* out)
{
    if (pos == 0)
        return 0;
    int best = 0;
    int cur = DAT_00526ff0->root_larger;
    for (;;) {
        int n, j, diff;
        for (n = 0, j = pos; n < 0x11; n++, j++) {
            diff = DAT_00526ff4[j & 0xfff] - DAT_00526ff4[(cur + n) & 0xfff];
            if (diff != 0)
                break;
        }
        if (n >= best) {
            best = n;
            *out = cur;
            if (n >= 0x11) {
                Node_004d0de0* par = &DAT_00526ff0->nodes[DAT_00526ff0->nodes[cur].parent];
                if (par->smaller == (unsigned short)cur)
                    par->smaller = pos;
                else
                    par->larger = pos;
                DAT_00526ff0->nodes[pos] = DAT_00526ff0->nodes[cur];
                DAT_00526ff0->nodes[DAT_00526ff0->nodes[pos].smaller].parent = pos;
                DAT_00526ff0->nodes[DAT_00526ff0->nodes[pos].larger].parent = pos;
                DAT_00526ff0->nodes[cur].parent = 0;
                return best;
            }
        }
        unsigned short* p;
        if (diff >= 0)
            p = &DAT_00526ff0->nodes[cur].larger;
        else
            p = &DAT_00526ff0->nodes[cur].smaller;
        if (*p == 0) {
            *p = pos;
            DAT_00526ff0->nodes[pos].parent = cur;
            DAT_00526ff0->nodes[pos].larger = 0;
            DAT_00526ff0->nodes[pos].smaller = 0;
            return best;
        }
        cur = *p;
    }
}
