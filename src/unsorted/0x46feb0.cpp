// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; DAT_0051e598
// is the tree's _Nil node and head->parent is the root.
#include <yvals.h>

struct Node_0046feb0 {
    Node_0046feb0* left;            // +0x0
    Node_0046feb0* parent;          // +0x4
    Node_0046feb0* right;           // +0x8
};

extern Node_0046feb0* DAT_0051e598;

class Class_0046feb0 {
public:
    int unknown_0;
    Node_0046feb0* head;            // +0x4
    void FUN_0046feb0(Node_0046feb0* x);
};

// FUNCTION: 0x46feb0
void Class_0046feb0::FUN_0046feb0(Node_0046feb0* x)
{
    std::_Lockit lock;
    Node_0046feb0* y = x->right;
    x->right = y->left;
    if (y->left != DAT_0051e598)
        y->left->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}
