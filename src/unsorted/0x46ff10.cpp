// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Rrotate(_Nodeptr) from MSVC 5's <xtree>
// (right rotation of a red-black tree node) under a lock object; DAT_0051e598
// is the tree's _Nil node and head->parent is the root. Mirror of 0x46feb0.
#include <yvals.h>

struct Node_0046ff10 {
    Node_0046ff10* left;            // +0x0
    Node_0046ff10* parent;          // +0x4
    Node_0046ff10* right;           // +0x8
};

extern Node_0046ff10* DAT_0051e598;

class Class_0046ff10 {
public:
    int unknown_0;
    Node_0046ff10* head;            // +0x4
    void FUN_0046ff10(Node_0046ff10* x);
};

// FUNCTION: 0x46ff10
void Class_0046ff10::FUN_0046ff10(Node_0046ff10* x)
{
    std::_Lockit lock;
    Node_0046ff10* y = x->left;
    x->left = y->right;
    if (y->right != DAT_0051e598)
        y->right->parent = x;
    y->parent = x->parent;
    if (x == head->parent)
        head->parent = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}
