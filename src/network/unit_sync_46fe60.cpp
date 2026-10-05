// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Lbound(const _K&) from MSVC 5's <xtree> for a
// tree keyed by unsigned int (the std::map<unsigned int, Rect> of 0x46e330),
// under a lock object; DAT_0051e598 is the tree's _Nil node and head->parent
// is the root.
#include <yvals.h>

struct Node_0046fe60 {
    Node_0046fe60* left;            // +0x0
    Node_0046fe60* parent;          // +0x4
    Node_0046fe60* right;           // +0x8
    unsigned int key;               // +0xc
};

struct Less_0046fe60 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

extern Node_0046fe60* DAT_0051e598;

class Class_0046fe60 {
public:
    char allocator;                 // +0x0
    Less_0046fe60 key_compare;      // +0x1
    Node_0046fe60* head;            // +0x4
    Node_0046fe60* FUN_0046fe60(const unsigned int* key);
};

// FUNCTION: 0x46fe60
Node_0046fe60* Class_0046fe60::FUN_0046fe60(const unsigned int* key)
{
    std::_Lockit lock;
    Node_0046fe60* x = head->parent;
    Node_0046fe60* y = head;
    while (x != DAT_0051e598)
        if (key_compare(x->key, *key))
            x = x->right;
        else
            y = x, x = x->left;
    return y;
}
