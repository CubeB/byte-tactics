// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, DeepSeek V4.1 Flash, space-bunny-free, Space Bunny Free, claude-opus-5-5, Claude Opus 5.5, claude-sonnet-5-5, Sonnet 5.5, GPT-6.1-sol, Haiku, Opus and Sonnet. Names are provisional.
// The debug library's allocators: the free-block set (std::map<unsigned int,
// unsigned int> as hand-walked red-black trees, GetFreeBlockSet 0x4db610)
// and the game's file-record map (0x4dc680 insert, 0x4dc910 erase, 0x4dce00
// find_, GetBlockMap 0x4da8d0), plus the single copies of the tree helpers
// the linker folded together, the memfussy command-line switches, the debug
// fill pattern and the block descriptions. AllocDebugBlock (0x4dacf0),
// GetFreeBlockSet (0x4db610) and FreeDebugBlock (0x4db7d0) stay in their own
// files: each inlines a tree helper (or constructs the std::map member) from
// that file's own view of the types, which cannot agree with the gathered
// file's one view of each class.
#include <windows.h>
#include <string.h>
#include <set>
#include <yvals.h>
#include <map>
#include <list>
#include <time.h>
// The real <vector> header must supply the body; a hand-rolled copy does not match.
#include <vector>
#include <algorithm>
// ---- the types the gathered files share, unified ---------------------------

// The debug allocator keeps its free blocks and the game its 0x30-byte file
// records in hand-walked red-black trees. Each matched file named the nodes
// and iterators in its own view; a name a symbol pins (data/progress.csv) is
// kept, and the rest of a tree's views share the fullest one.

class FreeBlockMap;
struct Node_004dbec0;
struct Node_004dc680;
struct Node_004dbd00;
struct Pair_004db450;
struct InsertResult_004db450;
struct Pair_004dbec0;
struct Pair_004dc680;
struct Node_004dd150;
struct Node_004dd1f0;
struct Node_004dd710;
struct Node_004dd770;

extern void* DAT_00528a54;             // the free-block tree's _Nil node
extern void* DAT_00528a50;             // the file-record tree's _Nil node
extern void* DAT_00528a10;             // the free-block tree's node free list
extern void* DAT_005289e0;             // the file-record tree's node free list
extern void (*DAT_005289bc)();         // out-of-memory handler
extern unsigned int DAT_005289d4;      // offset the last block was handed out at
extern unsigned int DAT_00528a00;      // how often the search wrapped around
extern unsigned int DAT_005289f0;      // bytes committed
extern unsigned int DAT_005289d0;      // high-water mark of DAT_005289f0
extern unsigned int DAT_00528a04;
extern FreeBlockMap* DAT_00528a40;     // the free-block set singleton

class Class_004d8820 {
public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d, const char* e);
};

// The file-record tree's node, the fullest of its views.
struct Node_004daa30 {
    Node_004daa30* left;           // +0x0
    Node_004daa30* parent;         // +0x4
    Node_004daa30* right;          // +0x8
    Class_004d8820 value;          // +0xc
    unsigned int color;            // +0x3c
};

struct BlockInfo {
    int field_0;                 // +0x00
    int field_4;                 // +0x04
    int field_8;                 // +0x08
    char unknown_c[0x24];        // +0x0c
    BlockInfo(void);
};

// The freed-block ring as 0x4dabb0 walks it.
struct Container_004da9f0 {
    int field_0;                 // +0x0
    void* field_4;               // +0x4
    void* field_8;               // +0x8
};

// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings (see 0x4df160 for the "fpufussy" twin).
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
};

// The trees' pool allocators: the methods ignore `this` and take the node
// size; each carves nodes from its own free list.
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

// The node a pool carves, named by the files that only return it.
class Class_004ddc00 {
public:
    Node_004dbec0* FUN_004ddc00(int parent, int color);
};

class Class_004ddc90 {
public:
    Node_004daa30* FUN_004ddc90(const unsigned int& key);
};

class Class_004ddce0 {
public:
    Node_004daa30* FUN_004ddce0(int parent, int color);
};

struct Pair_004db000 {
    unsigned int offset;               // +0x0
    unsigned int length;               // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    Pair_004db000 value;               // +0xc
    int color;                         // +0x14
};

// The out-of-line _Min, taken on its own opaque node type.
struct Node_004dd1b0 {
    Node_004dd1b0* left;               // +0x0
};

Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p);

// The other out-of-line moves, on the shared node type.
static inline Node_004db000* Max_004dd2a0(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

static inline Node_004db000* Min(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}

// The free-block tree's iterator: one pointer. FUN_004dd2a0 is its _Dec().
class Class_004dd2a0 {
public:
    Node_004db000* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dd2a0& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dd2a0& operator++() { Inc(); return *this; }
    Class_004dd2a0 operator++(int) { Class_004dd2a0 tmp = *this; ++*this; return tmp; }
    Class_004dd2a0& operator--() { FUN_004dd2a0(); return *this; }
    Class_004dd2a0 operator--(int) { Class_004dd2a0 tmp = *this; --*this; return tmp; }
    Node_004db000* Mynode() const { return ptr; }
    void FUN_004dd2a0();
    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54)
            ptr = (Node_004db000*)FUN_004dd1b0((Node_004dd1b0*)ptr->right);
        else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

class Class_004dd820 {
public:
    Node_004daa30* ptr;                // +0x0

    Class_004dd820() {}
    Class_004dd820(Node_004daa30* q) : ptr(q) {}
    bool operator==(const Class_004dd820& o) const { return ptr == o.ptr; }
    void FUN_004dd820();
};

// The (iterator, inserted) pair the trees' insert functions return: an
// iterator then a byte. 0x4ddbe0 is its out-of-line constructor.
class Class_004ddbe0 {
public:
    Class_004dd2a0 field_0;            // +0x0
    unsigned char field_4;             // +0x4

    Class_004ddbe0() {}
    Class_004ddbe0(const Class_004dd2a0& i, const unsigned char& b) : field_0(i), field_4(b) {}
    Class_004ddbe0(const Class_004dd820& i, unsigned char b) : field_0((Node_004db000*)i.ptr), field_4(b) {}
    // The byte is copied before the iterator on purpose: 0x4dbbc0 needs the
    // byte in cl and the dword in edx at every return.
    inline Class_004ddbe0(const Class_004ddbe0& o)
    {
        field_4 = o.field_4;
        field_0 = o.field_0;
    }
    Class_004ddbe0* FUN_004ddbe0(int* param_1, unsigned char* param_2);
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, unsigned char& second);
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, const bool& second);
};

// The free-block tree's iterator _Inc, out of line at 0x4dd340.
class Class_004dd340 {
public:
    Node_004db000* ptr;                // +0x0

    void FUN_004dd340();               // _Inc, out of line at 0x4dd340

    Class_004dd340& operator++()
    {
        FUN_004dd340();
        return *this;
    }

    Class_004dd340 operator++(int)
    {
        Class_004dd340 _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    Node_004db000* _Mynode() const { return ptr; }
};

static inline Node_004db000* Min_004dbd80(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54) {
        p = p->left;
    }
    return p;
}

static inline Node_004db000* Max_004dbd80(Node_004db000* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

static inline Node_004daa30* Max_004dd820(Node_004daa30* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a50) {
        p = p->right;
    }
    return p;
}

// std::_Tree<...>::upper_bound: the first node whose key is greater than the
// given key, or the head node; the out-of-line copy is 0x4dbd20.
struct Less_004dbd20 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Node_004dbd20 {
    Node_004dbd20* left;               // +0x0
    Node_004dbd20* parent;             // +0x4
    Node_004dbd20* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Iter_004dbd20 {
public:
    Node_004dbd20* ptr;
    Iter_004dbd20() : ptr(0) {}
    Iter_004dbd20(Node_004dbd20* p) : ptr(p) {}
};

class Class_004dbd20 {
public:
    Less_004dbd20 key_compare;
    Node_004dbd20* head;               // +0x4

    Iter_004dbd20 FUN_004dbd20(const unsigned int& kv);
};

// Inlined std::_Tree<...>::_Ubound(const _K&): its own lock scope.
static inline Node_004dbd20* Ubound_004dbd20(Class_004dbd20* self,
                                             const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dbd20* x = self->head->parent;
    Node_004dbd20* y = self->head;
    while (x != DAT_00528a54)
        if (self->key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}

// The free-block tree's iterator as 0x4dbd80 and 0x4dbe10 move it; the
// callers in 0x4dacf0 and 0x4db000 name the same class.
class Class_004dbe10 {
public:
    Node_004db000* ptr;                // +0x0

    Class_004dbe10() {}
    Class_004dbe10(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dbe10& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dbe10 FUN_004dbe10(int); // operator--(int)
    Class_004dbe10 FUN_004dbd80(int); // operator++(int)

    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54) {
            ptr = Min_004dbd80(ptr->right);
        } else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->right) {
                ptr = p;
            }
            if (ptr->right != p) {
                ptr = p;
            }
        }
    }

    void Dec()
    {
        std::_Lockit lock;
        if (ptr->color == 0 && ptr->parent->parent == ptr) {
            ptr = ptr->right;
        } else if (ptr->left != DAT_00528a54) {
            ptr = Max_004dbd80(ptr->left);
        } else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->left) {
                ptr = p;
            }
            ptr = p;
        }
    }
};

class Class_004dbeb0 {
public:
    char unknown_0[4];
    int* field_4;

    Class_004dbe10 FUN_004dbeb0();
    int* FUN_004dbeb0(int* param_1);
};

class Iter_004dbd00 {
public:
    Node_004dbd00* ptr;                // +0x0

    Iter_004dbd00() {}
    Iter_004dbd00(Node_004dbd00* p) : ptr(p) {}
};

class ConstIter_004dbd00 : public Iter_004dbd00 {
public:
    ConstIter_004dbd00() {}
    ConstIter_004dbd00(Node_004dbd00* p) : Iter_004dbd00(p) {}
    ConstIter_004dbd00(const Iter_004dbd00& x) : Iter_004dbd00(x) {}
};

struct Alloc_004dc130 { };             // empty, at the tree's +0
struct Less_004dc130 { };              // empty, at the tree's +1

class Class_004dc130 {
public:
    enum Redbl { _Red, _Black };

    Alloc_004dc130 allocator;          // +0x0
    Less_004dc130 key_compare;         // +0x1
    Node_004db000* head;               // +0x4
    bool multi;                        // +0x8
    unsigned int size;                 // +0xc

    static Node_004db000*& Left(Node_004db000* p) { return p->left; }
    static Node_004db000*& Parent(Node_004db000* p) { return p->parent; }
    static Node_004db000*& Right(Node_004db000* p) { return p->right; }
    static int& Color(Node_004db000* p) { return p->color; }
    static unsigned int* _Value(Node_004db000* p) { return (unsigned int*)&p->value; }

    Node_004db000*& Root() { return Parent(head); }
    Node_004db000*& Lmost() { return Left(head); }
    Node_004db000*& Rmost() { return Right(head); }

    static Node_004db000* Min(Node_004db000* p)
    {
        std::_Lockit _Lk;
        while (Left(p) != DAT_00528a54)
            p = Left(p);
        return p;
    }

    static Node_004db000* Max(Node_004db000* p)
    {
        std::_Lockit _Lk;
        while (Right(p) != DAT_00528a54)
            p = Right(p);
        return p;
    }

    void Lrotate(Node_004db000* x)
    {
        std::_Lockit _Lk;
        Node_004db000* y = Right(x);
        Right(x) = Left(y);
        if (Left(y) != DAT_00528a54)
            Parent(Left(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Left(Parent(x)))
            Left(Parent(x)) = y;
        else
            Right(Parent(x)) = y;
        Left(y) = x;
        Parent(x) = y;
    }

    void Rrotate(Node_004db000* x)
    {
        std::_Lockit _Lk;
        Node_004db000* y = Left(x);
        Left(x) = Right(y);
        if (Right(y) != DAT_00528a54)
            Parent(Right(y)) = x;
        Parent(y) = Parent(x);
        if (x == Root())
            Root() = y;
        else if (x == Right(Parent(x)))
            Right(Parent(x)) = y;
        else
            Left(Parent(x)) = y;
        Right(y) = x;
        Parent(x) = y;
    }

    static void Destval(unsigned int*) { }

    static void Freenode(Node_004db000* p)
    {
        if (p != 0) {
            *(void**)p = DAT_00528a10;
            DAT_00528a10 = p;
        }
    }

    Class_004dd340 FUN_004dc130(Class_004dd340 _P);
    Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it);
    void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
    Iter_004dbd00 FUN_004dc130(Iter_004dbd00 it);
};

class Class_004dbd00 {
public:
    Class_004dc130 tree;               // +0x0

    Class_004dbe10 FUN_004dbd00(Class_004dbe10 it);
    ConstIter_004dbd00 FUN_004dbd00(ConstIter_004dbd00 it);
};

struct Less_004dd250 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Node_004dd250 {
    Node_004dd250* left;               // +0x0
    Node_004dd250* parent;             // +0x4
    Node_004dd250* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Class_004dd250 {
public:
    Less_004dd250 key_compare;
    Node_004dd250* head;               // +0x4

    Node_004db000* FUN_004dd250(const Pair_004db000& k);
    Node_004dd250* FUN_004dd250(const unsigned int& kv);
};

struct Node_004dd7d0 {
    Node_004dd7d0* left;               // +0x0
    Node_004dd7d0* parent;             // +0x4
    Node_004dd7d0* right;              // +0x8
    unsigned int key;                  // +0xc
    int color;                         // +0x14
};

struct Less_004dd7d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_004dd7d0 {
public:
    Less_004dd7d0 key_compare;         // +0x0
    Node_004dd7d0* head;               // +0x4

    Class_004dd820 End() { return Class_004dd820((Node_004daa30*)head); }
    Class_004dd820 Begin() { return Class_004dd820((Node_004daa30*)head->left); }
    Node_004dd7d0* FUN_004dd7d0(const unsigned int& kv);
};

struct Less_004dd3d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dd3d0 {
public:
    Node_004daa30* ptr;                // +0x0

    Iter_004dd3d0() {}
    Iter_004dd3d0(Node_004daa30* p) : ptr(p) {}
    bool operator==(const Iter_004dd3d0& o) const { return ptr == o.ptr; }
};

class Class_004dd3d0 {
public:
    Less_004dd3d0 compare;             // +0x0
    Node_004daa30* head;               // +0x4

    Iter_004dd3d0 End() { return Iter_004dd3d0(head); }
    Iter_004dd3d0 FUN_004dd3d0(const unsigned int& key);
    Iter_004dd3d0 find(const unsigned int& key)
    {
        Iter_004dd3d0 it = FUN_004dd3d0(key);
        return (it == End() || compare(key, it.ptr->value.base)) ? End() : it;
    }

    Node_004daa30* Lbound(const unsigned int& key) const
    {
        std::_Lockit lock;
        Node_004daa30* x = head->parent;
        Node_004daa30* y = head;
        while (x != DAT_00528a50) {
            if (compare(x->value.base, key))
                x = x->right;
            else
                y = x, x = x->left;
        }
        return y;
    }
};

struct Less_004dc620 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dc620 {
public:
    Node_004db000* ptr;
    Iter_004dc620() : ptr(0) {}
    Iter_004dc620(Node_004db000* p) : ptr(p) {}
};

class Class_004dc620 {
public:
    Less_004dc620 key_compare;
    Node_004db000* head;               // +0x4

    Class_004dbe10 FUN_004dc620(const Pair_004db000& k);
    void FUN_004dc620(Class_004dd2a0* out, const unsigned int& kv);
    Iter_004dc620 FUN_004dc620(const unsigned int& kv);
};

class Class_004dd150 {
public:
    int unknown_0;
    Node_004dd150* head;            // +0x4
    void FUN_004dd150(Node_004dd150* x);
    void FUN_004dd150(Node_004db000* x);
};

struct Node_004dd150 {
    Node_004dd150* left;            // +0x0
    Node_004dd150* parent;          // +0x4
    Node_004dd150* right;           // +0x8
};

class Class_004dd1f0 {
public:
    int unknown_0;
    Node_004dd1f0* head;            // +0x4
    void FUN_004dd1f0(Node_004dd1f0* x);
    void FUN_004dd1f0(Node_004db000* x);
};

struct Node_004dd1f0 {
    Node_004dd1f0* left;            // +0x0
    Node_004dd1f0* parent;          // +0x4
    Node_004dd1f0* right;           // +0x8
};

struct Node_004dd710 {
    Node_004dd710* left;               // +0x0
    Node_004dd710* parent;             // +0x4
    Node_004dd710* right;              // +0x8
};

class Class_004dd710 {
public:
    int unknown_0;
    Node_004dd710* head;               // +0x4
    void FUN_004dd710(Node_004dd710* x);
    void FUN_004dd710(Node_004daa30* x);
};

struct Node_004dd770 {
    Node_004dd770* left;               // +0x0
    Node_004dd770* parent;             // +0x4
    Node_004dd770* right;              // +0x8
};

class Class_004dd770 {
public:
    int unknown_0;
    Node_004dd770* head;               // +0x4
    void FUN_004dd770(Node_004dd770* x);
    void FUN_004dd770(Node_004daa30* x);
};

struct Less_004dd430 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

struct Pair_004dd430 {
    unsigned int key;                 // +0x0
    char unknown_4[44];
};

struct Node_004dd430 {
    Node_004dd430* left;              // +0x0
    Node_004dd430* parent;            // +0x4
    Node_004dd430* right;             // +0x8
    Pair_004dd430 value;              // +0xc
    int color;                        // +0x3c
};

class Class_004dd430 {
public:
    Less_004dd430 key_compare;        // +0x0
    Node_004dd430* head;              // +0x4
    unsigned char unknown_8;          // +0x8
    int size;                         // +0xc

    void Lrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a50)
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

    void Rrotate(Node_004dd430* x)
    {
        std::_Lockit lock;
        Node_004dd430* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a50)
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

    Class_004dd2a0 FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                const Pair_004dd430& v);
    Class_004dd820* FUN_004dd430(Class_004dd820* out, Node_004daa30* x,
                                 Node_004daa30* y, const Pair_004dc680* v);
};

struct Pair_004dc680 {
    unsigned int key;                  // +0x0
    char unknown_4[44];
};

struct Less_004dc680 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

class Class_004dc680 {
public:
    Less_004dc680 key_compare;         // +0x0
    Node_004daa30* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd820 Begin() { return Class_004dd820(head->left); }

    Class_004ddbe0 FUN_004dc680(Pair_004dc680* p);
    Class_004ddbe0 FUN_004dc680(const Class_004d8820& v);
};

struct Value_004dc910 { char unknown_0[0x30]; };

struct Node_004dc910 {
    Node_004dc910* _Left;
    Node_004dc910* _Parent;
    Node_004dc910* _Right;
    Value_004dc910 _Value;
    int _Color;
};

class Class_004dde70 {
public:
    void FUN_004dde70();
};

class Iter_004dc910 {
public:
    Node_004dc910* _Ptr;
    Iter_004dc910() {}
    Iter_004dc910(Node_004dc910* _P) : _Ptr(_P) {}
    // Calls Class_004dde70::FUN_004dde70 (the _Inc at 0x4dde70) by that exact name.
    Iter_004dc910 operator++(int)
        { Iter_004dc910 _Tmp = *this;
          ((Class_004dde70*)this)->FUN_004dde70();
          return (_Tmp); }
    Node_004dc910* _Mynode() const { return _Ptr; }
};

class Class_004dc910 {
public:
    int unknown_0;
    Node_004dc910* _Head;              // +4
    unsigned char _Multi;              // +8
    int _Size;                         // +0xc

    Node_004dc910*& _Root() { return _Head->_Parent; }
    Node_004dc910*& _Lmost() { return _Head->_Left; }
    Node_004dc910*& _Rmost() { return _Head->_Right; }

    static Node_004dc910* _Min(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Left != DAT_00528a50)
              _P = _P->_Left;
          return _P; }

    static Node_004dc910* _Max(Node_004dc910* _P)
        { std::_Lockit _Lk;
          while (_P->_Right != DAT_00528a50)
              _P = _P->_Right;
          return _P; }

    void _Lrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Right;
          _X->_Right = _Y->_Left;
          if (_Y->_Left != DAT_00528a50)
              _Y->_Left->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Left)
              _X->_Parent->_Left = _Y;
          else
              _X->_Parent->_Right = _Y;
          _Y->_Left = _X;
          _X->_Parent = _Y; }

    void _Rrotate(Node_004dc910* _X)
        { std::_Lockit _Lk;
          Node_004dc910* _Y = _X->_Left;
          _X->_Left = _Y->_Right;
          if (_Y->_Right != DAT_00528a50)
              _Y->_Right->_Parent = _X;
          _Y->_Parent = _X->_Parent;
          if (_X == _Root())
              _Root() = _Y;
          else if (_X == _X->_Parent->_Right)
              _X->_Parent->_Right = _Y;
          else
              _X->_Parent->_Left = _Y;
          _Y->_Right = _X;
          _X->_Parent = _Y; }

    Iter_004dc910 erase(Iter_004dc910 _P);
};

struct Less_004dce00 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Iter_004dce00 {
public:
    Node_004daa30* ptr;

    Iter_004dce00() {}
    Iter_004dce00(Node_004daa30* p) : ptr(p) {}
    bool operator==(const Iter_004dce00& other) const { return ptr == other.ptr; }
    Class_004d8820& operator*() const { return ptr->value; }
    Class_004d8820* operator->() const { return &ptr->value; }
};

class Class_004dce00 {
public:
    Less_004dce00 compare;             // +0x0
    Node_004daa30* head;               // +0x4

    Iter_004dce00 end() { return Iter_004dce00(head); }
    Iter_004dce00 End() { return Iter_004dce00(head); }
    Iter_004dce00 FUN_004dce00(const unsigned int& key);
};

struct Less_004db000 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

struct Less_004dbec0 {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};

struct Pair_004dbec0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

class Class_004dbec0 {
public:
    Less_004dbec0 key_compare;         // +0x0
    Node_004db000* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }

    Class_004ddbe0 FUN_004dbec0(const Pair_004db000& v);
    void FUN_004dbec0(InsertResult_004db450* it, Pair_004db450* p);
    Class_004ddbe0 FUN_004dbec0(Pair_004dbec0* p);
};

struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct InsertResult_004db450 { Class_004dd2a0 first; int second; };
// The helper's one frame object: the pair, then the value_type.

struct Frame_004db450 { InsertResult_004db450 r; Pair_004db450 p; };

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db000* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    inline void Tail(unsigned base, unsigned len, Class_004dd2a0& it) {
    total += len;
    Class_004dd2a0 n;
    // One frame object (pair, then value_type): separate locals move the stack slots.
    Frame_004db450 f;
    f.p.offset = base;
    f.p.length = len;
    ((Class_004dc620*)this)->FUN_004dc620(&n, f.p.offset);
    it = n;
    ((Class_004dbeb0*)this)->FUN_004dbeb0((int*)&f.r);
    if (it == f.r.first)
        it.ptr = head;
    else
        it.FUN_004dd2a0();
    if (Neq(n, Class_004dd2a0(head))) {
        if (n.ptr->value.offset == f.p.offset + f.p.length) {
            f.p.length = f.p.length + n.ptr->value.length;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, n);
        }
    }
    if (Neq(it, Class_004dd2a0(head))) {
        if (it.ptr->value.offset + it.ptr->value.length == f.p.offset) {
            f.p.length = f.p.length + it.ptr->value.length;
            f.p.offset = it.ptr->value.offset;
            ((Class_004dc130*)this)->FUN_004dc130(&f.r.first, it);
        }
    }
    ((Class_004dbec0*)this)->FUN_004dbec0(&f.r, &f.p);
    }
    bool GrowReservation(unsigned int size);
};

struct Pair_004dbbc0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dbbc0 {
    Node_004dbbc0* left;               // +0x0
    Node_004dbbc0* parent;             // +0x4
    Node_004dbbc0* right;              // +0x8
    Pair_004dbbc0 value;               // +0xc
    int color;                         // +0x14
};

struct Kfn_004dbbc0 {
    const unsigned int& operator()(const Pair_004dbbc0& x) const { return x.offset; }
};

struct Less_004dbbc0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004dce60 {
public:
    unsigned char allocator;           // +0x0
    Less_004dbbc0 key_compare;         // +0x1
    Node_004dbbc0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc

    static Node_004dbbc0*& Left(Node_004dbbc0* p) { return p->left; }
    static Node_004dbbc0*& Right(Node_004dbbc0* p) { return p->right; }
    static const unsigned int& Key(Node_004dbbc0* p) { return Kfn_004dbbc0()(p->value); }
    Node_004dbbc0*& Root() { return head->parent; }
    Node_004dbbc0*& Lmost() { return Left(head); }
    Class_004dd2a0 begin() { return Class_004dd2a0((Node_004db000*)Lmost()); }

    // Left rotation of x, shaped like std::_Tree<...>::_Lrotate.
    void Lrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->right;
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
    void Rrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->left;
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

    Class_004dd2a0 FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                const Pair_004dbbc0* v);
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                Pair_004db000* v);
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                const Pair_004dbec0* v);

    Class_004ddbe0 TreeInsert(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004db000& v);
};

class FreeBlockMap {
public:
    Less_004db000 key_compare;         // +0x0
    Node_004db000* head;               // +0x4
    unsigned char rebuild;             // +0x8
    unsigned int count;                // +0xc
    unsigned int total;                // +0x10

    // The constructor (0x4db610) stays in its own file: it is the one function
    // that constructs the std::map member, whose _Init allocates the two
    // sentinel nodes; the walkers below use the same bytes as raw fields.
    Class_004dd2a0 begin() { return Class_004dd2a0(head->left); }
    Class_004dd2a0 end() { return Class_004dd2a0(head); }
    unsigned int size() const { return count; }
    // The original tests this as a value (sete; neg; sbb; inc; test), which
    // MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }
    Class_004dd2a0 upper_bound(const Pair_004db000& k)
    {
        return Class_004dd2a0(((Class_004dd250*)this)->FUN_004dd250(k));
    }

    void AddFreeBlock(Pair_004db000 p);
    unsigned int TakeFreeBlock(unsigned int bytes);

    // 0x4db450: reserve more address space and add it to the free blocks.
    bool Grow(unsigned int size)
    {
        // len declared before base: the other order swaps the operands of base + len.
        unsigned int len = 0x10000000;
        unsigned int base;

        if (2 * size > len && size < 0x40000000u)
            len = ((size + 0x1fff) & 0xffffe000) * 2;
        if (size > len)
            len = (size + 0x1fff) & 0xffffe000;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
        for (;;) {
            if (base != 0 && base + len <= 0x80000000u)
                break;
            if (base != 0)
                VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < size)
                return false;
            base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                              PAGE_READWRITE);
        }
        total += len;
        AddFreeBlock(Pair_004db000(base, len));
        return true;
    }

};

FreeBlockMap* GetFreeBlockSet();

// The <vector> the freed-block ring is (the real header supplies insert).
struct Elem_004dd8c0 {
    unsigned int w[0xc];               // +0x0, 0x30 bytes
};

class Alloc_004dd8c0 {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef Elem_004dd8c0* pointer;
    typedef const Elem_004dd8c0* const_pointer;
    typedef Elem_004dd8c0& reference;
    typedef const Elem_004dd8c0& const_reference;
    typedef Elem_004dd8c0 value_type;

    pointer allocate(size_type _N, const void* = 0)
    {
        pointer _P;
        do {
            _P = (pointer)GlobalAlloc(0, _N * sizeof(value_type));
            if (_P == 0 && DAT_005289bc != 0)
                DAT_005289bc();
        } while (_P == 0 && DAT_005289bc != 0);
        return _P;
    }
    void deallocate(pointer _P, size_type)
    {
        // Must test the pointer before freeing.
        if (_P != 0)
            GlobalFree(_P);
    }
    void construct(pointer _P, const value_type& _V)
    {
        std::_Construct(_P, _V);
    }
    void destroy(pointer) {}
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }
};

typedef std::vector<Elem_004dd8c0, Alloc_004dd8c0> Vec_004dd8c0;
typedef void (Vec_004dd8c0::*InsertFn_004dd8c0)(
    Vec_004dd8c0::iterator, Vec_004dd8c0::size_type, const Elem_004dd8c0&);

// The free functions the gathered functions call.
// AllocDebugBlock (0x4dacf0), GetFreeBlockSet (0x4db610) and FreeDebugBlock
// (0x4db7d0) stay in their own files: each inlines a tree helper written
// against that file's own view of the classes (Class_004dbe10's begin/end in
// 0x4dacf0, the std::map member's _Init in 0x4db610, the Class_004dbd20
// upper_bound stub in 0x4db7d0), and the gathered file holds only one view
// of each class.
char IsMemFussy();
char IsBackAlign();
int FUN_004db7c0();
CRITICAL_SECTION* FUN_004da780();
void* GetBlockMap();
Container_004da9f0* GetFreedBlockRing();
unsigned int __cdecl RoundUpToPage(unsigned int size);
unsigned int __cdecl FUN_004da8a0(unsigned int size);
void __cdecl CountAlloc(unsigned int size);
void __cdecl CountFree(unsigned int size);
void __cdecl FillPattern(void* at, int value, unsigned int count);
void __cdecl CheckFillPattern(void* at, int value, unsigned int count);
void __cdecl FindBlocksAroundAddress(unsigned int key, BlockInfo* prev, BlockInfo* next);
void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);
unsigned int __cdecl LookupBlockSize(unsigned int key);
void __cdecl RoundRangeToPages(unsigned int* lo, unsigned int* hi);
void* __cdecl AllocDebugBlock(unsigned int size, int flags);
void __cdecl FreeDebugBlock(void* p, int flags);
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags);
size_t __cdecl GetBlockSize(void* p);

// ---- from src/debug/debug_lib_4daa30.cpp --------------------

// FUNCTION: 0x4daa30
void __cdecl FindBlocksAroundAddress(unsigned int key, Class_004d8820* prev, Class_004d8820* next)
{
    LPCRITICAL_SECTION cs = FUN_004da780();
    EnterCriticalSection(cs);
    Class_004d8820 rec(key, 0, 0, 0, 0);
    // Named local: chaining the call changes the register allocation.
    Class_004dd7d0* tree = (Class_004dd7d0*)GetBlockMap();
    // Must be Class_004dd820 (the map's _Dec), not an ad hoc type.
    Class_004dd820 it;
    it.ptr = (Node_004daa30*)tree->FUN_004dd7d0(rec.base);
    if (it == ((Class_004dd7d0*)GetBlockMap())->End()) {
        next->base = 0;
        next->size = 0;
    } else {
        *next = it.ptr->value;
    }
    if (it == ((Class_004dd7d0*)GetBlockMap())->Begin()) {
        prev->base = 0;
        prev->size = 0;
    } else {
        it.FUN_004dd820();
        *prev = it.ptr->value;
    }
    LeaveCriticalSection(cs);
}

// ---- from src/debug/debug_lib_4dab10.cpp --------------------

// FUNCTION: 0x4dab10
void __cdecl DescribeAddress(unsigned int address, char* buf, int unused)
{
    BlockInfo local1;
    BlockInfo local2;
    FindBlocksAroundAddress(address, &local1, &local2);
    if (local1.field_0 != 0) {
        bool inRange = address >= (unsigned int)local1.field_0
                    && address < (unsigned int)local1.field_4 + (unsigned int)local1.field_0;
        if (inRange) {
            FormatBlockInfo(local1, buf, unused);
            return;
        }
    }
    strcpy(buf, "Address is not within an allocated block.");
}

// ---- from src/debug/debug_lib_4dabb0.cpp --------------------

// FUNCTION: 0x4dabb0
char __cdecl FUN_004dabb0(unsigned int address, char* buf, unsigned int n)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    *buf = 0;
    if (IsMemFussy()) {
        BlockInfo* p = (BlockInfo*)GetFreedBlockRing()->field_8;
        char found = 0;
        // One `&&` loop condition: a do/while with a break duplicates the size test.
        while (p != (BlockInfo*)GetFreedBlockRing()->field_4 && n > 100) {
            p = (BlockInfo*)((char*)p - 0x30);
            BlockInfo info = *p;
            bool inRange = address >= (unsigned int)info.field_0
                        && address < (unsigned int)info.field_4 + (unsigned int)info.field_0;
            if (inRange) {
                if (found)
                    strcat(buf, "\n");
                FormatBlockInfo(info, buf + strlen(buf), n);
                unsigned int len = strlen(buf);
                found = 1;
                buf += len;
                n -= len;
            }
        }
        LeaveCriticalSection(cs);
        return found;
    }
    LeaveCriticalSection(cs);
    return 0;
}

// ---- from src/debug/free_block_map.cpp --------------------

// FUNCTION: 0x4db000
void FreeBlockMap::AddFreeBlock(Pair_004db000 p)
{
    Class_004dd2a0 it;
    Class_004dd2a0 it2;
    // One function-scope object for all three calls: lets their identical endings merge.
    Class_004ddbe0 result;
    unsigned char inserted;

    Class_004dd2a0 n(((Class_004dd250*)this)->FUN_004dd250(p));
    it = n;
    if (n == begin()) {
        it = end();
    } else {
        it.FUN_004dd2a0();
    }
    if (Neq(n, end())) {
        if (n->offset == p.length + p.offset) {
            p.length = p.length + n->length;
            ((Class_004dc130*)this)->FUN_004dc130(n);
        }
    }
    if (Neq(it, end())) {
        Node_004db000* m = it.ptr;
        if (m->value.length + m->value.offset == p.offset) {
            p.length = p.length + m->value.length;
            p.offset = m->value.offset;
            ((Class_004dc130*)this)->FUN_004dc130(it);
        }
    }

    Node_004db000* y = head;
    bool less = true;
    Node_004db000* x = y->parent;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p.offset < x->value.offset;
            x = less ? x->left : x->right;
        }
    }

    if (rebuild) {
        Class_004dd2a0 t(((Class_004dce60*)this)->FUN_004dce60(x, y, &p));
        return;
    }
    it2.ptr = y;
    if (less) {
        if (Class_004dd2a0(y) == begin()) {
            inserted = 1;
            // Insert call passed straight in, not via a local: fixes the argument push order.
            result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
            goto done;
        }
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2->offset, p.offset)) {
        inserted = 1;
        result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
    } else {
        inserted = 0;
        result.FUN_004ddbe0(it2, inserted);
    }
done: ;
}

// The allocator's alloc(): look for a free block of `bytes` in the free-block
// set, preferring the block the last allocation came from (DAT_005289d4),
// take it out, hand back the leftovers on either side as new free blocks, and
// if two passes over the set find nothing, reserve more address space (Grow,
// the out-of-line copy of which is 0x4db450) and try again. DAT_00528a00
// counts the wraps around the set and DAT_00528a54 is the tree's _Nil node.
// FUNCTION: 0x4db1c0
unsigned int FreeBlockMap::TakeFreeBlock(unsigned int bytes)
{
    // Function scope, not inside the if: otherwise the recursive return becomes a jump.
    Pair_004db000 k;
    if (size() > 0) {
        k.offset = DAT_005289d4;
        k.length = 0;
        // STL-style iterator operators (--, ++, ->) throughout: their inlining shapes the code.
        Class_004dd2a0 lb = upper_bound(k);
        if (lb != begin()) {
            Class_004dd2a0 it = lb;
            it--;
            if (DAT_005289d4 >= it->offset && DAT_005289d4 + bytes <= it->offset + it->length)
                lb = it;
        }
        Class_004dd2a0 cur = lb;
        int tries = 0;
        do {
            if (cur == end()) {
                cur = begin();
                DAT_005289d4 = 0;
                DAT_00528a00++;
                tries++;
            }
            if (cur->length >= bytes) {
                // len declared before base but base read first: sets operand and load order.
                unsigned int len, base;
                base = cur->offset;
                len = cur->length;
                ((Class_004dc130*)this)->FUN_004dc130(cur);
                if (DAT_005289d4 == 0)
                    DAT_005289d4 = base;
                unsigned int mark;
                if (DAT_005289d4 >= base && DAT_005289d4 + bytes <= base + len)
                    mark = DAT_005289d4;
                else
                    mark = base;
                if (mark > base)
                    ((Class_004dbec0*)this)->FUN_004dbec0(Pair_004db000(base, mark - base));
                unsigned int end = mark + bytes;
                if (end < base + len)
                    ((Class_004dbec0*)this)->FUN_004dbec0(Pair_004db000(end, base - mark + len - bytes));
                DAT_005289d4 = end;
                return mark;
            }
            cur++;
        } while (tries < 2);
    }
    if (Grow(bytes))
        return TakeFreeBlock(bytes);
    return 0;
}

// ---- from src/debug/debug_lib_4db450.cpp --------------------

// FUNCTION: 0x4db450
bool Class_004db450::GrowReservation(unsigned int size)
{
    unsigned int len = 0x10000000;
    unsigned int base;
    // Once the reservation loop is done the parameter is dead, and the original
    // reuses its stack slot for the map iterator.
    Class_004dd2a0& it = *(Class_004dd2a0*)&size;

    if (2 * size > len && size < 0x40000000u)
        len = ((size + 0x1fff) & 0xffffe000) * 2;
    if (size > len)
        len = (size + 0x1fff) & 0xffffe000;
    base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                      PAGE_READWRITE);
    for (;;) {
        if (base != 0 && base + len <= 0x80000000u)
            break;
        if (base != 0)
            VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
        len = (len >> 1) & 0x7fffe000;
        if (len < 0x10000 || len < size)
            return false;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
    }
    Tail(base, len, *(Class_004dd2a0*)&size);
    return true;
}
// ---- from src/debug/debug_lib_4db760.cpp --------------------

// FUNCTION: 0x4db760
char IsBackAlign(void)
{
    static Class_004d9fe0 DAT_00528a20("backalign", 1, 1, 0, "-memfrontalign", 0, 0);
    return DAT_00528a20.on;
}

// ---- from src/debug/debug_lib_4db7b0.cpp --------------------

// FUNCTION: 0x4db7b0
void FUN_004db7b0(void)
{
}

// ---- from src/debug/debug_lib_4db7c0.cpp --------------------

// FUNCTION: 0x4db7c0
int FUN_004db7c0(void)
{
    return 0xcccccccc;
}

// ---- from src/debug/debug_lib_4dba40.cpp --------------------

// FUNCTION: 0x4dba40
void* __cdecl ReallocDebugBlock(void* p, unsigned int size, int flags)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    void* q = 0;
    size_t old = 0;
    if (p)
        old = GetBlockSize(p);
    if (size > 0) {
        q = AllocDebugBlock(size, flags);
        if (!q) {
            LeaveCriticalSection(cs);
            return 0;
        }
        unsigned int n = old >= size ? size : old;
        if (n > 0)
            memcpy(q, p, n);
    }
    if (p)
        FreeDebugBlock(p, flags);
    LeaveCriticalSection(cs);
    return q;
}

// ---- from src/debug/debug_lib_4dbae0.cpp --------------------

// FUNCTION: 0x4dbae0
unsigned int __cdecl LookupBlockSize(unsigned int key)
{
    LPCRITICAL_SECTION cs = FUN_004da780();
    EnterCriticalSection(cs);
    // Named local fetched before rec is built: GetBlockMap must be called first.
    Class_004dd3d0* tree = (Class_004dd3d0*)GetBlockMap();
    Class_004d8820 rec(key, 0, 0, 0, 0);
    Iter_004dd3d0 it = tree->find(rec.base);
    if (it == ((Class_004dd3d0*)GetBlockMap())->End()) {
        LeaveCriticalSection(cs);
        return 0;
    }
    unsigned int value = it.ptr->value.size;
    LeaveCriticalSection(cs);
    return value;
}

// ---- from src/debug/debug_lib_4dbb90.cpp --------------------

// FUNCTION: 0x4dbb90
void __cdecl RoundRangeToPages(unsigned int* param_1, unsigned int* param_2)
{
    *param_1 &= 0xfffff000;
    *param_2 = (*param_2 + 0xfff) & 0xfffff000;
}

// ---- from src/debug/debug_lib_4dbbc0.cpp --------------------

inline Class_004ddbe0 Class_004dce60::TreeInsert(const Pair_004dbbc0& V)
{
    Node_004dbbc0* X = Root();
    Node_004dbbc0* Y = head;
    bool Ans = true;
    {
        std::_Lockit Lk;
        while (X != DAT_00528a54) {
            Y = X;
            Ans = key_compare(Kfn_004dbbc0()(V), Key(X));
            X = Ans ? Left(X) : Right(X);
        }
    }
    if (multi)
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004dd2a0 P = Class_004dd2a0((Node_004db000*)Y);
    if (!Ans)
        ;
    else if (P == begin())
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    else
        P.FUN_004dd2a0();
    if (key_compare(Key((Node_004dbbc0*)P.Mynode()), Kfn_004dbbc0()(V)))
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004ddbe0 res;
    res.FUN_004ddbe0(P, false);
    return res;
}

// FUNCTION: 0x4dbbc0
Class_004ddbe0 Class_004dce60::FUN_004dbbc0(const Pair_004dbbc0& V)
{
    Class_004ddbe0 ans = TreeInsert(V);
    return Class_004ddbe0(ans.field_0, ans.field_4);
}

// The out-of-line tree insert for the allocator's free-block map, shaped like
// std::_Tree<...>::_Insert from MSVC 5's <xtree> but written out by hand.
//
// Under the outer std::_Lockit a 0x18-byte node is carved from the pool
// (0x4ddd70), the pair is placement-new'd into it, the map's size is bumped and
// the node is linked in. Then the red-black fixup walks up from the new node
// with a cursor z, colouring and rotating until z's parent is black or the root
// is reached, and finally blackens the root.
// FUNCTION: 0x4dce60
Class_004dd2a0 Class_004dce60::FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                            const Pair_004dbbc0* v)
{
    std::_Lockit lock;
    Node_004dbbc0* p = (Node_004dbbc0*)((Class_004ddd70*)this)->FUN_004ddd70(0x18);
    p->parent = y;
    p->color = 0;                      // red
    p->left = (Node_004dbbc0*)DAT_00528a54;
    p->right = (Node_004dbbc0*)DAT_00528a54;
    new ((void*)&p->value) Pair_004dbbc0(*v);
    ++size;

    // A positive disjunction through the bool comparator: keeps the left-child block as the then-part.
    if (y == head || x != DAT_00528a54 || key_compare(v->offset, y->value.offset)) {
        y->left = p;
        // The empty-tree case comes first and updates head->right, not head->left.
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

    Node_004dbbc0* z = p;
    // Explicit break, and z->parent->... re-read from the cursor: parent/grandparent locals cost a register.
    while (z != head->parent) {
        if (z->parent->color != 0)
            break;

        if (z->parent == z->parent->parent->left) {
            Node_004dbbc0* u = z->parent->parent->right;
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
            Node_004dbbc0* u = z->parent->parent->left;
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
    return Class_004dd2a0((Node_004db000*)p);
}

// ---- from src/debug/debug_lib_4dbd00.cpp --------------------

// FUNCTION: 0x4dbd00
ConstIter_004dbd00 Class_004dbd00::FUN_004dbd00(ConstIter_004dbd00 it)
{
    return tree.FUN_004dc130((Iter_004dbd00&)it);
}

// ---- from src/debug/debug_lib_4dbd20.cpp --------------------

// FUNCTION: 0x4dbd20
Iter_004dbd20 Class_004dbd20::FUN_004dbd20(const unsigned int& kv)
{
    return Iter_004dbd20(Ubound_004dbd20(this, kv));
}

// ---- from src/debug/debug_lib_4dbd80.cpp --------------------

// FUNCTION: 0x4dbd80
Class_004dbe10 Class_004dbe10::FUN_004dbd80(int)
{
    Class_004dbe10 tmp = *this;
    Inc();
    return tmp;
}

// Tree iterator operator--(int): copy the iterator, step it to the in-order
// predecessor and return the copy. DAT_00528a54 is the tree's _Nil node.
// FUNCTION: 0x4dbe10
Class_004dbe10 Class_004dbe10::FUN_004dbe10(int)
{
    Class_004dbe10 tmp = *this;
    Dec();
    return tmp;
}

// ---- from src/debug/debug_lib_4dbeb0.cpp --------------------

// 0x4db450's inlined Tail calls this out of line in the original.
#pragma auto_inline(off)
// FUNCTION: 0x4dbeb0
int* Class_004dbeb0::FUN_004dbeb0(int* param_1)
{
    *param_1 = *field_4;
    return param_1;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dbec0.cpp --------------------

// FUNCTION: 0x4dbec0
Class_004ddbe0 Class_004dbec0::FUN_004dbec0(Pair_004dbec0* p)
{
    // The pair is returned by value (no out parameter) and _Insert returns its
    // iterator by value too; a &p result slot would make p address-taken.
    Node_004db000* y = head;
    bool less = true;
    Node_004db000* x = y->parent;
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
            it = Class_004dd2a0((Node_004db000*)((Class_004ddc00*)this)->FUN_004ddc00((int)y, 0));
            Node_004db000* z = it.ptr;
            z->left = (Node_004db000*)DAT_00528a54;
            z->right = (Node_004db000*)DAT_00528a54;
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
            for (Node_004db000* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004db000* w = q->parent->parent->right;
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
                    Node_004db000* w = q->parent->parent->left;
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

// ---- from src/debug/debug_lib_4dc130.cpp --------------------

// FUNCTION: 0x4dc130 ?FUN_004dc130@Class_004dc130@@QAE?AVClass_004dd340@@V2@@Z
Class_004dd340 Class_004dc130::FUN_004dc130(Class_004dd340 _P)
{
    Node_004db000* _X;
    Node_004db000* _Y = (_P++)._Mynode();
    Node_004db000* _Z = _Y;
    std::_Lockit _Lk;
    if (Left(_Y) == DAT_00528a54)
        _X = Right(_Y);
    else if (Right(_Y) == DAT_00528a54)
        _X = Left(_Y);
    else
        _Y = Min(Right(_Y)), _X = Right(_Y);
    if (_Y != _Z) {
        Parent(Left(_Z)) = _Y;
        Left(_Y) = Left(_Z);
        if (_Y == Right(_Z))
            Parent(_X) = _Y;
        else {
            Parent(_X) = Parent(_Y);
            Left(Parent(_Y)) = _X;
            Right(_Y) = Right(_Z);
            Parent(Right(_Z)) = _Y;
        }
        if (Root() == _Z)
            Root() = _Y;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _Y;
        else
            Right(Parent(_Z)) = _Y;
        Parent(_Y) = Parent(_Z);
        std::swap(Color(_Y), Color(_Z));
        _Y = _Z;
    } else {
        Parent(_X) = Parent(_Y);
        if (Root() == _Z)
            Root() = _X;
        else if (Left(Parent(_Z)) == _Z)
            Left(Parent(_Z)) = _X;
        else
            Right(Parent(_Z)) = _X;
        if (Lmost() != _Z)
            ;
        else if (Right(_Z) == DAT_00528a54)
            Lmost() = Parent(_Z);
        else
            Lmost() = Min(_X);
        if (Rmost() != _Z)
            ;
        else if (Left(_Z) == DAT_00528a54)
            Rmost() = Parent(_Z);
        else
            Rmost() = Max(_X);
    }
    if (Color(_Y) == _Black) {
        while (_X != Root() && Color(_X) == _Black)
            if (_X == Left(Parent(_X))) {
                Node_004db000* _W = Right(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Lrotate(Parent(_X));
                    _W = Right(Parent(_X));
                }
                if (Color(Left(_W)) == _Black && Color(Right(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Right(_W)) == _Black) {
                        Color(Left(_W)) = _Black;
                        Color(_W) = _Red;
                        Rrotate(_W);
                        _W = Right(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Right(_W)) = _Black;
                    Lrotate(Parent(_X));
                    break;
                }
            } else {
                Node_004db000* _W = Left(Parent(_X));
                if (Color(_W) == _Red) {
                    Color(_W) = _Black;
                    Color(Parent(_X)) = _Red;
                    Rrotate(Parent(_X));
                    _W = Left(Parent(_X));
                }
                if (Color(Right(_W)) == _Black && Color(Left(_W)) == _Black) {
                    Color(_W) = _Red;
                    _X = Parent(_X);
                } else {
                    if (Color(Left(_W)) == _Black) {
                        Color(Right(_W)) = _Black;
                        Color(_W) = _Red;
                        Lrotate(_W);
                        _W = Left(Parent(_X));
                    }
                    Color(_W) = Color(Parent(_X));
                    Color(Parent(_X)) = _Black;
                    Color(Left(_W)) = _Black;
                    Rrotate(Parent(_X));
                    break;
                }
            }
        Color(_X) = _Black;
    }
    Destval(_Value(_Y));
    Freenode(_Y);
    --size;
    return (_P);
}

// ---- from src/debug/debug_lib_4dc620.cpp --------------------

// FUNCTION: 0x4dc620
Iter_004dc620 Class_004dc620::FUN_004dc620(const unsigned int& kv)
{
    Iter_004dc620 y;
    {
        std::_Lockit lock;
        Node_004db000* x = head->parent;
        y.ptr = head;
        while (x != DAT_00528a54)
            if (key_compare(kv, x->value.offset))
                y.ptr = x, x = x->left;
            else
                x = x->right;
    }
    return y;
}

// ---- from src/debug/debug_lib_4dc680.cpp --------------------

// FUNCTION: 0x4dc680
Class_004ddbe0 Class_004dc680::FUN_004dc680(Pair_004dc680* p)
{
    Node_004daa30* y = head;
    bool less = true;
    // Read before the first _Lockit.
    Node_004daa30* x = y->parent;
    Class_004dd820 it2;
    Class_004dd820 it;
    {
        std::_Lockit lock;
        while (x != DAT_00528a50) {
            y = x;
            less = p->key < x->value.base;
            x = less ? x->left : x->right;
        }
    }
    if (rebuild) {
        {
            std::_Lockit lock;
            it = Class_004dd820(((Class_004ddce0*)this)->FUN_004ddce0((int)y, 0));
            Node_004daa30* z = it.ptr;
            z->left = (Node_004daa30*)DAT_00528a50;
            z->right = (Node_004daa30*)DAT_00528a50;
            new ((void*)&z->value) Pair_004dc680(*p);
            size++;
            if (y == head || x != DAT_00528a50 || key_compare(p->key, y->value.base)) {
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
            for (Node_004daa30* q = z; q != head->parent && q->parent->color == 0; ) {
                if (q->parent == q->parent->parent->left) {
                    Node_004daa30* w = q->parent->parent->right;
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
                    Node_004daa30* w = q->parent->parent->left;
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
        // Outside the _Lockit scope: the pair is built after ~_Lockit.
        return Class_004ddbe0(it, 1);
    }
    it2 = Class_004dd820(y);
    if (less) {
        if (Class_004dd820(y) == Begin())
            return Class_004ddbe0(*((Class_004dd430*)this)->FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
        it2.FUN_004dd820();
    }
    if (key_compare(it2.ptr->value.base, p->key))
        return Class_004ddbe0(*((Class_004dd430*)this)->FUN_004dd430((Class_004dd820*)&p, x, y, p), 1);
    return Class_004ddbe0(it2, 0);
}

// ---- from src/debug/debug_lib_4dc910.cpp --------------------

// FUNCTION: 0x4dc910
Iter_004dc910 Class_004dc910::erase(Iter_004dc910 _P)
{
    Node_004dc910* _X;
    Node_004dc910* _Y = (_P++)._Mynode();
    Node_004dc910* _Z = _Y;
    std::_Lockit _Lk;
    if (_Y->_Left == DAT_00528a50)
        _X = _Y->_Right;
    else if (_Y->_Right == DAT_00528a50)
        _X = _Y->_Left;
    else
        _Y = _Min(_Y->_Right), _X = _Y->_Right;
    if (_Y != _Z)
        {
        _Z->_Left->_Parent = _Y;
        _Y->_Left = _Z->_Left;
        if (_Y == _Z->_Right)
            _X->_Parent = _Y;
        else
            {
            _X->_Parent = _Y->_Parent;
            _Y->_Parent->_Left = _X;
            _Y->_Right = _Z->_Right;
            _Z->_Right->_Parent = _Y;
            }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _Y;
        else
            _Z->_Parent->_Right = _Y;
        _Y->_Parent = _Z->_Parent;
        std::swap(_Y->_Color, _Z->_Color);
        _Y = _Z;
        }
    else
        {
        _X->_Parent = _Y->_Parent;
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Z->_Parent->_Left == _Z)
            _Z->_Parent->_Left = _X;
        else
            _Z->_Parent->_Right = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Z->_Right == DAT_00528a50)
            _Lmost() = _Z->_Parent;
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Z->_Left == DAT_00528a50)
            _Rmost() = _Z->_Parent;
        else
            _Rmost() = _Max(_X);
        }
    if (_Y->_Color == 1)
        {
        while (_X != _Root() && _X->_Color == 1)
            if (_X == _X->_Parent->_Left)
                {
                Node_004dc910* _W = _X->_Parent->_Right;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Lrotate(_X->_Parent);
                    _W = _X->_Parent->_Right;
                    }
                if (_W->_Left->_Color == 1 && _W->_Right->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Right->_Color == 1)
                        {
                        _W->_Left->_Color = 1;
                        _W->_Color = 0;
                        _Rrotate(_W);
                        _W = _X->_Parent->_Right;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Right->_Color = 1;
                    _Lrotate(_X->_Parent);
                    break;
                    }
                }
            else
                {
                Node_004dc910* _W = _X->_Parent->_Left;
                if (_W->_Color == 0)
                    {
                    _W->_Color = 1;
                    _X->_Parent->_Color = 0;
                    _Rrotate(_X->_Parent);
                    _W = _X->_Parent->_Left;
                    }
                if (_W->_Right->_Color == 1 && _W->_Left->_Color == 1)
                    {
                    _W->_Color = 0;
                    _X = _X->_Parent;
                    }
                else
                    {
                    if (_W->_Left->_Color == 1)
                        {
                        _W->_Right->_Color = 1;
                        _W->_Color = 0;
                        _Lrotate(_W);
                        _W = _X->_Parent->_Left;
                        }
                    _W->_Color = _X->_Parent->_Color;
                    _X->_Parent->_Color = 1;
                    _W->_Left->_Color = 1;
                    _Rrotate(_X->_Parent);
                    break;
                    }
                }
        _X->_Color = 1;
        }
    if (_Y != 0)
        {
        *(void**)_Y = DAT_005289e0;
        DAT_005289e0 = _Y;
        }
    --_Size;
    return (_P);
}

// ---- from src/debug/debug_lib_4dce00.cpp --------------------

// FUNCTION: 0x4dce00
Iter_004dce00 Class_004dce00::FUN_004dce00(const unsigned int& key)
{
    Iter_004dce00 p = Iter_004dce00(((Class_004ddc90*)this)->FUN_004ddc90(key));
    return (p == End() || compare(key, p.ptr->value.base)) ? End() : p;
}

// ---- from src/debug/debug_lib_4dd150.cpp --------------------

// FUNCTION: 0x4dd150
void Class_004dd150::FUN_004dd150(Node_004dd150* x)
{
    std::_Lockit lock;
    Node_004dd150* y = x->right;
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

// ---- from src/debug/debug_lib_4dd1b0.cpp --------------------

// 0x4db1c0 calls this out of line in the original (Inc repeats the loop
// inline only where the original did).
#pragma auto_inline(off)
// FUNCTION: 0x4dd1b0
Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a54)
        p = p->left;
    return p;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd1f0.cpp --------------------

// FUNCTION: 0x4dd1f0
void Class_004dd1f0::FUN_004dd1f0(Node_004dd1f0* x)
{
    std::_Lockit lock;
    Node_004dd1f0* y = x->left;
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

// ---- from src/debug/debug_lib_4dd250.cpp --------------------

// FUNCTION: 0x4dd250
Node_004dd250* Class_004dd250::FUN_004dd250(const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dd250* x = head->parent;
    Node_004dd250* y = head;
    while (x != DAT_00528a54)
        if (key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}

// ---- from src/debug/debug_lib_4dd2a0.cpp --------------------

// The original calls this out of line from 0x4daa30 and 0x4db1c0;
// their files had no definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd2a0
void Class_004dd2a0::FUN_004dd2a0()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a54) {
        ptr = Max_004dd2a0(ptr->left);
    } else {
        Node_004db000* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd340.cpp --------------------

// FUNCTION: 0x4dd340
void Class_004dd340::FUN_004dd340()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a54)
        ptr = Min(ptr->right);
    else {
        Node_004db000* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}

// ---- from src/debug/debug_lib_4dd3d0.cpp --------------------

// 0x4dbae0's inlined find calls this out of line in the original; its
// file had no definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd3d0
Iter_004dd3d0 Class_004dd3d0::FUN_004dd3d0(const unsigned int& key)
{
    return Iter_004dd3d0(Lbound(key));
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd430.cpp --------------------

// FUNCTION: 0x4dd430
Class_004dd2a0 Class_004dd430::FUN_004dd430(Node_004dd430* x, Node_004dd430* y,
                                            const Pair_004dd430& v)
{
    std::_Lockit lock;
    Node_004dd430* z = (Node_004dd430*)
                       ((Class_004dddf0*)this)->FUN_004dddf0(0x40);
    z->parent = y;
    z->color = 0;
    z->left = (Node_004dd430*)DAT_00528a50;
    z->right = (Node_004dd430*)DAT_00528a50;
    new ((void*)&z->value) Pair_004dd430(v);
    size++;
    if (y == head || x != DAT_00528a50 || key_compare(v.key, y->value.key)) {
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
    for (x = z; x != head->parent && x->parent->color == 0; ) {
        if (x->parent == x->parent->parent->left) {
            Node_004dd430* w = x->parent->parent->right;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    Lrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Rrotate(x->parent->parent);
            }
        } else {
            Node_004dd430* w = x->parent->parent->left;
            if (w->color == 0) {
                x->parent->color = 1;
                w->color = 1;
                x->parent->parent->color = 0;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    Rrotate(x);
                }
                x->parent->color = 1;
                x->parent->parent->color = 0;
                Lrotate(x->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0((Node_004db000*)z);
}

// ---- from src/debug/debug_lib_4dd710.cpp --------------------

// FUNCTION: 0x4dd710
void Class_004dd710::FUN_004dd710(Node_004dd710* x)
{
    std::_Lockit lock;
    Node_004dd710* y = x->right;
    x->right = y->left;
    if (y->left != DAT_00528a50)
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

// ---- from src/debug/debug_lib_4dd770.cpp --------------------

// FUNCTION: 0x4dd770
void Class_004dd770::FUN_004dd770(Node_004dd770* x)
{
    std::_Lockit lock;
    Node_004dd770* y = x->left;
    x->left = y->right;
    if (y->right != DAT_00528a50)
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

// ---- from src/debug/debug_lib_4dd7d0.cpp --------------------

// The original calls this out of line from 0x4daa30; its file had no
// definition to inline.
#pragma auto_inline(off)
// FUNCTION: 0x4dd7d0
Node_004dd7d0* Class_004dd7d0::FUN_004dd7d0(const unsigned int& kv)
{
    std::_Lockit lock;
    Node_004dd7d0* x = head->parent;
    Node_004dd7d0* y = head;
    while (x != DAT_00528a50)
        if (key_compare(kv, x->key))
            y = x, x = x->left;
        else
            x = x->right;
    return y;
}
#pragma auto_inline(on)

// ---- from src/debug/debug_lib_4dd820.cpp --------------------

// FUNCTION: 0x4dd820
void Class_004dd820::FUN_004dd820()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a50) {
        ptr = Max_004dd820(ptr->left);
    } else {
        Node_004daa30* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}

// ---- from src/debug/debug_lib_4dd8c0.cpp --------------------

// FUNCTION: 0x4dd8c0 ?insert@?$vector@UElem_004dd8c0@@VAlloc_004dd8c0@@@std@@QAEXPAUElem_004dd8c0@@IABU3@@Z
InsertFn_004dd8c0 g_insert_004dd8c0 = &Vec_004dd8c0::insert;

// ---- from src/debug/debug_lib_4ddbe0.cpp --------------------

// FUNCTION: 0x4ddbe0
Class_004ddbe0* Class_004ddbe0::FUN_004ddbe0(int* param_1, unsigned char* param_2)
{
    Class_004ddbe0* eax = this;
    int* ecx = param_1;
    int edx = *ecx;
    unsigned char* ecx2 = (unsigned char*)param_2;
    eax->field_0.ptr = (Node_004db000*)edx;
    unsigned char dl = *ecx2;
    eax->field_4 = dl;
    return eax;
}
