// Decompiled by Opus. Names are provisional.
// Looks up the unit's entry in a std::map<unsigned int, Rect> (inlined
// find(), as in 0x46e330.cpp), clears its width, has NotifyEntryChanged recompute
// it, and returns whether the entry now has a non-empty size.

struct Rect_0046e450 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

struct Node_0046e450 {
    Node_0046e450* left;               // +0x0
    Node_0046e450* parent;             // +0x4
    Node_0046e450* right;              // +0x8
    unsigned int key;                  // +0xc
    Rect_0046e450 value;               // +0x10
};

class Iter_0046e450 {
public:
    Node_0046e450* ptr;

    Iter_0046e450() {}
    Iter_0046e450(Node_0046e450* p) : ptr(p) {}
    bool operator==(const Iter_0046e450& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e450 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};
#pragma pack(pop)

struct Less_0046e450 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e450* FUN_0046fe60(const unsigned int* key);
};

class UnitSync {
public:
    Less_0046e450 compare;
    Node_0046e450* head;               // +0x4

    Iter_0046e450 End() { return Iter_0046e450(head); }
    Iter_0046e450 Find(const unsigned int* key)
    {
        Iter_0046e450 p = Iter_0046e450(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    int DisallowUnit(Unit_0046e450* unit);
    void NotifyEntryChanged(unsigned int key);
};

// FUNCTION: 0x46e450
int UnitSync::DisallowUnit(Unit_0046e450* unit)
{
    Node_0046e450* n = Find(&unit->key).ptr;
    n->value.w = 0;
    ((UnitSync*)this)->NotifyEntryChanged(unit->key);
    return n->value.w != 0 && n->value.h != 0;
}
