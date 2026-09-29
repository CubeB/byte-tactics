// Decompiled by space-bunny-free. Names are provisional.
// check.py: MATCH, 643 of 643 bytes, every reference the linker fills in ok.
//
// The tree's insert for the game's file-record map: the value_type is a 0x30
// byte record whose first dword is the key, the node is {left, parent, right,
// value, color} with the color at +0x3c, and DAT_00528a50 is the tree's _Nil
// node (head->parent is the root, head->left is begin()). The argument is a
// POINTER to the record, not a reference, and that is what the code proves: the
// call to the out-of-line _Insert (0x4dd430) passes `&p`, the address of the
// parameter variable itself (lea edx,[esp+0x30]), and the callee stores the new
// node's iterator there and hands the same pointer back in eax, which the
// caller dereferences. So the parameter slot is reused as the callee's result
// slot. With a `const Pair&` parameter the address of the *referred-to* object
// is taken instead, the two call sites collapse into one, the frame layout
// changes and the function no longer matches.
//
// The search runs under one std::_Lockit and leaves y (the node to hang the new
// one off) and x (the _Nil it stopped at) plus the direction flag `less`.
// When the map's +0x8 flag is set the whole insert is done here under a second
// _Lockit: a node from the pool allocator (0x4ddce0, which takes the parent and
// the color and sets them in the callee), both children on _Nil, the record
// placement-new'd into it, the size bumped, the node linked in and the
// red-black fixup run, with _Lrotate (0x4dd710) and _Rrotate (0x4dd770) out of
// line. The return is OUTSIDE that _Lockit's scope, so the (iterator, 1) pair
// is built after ~_Lockit; `x = y->parent` sits before the first _Lockit, which
// is why the load precedes the constructor call.
//
// Otherwise the node to insert is handed to the out-of-line _Insert, twice:
// once when the search went left and y is begin(), and once when the
// predecessor's key is still below the new key. The two argument setups are
// kept separate by MSVC (the first uses ecx/edx, the second eax) and only the
// call and the result stores are shared, which is what the original does.
//
// The (iterator, inserted) result is built by an inlined constructor taking the
// iterator by value and the flag, so the first field is copied out of the
// iterator's stack slot and the flag is stored as a byte.
//
// Casts: `(Class_004dd820*)&p` is a type pun, the callee only ever stores a
// four-byte node pointer through that argument; `(int)y` is only there because
// 0x4ddce0's own file declares that method with int parameters.
#include <yvals.h>
#include <new.h>

// The tree's value_type: 48 bytes copied whole into the new node.
struct Pair_004dc680 {
    unsigned int key;                  // +0x0
    char unknown_4[44];
};

struct Node_004dc680 {
    Node_004dc680* left;               // +0x0
    Node_004dc680* parent;             // +0x4
    Node_004dc680* right;              // +0x8
    Pair_004dc680 value;               // +0xc
    int color;                         // +0x3c (0 = red)
};

extern Node_004dc680* DAT_00528a50;    // the tree's _Nil node

// The tree's iterator: one pointer. FUN_004dd820 is its _Dec().
class Class_004dd820 {
public:
    Node_004dc680* ptr;

    Class_004dd820() {}
    Class_004dd820(Node_004dc680* q) : ptr(q) {}
    bool operator==(const Class_004dd820& o) const { return ptr == o.ptr; }
    void FUN_004dd820();
};

// The (iterator, inserted) pair this function returns.
class Class_004ddbe0 {
public:
    Class_004dd820 field_0;
    unsigned char field_4;

    Class_004ddbe0() {}
    Class_004ddbe0(Class_004dd820 i, unsigned char b) : field_0(i), field_4(b) {}
};

struct Less_004dc680 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

// The pool allocator's node builder: parent and color, both set in the callee.
class Class_004ddce0 {
public:
    Node_004dc680* FUN_004ddce0(int parent, int color);
};

// The out-of-line rotations, reached through `this`.
class Class_004dd710 {
public:
    void FUN_004dd710(Node_004dc680* x);
};

class Class_004dd770 {
public:
    void FUN_004dd770(Node_004dc680* x);
};

class Class_004dc680 {
public:
    Less_004dc680 key_compare;         // +0x0
    Node_004dc680* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd820 Begin() { return Class_004dd820(head->left); }

    // The tree's out-of-line _Insert (0x4dd430), declared here with the result
    // slot as an explicit argument: the callee stores the new node's iterator
    // through it and returns it. (Its own file spells the same four dwords of
    // arguments as a class return through a hidden pointer, which is the same
    // call, but then the caller needs a temporary and MSVC puts that in the
    // wrong stack slot.)
    Class_004dd820* FUN_004dd430(Class_004dd820* out, Node_004dc680* x,
                                 Node_004dc680* y, const Pair_004dc680* v);

    Class_004ddbe0 FUN_004dc680(Pair_004dc680* p);
};

// FUNCTION: 0x4dc680
Class_004ddbe0 Class_004dc680::FUN_004dc680(Pair_004dc680* p)
{
    Node_004dc680* y = head;
    bool less = true;
    Node_004dc680* x = y->parent;
    Class_004dd820 it2;
    Class_004dd820 it;
    {
        std::_Lockit lock;
        while (x != DAT_00528a50) {
            y = x;
            less = p->key < x->value.key;
            x = less ? x->left : x->right;
        }
    }
    if (rebuild) {
        {
            std::_Lockit lock;
            it = Class_004dd820(((Class_004ddce0*)this)->FUN_004ddce0((int)y, 0));
            Node_004dc680* z = it.ptr;
            z->left = DAT_00528a50;
            z->right = DAT_00528a50;
            new ((void*)&z->value) Pair_004dc680(*p);
            size++;
            if (y == head || x != DAT_00528a50 || key_compare(p->key, y->value.key)) {
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
            for (Node_004dc680* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004dc680* w = q->parent->parent->right;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->right) {
                            q = q->parent;
                            ((Class_004dd710*)this)->FUN_004dd710(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd770*)this)->FUN_004dd770(q->parent->parent);
                    }
                } else {
                    Node_004dc680* w = q->parent->parent->left;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->left) {
                            q = q->parent;
                            ((Class_004dd770*)this)->FUN_004dd770(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd710*)this)->FUN_004dd710(q->parent->parent);
                    }
                }
            }
            head->parent->color = 1;
        }
        return Class_004ddbe0(it, 1);
    }
    it2 = Class_004dd820(y);
    if (less) {
        if (Class_004dd820(y) == Begin())
            return Class_004ddbe0(*FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
        it2.FUN_004dd820();
    }
    if (key_compare(it2.ptr->value.key, p->key))
        return Class_004ddbe0(*FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
    return Class_004ddbe0(it2, 0);
}
