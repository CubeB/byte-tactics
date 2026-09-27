// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Rrotate(_Nodeptr) from MSVC 5's <xtree>
// (right rotation of a red-black tree node) under a lock object; DAT_005292c4
// is the tree's _Nil node and head->parent is the root.
#include <yvals.h>

struct Node_004e29b0 {
    Node_004e29b0* left;            // +0x0
    Node_004e29b0* parent;          // +0x4
    Node_004e29b0* right;           // +0x8
};

extern Node_004e29b0* DAT_005292c4;

class Class_004e29b0 {
public:
    int unknown_0;
    Node_004e29b0* head;            // +0x4
    void FUN_004e29b0(Node_004e29b0* x);
};

// FUNCTION: 0x4e29b0
void Class_004e29b0::FUN_004e29b0(Node_004e29b0* x)
{
    std::_Lockit lock;
    Node_004e29b0* y = x->left;
    x->left = y->right;
    if (y->right != DAT_005292c4)
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
