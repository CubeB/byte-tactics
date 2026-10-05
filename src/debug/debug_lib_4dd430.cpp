// Decompiled by space-bunny-free. Names are provisional.
// check.py: MATCH, 730 of 730 bytes, both the original and this file.
//
// The tree's _Insert(node search result, node to hang the new node on,
// value), shaped like std::_Tree<...>::_Insert from MSVC 5's <xtree>: take a
// _Lockit, carve a 0x40-byte node from the pool allocator (Class_004dddf0,
// reached through `this` because the allocator sits at +0), build it red with
// both children on the _Nil node DAT_00528a50, construct the 48-byte
// value_type into it, bump the size at +0xc, then hang it off _Y as the left
// or right child (the "right" case is the one where the search ran all the
// way down, _X == _Nil, and the key compare is false) and fix up the
// leftmost/rightmost/root pointers. The red-black rebalance loop follows, and
// both _Lrotate and _Rrotate are inlined, each bringing its own _Lockit; the
// compiler then parks four of those lock objects in the stack slots of the
// three parameters that are dead by then (x at +0x24, y at +0x28, v at +0x2c).
// The iterator result comes back through the hidden return pointer, which is
// also the value in eax at the ret.
#include <yvals.h>
#include <new.h>

// The tree's value_type: 48 bytes copied whole into the new node.
struct Pair_004dd430 {
    unsigned int key;                 // +0x0
    char unknown_4[44];
};

struct Node_004dd430 {
    Node_004dd430* left;              // +0x0
    Node_004dd430* parent;            // +0x4
    Node_004dd430* right;             // +0x8
    Pair_004dd430 value;              // +0xc
    int color;                        // +0x3c
};

extern Node_004dd430* DAT_00528a50;

struct Less_004dd430 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

class Class_004dd2a0 {
public:
    Node_004dd430* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dd430* p) : ptr(p) {}
};

// The pool allocator sits at +0 and its method ignores `this`.
class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

class Class_004dd430 {
public:
    Less_004dd430 key_compare;        // +0x0
    Node_004dd430* head;              // +0x4
    unsigned char unknown_8;          // +0x8
    int size;                         // +0xc

    void Lrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a50)
            y->left->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void Rrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a50)
            y->right->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    Class_004dd2a0 FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                const Pair_004dd430& v);
};

// FUNCTION: 0x4dd430
Class_004dd2a0 Class_004dd430::FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                            const Pair_004dd430& v)
{
    std::_Lockit lock;
    Node_004dd430* z = (Node_004dd430*)
                       ((Class_004dddf0*)this)->FUN_004dddf0(0x40);
    z->parent = y;
    z->color = 0;
    z->left = DAT_00528a50;
    z->right = DAT_00528a50;
    new ((void*)&z->value) Pair_004dd430(v);
    size++;
    if (y == head || x != DAT_00528a50 || key_compare(v.key, y->value.key)) {
        y->left = z;
        if (y == head) {
            head->parent = z;
            head->right = z;
        } else if (y == head->left) {
            head->left = z;
        }
    } else {
        y->right = z;
        if (y == head->right) {
            head->right = z;
        }
    }
    for (x = z; x != head->parent && x->parent->color == 0; ) {
        if (x->parent == x->parent->parent->left) {
            Node_004dd430* w = x->parent->parent->right;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    Lrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Rrotate(x->parent->parent);
            }
        } else {
            Node_004dd430* w = x->parent->parent->left;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    Rrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Lrotate(x->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0(z);
}
