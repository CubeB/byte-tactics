// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Compiler-generated atexit destructor of the file-local std::map<int,int> at
// 0x51fbc0, whose initialiser is 0x4b2290.
//
// The tree layout (see 0x4b2290 and 0x4b3490): allocator byte at +0, empty
// comparator byte at +1, _Head at +4, the _Multi flag at +8 and _Size at +0xc.
// A node is 0x18 bytes: left, parent, right, key, value, then the red/black
// colour at +0x14. DAT_0051fbbc is the tree's shared _Nil node and
// DAT_0051fbb8 its reference count.
#include <yvals.h>
#include <new>

struct Node_004b2340 {
    Node_004b2340* left;            // +0x00
    Node_004b2340* parent;          // +0x04
    Node_004b2340* right;           // +0x08
    int key;                        // +0x0c
    int value;                      // +0x10
    int color;                      // +0x14
};

extern Node_004b2340* DAT_0051fbbc;
extern int DAT_0051fbb8;

class Class_004b3590 {
public:
    Node_004b2340* ptr;             // +0x0
    void FUN_004b3590();            // iterator::_Inc, 0x4b3590
};

class Class_004b2fb0 {
public:
    struct iterator {
        Node_004b2340* ptr;         // +0x0
        iterator() {}
        iterator(Node_004b2340* p) : ptr(p) {}
        bool operator==(const iterator& x) const { return ptr == x.ptr; }
        bool operator!=(const iterator& x) const { return !(*this == x); }
        iterator& operator++()
        {
            ((Class_004b3590*)&ptr)->FUN_004b3590();
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            ((Class_004b3590*)&ptr)->FUN_004b3590();
            return tmp;
        }
    };
    char alloc;                     // +0x0
    char comp;                      // +0x1
    Node_004b2340* head;            // +0x4
    char multi;                     // +0x8
    int size;                       // +0xc

    iterator begin() { return head->left; }
    iterator end() { return head; }
    iterator FUN_004b2ac0(iterator _P);          // erase(iterator), 0x4b2ac0
    void FUN_004b2fb0(Node_004b2340* p);         // _Erase, 0x4b2fb0
    void _Freenode(Node_004b2340* p) { operator delete(p); }

    iterator erase(iterator _F, iterator _L)
    {
        if (size == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                FUN_004b2ac0(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            FUN_004b2fb0(head->parent);
            head->parent = DAT_0051fbbc;
            size = 0;
            head->left = head;
            head->right = head;
            return begin();
        }
    }

    ~Class_004b2fb0()
    {
        erase(begin(), end());
        _Freenode(head);
        head = 0;
        size = 0;
        {
            std::_Lockit Lk;
            if (--DAT_0051fbb8 == 0) {
                _Freenode(DAT_0051fbbc);
                DAT_0051fbbc = 0;
            }
        }
    }
};

// The global must stay static (internal linkage): it changes how erase compiles.
// FUNCTION: 0x4b2340 _$E2
static Class_004b2fb0 DAT_0051fbc0;