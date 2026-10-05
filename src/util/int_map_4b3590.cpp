// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree>, with
// the inlined _Min taking its own lock, for the tree whose _Nil node is
// DAT_0051fbbc (see 0x4b3430.cpp).
#include <yvals.h>

struct Node_004b3590 {
    Node_004b3590* left;            // +0x0
    Node_004b3590* parent;          // +0x4
    Node_004b3590* right;           // +0x8
};

extern Node_004b3590* DAT_0051fbbc;

static inline Node_004b3590* Min(Node_004b3590* p)
{
    std::_Lockit lock;
    while (p->left != DAT_0051fbbc)
        p = p->left;
    return p;
}

class Class_004b3590 {
public:
    Node_004b3590* ptr;             // +0x0
    void FUN_004b3590();
};

// FUNCTION: 0x4b3590
void Class_004b3590::FUN_004b3590()
{
    std::_Lockit lock;
    if (ptr->right != DAT_0051fbbc)
        ptr = Min(ptr->right);
    else {
        Node_004b3590* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
