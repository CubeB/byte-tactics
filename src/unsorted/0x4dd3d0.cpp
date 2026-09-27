// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<unsigned int, ...>::lower_bound(const key&) from MSVC 5's
// <xtree>: the body of _Lbound (0x4ddc90 for another tree) is inlined and its
// lock is scoped inside it, so ~_Lockit runs before the iterator is built from
// the node pointer. DAT_00528a50 is the tree's _Nil node and head->parent is
// the root.
#include <yvals.h>

struct Node_004dd3d0 {
    Node_004dd3d0* left;               // +0x0
    Node_004dd3d0* parent;             // +0x4
    Node_004dd3d0* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern Node_004dd3d0* DAT_00528a50;

struct Less_004dd3d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dd3d0 {
public:
    Node_004dd3d0* ptr;                // +0x0

    Iter_004dd3d0() {}
    Iter_004dd3d0(Node_004dd3d0* p) : ptr(p) {}
};

class Class_004dd3d0 {
public:
    Less_004dd3d0 compare;             // +0x0
    Node_004dd3d0* head;               // +0x4

    Node_004dd3d0* Lbound(const unsigned int& key) const
    {
        std::_Lockit lock;
        Node_004dd3d0* x = head->parent;
        Node_004dd3d0* y = head;
        while (x != DAT_00528a50) {
            if (compare(x->key, key))
                x = x->right;
            else
                y = x, x = x->left;
        }
        return y;
    }

    Iter_004dd3d0 FUN_004dd3d0(const unsigned int& key);
};

// FUNCTION: 0x4dd3d0
Iter_004dd3d0 Class_004dd3d0::FUN_004dd3d0(const unsigned int& key)
{
    return Iter_004dd3d0(Lbound(key));
}
