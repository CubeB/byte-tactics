// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree>
// (recursively frees a subtree) under a lock object; DAT_005292c4 is the
// tree's _Nil node. Nodes are returned to a free list (DAT_00529e58) linked
// through their first dword instead of being deleted.
#include <yvals.h>

struct Node_004e03f0 {
    Node_004e03f0* left;               // +0x0
    Node_004e03f0* parent;             // +0x4
    Node_004e03f0* right;              // +0x8
};

extern Node_004e03f0* DAT_005292c4;
extern Node_004e03f0* DAT_00529e58;

static inline void FreeNode(Node_004e03f0* p)
{
    if (p != 0) {
        p->left = DAT_00529e58;
        DAT_00529e58 = p;
    }
}

class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004e03f0* x);
};

// FUNCTION: 0x4e03f0
void Class_004e03f0::FUN_004e03f0(Node_004e03f0* x)
{
    std::_Lockit lock;
    for (Node_004e03f0* y = x; y != DAT_005292c4; x = y) {
        FUN_004e03f0(y->right);
        y = y->left;
        FreeNode(x);
    }
}
