// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Tree iterator _Dec(): steps an iterator back to its in-order predecessor;
// DAT_00528a50 is the tree's _Nil node. The counterpart of the _Dec wrapper at
// 0x4dbe10 (which copies the iterator first), for the tree whose _Inc is at
// 0x4dde70. The node's _Color sits at +0x3c (the value type is 0x30 bytes).
#include <yvals.h>

struct Node_004dd820 {
    Node_004dd820* left;               // +0x0
    Node_004dd820* parent;             // +0x4
    Node_004dd820* right;              // +0x8
    char value[0x30];                  // +0xc
    int color;                         // +0x3c (0 = red)
};

extern Node_004dd820* DAT_00528a50;

static inline Node_004dd820* Max_004dd820(Node_004dd820* p)
{
    std::_Lockit lock;
    while (p->right != DAT_00528a50) {
        p = p->right;
    }
    return p;
}

class Class_004dd820 {
public:
    Node_004dd820* ptr;                // +0x0

    void FUN_004dd820();
};

// FUNCTION: 0x4dd820
void Class_004dd820::FUN_004dd820()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_00528a50) {
        ptr = Max_004dd820(ptr->left);
    } else {
        Node_004dd820* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}
