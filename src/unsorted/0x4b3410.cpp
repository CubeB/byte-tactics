// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Buynode(_Nodeptr, _Redbl) from MSVC 5's
// <xtree>: allocates a 0x18-byte node and sets its parent and colour. Its one
// caller (0x4b2850, an inlined insert) sets ecx to the tree, whose _Nil node
// is DAT_0051fbbc. Named as a placeholder method like the rest of that
// tree's helpers (0x4b2fb0, 0x4b33b0, 0x4b3430).

struct Node_004b3410 {
    Node_004b3410* left;               // +0x0
    Node_004b3410* parent;             // +0x4
    Node_004b3410* right;              // +0x8
    char value[0x14 - 0xc];            // +0xc
    int color;                         // +0x14
};

class Class_004b3410 {
public:
    Node_004b3410* FUN_004b3410(Node_004b3410* parent, int color);
};

// FUNCTION: 0x4b3410
Node_004b3410* Class_004b3410::FUN_004b3410(Node_004b3410* parent, int color)
{
    Node_004b3410* node = (Node_004b3410*)operator new(sizeof(Node_004b3410));
    node->parent = parent;
    node->color = color;
    return node;
}
