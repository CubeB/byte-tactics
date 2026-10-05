// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::upper_bound(const _K&) from MSVC 5's <xtree>: it returns
// iterator(_Ubound(_Kv)); _Ubound is inlined here, so its std::_Lockit scope
// ends before the iterator is constructed into the hidden return buffer.
// DAT_00528a54 is the tree's _Nil node and head->parent is the root. Same
// shape as 0x4dd250 (the standalone _Ubound) and 0x4dc620.
#include <yvals.h>

struct Node_004dbd20 {
    Node_004dbd20* left;               // +0x0
    Node_004dbd20* parent;             // +0x4
    Node_004dbd20* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern Node_004dbd20* DAT_00528a54;

struct Less_004dbd20 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dbd20 {
public:
    Node_004dbd20* ptr;
    Iter_004dbd20() : ptr(0) {}
    Iter_004dbd20(Node_004dbd20* p) : ptr(p) {}
};

class Class_004dbd20 {
public:
    Less_004dbd20 key_compare;
    Node_004dbd20* head;               // +0x4

    Iter_004dbd20 FUN_004dbd20(const unsigned int& kv);
};

// Inlined std::_Tree<...>::_Ubound(const _K&): its own lock scope.
static inline Node_004dbd20* Ubound_004dbd20(Class_004dbd20* self,
                                             const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dbd20* x = self->head->parent;
    Node_004dbd20* y = self->head;
    while (x != DAT_00528a54)
        if (self->key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}

// FUNCTION: 0x4dbd20
Iter_004dbd20 Class_004dbd20::FUN_004dbd20(const unsigned int& kv)
{
    return Iter_004dbd20(Ubound_004dbd20(this, kv));
}
