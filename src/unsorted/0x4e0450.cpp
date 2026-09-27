// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree>: moves
// the iterator (a node pointer at +0) to the next node in order, under a lock.
// DAT_005292c4 is the tree's _Nil node; the inlined _Min (0x4e04e0) takes its
// own lock.
#include <yvals.h>

struct Node_004e0450 {
    Node_004e0450* left;               // +0x0
    Node_004e0450* parent;             // +0x4
    Node_004e0450* right;              // +0x8
};

extern Node_004e0450* DAT_005292c4;

static inline Node_004e0450* Min(Node_004e0450* p)
{
    std::_Lockit lock;
    while (p->left != DAT_005292c4)
        p = p->left;
    return p;
}

class Class_004e0450 {
public:
    Node_004e0450* ptr;                // +0x0

    void FUN_004e0450();
};

// FUNCTION: 0x4e0450
void Class_004e0450::FUN_004e0450()
{
    std::_Lockit lock;
    if (ptr->right != DAT_005292c4)
        ptr = Min(ptr->right);
    else {
        Node_004e0450* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
