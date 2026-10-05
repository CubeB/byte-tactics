// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::const_iterator::_Inc() from MSVC 5's <xtree>
// (step an iterator to the in-order successor) under a lock object;
// DAT_00528a50 is the tree's _Nil node. Compare 0x4dd710 and 0x4ddc90.
#include <yvals.h>

struct Node_004dde70 {
    Node_004dde70* left;               // +0x0
    Node_004dde70* parent;             // +0x4
    Node_004dde70* right;              // +0x8
};

extern Node_004dde70* DAT_00528a50;

static inline Node_004dde70* Min(Node_004dde70* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a50)
        p = p->left;
    return p;
}

class Class_004dde70 {
public:
    Node_004dde70* ptr;                // +0x0
    void FUN_004dde70();
};

// FUNCTION: 0x4dde70
void Class_004dde70::FUN_004dde70()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a50)
        ptr = Min(ptr->right);
    else {
        Node_004dde70* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
