// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lrotate(_Nodeptr) from MSVC 5's <xtree>
// (left rotation of a red-black tree node) under a lock object; DAT_00528a54
// is the tree's _Nil node and head->parent is the root. Same code as 0x4b3310.
#include <yvals.h>

struct Node_004dd150 {
    Node_004dd150* left;            // +0x0
    Node_004dd150* parent;          // +0x4
    Node_004dd150* right;           // +0x8
};

extern Node_004dd150* DAT_00528a54;

class Class_004dd150 {
public:
    int unknown_0;
    Node_004dd150* head;            // +0x4
    void FUN_004dd150(Node_004dd150* x);
};

// FUNCTION: 0x4dd150
void Class_004dd150::FUN_004dd150(Node_004dd150* x)
{
    std::_Lockit lock;
    Node_004dd150* y = x->right;
    x->right = y->left;
    if (y->left != DAT_00528a54)
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
