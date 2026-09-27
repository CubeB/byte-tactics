// Decompiled by Opus. Names are provisional.
// Like 0x46e450, but sets the entry's width to 1 before FUN_0046d860
// recomputes it; returns whether the entry now has a non-empty size.

struct Rect_0046e4d0 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

struct Node_0046e4d0 {
    Node_0046e4d0* left;               // +0x0
    Node_0046e4d0* parent;             // +0x4
    Node_0046e4d0* right;              // +0x8
    unsigned int key;                  // +0xc
    Rect_0046e4d0 value;               // +0x10
};

class Iter_0046e4d0 {
public:
    Node_0046e4d0* ptr;

    Iter_0046e4d0() {}
    Iter_0046e4d0(Node_0046e4d0* p) : ptr(p) {}
    bool operator==(const Iter_0046e4d0& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e4d0 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};
#pragma pack(pop)

struct Less_0046e4d0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e4d0* FUN_0046fe60(const unsigned int* key);
};

class Class_0046d860 {
public:
    void FUN_0046d860(unsigned int key);
};

class Class_0046e4d0 {
public:
    Less_0046e4d0 compare;
    Node_0046e4d0* head;               // +0x4

    Iter_0046e4d0 End() { return Iter_0046e4d0(head); }
    Iter_0046e4d0 Find(const unsigned int* key)
    {
        Iter_0046e4d0 p = Iter_0046e4d0(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    int FUN_0046e4d0(Unit_0046e4d0* unit);
};

// FUNCTION: 0x46e4d0
int Class_0046e4d0::FUN_0046e4d0(Unit_0046e4d0* unit)
{
    Node_0046e4d0* n = Find(&unit->key).ptr;
    n->value.w = 1;
    ((Class_0046d860*)this)->FUN_0046d860(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}
