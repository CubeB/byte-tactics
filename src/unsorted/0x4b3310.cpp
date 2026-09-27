// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; DAT_0051fbbc
// is the tree's _Nil node and head->parent is the root. Same code as 0x46feb0.
#include <yvals.h>

struct Node_004b3310 {
    Node_004b3310* left;            // +0x0
    Node_004b3310* parent;          // +0x4
    Node_004b3310* right;           // +0x8
};

extern Node_004b3310* DAT_0051fbbc;

class Class_004b3310 {
public:
    int unknown_0;
    Node_004b3310* head;            // +0x4
    void FUN_004b3310(Node_004b3310* x);
};

// FUNCTION: 0x4b3310
void Class_004b3310::FUN_004b3310(Node_004b3310* x)
{
    std::_Lockit lock;
    Node_004b3310* y = x->right;
    x->right = y->left;
    if (y->left != DAT_0051fbbc)
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
