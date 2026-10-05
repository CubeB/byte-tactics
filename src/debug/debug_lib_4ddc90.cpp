// Decompiled by Opus. Names are provisional.
// std::_Tree<unsigned int, ...>::_Lbound(const key&) from MSVC 5's <xtree>,
// written out by hand: DAT_00528a50 is the tree's _Nil node, keys are
// unsigned ints compared with less<>.
#include <yvals.h>

struct Node_004ddc90 {
    Node_004ddc90* left;               // +0x0
    Node_004ddc90* parent;             // +0x4
    Node_004ddc90* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern Node_004ddc90* DAT_00528a50;

struct Less_004ddc90 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004ddc90 {
public:
    Less_004ddc90 compare;             // +0x0
    Node_004ddc90* head;               // +0x4

    Node_004ddc90* FUN_004ddc90(const unsigned int& key);
};

// FUNCTION: 0x4ddc90
Node_004ddc90* Class_004ddc90::FUN_004ddc90(const unsigned int& key)
{
    std::_Lockit lock;
    Node_004ddc90* x = head->parent;
    Node_004ddc90* y = head;
    while (x != DAT_00528a50) {
        if (compare(x->key, key))
            x = x->right;
        else
            y = x, x = x->left;
    }
    return y;
}
