// Decompiled by Opus. Names are provisional.
// Under the global critical section (GetNameTableLock), re-inserts a name key
// into the std::set-like tree at +0x0: an existing entry is erased first,
// otherwise the "changed" flag at +0x10 is set. Erase is
// erase(iterator) and InsertOrFind is insert(key), both returning through a
// hidden pointer.
#include <windows.h>

class CritSec_004e1ac0 {
public:
    CRITICAL_SECTION cs;
};

CritSec_004e1ac0* GetNameTableLock();

// The key: a C string ordered by strcmp.
class NameKey {
public:
    char* name;                        // +0x0
    bool LessThan(const NameKey& other) const;
};

struct Node_004e1990 {
    Node_004e1990* left;               // +0x0
    Node_004e1990* parent;             // +0x4
    Node_004e1990* right;              // +0x8
    NameKey key;                       // +0xc
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
    bool operator()(const NameKey& a, const NameKey& b) const
    {
        return a.LessThan(b);
    }
};

class NameMapTree {
public:
    Iter_004e1990 LowerBound(const NameKey& key);
    Iter_004e1990 Erase(Iter_004e1990 it);
    InsertResult_004e1990 InsertOrFind(const NameKey& key);
};

class NameTable : public NameMapTree {
public:
    Less_004e1990 compare;             // +0x0
    Node_004e1990* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc
    bool changed;                      // +0x10

    Iter_004e1990 End() { return Iter_004e1990(head); }
    Iter_004e1990 Find(const NameKey& key)
    {
        Iter_004e1990 p = LowerBound(key);
        return (p == End() || compare(key, p.ptr->key)) ? End() : p;
    }
    void Upsert(const NameKey& key);
};

// FUNCTION: 0x4e1990
void NameTable::Upsert(const NameKey& key)
{
    CritSec_004e1ac0* lock = GetNameTableLock();
    EnterCriticalSection(&lock->cs);
    Iter_004e1990 it = Find(key);
    if (it != End())
        Erase(it);
    else
        changed = 1;
    InsertOrFind(key);
    LeaveCriticalSection(&lock->cs);
}
