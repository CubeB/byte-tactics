// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree>
// (recursively frees a subtree) under a lock object; DAT_0051fbbc is the
// tree's _Nil node.
#include <yvals.h>

struct Node_004b2fb0 {
    Node_004b2fb0* left;               // +0x0
    Node_004b2fb0* parent;             // +0x4
    Node_004b2fb0* right;              // +0x8
};

extern Node_004b2fb0* DAT_0051fbbc;

class Class_004b2fb0 {
public:
    void FUN_004b2fb0(Node_004b2fb0* x);
};

// FUNCTION: 0x4b2fb0
void Class_004b2fb0::FUN_004b2fb0(Node_004b2fb0* x)
{
    std::_Lockit lock;
    for (Node_004b2fb0* y = x; y != DAT_0051fbbc; x = y) {
        FUN_004b2fb0(y->right);
        y = y->left;
        operator delete(x);
    }
}
