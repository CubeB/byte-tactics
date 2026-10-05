// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Dequeues the front event of an insertion-ordered queue: the class holds a
// std::map<unsigned int, Event> (key at map node +0xc, Event at +0x10) for
// lookup and a std::list<Event> at +0x24 (_Head +0x24, _Size +0x28) for the
// order. The list front's first field is the map key. The inlined find() is
// copied from 0x46e330 (FUN_0046fe60 is the tree's lower_bound(); a missing
// key yields the head node, end()).
#include <list>
#include <map>

struct Event_0046e280 {                // 0x10 bytes
    unsigned int key;                  // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
};

struct Node_0046e280 {
    Node_0046e280* left;               // +0x0
    Node_0046e280* parent;             // +0x4
    Node_0046e280* right;              // +0x8
    unsigned int key;                  // +0xc
    Event_0046e280 value;              // +0x10
};

class Iter_0046e280 {
public:
    Node_0046e280* ptr;

    Iter_0046e280() {}
    Iter_0046e280(Node_0046e280* p) : ptr(p) {}
    bool operator==(const Iter_0046e280& other) const { return ptr == other.ptr; }
};

struct Less_0046e280 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class Class_0046fe60 {
public:
    Node_0046e280* FUN_0046fe60(const unsigned int* key);
};

class Map_0046e280 {
public:
    Less_0046e280 compare;
    Node_0046e280* head;               // +0x4

    Iter_0046e280 End() { return Iter_0046e280(head); }
    Iter_0046e280 Find(const unsigned int* key)
    {
        Iter_0046e280 p = Iter_0046e280(((Class_0046fe60*)this)->FUN_0046fe60(key));
        return (p == End() || compare(*key, p.ptr->key)) ? End() : p;
    }
};

class UnitSync {
public:
    std::map<unsigned int, Event_0046e280> map;   // +0x00
    char unknown_14[0x10];                        // +0x14
    std::list<Event_0046e280> queue;              // +0x24
    int PopChangedEntry(Event_0046e280* out);
};

// FUNCTION: 0x46e280
int UnitSync::PopChangedEntry(Event_0046e280* out)
{
    if (queue.empty())
        return 0;
    Iter_0046e280 p = ((Map_0046e280*)&map)->Find(&queue.front().key);
    *out = p.ptr->value;
    queue.pop_front();
    return 1;
}
