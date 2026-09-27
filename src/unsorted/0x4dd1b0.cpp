// Decompiled by Sonnet. Names are provisional.
// Shaped like std::_Tree<...>::_Min(_Nodeptr) from MSVC 5's <xtree>: walks
// down the left-child chain to the leftmost node. DAT_00528a54 is the tree's
// _Nil sentinel node.
#include <yvals.h>

struct Node_004dd1b0 {
    Node_004dd1b0* left;                // +0x0
};

extern Node_004dd1b0* DAT_00528a54;

// FUNCTION: 0x4dd1b0
Node_004dd1b0* FUN_004dd1b0(Node_004dd1b0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}
