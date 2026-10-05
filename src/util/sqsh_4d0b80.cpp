// Decompiled by Opus. Names are provisional.
// Looks like ReplaceNode() from the classic LZSS binary-tree compressor:
// newNode takes oldNode's place in the tree, and oldNode is unlinked.
// The headers are those of the LZSS file (see 0x4d0ae0.cpp); without any
// header MSVC swaps base and index in the two child loads.
#include <stdio.h>
#include <stdlib.h>

struct Node_004d0b80 {
    unsigned short parent;           // +0x0
    unsigned short smaller;          // +0x2
    unsigned short larger;           // +0x4
};

extern Node_004d0b80* DAT_00526ff0;

// FUNCTION: 0x4d0b80
void __stdcall LzssReplaceNode(int oldNode, int newNode)
{
    int parent = DAT_00526ff0[oldNode].parent;
    if (DAT_00526ff0[parent].smaller == (unsigned short)oldNode)
        DAT_00526ff0[parent].smaller = newNode;
    else
        DAT_00526ff0[parent].larger = newNode;
    DAT_00526ff0[newNode] = DAT_00526ff0[oldNode];
    DAT_00526ff0[DAT_00526ff0[newNode].smaller].parent = newNode;
    DAT_00526ff0[DAT_00526ff0[newNode].larger].parent = newNode;
    DAT_00526ff0[oldNode].parent = 0;
}
