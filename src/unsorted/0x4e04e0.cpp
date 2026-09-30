// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Min(_Nodeptr) from MSVC 5's <xtree>: follows
// left links under a lock until the tree's _Nil node (DAT_005292c4). Its
// caller (0x4df380) uses it for an inlined iterator increment.
#include <yvals.h>

struct Node_004e04e0 {
    Node_004e04e0* left;               // +0x0
    Node_004e04e0* parent;             // +0x4
    Node_004e04e0* right;              // +0x8
};

extern Node_004e04e0* DAT_005292c4;

// FUNCTION: 0x4e04e0
Node_004e04e0* __cdecl FUN_004e04e0(Node_004e04e0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_005292c4)
        p = p->left;
    return p;
}
