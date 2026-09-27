// Decompiled by Opus. Names are provisional.
// Walks a list of nodes (and, recursively, their children) and returns the
// largest element value plus the owning node's offset, or 0 if none.

struct Elem_004cb5f0 {                 // 0xc bytes
    int unknown_0;
    int value;                         // +0x4
    int unknown_8;
};

struct Node_004cb5f0 {
    char unknown_0[4];
    int count;                         // +0x4
    char unknown_8[0xc];
    int offset;                        // +0x14
    char unknown_18[0xc];
    Elem_004cb5f0* elems;              // +0x24
    char unknown_28[4];
    Node_004cb5f0* next;               // +0x2c
    Node_004cb5f0* child;              // +0x30
};

int __stdcall FUN_004cb5f0(Node_004cb5f0* node);

// FUNCTION: 0x4cb5f0
int __stdcall FUN_004cb5f0(Node_004cb5f0* node)
{
    int best = 0;
    for (; node; node = node->next) {
        for (int i = 0; i < node->count; i++) {
            int v = node->elems[i].value + node->offset;
            if (v > best)
                best = v;
        }
        if (node->child) {
            int v = FUN_004cb5f0(node->child) + node->offset;
            if (v > best)
                best = v;
        }
    }
    return best;
}
