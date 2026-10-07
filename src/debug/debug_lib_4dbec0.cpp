// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by
// deepseek-v4.1, finished by Claude Opus 5.5. Names are provisional.
// The red-black tree insert behind std::map<unsigned int, Pair> (MSVC 5's
// _Tree::insert, XTREE lines 211-232). DAT_00528a54 is the tree's _Nil
// node, head->left is begin() and head->parent is the root. When the tree's
// +0x8 flag (_Multi) is set, _Insert is inlined here under its own _Lockit;
// otherwise the out-of-line _Insert (0x4dce60) is called, twice.
#include <yvals.h>
#include <new.h>

struct Pair_004dbec0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dbec0 {
    Node_004dbec0* left;               // +0x0
    Node_004dbec0* parent;             // +0x4
    Node_004dbec0* right;              // +0x8
    Pair_004dbec0 value;               // +0xc
    int color;                         // +0x14 (0 = red)
};

extern Node_004dbec0* DAT_00528a54;

class Class_004dd2a0 {
public:
    Node_004dbec0* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dbec0* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

class Class_004ddbe0 {
public:
    Class_004dd2a0 field_0;
    unsigned char field_4;

    Class_004ddbe0() {}
    Class_004ddbe0(const Class_004dd2a0& i, const unsigned char& b) : field_0(i), field_4(b) {}
};

struct Less_004dbec0 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

class Class_004ddc00 {
public:
    Node_004dbec0* FUN_004ddc00(int parent, int color);
};

class Class_004dd150 {
public:
    void FUN_004dd150(Node_004dbec0* x);
};

class Class_004dd1f0 {
public:
    void FUN_004dd1f0(Node_004dbec0* x);
};

class Class_004dce60 {
public:
    Class_004dd2a0 FUN_004dce60(Node_004dbec0* x,
                                 Node_004dbec0* y, const Pair_004dbec0* v);
};

class Class_004dbec0 {
public:
    Less_004dbec0 key_compare;         // +0x0
    Node_004dbec0* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }

    Class_004ddbe0 FUN_004dbec0(Pair_004dbec0* p);
};

// FUNCTION: 0x4dbec0
Class_004ddbe0 Class_004dbec0::FUN_004dbec0(Pair_004dbec0* p)
{
    // The pair is returned by value (no out parameter) and _Insert returns its
    // iterator by value too; a &p result slot would make p address-taken.
    Node_004dbec0* y = head;
    bool less = true;
    Node_004dbec0* x = y->parent;
    Class_004dd2a0 it2;
    Class_004dd2a0 it;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p->offset < x->value.offset;
            x = less ? x->left : x->right;
        }
    }
    if (rebuild) {
        {
            std::_Lockit lock;
            it = Class_004dd2a0(((Class_004ddc00*)this)->FUN_004ddc00((int)y, 0));
            Node_004dbec0* z = it.ptr;
            z->left = DAT_00528a54;
            z->right = DAT_00528a54;
            new ((void*)&z->value) Pair_004dbec0(*p);
            size++;
            if (y == head || x != DAT_00528a54 || key_compare(p->offset, y->value.offset)) {
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
            for (Node_004dbec0* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004dbec0* w = q->parent->parent->right;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->right) {
                            q = q->parent;
                            ((Class_004dd150*)this)->FUN_004dd150(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd1f0*)this)->FUN_004dd1f0(q->parent->parent);
                    }
                } else {
                    Node_004dbec0* w = q->parent->parent->left;
                    if (w->color == 0) {
                        q->parent->color = 1;
                        w->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->left) {
                            q = q->parent;
                            ((Class_004dd1f0*)this)->FUN_004dd1f0(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd150*)this)->FUN_004dd150(q->parent->parent);
                    }
                }
            }
            head->parent->color = 1;
        }
        return Class_004ddbe0(it, 1);
    }
    it2 = Class_004dd2a0(y);
    if (less) {
        if (Class_004dd2a0(y) == Begin())
            return Class_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, p), 1);
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2.ptr->value.offset, p->offset))
        return Class_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, p), 1);
    return Class_004ddbe0(it2, 0);
}
