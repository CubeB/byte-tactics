// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::iterator::operator--(int) from MSVC 5's <xtree>: copy the
// iterator, step it to the in-order predecessor (_Dec, with _Max inlined,
// each under a std::_Lockit) and return the copy. DAT_00528a54 is the tree's
// _Nil node. The iterator has constructors, so it is returned through a
// hidden pointer.
#include <yvals.h>

struct Node_004dbe10 {
    Node_004dbe10* left;               // +0x0
    Node_004dbe10* parent;             // +0x4
    Node_004dbe10* right;              // +0x8
    int value[2];                      // +0xc
    int color;                         // +0x14 (0 = red)
};

extern Node_004dbe10* DAT_00528a54;

static inline Node_004dbe10* Max_004dbe10(Node_004dbe10* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

class Class_004dbe10 {
public:
    Node_004dbe10* ptr;                // +0x0

    Class_004dbe10() {}
    Class_004dbe10(Node_004dbe10* p) : ptr(p) {}

    void Dec()
    {
        std::_Lockit lock;
        if (ptr->color == 0 && ptr->parent->parent == ptr) {
            ptr = ptr->right;
        } else if (ptr->left != DAT_00528a54) {
            ptr = Max_004dbe10(ptr->left);
        } else {
            Node_004dbe10* p;
            while (ptr == (p = ptr->parent)->left) {
                ptr = p;
            }
            ptr = p;
        }
    }

    Class_004dbe10 FUN_004dbe10(int);
};

// FUNCTION: 0x4dbe10
Class_004dbe10 Class_004dbe10::FUN_004dbe10(int)
{
    Class_004dbe10 tmp = *this;
    Dec();
    return tmp;
}
