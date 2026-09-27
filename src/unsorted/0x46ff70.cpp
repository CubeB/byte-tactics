// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Buynode(_Nodeptr, _Redbl) from MSVC 5's
// <xtree> for the std::map<unsigned int, Rect> tree whose _Nil node is
// DAT_0051e598: allocates a 0x24-byte node and sets its parent and colour.
// Its one caller (0x46ef50, an inlined insert) sets ecx to the tree. Named
// as a placeholder method like the rest of that tree's helpers (0x46fe60,
// 0x46feb0, 0x46ff10, 0x46ff90).

struct Node_0046ff70 {
    Node_0046ff70* left;               // +0x0
    Node_0046ff70* parent;             // +0x4
    Node_0046ff70* right;              // +0x8
    char value[0x20 - 0xc];            // +0xc (key and Rect)
    int color;                         // +0x20
};

class Class_0046ff70 {
public:
    Node_0046ff70* FUN_0046ff70(Node_0046ff70* parent, int color);
};

// FUNCTION: 0x46ff70
Node_0046ff70* Class_0046ff70::FUN_0046ff70(Node_0046ff70* parent, int color)
{
    Node_0046ff70* node = (Node_0046ff70*)operator new(sizeof(Node_0046ff70));
    node->parent = parent;
    node->color = color;
    return node;
}
