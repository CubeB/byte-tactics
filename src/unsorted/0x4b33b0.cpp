// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Rrotate(_Nodeptr) from MSVC 5's <xtree>
// (right rotation of a red-black tree node) under a lock object; DAT_0051fbbc
// is the tree's _Nil node and head->parent is the root. Mirror of 0x4b3310.
#include <yvals.h>

struct Node_004b33b0 {
    Node_004b33b0* left;            // +0x0
    Node_004b33b0* parent;          // +0x4
    Node_004b33b0* right;           // +0x8
};

extern Node_004b33b0* DAT_0051fbbc;

class Class_004b33b0 {
public:
    int unknown_0;
    Node_004b33b0* head;            // +0x4
    void FUN_004b33b0(Node_004b33b0* x);
};

// FUNCTION: 0x4b33b0
void Class_004b33b0::FUN_004b33b0(Node_004b33b0* x)
{
    std::_Lockit lock;
    Node_004b33b0* y = x->left;
    x->left = y->right;
    if (y->right != DAT_0051fbbc)
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
