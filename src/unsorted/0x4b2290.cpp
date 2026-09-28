// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Compiler-generated dynamic initialiser for the global red-black tree (map)
// at DAT_0051fbc0, the tree whose methods are 0x4b26f0, 0x4b3020, 0x4b3430 and
// friends. DAT_0051fbbc is its shared _Nil node and DAT_0051fbb8 its reference
// count, so the compiler emits this function (and the atexit destructor at
// 0x4b2340) for the object; like other compiler-generated initialisers it is
// annotated with its own symbol name.
//
// The object layout (from 0x4b3020 and 0x4b3490): an empty allocator byte at
// +0, an empty comparator byte at +1, the head node pointer at +4, a byte flag
// at +8 and the element count at +0xc. The inlined node allocation matches
// 0x4b3410 (_Buynode).
#include <yvals.h>

struct Node_004b2290 {
    Node_004b2290* left;            // +0x0
    Node_004b2290* parent;          // +0x4
    Node_004b2290* right;           // +0x8
    int key;                        // +0xc
    int value;                      // +0x10
    int color;                      // +0x14
};

struct Alloc_004b2290 {};
struct Comp_004b2290 {};

extern Node_004b2290* DAT_0051fbbc;
extern int DAT_0051fbb8;

static Node_004b2290* Buynode(Node_004b2290* parent, int color)
{
    Node_004b2290* node = (Node_004b2290*)operator new(0x18);
    node->parent = parent;
    node->color = color;
    return node;
}

class Class_004b2290 {
public:
    Comp_004b2290 comp;             // +0x0
    Alloc_004b2290 alloc;           // +0x1
    Node_004b2290* head;            // +0x4
    char flag;                      // +0x8
    int size;                       // +0xc

    Class_004b2290(const Comp_004b2290& c = Comp_004b2290(),
                   const Alloc_004b2290& a = Alloc_004b2290())
        : comp(c), alloc(a), flag(0)
    {
        std::_Lockit lock;
        if (DAT_0051fbbc == 0) {
            DAT_0051fbbc = Buynode(0, 1);
            DAT_0051fbbc->left = 0;
            DAT_0051fbbc->right = 0;
        }
        ++DAT_0051fbb8;
        head = Buynode(DAT_0051fbbc, 0);
        size = 0;
        head->left = head;
        head->right = head;
    }
    ~Class_004b2290();
};

// FUNCTION: 0x4b2290 _$E4
Class_004b2290 DAT_0051fbc0;
