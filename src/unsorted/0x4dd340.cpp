// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree> (operator++ on the
// iterator of the tree whose _Nil node is DAT_00528a54, as in 0x4dd1b0 and
// 0x4dd250), with _Min inlined.
#include <yvals.h>

struct Node_004dd340 {
    Node_004dd340* left;               // +0x0
    Node_004dd340* parent;             // +0x4
    Node_004dd340* right;              // +0x8
};

extern Node_004dd340* DAT_00528a54;

static inline Node_004dd340* Min(Node_004dd340* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}

class Class_004dd340 {
public:
    Node_004dd340* ptr;                // +0x0

    void FUN_004dd340();
};

// FUNCTION: 0x4dd340
void Class_004dd340::FUN_004dd340()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a54)
        ptr = Min(ptr->right);
    else {
        Node_004dd340* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
