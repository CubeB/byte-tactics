// Decompiled by Opus. Names are provisional.
// Keys are C strings ordered by strcmp; DAT_005292c4 is the tree's _Nil
// node. The iterator has constructors, so it is returned through a hidden
// pointer.
#include <string.h>
#include <yvals.h>

struct Node_004e2580 {
    Node_004e2580* left;               // +0x0
    Node_004e2580* parent;             // +0x4
    Node_004e2580* right;              // +0x8
    const char* key;                   // +0xc
};

extern Node_004e2580* DAT_005292c4;

struct Less_004e2580 {
    bool operator()(const char* const& a, const char* const& b) const
    {
        return a != b && strcmp(a, b) < 0;
    }
};

class Iter_004e2580 {
public:
    Node_004e2580* ptr;

    Iter_004e2580() {}
    Iter_004e2580(Node_004e2580* p) : ptr(p) {}
};

class NameMapTree {
public:
    Less_004e2580 compare;             // +0x0
    Node_004e2580* head;               // +0x4

    Node_004e2580* Lbound(const char* const& key)
    {
        std::_Lockit lock;
        Node_004e2580* x = head->parent;
        Node_004e2580* y = head;
        while (x != DAT_005292c4) {
            if (compare(x->key, key))
                x = x->right;
            else
                y = x, x = x->left;
        }
        return y;
    }
    Iter_004e2580 LowerBound(const char* const& key);
};

// FUNCTION: 0x4e2580
Iter_004e2580 NameMapTree::LowerBound(const char* const& key)
{
    return Iter_004e2580(Lbound(key));
}
