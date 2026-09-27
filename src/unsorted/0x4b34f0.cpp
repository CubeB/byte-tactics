// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::iterator::_Dec() from MSVC 5's <xtree>, with
// the inlined _Max taking its own lock, for the tree whose _Nil node is
// DAT_0051fbbc (see 0x4b3430.cpp and the _Inc twin 0x4b3590.cpp).
#include <yvals.h>

struct Node_004b34f0 {
    Node_004b34f0* left;            // +0x0
    Node_004b34f0* parent;          // +0x4
    Node_004b34f0* right;           // +0x8
    int key;                        // +0xc
    int value;                      // +0x10
    int color;                      // +0x14, 0 = red
};

extern Node_004b34f0* DAT_0051fbbc;

static inline Node_004b34f0* Max(Node_004b34f0* p)
{
    std::_Lockit lock;
    while (p->right != DAT_0051fbbc)
        p = p->right;
    return p;
}

class Class_004b34f0 {
public:
    Node_004b34f0* ptr;             // +0x0
    void FUN_004b34f0();
};

// FUNCTION: 0x4b34f0
void Class_004b34f0::FUN_004b34f0()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr)
        ptr = ptr->right;
    else if (ptr->left != DAT_0051fbbc)
        ptr = Max(ptr->left);
    else {
        Node_004b34f0* p;
        while (ptr == (p = ptr->parent)->left)
            ptr = p;
        ptr = p;
    }
}
