// Decompiled by Opus. Names are provisional.
// Sets the last field of the unit's entry in a std::map<unsigned int, Rect>
// (the find() is inlined as in 0x46e330, whose declarations this copies) and
// then calls FUN_0046d860 with the unit's key. FUN_0046fe60 is the tree's
// lower_bound(), and a missing key yields the head node (end()).

struct Rect_0046e330 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

struct Node_0046e330 {
    Node_0046e330* left;               // +0x0
    Node_0046e330* parent;             // +0x4
    Node_0046e330* right;              // +0x8
    unsigned int key;                  // +0xc
    Rect_0046e330 value;               // +0x10
};

class Iter_0046e330 {
public:
    Node_0046e330* ptr;

    Iter_0046e330() {}
    Iter_0046e330(Node_0046e330* p) : ptr(p) {}
    bool operator==(const Iter_0046e330& other) const { return ptr == other.ptr; }
};

#pragma pack(push, 1)
struct Unit_0046e330 {
    char unknown_0[0x13e];
    unsigned int key;                  // +0x13e
};
#pragma pack(pop)

struct Less_0046e330 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e330* FUN_0046fe60(const unsigned int* key);
};

class Class_0046d860 {
public:
    void FUN_0046d860(unsigned int key);
};

class Class_0046e330 {
public:
    Less_0046e330 compare;
    Node_0046e330* head;               // +0x4

    Iter_0046e330 End() { return Iter_0046e330(head); }
    Iter_0046e330 Find(const unsigned int* key)
    {
        Iter_0046e330 p = Iter_0046e330(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
    void FUN_0046e550(Unit_0046e330* unit, int value);
};


// FUNCTION: 0x46e550
void Class_0046e330::FUN_0046e550(Unit_0046e330* unit, int value)
{
    Iter_0046e330 it = Find(&unit->key);
    if (!(it == End())) {
        it.ptr->value.unknown_c = value;
        ((Class_0046d860*)this)->FUN_0046d860(unit->key);
    }
}
