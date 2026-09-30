// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by
// space-bunny-free. Names are provisional.
// NOT A MATCH (69.4 percent, 801 bytes against our 837). Still differs:
//  * our frame is one dword out of step: the original homes `this` at
//    [esp+0x18] and `_Y` at [esp+0x14] and puts the `it` iterator object at
//    [esp+0x1c], we home `this` at [esp+0x1c], `_Y` at [esp+0x18] and `it` at
//    [esp+0x14], so every spill after that is 4 out of step, which is most of
//    the diff. The two _Lockit slots ([esp+0x20] and [esp+0x24]) and `z`
//    ([esp+0x10]) do match, so the frame size (sub esp,0x18 plus four pushes)
//    is right. In the original the order of first home assignment must be
//    `_Y` 0x14, `z` 0x10 for the address-taken pool below the saved registers
//    and `this` 0x18, `it` 0x1c above them; the `it` object is the one that
//    has to be forced out of the low pool, and no spelling tried here moved it.
//  * the search loop: the original branches on the strcmp result's own flags
//    (0x4e22ba `test eax,eax / jge`) and sets the bool in each arm, ours
//    materialises the bool and re-tests it (`xor bl,bl / test bl,bl`). Writing
//    the arm as an if/else that assigns `ans` in each arm does fix the branch
//    but costs more elsewhere: it drops the whole file to 63.8 percent
//    (build/scratch note in the board).
//  * ++size: the original does the increment into ecx and stores it after the
//    `y == head` compare; we keep the size in esi and store before.
// What did work this pass: the key compare must return a one byte value
// (bool, as in the matched 0x4e2620.cpp), not int, or 0x4e235d reads
// `test eax,eax` where the original has `test al,al` (68.7 -> 69.4 percent).
// Shaped like std::_Tree<...>::insert(const value_type&) from MSVC 5's
// <xtree> (lines 211-232), with the _Insert body inlined on the _Multi path
// (the out-of-line copy of it is 0x4e2620, the _Lrotate/_Rrotate copies are
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
    bool FUN_004e1a30(const Class_004e1a30& other) const;
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
    if (a != b && strcmp(a, b) < 0)
        return true;
    return false;
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
