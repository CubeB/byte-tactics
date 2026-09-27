// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::iterator::_Dec() from MSVC 5's <xtree> (operator-- on the
// iterator of the tree whose _Nil node is DAT_00528a54, as in 0x4dd1b0,
// 0x4dd250 and 0x4dd2a0's sibling 0x4dd340), with _Max inlined. _Color is at
// +0x14 (the node's _Value is a 8-byte pair at +0xc) and _Red is 0.
#include <yvals.h>

struct Node_004dd2a0 {
    Node_004dd2a0* left;               // +0x0
    Node_004dd2a0* parent;             // +0x4
    Node_004dd2a0* right;              // +0x8
    char unknown_c[8];                 // +0xc (_Value)
    int color;                         // +0x14 (_Color)
};

extern Node_004dd2a0* DAT_00528a54;

static inline Node_004dd2a0* Max_004dd2a0(Node_004dd2a0* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a54) {
        p = p->right;
    }
    return p;
}

class Class_004dd2a0 {
public:
    Node_004dd2a0* ptr;                // +0x0

    void FUN_004dd2a0();
};

// FUNCTION: 0x4dd2a0
void Class_004dd2a0::FUN_004dd2a0()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a54) {
        ptr = Max_004dd2a0(ptr->left);
    } else {
        Node_004dd2a0* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}
