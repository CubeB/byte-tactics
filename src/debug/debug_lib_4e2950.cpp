// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; DAT_005292c4
// is the tree's _Nil node and head->parent is the root. Its _Rrotate is the
// next function, 0x4e29b0.
#include <yvals.h>

struct Node_004e2950 {
    Node_004e2950* left;            // +0x0
    Node_004e2950* parent;          // +0x4
    Node_004e2950* right;           // +0x8
};

extern Node_004e2950* DAT_005292c4;

class Class_004e2950 {
public:
    int unknown_0;
    Node_004e2950* head;            // +0x4
    void FUN_004e2950(Node_004e2950* x);
};

// FUNCTION: 0x4e2950
void Class_004e2950::FUN_004e2950(Node_004e2950* x)
{
    std::_Lockit lock;
    Node_004e2950* y = x->right;
    x->right = y->left;
    if (y->left != DAT_005292c4)
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
