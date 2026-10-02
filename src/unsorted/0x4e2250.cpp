// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Retry 5 notes (GPT-6.1-sol): the preserved best remains 74.5 percent (821 bytes), confirmed by check.py at start. Changing local ans from bool to int reduced the score to 49.4 percent. A constructor declared out of line and a combined `ans && y == head->left` branch scored 68.7 and 70.5 percent. Two additional constructor-shape probes did not compile. Existing search-loop allocation and non-multi tail differences remain unresolved.

// NOT A MATCH (74.2 percent, 801 bytes against our 811). Retry 3
// (deepseek-v4.1-flash) gained only the `test al,al` fix by declaring
// Class_004e1a30::FUN_004e1a30 as returning bool (was int), worth +0.8
// percent. Everything else from retry 2 still stands.
// The search loop is now the only source-level difference left in the first
// half: the original branches on the strcmp result's own flags
// (0x4e22ba `test eax,eax / jge`) and writes the bool in each arm
// (`mov ebp,[ebp] / mov bl,1` vs `mov ebp,[ebp+8] / xor bl,bl`), i.e. it was
// written as `if (a != b && strcmp(a,b) < 0) { x = x->left; ans = true; }
// else { x = x->right; ans = false; }`. Writing exactly that DOES produce the
// 801's loop shape, but it changes MSVC's global register/stack allocation:
// `this` moves from [esp+0x18] to [esp+0x20], the first _Lockit from
// [esp+0x20] to [esp+0x1c], and the key pointer lands in edx instead of edi.
// That drops the score to 55 percent even though the loop bytes match. All
// four combinations (assignment from an inlined helper vs if/else, int vs
// bool compare result) were scored: helper-assignment keeps the frame and
// stays at 74.2, if/else gets the loop and loses the frame at 55.
// So the frame/register allocation is the real blocker, not the loop shape.
// The non-multi tail is also still wrong: the original calls the
// pair<iterator,bool> ctor out of line at 0x4e253a (and 0x4e2a10 inlined only
// on the _Multi path), and lays the `if (ans)` arm out with the ++size stored
// before the `y == head` compare rather than after.
// Shaped like std::_Tree<...>::insert(const value_type&) from MSVC 5's
// <xtree> (lines 211-232); the exact XTREE source is at
// toolchain/msvc5-sp3/INCLUDE/XTREE. The out-of-line _Insert is 0x4e2620
// (ret 0x10, four stack args: hidden return slot, _X, _Y, _V), _Lrotate and
// _Rrotate are 0x4e2950/0x4e29b0, _Buynode is 0x4e2a30. DAT_005292c4 is the
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
// (MSVC 5 in this version mangles `*r` in a mem-initializer); making
// FUN_004e1a30 an inline member operator() and calling it in the loop
// (30.6 percent, it stops being an out-of-line call in the tail too);
// `if (this != &v)` as the copy guard (64.0 percent against 66.4 for
// `if (this)`).
// Retry 4 notes (deepseek-v4.1-flash): re-confirmed that the if/else loop
// form reproduces the original loop bytes but moves `this` from [esp+0x18] to
// [esp+0x20] and lock1 from +0x20 to +0x1c, scoring 55.2 percent (799 bytes);
// the helper-assignment form keeps the frame but materialises the bool then
// re-tests it, which is the +10 byte loop difference. Calling the comparator as
// an out-of-line member in the loop (v.key.FUN_004e1a30(x->val.key)) drops the
// loop to a real call and scores 42.1 percent. Best kept here: the helper's
// return type changed from bool to int (return 1/0) scored 74.5 percent
// (821 bytes) vs 74.2 for bool, but the loop fold and the non-multi tail
// (out-of-line pair ctor at 0x4e253a, ++size ordering) still differ.
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

static inline int Less_004e2250(const char* a, const char* b)
{
    if (a != b && strcmp(a, b) < 0)
        return 1;
    return 0;
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

    Class_004e2a10(const Class_004e2ab0& it, const char& flag)
        : first(it.ptr), second(flag) {}
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
    bool flag = false;
    if (it.ptr->val.key.FUN_004e1a30(v.key)) {
        Node_004e2250* slot;
        it.ptr = FUN_004e2620(slot, x, y, v);
        flag = true;
    }
    return Class_004e2a10(it, flag);
}
