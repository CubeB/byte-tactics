// Decompiled by Sonnet. Names are provisional.
// Shaped like std::_Tree<...>::_Min(_Nodeptr) from MSVC 5's <xtree>: walk
// left children under a lock object until the tree's _Nil node is reached.
#include <yvals.h>

struct Node_004b3370 {
    Node_004b3370* left;  // +0x0
};

extern Node_004b3370* DAT_0051fbbc;

// FUNCTION: 0x4b3370
Node_004b3370* __stdcall FUN_004b3370(Node_004b3370* p)
{
    std::_Lockit lock;
    while (p->left != DAT_0051fbbc)
        p = p->left;
    return p;
}
