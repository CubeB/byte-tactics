// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lbound(const _K&) from MSVC 5's <xtree> for a
// tree keyed by int (std::less<int>), under a lock object; DAT_0051fbbc is the
// tree's _Nil node and head->parent is the root.
#include <yvals.h>

struct Node_004b3430 {
    Node_004b3430* left;            // +0x0
    Node_004b3430* parent;          // +0x4
    Node_004b3430* right;           // +0x8
    int key;                        // +0xc
};

struct IntLess_004b3430 {
    bool operator()(const int& a, const int& b) const
    {
        return a < b;
    }
};

extern Node_004b3430* DAT_0051fbbc;

class Class_004b3430 {
public:
    char allocator;                 // +0x0
    IntLess_004b3430 key_compare;   // +0x1
    Node_004b3430* head;            // +0x4
    Node_004b3430* FUN_004b3430(const int& key);
};

// FUNCTION: 0x4b3430
Node_004b3430* Class_004b3430::FUN_004b3430(const int& key)
{
    std::_Lockit lock;
    Node_004b3430* x = head->parent;
    Node_004b3430* y = head;
    while (x != DAT_0051fbbc)
        if (key_compare(x->key, key))
            x = x->right;
        else
            y = x, x = x->left;
    return y;
}
