// Decompiled by space-bunny-free. Names are provisional.
// Shaped like std::_Tree<...>::iterator::_Dec() from MSVC 5's <XTREE> (the
// toolchain's own copy is in toolchain/msvc5-sp3/INCLUDE/XTREE): moves the
// iterator's node pointer (at +0) to the previous node in order, under a
// std::_Lockit. DAT_005292c4 is the tree's _Nil node and the node colour sits
// at +0x204 (see the _Buynode copy, 0x4e2a30). The _Max helper takes its own
// lock, the mirror of _Min (0x4e04e0) inside _Inc (0x4e0450).
// The accessors return references, as _REFERENCE_X does in <XTREE>; that is
// what puts the node pointer itself in eax and makes the parent walk re-read
// the member instead of reusing the loaded value.
#include <yvals.h>

enum Redbl_004e2ab0 { _Red, _Black };

struct Node_004e2ab0 {
    Node_004e2ab0* left;               // +0x0
    Node_004e2ab0* parent;             // +0x4
    Node_004e2ab0* right;              // +0x8
    char unknown_c[0x204 - 0xc];       // the key/value payload
    Redbl_004e2ab0 colour;             // +0x204
};

extern Node_004e2ab0* DAT_005292c4;

static inline Redbl_004e2ab0& Colour(Node_004e2ab0* p) { return p->colour; }
static inline Node_004e2ab0*& Left(Node_004e2ab0* p) { return p->left; }
static inline Node_004e2ab0*& Parent(Node_004e2ab0* p) { return p->parent; }
static inline Node_004e2ab0*& Right(Node_004e2ab0* p) { return p->right; }

static inline Node_004e2ab0* Max(Node_004e2ab0* p)
{
    std::_Lockit lock;
    while (Right(p) != DAT_005292c4)
        p = Right(p);
    return p;
}

class Class_004e2ab0 {
public:
    Node_004e2ab0* ptr;                // +0x0

    void FUN_004e2ab0();
};

// FUNCTION: 0x4e2ab0
void Class_004e2ab0::FUN_004e2ab0()
{
    std::_Lockit lock;
    if (Colour(ptr) == _Red && Parent(Parent(ptr)) == ptr)
        ptr = Right(ptr);
    else if (Left(ptr) != DAT_005292c4)
        ptr = Max(Left(ptr));
    else {
        Node_004e2ab0* p;
        while (ptr == Left(p = Parent(ptr)))
            ptr = p;
        ptr = p;
    }
}
