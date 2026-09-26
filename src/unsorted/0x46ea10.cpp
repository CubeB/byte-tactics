// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree> (the in-order
// successor of a red-black tree node), with its std::_Lockit guards
// (constructor 0x4e39b0, destructor 0x4e3a70); DAT_0051e598 is the tree's
// _Nil node. Built without /GX, as Cavedog did, so the locks need no EH frame.
#include <yvals.h>

struct Node_0046ea10 {
    Node_0046ea10* left;               // +0x0
    Node_0046ea10* parent;             // +0x4
    Node_0046ea10* right;              // +0x8
};

extern Node_0046ea10* DAT_0051e598;

static inline Node_0046ea10* Min_0046ea10(Node_0046ea10* p)
{
    std::_Lockit lock;
    while (p->left != DAT_0051e598) {
        p = p->left;
    }
    return p;
}

class Class_0046ea10 {
public:
    Node_0046ea10* ptr;                // +0x0

    void FUN_0046ea10();
};

// FUNCTION: 0x46ea10
void Class_0046ea10::FUN_0046ea10()
{
    std::_Lockit lock;
    if (ptr->right != DAT_0051e598) {
        ptr = Min_0046ea10(ptr->right);
    } else {
        Node_0046ea10* p;
        while (ptr == (p = ptr->parent)->right) {
            ptr = p;
        }
        if (ptr->right != p) {
            ptr = p;
        }
    }
}
