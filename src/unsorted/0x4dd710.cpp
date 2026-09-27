// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; DAT_00528a50
// is the tree's _Nil node and head->parent is the root.
#include <yvals.h>

struct Node_004dd710 {
    Node_004dd710* left;               // +0x0
    Node_004dd710* parent;             // +0x4
    Node_004dd710* right;              // +0x8
};

extern Node_004dd710* DAT_00528a50;

class Class_004dd710 {
public:
    int unknown_0;
    Node_004dd710* head;               // +0x4
    void FUN_004dd710(Node_004dd710* x);
};

// FUNCTION: 0x4dd710
void Class_004dd710::FUN_004dd710(Node_004dd710* x)
{
    std::_Lockit lock;
    Node_004dd710* y = x->right;
    x->right = y->left;
    if (y->left != DAT_00528a50)
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
