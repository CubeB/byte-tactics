// Decompiled by Opus. Names are provisional.
// Looks like ContractNode() from the classic LZSS binary-tree compressor:
// the node's only child takes its place under the node's parent.

struct Node_004d0b10 {
    unsigned short parent;           // +0x0
    unsigned short smaller;          // +0x2
    unsigned short larger;           // +0x4
};

extern Node_004d0b10* DAT_00526ff0;

// FUNCTION: 0x4d0b10
void __stdcall FUN_004d0b10(int oldNode, int newNode)
{
    DAT_00526ff0[newNode].parent = DAT_00526ff0[oldNode].parent;
    if (DAT_00526ff0[DAT_00526ff0[oldNode].parent].larger == (unsigned short)oldNode)
        DAT_00526ff0[DAT_00526ff0[oldNode].parent].larger = newNode;
    else
        DAT_00526ff0[DAT_00526ff0[oldNode].parent].smaller = newNode;
    DAT_00526ff0[oldNode].parent = 0;
}
