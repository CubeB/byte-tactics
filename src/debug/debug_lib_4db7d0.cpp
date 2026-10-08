// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// The game's free() for its own heap, the partner of malloc() at 0x4dacf0:
// under the allocator lock it finds the block's record in the live-block map,
// fills the slack after (or before) the block with the debug pattern, keeps a
// copy of the record in the debug ring (the last 0x2000 freed blocks), drops
// it from the live map, decommits the pages and gives the reserved range back
// to the free-block set.
#include <windows.h>
#include <vector>

extern unsigned int g_committedBytes; // bytes committed
extern void (*g_outOfMemoryHandler)();

// ---- the live-block map ----------------------------------------------------

// The per-block record; its first dword is the map's key.
class Class_004d8820 {
public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d, const char* e);
};

struct LiveNode {
    LiveNode* left;        // +0x0
    LiveNode* parent;      // +0x4
    LiveNode* right;       // +0x8
    Class_004d8820 value;  // +0xc
};

class Iter_004dce00 {
public:
    LiveNode* ptr;

    Iter_004dce00() {}
    Iter_004dce00(LiveNode* q) : ptr(q) {}
    bool operator==(const Iter_004dce00& o) const { return ptr == o.ptr; }
    Class_004d8820& operator*() const { return ptr->value; }
    Class_004d8820* operator->() const { return &ptr->value; }
};

class Class_004dc910 {
public:
    Iter_004dce00 erase(Iter_004dce00 it);
};

class Class_004dce00 {
public:
    char unknown_0[4];
    LiveNode* head;        // +0x4

    Iter_004dce00 end() { return Iter_004dce00(head); }
    Iter_004dce00 Find(const unsigned int& key); // find
    Iter_004dce00 erase(Iter_004dce00 it) { return ((Class_004dc910*)this)->erase(it); }
};

// ---- the debug arena: a ring of the last 0x2000 freed records ---------------

struct Elem_004dd8c0 {
    unsigned int w[0xc];               // +0x0, 0x30 bytes
};

class Alloc_004dd8c0 {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef Elem_004dd8c0* pointer;
    typedef const Elem_004dd8c0* const_pointer;
    typedef Elem_004dd8c0& reference;
    typedef const Elem_004dd8c0& const_reference;
    typedef Elem_004dd8c0 value_type;

    pointer allocate(size_type _N, const void* = 0)
    {
        pointer _P;
        do {
            _P = (pointer)GlobalAlloc(0, _N * sizeof(value_type));
            if (_P == 0 && g_outOfMemoryHandler != 0)
                g_outOfMemoryHandler();
        } while (_P == 0 && g_outOfMemoryHandler != 0);
        return _P;
    }
    void deallocate(pointer _P, size_type)
    {
        if (_P != 0)
            GlobalFree(_P);
    }
    void construct(pointer _P, const value_type& _V)
    {
        std::_Construct(_P, _V);
    }
    void destroy(pointer) {}
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }
};

struct Arena_004da9f0 {
    std::vector<Elem_004dd8c0, Alloc_004dd8c0> ring; // +0x0
    unsigned int count;                              // +0x10
};

// ---- the allocator's free-block set -----------------------------------------

struct Pair_004db000 {
    unsigned int offset; // +0x0
    unsigned int length; // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004dacf0 {
    Node_004dacf0* left;   // +0x0
    Node_004dacf0* parent; // +0x4
    Node_004dacf0* right;  // +0x8
    Pair_004db000 value;   // +0xc
    int color;             // +0x14
};

class Class_004dbe10 {
public:
    Node_004dacf0* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dbe10& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dbe10 Previous(int); // operator--(int)
};

class Class_004ddbe0 {
public:
    Class_004dbe10 first;
    unsigned char second;
    Class_004ddbe0() {}
};

class Class_004dbd20 { public: Class_004dbe10 UpperBound(const unsigned int& k); };
class Class_004dbeb0 { public: Class_004dbe10 Begin(); };
class Class_004dbd00 { public: Class_004dbe10 Erase(Class_004dbe10 it); };
class Class_004dce60 { public: Class_004ddbe0 InsertOrFind(const Pair_004db000& v); };

class FreeBlockMap {
public:
    char unknown_0[4];     // +0x0
    Node_004dacf0* head;   // +0x4
    unsigned char rebuild; // +0x8
    unsigned int count;    // +0xc
    unsigned int total;    // +0x10

    Class_004dbe10 begin() { return ((Class_004dbeb0*)this)->Begin(); }
    Class_004dbe10 end() { return Class_004dbe10(head); }
    Class_004dbe10 upper_bound(const unsigned int& k)
    {
        return ((Class_004dbd20*)this)->UpperBound(k);
    }
    Class_004dbe10 erase(Class_004dbe10 it) { return ((Class_004dbd00*)this)->Erase(it); }
    Class_004ddbe0 insert(const Pair_004db000& v) { return ((Class_004dce60*)this)->InsertOrFind(v); }

    // 0x4db000: add a free block, merged with the free blocks on either side.
    void AddFreeBlock(Pair_004db000 p)
    {
        Class_004dbe10 it;
        Class_004dbe10 n = upper_bound(p.offset);
        it = n;
        if (it == begin())
            it = end();
        else
            it.Previous(0);
        if (n != end()) {
            if (n->offset == p.offset + p.length) {
                p.length = p.length + n->length;
                erase(n);
            }
        }
        if (it != end()) {
            if (it->length + it->offset == p.offset) {
                p.length = p.length + it->length;
                p.offset = it->offset;
                erase(it);
            }
        }
        insert(p);
    }
};

CRITICAL_SECTION* GetAllocLock();
Class_004dce00* GetBlockMap();
Arena_004da9f0* GetFreedBlockRing();
FreeBlockMap* GetFreeBlockSet();
char IsBackAlign();
int GetDebugFillPattern();
void __cdecl CheckFillPattern(void* at, int value, unsigned int count);
void __cdecl CountFree(unsigned int size);
unsigned int __cdecl RoundUpToPage(unsigned int size);
unsigned int __cdecl RoundUpToDoublePage(unsigned int size);

// FUNCTION: 0x4db7d0
void __cdecl FreeDebugBlock(void* p, int flags)
{
    if (p == 0)
        return;
    CRITICAL_SECTION* lock = GetAllocLock();
    EnterCriticalSection(lock);
    Class_004dce00* live = GetBlockMap();
    Class_004d8820 rec((unsigned int)p, 0, 0, 0, 0);
    Iter_004dce00 it = live->Find(rec.base);
    if (it == GetBlockMap()->end()) {
        LeaveCriticalSection(lock);
        return;
    }
    unsigned int size = it->size;
    unsigned int pad = (0 - (size & 0xfff)) & 0xfff;
    if (IsBackAlign())
        CheckFillPattern((char*)p - pad, GetDebugFillPattern(), pad);
    else
        CheckFillPattern((char*)p + size, GetDebugFillPattern(), pad);
    Elem_004dd8c0* old = (Elem_004dd8c0*)&*it;
    Arena_004da9f0* arena = GetFreedBlockRing();
    if (arena->count < 0x2000)
        arena->ring.push_back(*old);
    else
        arena->ring[arena->count & 0x1fff] = *old;
    arena->count++;
    GetBlockMap()->erase(it);
    CountFree(size);
    g_committedBytes -= (size + 0xfff) & 0xfffff000;
    p = (void*)((unsigned int)p & 0xfffff000);
    if (size == 0)
        size = 1;
    unsigned int commit = RoundUpToPage(size);
    VirtualFree(p, commit, MEM_DECOMMIT);
    unsigned int reserve = RoundUpToDoublePage(size);
    FreeBlockMap* blocks = GetFreeBlockSet();
    blocks->AddFreeBlock(Pair_004db000((unsigned int)p, reserve));
    LeaveCriticalSection(lock);
}
