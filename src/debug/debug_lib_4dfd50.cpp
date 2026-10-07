// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The compiler-generated atexit term function for the function-local static
// object at 0x5292d0 (guard byte 0x5292c8, constructor 0x4df1e0, registered by
// the guard function 0x4dfd10).
//
// `_Freenode` is the pooled allocator's deallocate: it pushes the node onto
// DAT_00529e58.
#include <yvals.h>

struct Node_004dfd50 {
    Node_004dfd50* left;               // +0x0
    Node_004dfd50* parent;             // +0x4
    Node_004dfd50* right;              // +0x8
};

// The tree's iterator (std::_Tree<...>::iterator); passed by value and
// returned by value, so the caller supplies a hidden return buffer.
// Hand-written <xtree>, not <map>: only classes with the literal callee names
// (Class_004e03f0, Class_004e0450, Class_004dfea0) produce those symbols.
class Class_004e0450 {
public:
    Node_004dfd50* ptr;                // +0x0

    void FUN_004e0450();               // _Inc
    Class_004e0450& operator++() { FUN_004e0450(); return *this; }
    Class_004e0450 operator++(int)
    {
        Class_004e0450 t = *this;
        ++*this;
        return t;
    }
    bool operator==(const Class_004e0450& x) const { return ptr == x.ptr; }
    bool operator!=(const Class_004e0450& x) const { return !(*this == x); }
};

// std::_Tree<...>::_Erase(_Nodeptr): frees a whole subtree.
class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004dfd50* x);
};

// std::_Tree<...>::erase(iterator): erases one node, returns the next.
class Class_004dfea0 {
public:
    Class_004e0450 FUN_004dfea0(Class_004e0450 it);
};

// _Nil and _Nilrefs are shared with the instantiation in 0x4e17c0.
extern Node_004dfd50* DAT_005292c4;    // tree _Nil
extern void* DAT_00529e58;             // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs

// The map's _Tree. Only the fields the destructor touches are modelled:
// the empty pooled allocator and comparator occupy +0x0, _Head sits at +0x4,
// _Multi at +0x8 and _Size at +0xc, exactly as in <xtree>.
class Tree_004dfd50 {
public:
    char unknown_0[4];
    Node_004dfd50* _Head;              // +0x4
    bool _Multi;                       // +0x8
    char pad_9[3];
    unsigned int _Size;                // +0xc

    Node_004dfd50* &_Root() const { return _Head->parent; }
    Node_004dfd50* &_Lmost() const { return _Head->left; }
    Node_004dfd50* &_Rmost() const { return _Head->right; }
    unsigned int size() const { return _Size; }
    Class_004e0450 begin() const { Class_004e0450 i; i.ptr = _Head->left; return i; }
    Class_004e0450 end() const { Class_004e0450 i; i.ptr = _Head; return i; }

    // Shaped exactly like the MSVC 5 STL, including the dead `_F != begin()` test.
    Class_004e0450 erase(Class_004e0450 _F, Class_004e0450 _L)
    {
        if (size() == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            ((Class_004e03f0*)this)->FUN_004e03f0(_Root());
            _Root() = DAT_005292c4;
            _Size = 0;
            _Lmost() = _Head;
            _Rmost() = _Head;
            return begin();
        }
    }

    ~Tree_004dfd50()
    {
        erase(begin(), end());
        // Loaded nodes kept in locals (h, n) for the free-list push: re-reading shifts registers.
        Node_004dfd50* h = _Head;
        if (h != 0) {
            *(void**)h = DAT_00529e58;
            DAT_00529e58 = h;
        }
        _Head = 0, _Size = 0;
        {
            std::_Lockit Lk;
            if (--DAT_00529500 == 0) {
                Node_004dfd50* n = DAT_005292c4;
                if (n != 0) {
                    *(void**)n = DAT_00529e58;
                    DAT_00529e58 = n;
                }
                DAT_005292c4 = 0;
            }
        }
    }
};

// The object at 0x5292d0 (0x4df1e0 builds it, 0x4dfd10 hands it out): only
// its map at +0x21c has anything to destroy.
class Class_004df1e0 {
public:
    char unknown_0[0x21c];
    Tree_004dfd50 map;                 // +0x21c
    bool changed;                      // +0x22c
};

// FUNCTION: 0x4dfd50 _$E2
void trigger_004dfd50()
{
    static Class_004df1e0 obj;
}
