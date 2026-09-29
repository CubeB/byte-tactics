// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH (66.4 percent, 801 bytes against our 830). Still differs:
//  * our frame is 0x1c where the original's is 0x18: the original keeps the
//    new node in a slot at [esp+0x10] and the hint node at [esp+0x14], we
//    keep the hint at [esp+0x18] and have no slot for the new node at all
//    (4 slots against the original's 6). Every spill in the rest of the
//    function is then 4 or 8 out of step, which is most of the diff. The
//    original's extra live local is a node pointer that is live ACROSS the
//    Class_004e1a30::FUN_004e1a30 call and gets spilled; ours keeps the new
//    node in a register across it, so adding one more live reference (see
//    the guide on allocation demotion) is the next thing to try.
// GPT-6 retry with longcat-2.5-preview-free: no source change within the 10 minute
// worker limit. The best file remains the 68.7% version reported by the current
// checker; the remaining frame-layout and search-loop differences above are unclosed.
//  * the search loop: the original branches on the strcmp result's own
//    flags (0x4e22ba `test eax,eax / jge`) and sets the bool in each arm,
//    ours materialises the bool and re-tests it (`xor bl,bl / test bl,bl`).
//    An if/else form with `ans = true/false` in the arms was tried earlier
//    against the old tail and was worse, but not against this one.
//  * ++size: the original does the increment into ecx and stores it after
//    the `y == head` compare; we keep the size in esi and store before.
// Shaped like std::map<Key, Val>::insert(const value_type&) from MSVC 5's
// <xtree>, with the _Tree::_Insert body inlined on the _Multi path (the
// out-of-line copy of it is 0x4e2620, the _Lrotate/_Rrotate copies are
// 0x4e2950/0x4e29b0 and the _Buynode copy is 0x4e2a30). DAT_005292c4 is the
// tree's _Nil node, head->parent is the root, the colour is the int at +0x204
// (_Red == 0) and the key is the char* at +0 of the 0x1f8 byte value, which is
// also where the key_compare instance lives, so the compare calls take the
// value's address as `this`.
// Suspected bug in the original: on the first !multi insert path (0x4e24c3)
// the call to FUN_004e2620 is given the caller's own value reference as its
// Node*& out-parameter, so the callee writes the new node over the caller's
// value pointer (and the caller then reads that same slot back at 0x4e24d6
// as the returned iterator). Same on the second path (0x4e250a/0x4e2512),
// where the out-parameter is a dead local reused from the _Lockit's slot.
// Reproduced here deliberately on the first path.
// Tried and rejected: a converting ctor from `Node*&` on the iterator type
// (MSVC 5 in this version mangles `*r` in a mem-initializer); the if/else
// search loop against the old two-iterator tail; `if (this != &v)` as the
// copy guard (64.0 percent against 66.4 for `if (this)`).
#include <string.h>
#include <yvals.h>

enum Redbl_004e2250 { _Red = 0, _Black = 1 };

class Class_004e1a30 {
public:
    char* name;                                 // +0x0
    int FUN_004e1a30(const Class_004e1a30& other) const;
};

struct Val_004e2250 {
    Class_004e1a30 key;                         // +0x0
    char unknown_4[0x1f4];                      // +0x4

    Val_004e2250& operator=(const Val_004e2250& v)
    {
        if (this)
            memcpy(this, &v, sizeof(Val_004e2250));
        return *this;
    }
};

struct Node_004e2250 {
    Node_004e2250* left;                        // +0x00
    Node_004e2250* parent;                      // +0x04
    Node_004e2250* right;                       // +0x08
    Val_004e2250 val;                           // +0x0c
    int colour;                                 // +0x204
};

extern Node_004e2250* DAT_005292c4;

static inline bool Less_004e2250(const char* a, const char* b)
{
    return a != b && strcmp(a, b) < 0;
}

class Class_004e2ab0 {
public:
    Node_004e2250* ptr;                         // +0x0

    Class_004e2ab0() {}
    Class_004e2ab0(Node_004e2250* p) : ptr(p) {}
    bool operator==(const Class_004e2ab0& x) const {return (ptr == x.ptr);}

    void FUN_004e2ab0();
};

class Class_004e2a10 {
public:
    Node_004e2250* first;                       // +0x0
    char second;                                // +0x4

    Class_004e2a10(const Class_004e2ab0& it, const char& flag);
};

class Class_004e2a30 {
public:
    Node_004e2250* FUN_004e2a30(Node_004e2250* param_1, int param_2);
};

class Class_004e2950 {
public:
    void FUN_004e2950(Node_004e2250* x);
};

class Class_004e29b0 {
public:
    void FUN_004e29b0(Node_004e2250* x);
};

class Class_004e2250 {
public:
    Class_004e1a30 cmp;                         // +0x0
    Node_004e2250* head;                        // +0x4
    char multi;                                 // +0x8
    int size;                                   // +0xc

    Node_004e2250*& FUN_004e2620(Node_004e2250*& ret, Node_004e2250* x, Node_004e2250* y, const Val_004e2250& v);
    Class_004e2a10 FUN_004e2250(const Val_004e2250& v);
};

// FUNCTION: 0x4e2250
Class_004e2a10 Class_004e2250::FUN_004e2250(const Val_004e2250& v)
{
    Node_004e2250* x = head->parent;
    Node_004e2250* y = head;
    bool ans = true;
    {
        std::_Lockit lk;
        while (x != DAT_005292c4) {
            y = x;
            ans = Less_004e2250(v.key.name, x->val.key.name);
            x = ans ? x->left : x->right;
        }
    }
    if (multi) {
        std::_Lockit lk;
        Node_004e2250* z = ((Class_004e2a30*)this)->FUN_004e2a30(y, _Red);
        z->left = DAT_005292c4;
        z->right = DAT_005292c4;
        z->val = v;
        ++size;
        if (y == head || x != DAT_005292c4 || v.key.FUN_004e1a30(y->val.key)) {
            y->left = z;
            if (y == head) {
                head->parent = z;
                head->right = z;
            } else if (y == head->left)
                head->left = z;
        } else {
            y->right = z;
            if (y == head->right)
                head->right = z;
        }
        for (x = z; x != head->parent && x->parent->colour == _Red; ) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;
                if (y->colour == _Red) {
                    x->parent->colour = _Black;
                    y->colour = _Black;
                    x->parent->parent->colour = _Red;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        ((Class_004e2950*)this)->FUN_004e2950(x);
                    }
                    x->parent->colour = _Black;
                    x->parent->parent->colour = _Red;
                    ((Class_004e29b0*)this)->FUN_004e29b0(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;
                if (y->colour == _Red) {
                    x->parent->colour = _Black;
                    y->colour = _Black;
                    x->parent->parent->colour = _Red;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        ((Class_004e29b0*)this)->FUN_004e29b0(x);
                    }
                    x->parent->colour = _Black;
                    x->parent->parent->colour = _Red;
                    ((Class_004e2950*)this)->FUN_004e2950(x->parent->parent);
                }
            }
        }
        head->parent->colour = _Black;
        return Class_004e2a10(Class_004e2ab0(z), (char)1);
    }
    Class_004e2ab0 it(y);
    if (ans) {
        bool b = (y == head->left);
        if (b) {
            it.ptr = FUN_004e2620(*(Node_004e2250**)(char*)const_cast<char*>((const char*)&v), x, y, v);
            return Class_004e2a10(it, (char)1);
        }
        it.FUN_004e2ab0();
    }
    if (it.ptr->val.key.FUN_004e1a30(v.key)) {
        Node_004e2250* slot;
        it.ptr = FUN_004e2620(slot, x, y, v);
        return Class_004e2a10(it, (char)1);
    }
    return Class_004e2a10(it, (char)0);
}
