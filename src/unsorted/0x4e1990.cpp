// Decompiled by Opus. Names are provisional.
// Under the global critical section (FUN_004e1ac0), re-inserts a name key
// into the std::set-like tree at +0x0: an existing entry is erased first,
// otherwise the "changed" flag at +0x10 is set. The tree's find() is
// inlined (lower_bound is FUN_004e2580, the key's operator< FUN_004e1a30);
// FUN_004dfea0 is erase(iterator) and FUN_004e2250 is insert(key), both
// returning through a hidden pointer.
#include <windows.h>

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;
};

CritSec_004e1ac0* FUN_004e1ac0();

// The key: a C string ordered by strcmp.
class Class_004e1a30 {
public:
    char* name;                        // +0x0
    bool FUN_004e1a30(const Class_004e1a30& other) const;
};

struct Node_004e1990 {
    Node_004e1990* left;               // +0x0
    Node_004e1990* parent;             // +0x4
    Node_004e1990* right;              // +0x8
    Class_004e1a30 key;                // +0xc
};

class Iter_004e1990 {
public:
    Node_004e1990* ptr;

    Iter_004e1990() {}
    Iter_004e1990(Node_004e1990* p) : ptr(p) {}
    bool operator==(const Iter_004e1990& other) const { return ptr == other.ptr; }
    bool operator!=(const Iter_004e1990& other) const { return !(*this == other); }
};

struct InsertResult_004e1990 {
    Iter_004e1990 first;
    bool second;

    InsertResult_004e1990() {}
};

struct Less_004e1990 {
    bool operator()(const Class_004e1a30& a, const Class_004e1a30& b) const
    {
        return a.FUN_004e1a30(b);
    }
};

class Class_004e2580 {
public:
    Iter_004e1990 FUN_004e2580(const Class_004e1a30& key);
};

class Class_004dfea0 {
public:
    Iter_004e1990 FUN_004dfea0(Iter_004e1990 it);
};

class Class_004e2250 {
public:
    InsertResult_004e1990 FUN_004e2250(const Class_004e1a30& key);
};

class Class_004e1990 {
public:
    Less_004e1990 compare;             // +0x0
    Node_004e1990* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc
    bool changed;                      // +0x10

    Iter_004e1990 End() { return Iter_004e1990(head); }
    Iter_004e1990 Find(const Class_004e1a30& key)
    {
        Iter_004e1990 p = ((Class_004e2580*)this)->FUN_004e2580(key);
        return (p == End() || compare(key, p.ptr->key)) ? End() : p;
    }
    void FUN_004e1990(const Class_004e1a30& key);
};

// FUNCTION: 0x4e1990
void Class_004e1990::FUN_004e1990(const Class_004e1a30& key)
{
    CritSec_004e1ac0* lock = FUN_004e1ac0();
    EnterCriticalSection(&lock->cs);
    Iter_004e1990 it = Find(key);
    if (it != End())
        ((Class_004dfea0*)this)->FUN_004dfea0(it);
    else
        changed = 1;
    ((Class_004e2250*)this)->FUN_004e2250(key);
    LeaveCriticalSection(&lock->cs);
}
