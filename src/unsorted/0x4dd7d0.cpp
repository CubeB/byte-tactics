// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::_Ubound(const _K&) from MSVC 5's <xtree>: the first node
// whose key is greater than the given key, or the head node. DAT_00528a50 is
// the tree's _Nil node and head->parent is the root. Same shape as 0x4dd250.
#include <yvals.h>

struct Node_004dd7d0 {
    Node_004dd7d0* left;               // +0x0
    Node_004dd7d0* parent;             // +0x4
    Node_004dd7d0* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern Node_004dd7d0* DAT_00528a50;

struct Less_004dd7d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_004dd7d0 {
public:
    Less_004dd7d0 key_compare;
    Node_004dd7d0* head;               // +0x4

    Node_004dd7d0* FUN_004dd7d0(const unsigned int& kv);
};

// FUNCTION: 0x4dd7d0
Node_004dd7d0* Class_004dd7d0::FUN_004dd7d0(const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dd7d0* x = head->parent;
    Node_004dd7d0* y = head;
    while (x != DAT_00528a50)
        if (key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}
