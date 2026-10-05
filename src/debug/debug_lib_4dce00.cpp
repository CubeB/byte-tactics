// Decompiled by Opus. Names are provisional.
// std::map<unsigned int, ...>::find() from MSVC 5's <xtree>, written out by
// hand: FUN_004ddc90 is the tree's _Lbound(), and a missing key yields the
// head node (end()). The iterator is returned by value through a hidden
// pointer.

struct Node_004dce00 {
    Node_004dce00* left;               // +0x0
    Node_004dce00* parent;             // +0x4
    Node_004dce00* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Iter_004dce00 {
public:
    Node_004dce00* ptr;

    Iter_004dce00() {}
    Iter_004dce00(Node_004dce00* p) : ptr(p) {}
    bool operator==(const Iter_004dce00& other) const { return ptr == other.ptr; }
};

struct Less_004dce00 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_004ddc90 {
public:
    Node_004dce00* FUN_004ddc90(const unsigned int& key);
};

class Class_004dce00 {
public:
    Less_004dce00 compare;             // +0x0
    Node_004dce00* head;               // +0x4

    Iter_004dce00 End() { return Iter_004dce00(head); }
    Iter_004dce00 FUN_004dce00(const unsigned int& key);
};

// FUNCTION: 0x4dce00
Iter_004dce00 Class_004dce00::FUN_004dce00(const unsigned int& key)
{
    Iter_004dce00 p = Iter_004dce00(((Class_004ddc90*)this)->FUN_004ddc90(key));
    return (p == End() || compare(key, p.ptr->key)) ? End() : p;
}
