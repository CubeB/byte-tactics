// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::iterator::_Dec() from MSVC 5's <xtree> (the in-order
// predecessor of a red-black tree node), with the inlined _Max taking its own
// lock, for the std::map<unsigned int, Rect> tree whose _Nil node is
// DAT_0051e598 (_Inc is 0x46ea10). The node colour is at +0x20 (_Red is 0).
#include <yvals.h>

struct Node_0046ff90 {
    Node_0046ff90* left;               // +0x0
    Node_0046ff90* parent;             // +0x4
    Node_0046ff90* right;              // +0x8
    char value[0x20 - 0xc];            // +0xc (key and Rect)
    int color;                         // +0x20
};

extern Node_0046ff90* DAT_0051e598;

static inline Node_0046ff90* Max_0046ff90(Node_0046ff90* p)
{
    std::_Lockit lock;
    while (p->right != DAT_0051e598) {
        p = p->right;
    }
    return p;
}

class Class_0046ff90 {
public:
    Node_0046ff90* ptr;                // +0x0

    void FUN_0046ff90();
};

// FUNCTION: 0x46ff90
void Class_0046ff90::FUN_0046ff90()
{
    std::_Lockit lock;
    if (ptr->color == 0 && ptr->parent->parent == ptr) {
        ptr = ptr->right;
    } else if (ptr->left != DAT_0051e598) {
        ptr = Max_0046ff90(ptr->left);
    } else {
        Node_0046ff90* p;
        while (ptr == (p = ptr->parent)->left) {
            ptr = p;
        }
        ptr = p;
    }
}
