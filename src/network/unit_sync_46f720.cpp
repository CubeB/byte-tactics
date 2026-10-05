// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::_Init() from MSVC 5's <xtree>, written out by hand:
// DAT_0051e598 is the tree's shared _Nil node and DAT_0051e59c its
// reference count (_Nilrefs). Nodes are 0x24 bytes (a 0x14-byte value).
#include <yvals.h>

struct Node_0046f720 {
    Node_0046f720* left;               // +0x0
    Node_0046f720* parent;             // +0x4
    Node_0046f720* right;              // +0x8
    char value[0x14];                  // +0xc
    int color;                         // +0x20 (0 red, 1 black)
};

extern Node_0046f720* DAT_0051e598;
extern int DAT_0051e59c;

class Class_0046f720 {
public:
    int unknown_0;
    Node_0046f720* head;               // +0x4
    int multi;                         // +0x8
    int size;                          // +0xc

    Node_0046f720* Buynode(Node_0046f720* parent, int color)
    {
        Node_0046f720* s = (Node_0046f720*)operator new(sizeof(Node_0046f720));
        s->parent = parent;
        s->color = color;
        return s;
    }
    void FUN_0046f720();
};

// FUNCTION: 0x46f720
void Class_0046f720::FUN_0046f720()
{
    std::_Lockit lock;
    if (DAT_0051e598 == 0) {
        DAT_0051e598 = Buynode(0, 1);
        DAT_0051e598->left = 0, DAT_0051e598->right = 0;
    }
    ++DAT_0051e59c;
    head = Buynode(DAT_0051e598, 0), size = 0;
    head->left = head, head->right = head;
}
