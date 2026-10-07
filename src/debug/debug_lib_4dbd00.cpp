// Decompiled by Opus. Names are provisional.
// Set erase(iterator): forwards to the tree's erase (0x4dc130; the tree is the
// set's first member) and converts the tree iterator it returns into the set's
// iterator (the tree's const_iterator).

struct Node_004dbd00;

class Iter_004dbd00 {
public:
    Node_004dbd00* ptr;                // +0x0

    Iter_004dbd00() {}
    Iter_004dbd00(Node_004dbd00* p) : ptr(p) {}
};

class ConstIter_004dbd00 : public Iter_004dbd00 {
public:
    ConstIter_004dbd00() {}
    ConstIter_004dbd00(Node_004dbd00* p) : Iter_004dbd00(p) {}
    ConstIter_004dbd00(const Iter_004dbd00& x) : Iter_004dbd00(x) {}
};

class Class_004dc130 {
public:
    Iter_004dbd00 FUN_004dc130(Iter_004dbd00 it);
};

class Class_004dbd00 {
public:
    Class_004dc130 tree;               // +0x0

    ConstIter_004dbd00 FUN_004dbd00(ConstIter_004dbd00 it);
};

// FUNCTION: 0x4dbd00
ConstIter_004dbd00 Class_004dbd00::FUN_004dbd00(ConstIter_004dbd00 it)
{
    return tree.FUN_004dc130((Iter_004dbd00&)it);
}
