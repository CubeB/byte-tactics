// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree>
// (recursively frees a subtree) under a lock object; DAT_0051e598 is the
// tree's _Nil node.
#include <yvals.h>

struct Node_0046f6d0 {
    Node_0046f6d0* left;               // +0x0
    Node_0046f6d0* parent;             // +0x4
    Node_0046f6d0* right;              // +0x8
};

extern Node_0046f6d0* DAT_0051e598;

class Class_0046f6d0 {
public:
    void FUN_0046f6d0(Node_0046f6d0* x);
};

// FUNCTION: 0x46f6d0
void Class_0046f6d0::FUN_0046f6d0(Node_0046f6d0* x)
{
    std::_Lockit lock;
    for (Node_0046f6d0* y = x; y != DAT_0051e598; x = y) {
        FUN_0046f6d0(y->right);
        y = y->left;
        operator delete(x);
    }
}
