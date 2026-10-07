// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// The wrapper around a std::_Tree whose other methods are 0x4e1990 (insert),
// 0x4dfea0 (erase one node) and 0x4e03f0 (_Erase a subtree). The fast path
// erases the whole tree under a std::_Lockit and returns begin(), which the
// caller discards; the slow path walks the nodes with operator++(int).
// DAT_005292c4 is the tree's shared _Nil node.
#include <yvals.h>

struct Node_004e18c0 {
    Node_004e18c0* left;               // +0x0
    Node_004e18c0* parent;             // +0x4
    Node_004e18c0* right;              // +0x8
};

extern Node_004e18c0* DAT_005292c4;

class Class_004e0450 {
public:
    Node_004e18c0* ptr;

    void FUN_004e0450();
};

class Iter_004e18c0 {
public:
    Node_004e18c0* ptr;

    Iter_004e18c0() {}
    Iter_004e18c0(Node_004e18c0* p) : ptr(p) {}
    bool operator==(const Iter_004e18c0& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004e18c0& x) const { return !(*this == x); }
    Iter_004e18c0& operator++()
    {
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return *this;
    }
    Iter_004e18c0 operator++(int)
    {
        Iter_004e18c0 tmp = *this;
        ((Class_004e0450*)&ptr)->FUN_004e0450();
        return tmp;
    }
};

class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004e18c0* x);
};

class Class_004dfea0 {
public:
    Iter_004e18c0 FUN_004dfea0(Iter_004e18c0 it);
};

class Class_004e2240 {
public:
    Iter_004e18c0 FUN_004e2240();
};

class Class_004e18c0 {
public:
    char unknown_0[4];                 // +0x0
    Node_004e18c0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc
    bool changed;                      // +0x10

    Iter_004e18c0 begin() { return head->left; }
    Iter_004e18c0 end() { return head; }

    Iter_004e18c0 erase(Iter_004e18c0 _F, Iter_004e18c0 _L)
    {
        if (size == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        }
        // Early return above, not an else: keeps the return slot apart from the lock.
        std::_Lockit Lk;
        ((Class_004e03f0*)this)->FUN_004e03f0(head->parent);
        head->parent = DAT_005292c4;
        size = 0;
        head->left = head;
        head->right = head;
        return ((Class_004e2240*)this)->FUN_004e2240();
    }

    void FUN_004e18c0();
};

// FUNCTION: 0x4e18c0
void Class_004e18c0::FUN_004e18c0()
{
    erase(begin(), end());
    changed = 1;
}
