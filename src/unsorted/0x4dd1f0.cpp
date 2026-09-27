// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Rrotate(_Nodeptr) from MSVC 5's <xtree>
// (right rotation of a red-black tree node) under a lock object, the same as
// 0x4e29b0 for another tree; DAT_00528a54 is the tree's _Nil node and
// head->parent is the root.
#include <yvals.h>

struct Node_004dd1f0 {
    Node_004dd1f0* left;            // +0x0
    Node_004dd1f0* parent;          // +0x4
    Node_004dd1f0* right;           // +0x8
};

extern Node_004dd1f0* DAT_00528a54;

class Class_004dd1f0 {
public:
    int unknown_0;
    Node_004dd1f0* head;            // +0x4
    void FUN_004dd1f0(Node_004dd1f0* x);
};

// FUNCTION: 0x4dd1f0
void Class_004dd1f0::FUN_004dd1f0(Node_004dd1f0* x)
{
    std::_Lockit lock;
    Node_004dd1f0* y = x->left;
    x->left = y->right;
    if (y->right != DAT_00528a54)
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
