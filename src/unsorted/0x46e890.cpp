// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// std::map<unsigned int, Rect>::_Imp::erase(iterator first, iterator last)
// from MSVC 5's <xtree>, emitted out of line (its caller is the destructor of
// Class_0046d040, which erases the map at +0x00 with begin() and end()).
// Written out by hand rather than through the real <map> so the callees keep
// the names already recorded: FUN_0046f6d0 is the tree's _Erase() (inlined
// once here, its recursive call is the out-of-line one) and FUN_0046ea10 the
// iterator's _Inc(). FUN_0046f1e0 is _Tree::erase(iterator), called once per
// element. DAT_0051e598 is the tree's shared _Nil node.
#include <yvals.h>

struct Node_0046e890 {
    Node_0046e890* left;               // +0x0
    Node_0046e890* parent;             // +0x4
    Node_0046e890* right;              // +0x8
};

extern Node_0046e890* DAT_0051e598;

class Class_0046ea10 {                 // the tree's iterator
public:
    Node_0046e890* ptr;                // +0x0

    Class_0046ea10() {}
    Class_0046ea10(Node_0046e890* p) : ptr(p) {}
    bool operator==(const Class_0046ea10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_0046ea10& o) const { return !(*this == o); }
    Class_0046ea10& operator++()
    {
        FUN_0046ea10();
        return *this;
    }
    Class_0046ea10 operator++(int)
    {
        Class_0046ea10 t = *this;
        ++*this;
        return t;
    }
    void FUN_0046ea10();
};

class Class_0046f6d0 {                 // the tree's _Erase()
public:
    void FUN_0046f6d0(Node_0046e890* x)
    {
        std::_Lockit lock;
        for (Node_0046e890* y = x; y != DAT_0051e598; x = y) {
            FUN_0046f6d0(y->right);
            y = y->left;
            operator delete(x);
        }
    }
};

class Class_0046e890 {                 // the std::map<unsigned int, Rect> tree
public:
    int field_0;                       // +0x0
    Node_0046e890* head;               // +0x4
    int field_8;                       // +0x8
    int size;                          // +0xc

    Node_0046e890* Root() { return head->parent; }
    Class_0046ea10 begin() { return Class_0046ea10(head->left); }
    Class_0046ea10 end() { return Class_0046ea10(head); }

    Class_0046ea10 erase(Class_0046ea10 _F, Class_0046ea10 _L);
    Class_0046ea10 erase(Class_0046ea10 _P);
};

// FUNCTION: 0x46e890 ?erase@Class_0046e890@@QAE?AVClass_0046ea10@@V2@0@Z
Class_0046ea10 Class_0046e890::erase(Class_0046ea10 _F, Class_0046ea10 _L)
{
    if (size == 0 || _F != begin() || _L != end()) {
        while (_F != _L)
            erase(_F++);
        return _F;
    } else {
        std::_Lockit Lk;
        ((Class_0046f6d0*)this)->FUN_0046f6d0(Root());
        head->parent = DAT_0051e598, size = 0;
        head->left = head, head->right = head;
        return begin();
    }
}