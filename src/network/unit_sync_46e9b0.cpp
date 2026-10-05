// Decompiled by Opus. Names are provisional.
// An out-of-line std::map<unsigned int, ...>::find() from MSVC 5's <xtree>:
// FUN_0046fe60 is the tree's lower_bound(), and a missing key yields the
// head node (end()).

struct Node_0046e9b0 {
    Node_0046e9b0* left;               // +0x0
    Node_0046e9b0* parent;             // +0x4
    Node_0046e9b0* right;              // +0x8
    unsigned int key;                  // +0xc
};

class Iter_0046e9b0 {
public:
    Node_0046e9b0* ptr;

    Iter_0046e9b0() {}
    Iter_0046e9b0(Node_0046e9b0* p) : ptr(p) {}
    bool operator==(const Iter_0046e9b0& other) const { return ptr == other.ptr; }
};

struct Less_0046e9b0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e9b0* FUN_0046fe60(const unsigned int* key);
};

class Class_0046e9b0 {
public:
    Less_0046e9b0 compare;
    Node_0046e9b0* head;               // +0x4

    Iter_0046e9b0 End() { return Iter_0046e9b0(head); }
    Iter_0046e9b0 FUN_0046e9b0(const unsigned int& key);
};

// FUNCTION: 0x46e9b0
Iter_0046e9b0 Class_0046e9b0::FUN_0046e9b0(const unsigned int& key)
{
    Iter_0046e9b0 p = Iter_0046e9b0(((Class_0046fe60*)this)->FUN_0046fe60(&key));
    return (p == End() || compare(key, p.ptr->key)) ? End() : p;
}
