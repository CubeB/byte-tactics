// Decompiled by space-bunny-free. Names are provisional.
// DeleteNode() of the classic LZSS binary-tree window: unlinks node t from
// the tree, promoting its only child, or its in-order successor when it has
// two children. Field names follow the matched 0x4d0b80.cpp (parent, smaller,
// larger). The parent index is read back from the array on each use rather
// than kept in a local: that is what puts the index in edi and the 16-bit
// parent in bx, and it needs <stdio.h> plus <stdlib.h> to hold.

#include <stdio.h>
#include <stdlib.h>

struct Node_004d0c50 {
    unsigned short parent;           // +0x0
    unsigned short smaller;          // +0x2
    unsigned short larger;           // +0x4
};

extern Node_004d0c50* DAT_00526ff0;

void __stdcall LzssDeleteString(int t);

// FUNCTION: 0x4d0c50
void __stdcall LzssDeleteString(int t)
{
    if (DAT_00526ff0[t].parent == 0) {
        return;
    }
    if (DAT_00526ff0[t].larger == 0) {
        int q = DAT_00526ff0[t].smaller;
        DAT_00526ff0[q].parent = DAT_00526ff0[t].parent;
        if (DAT_00526ff0[DAT_00526ff0[t].parent].larger == (unsigned short)t) {
            DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
        } else {
            DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
        }
        DAT_00526ff0[t].parent = 0;
        return;
    }
    if (DAT_00526ff0[t].smaller == 0) {
        int q = DAT_00526ff0[t].larger;
        DAT_00526ff0[q].parent = DAT_00526ff0[t].parent;
        if (DAT_00526ff0[DAT_00526ff0[t].parent].larger == (unsigned short)t) {
            DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
        } else {
            DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
        }
        DAT_00526ff0[t].parent = 0;
        return;
    }
    int q = DAT_00526ff0[t].smaller;
    while (DAT_00526ff0[q].larger != 0) {
        q = DAT_00526ff0[q].larger;
    }
    LzssDeleteString(q);
    if (DAT_00526ff0[DAT_00526ff0[t].parent].smaller == (unsigned short)t) {
        DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
    } else {
        DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
    }
    DAT_00526ff0[q] = DAT_00526ff0[t];
    DAT_00526ff0[DAT_00526ff0[q].smaller].parent = q;
    DAT_00526ff0[DAT_00526ff0[q].larger].parent = q;
    DAT_00526ff0[t].parent = 0;
}
