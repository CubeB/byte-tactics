// Decompiled by space-bunny-free. Names are provisional.
// check.py: MATCH, 746 of 746 bytes, both the original and this file.
// The out-of-line tree insert for the allocator's free-block map, shaped like
// std::_Tree<...>::_Insert from MSVC 5's <xtree> but written out by hand. The
// caller passes the _Nil node its search stopped at, the node to hang the new
// one off, and the map's value_type (a block's base offset and its length, 8
// bytes) by pointer; the tree's iterator comes back through a hidden pointer,
// so the callee hands the caller's buffer back in eax and does `ret 0x10`.
// DAT_00528a54 is the tree's _Nil node, head->left is begin() (the leftmost
// node), head->parent is the root and head->right is the rightmost node.
//
// Under the outer std::_Lockit a 0x18-byte node is carved from the pool
// (0x4ddd70), the pair is placement-new'd into it, the map's size is bumped and
// the node is linked in. Then the red-black fixup walks up from the new node
// with a cursor z, colouring and rotating until z's parent is black or the root
// is reached, and finally blackens the root. Lrotate and Rrotate are the tree's
// own rotation methods (0x4dd150 and 0x4dd1f0 as separate functions); /Ob2
// inlines them here, and each inlined copy brings its own std::_Lockit scope
// with it, which is where the four extra lock objects come from.
//
// Three spellings are load-bearing for the code that comes out:
//   * the link-in test is a POSITIVE disjunction, `y == head || x != _Nil ||
//     key_compare(v->offset, y->value.offset)`, with the shared left-child block
//     as its then-part. That puts the right-child block in the fallthrough and
//     makes all three tests jump to the left-child block. Because the third
//     operand goes through the bool-returning comparator, MSVC 5 materialises it
//     (cmp; sbb; neg; test cl,cl) instead of branching on the flags; writing the
//     comparison inline gives a plain `jb` instead.
//   * inside the left-child block the empty-tree case is the FIRST `if`, and it
//     updates head->right (the rightmost node), not head->left: the leftmost
//     node is already covered by the `y->left = p` above it.
//   * the fixup loop tests its exit through an explicit `break`, and the
//     re-colouring and the rotation argument are written as
//     `z->parent->...` expressions rather than through the parent and
//     grandparent locals, so each is re-read from the cursor. Declaring those
//     two as locals costs a register: the cursor then shares p's and the whole
//     fixup changes shape.
#include <yvals.h>
#include <new.h>

// The map's value_type: the block's base offset and its length, 8 bytes.
struct Pair_004dce60 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dce60 {
    Node_004dce60* left;               // +0x0
    Node_004dce60* parent;             // +0x4
    Node_004dce60* right;              // +0x8
    Pair_004dce60 value;               // +0xc
    int color;                         // +0x14 (0 = red, 1 = black)
};

extern Node_004dce60* DAT_00528a54;   // the tree's _Nil node

// The tree's key ordering, unsigned, at +0 of the tree (as in 0x4db000).
struct Less_004dce60 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

// A method that ignores `this`: its caller sets ecx to the tree, which sits at
// the allocator's +0, and pushes the node size.
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// The tree's iterator: one pointer, with constructors, so it is returned
// through a hidden pointer.
class Class_004dd2a0 {
public:
    Node_004dce60* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dce60* p) : ptr(p) {}
};

class Class_004dce60 {
public:
    Less_004dce60 key_compare;         // +0x0
    Node_004dce60* head;               // +0x4
    int unknown_8;                     // +0x8
    int size;                          // +0xc

    // Left rotation of x, shaped like std::_Tree<...>::_Lrotate.
    void Lrotate(Node_004dce60* x)
    {
        std::_Lockit lock;
        Node_004dce60* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a54)
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

    // Right rotation of x, shaped like std::_Tree<...>::_Rrotate.
    void Rrotate(Node_004dce60* x)
    {
        std::_Lockit lock;
        Node_004dce60* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a54)
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

    Class_004dd2a0 FUN_004dce60(Node_004dce60* x, Node_004dce60* y,
                                Pair_004dce60* v);
};

// FUNCTION: 0x4dce60
Class_004dd2a0 Class_004dce60::FUN_004dce60(Node_004dce60* x, Node_004dce60* y,
                                            Pair_004dce60* v)
{
    std::_Lockit lock;
    Node_004dce60* p = (Node_004dce60*)((Class_004ddd70*)this)->FUN_004ddd70(0x18);
    p->parent = y;
    p->color = 0;                      // red
    p->left = DAT_00528a54;
    p->right = DAT_00528a54;
    new ((void*)&p->value) Pair_004dce60(*v);
    ++size;

    if (y == head || x != DAT_00528a54 || key_compare(v->offset, y->value.offset)) {
        y->left = p;
        if (y == head) {
            head->parent = p;          // the root
            head->right = p;           // the rightmost node
        } else if (y == head->left) {
            head->left = p;            // the leftmost node
        }
    } else {
        y->right = p;
        if (y == head->right)
            head->right = p;
    }

    Node_004dce60* z = p;
    while (z != head->parent) {
        if (z->parent->color != 0)
            break;

        if (z->parent == z->parent->parent->left) {
            Node_004dce60* u = z->parent->parent->right;
            if (u->color == 0) {
                // Red uncle: recolour and carry on two levels up.
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    Lrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Rrotate(z->parent->parent);
            }
        } else {
            Node_004dce60* u = z->parent->parent->left;
            if (u->color == 0) {
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    Rrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Lrotate(z->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0(p);
}
