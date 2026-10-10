// NameMapTree: the debug library's pooled name map, a std::map from a name (a
// C string ordered by strcmp, held in NameKey) to a 500-byte text value, built
// as MSVC 5's std::_Tree. The one declaration of the map and its key for
// debug_lib_4dfd50.cpp (the performance singleton's map teardown) and
// debug_lib_4e2620.cpp (_Tree::_Insert). The header includes nothing and
// expects <yvals.h> for std::_Lockit in the inlined rotations and teardown. The
// other views stay in their own files: debug_lib_4e1990.cpp is the
// NameKey-set interface NameTable derives from, with its own Iter_004e1990
// and InsertResult_004e1990, and out-of-line tree methods; debug_lib_4e2580.cpp
// is keyed by const char*; and debug_lib.cpp spells the tree through its own
// _Nodeptr, pair value and NameMapIter view.
#ifndef NAME_MAP_TREE_H
#define NAME_MAP_TREE_H

// The key: a C string ordered by strcmp.
class NameKey {
public:
    char* name;                        // +0x0
    bool LessThan(const NameKey& other) const;
};

struct Value_004e2620 {
    NameKey key;                       // +0x0
    char text[500];                    // +0x4

    Value_004e2620& operator=(const Value_004e2620& v);
};

struct Node_004e2620 {
    Node_004e2620* left;               // +0x0
    Node_004e2620* parent;             // +0x4
    Node_004e2620* right;              // +0x8
    Value_004e2620 value;              // +0xc
    int color;                         // +0x204
};

extern Node_004e2620* DAT_005292c4;    // the tree's _Nil
extern void* g_nameMapFreeList;        // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs

class NameMapAllocator {
public:
    void* Allocate(unsigned int n);
};

struct Less_004e2620 {
    bool operator()(const NameKey& a, const NameKey& b) const
    {
        return a.LessThan(b);
    }
};

class Iter_004e2620 {
public:
    Node_004e2620* ptr;

    Iter_004e2620() {}
    Iter_004e2620(Node_004e2620* p) : ptr(p) {}
};

// The tree's iterator; its _Inc is out of line.
class NameMapIter {
public:
    Node_004e2620* ptr;                // +0x0

    void NextNode();
    NameMapIter& operator++() { NextNode(); return *this; }
    NameMapIter operator++(int)
    {
        NameMapIter t = *this;
        ++*this;
        return t;
    }
    bool operator==(const NameMapIter& x) const { return ptr == x.ptr; }
    bool operator!=(const NameMapIter& x) const { return !(*this == x); }
};

class NameMapTree {
public:
    Less_004e2620 compare;             // +0x0
    Node_004e2620* _Head;              // +0x4
    bool _Multi;                       // +0x8
    char pad_9[3];
    unsigned int _Size;                // +0xc

    Node_004e2620*& _Root() { return _Head->parent; }
    Node_004e2620*& _Lmost() { return _Head->left; }
    Node_004e2620*& _Rmost() { return _Head->right; }
    unsigned int size() const { return _Size; }

    static Node_004e2620*& _Left(Node_004e2620* p) { return p->left; }
    static Node_004e2620*& _Right(Node_004e2620* p) { return p->right; }
    static Node_004e2620*& _Parent(Node_004e2620* p) { return p->parent; }
    static int& _Color(Node_004e2620* p) { return p->color; }

    void _Lrotate(Node_004e2620* _X)
    {
        std::_Lockit _Lk;
        Node_004e2620* _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (_Left(_Y) != DAT_005292c4)
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y;
    }
    void _Rrotate(Node_004e2620* _X)
    {
        std::_Lockit _Lk;
        Node_004e2620* _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (_Right(_Y) != DAT_005292c4)
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y;
    }

    Iter_004e2620 Insert(Node_004e2620* _X, Node_004e2620* _Y, const Value_004e2620& _V);

    NameMapIter begin() const { NameMapIter i; i.ptr = _Head->left; return i; }
    NameMapIter end() const { NameMapIter i; i.ptr = _Head; return i; }

    // Shaped exactly like the MSVC 5 STL, including the dead `_F != begin()` test.
    NameMapIter erase(NameMapIter _F, NameMapIter _L)
    {
        if (size() == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                Erase(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            EraseSubtree(_Root());
            _Root() = DAT_005292c4;
            _Size = 0;
            _Lmost() = _Head;
            _Rmost() = _Head;
            return begin();
        }
    }

    void EraseSubtree(Node_004e2620* x);
    NameMapIter Erase(NameMapIter it);

    ~NameMapTree()
    {
        erase(begin(), end());
        // Loaded nodes kept in locals (h, n) for the free-list push: re-reading shifts registers.
        Node_004e2620* h = _Head;
        if (h != 0) {
            *(void**)h = g_nameMapFreeList;
            g_nameMapFreeList = h;
        }
        _Head = 0, _Size = 0;
        {
            std::_Lockit Lk;
            if (--DAT_00529500 == 0) {
                Node_004e2620* n = DAT_005292c4;
                if (n != 0) {
                    *(void**)n = g_nameMapFreeList;
                    g_nameMapFreeList = n;
                }
                DAT_005292c4 = 0;
            }
        }
    }
};

#endif
