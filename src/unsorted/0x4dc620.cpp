// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::_Ubound(const _K&) from MSVC 5's <xtree>: the first node
// whose key is greater than the given key, or the head node. DAT_00528a54 is
// the tree's _Nil node and head->parent is the root. Same shape as 0x4dd250,
// but the result is an iterator object returned through a hidden pointer.
#include <yvals.h>

struct Node_004dc620 {
    Node_004dc620* left;               // +0x0
    Node_004dc620* parent;             // +0x4
    Node_004dc620* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern Node_004dc620* DAT_00528a54;

struct Less_004dc620 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dc620 {
public:
    Node_004dc620* ptr;
    Iter_004dc620() : ptr(0) {}
    Iter_004dc620(Node_004dc620* p) : ptr(p) {}
};

class Class_004dc620 {
public:
    Less_004dc620 key_compare;
    Node_004dc620* head;               // +0x4

    Iter_004dc620 FUN_004dc620(const unsigned int& kv);
};

// FUNCTION: 0x4dc620
Iter_004dc620 Class_004dc620::FUN_004dc620(const unsigned int& kv)
{
    Iter_004dc620 y;
    {
        std::_Lockit lock;
        Node_004dc620* x = head->parent;
        y.ptr = head;
        while (x != DAT_00528a54)
            if (key_compare(kv, x->key))
                y.ptr = x, x = x->left;
            else
                x = x->right;
    }
    return y;
}
