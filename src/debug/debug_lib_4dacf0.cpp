// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash and
// Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// Commits a block for a pool allocation: rounds the request into need/want,
// takes `want` bytes of reserved address space from the free-block set (the
// allocator's TakeFreeBlock, inlined here), commits `need` of it with
// VirtualAlloc, pads the block and records it in the second set.
#include <windows.h>
#include <set>

extern unsigned int g_lastAllocOffset; // offset the last block was handed out at
extern unsigned int g_freeBlockWraps; // how often the search wrapped around
extern unsigned int g_committedBytes; // bytes committed
extern unsigned int g_committedBytesPeak; // high-water mark of g_committedBytes
extern unsigned int g_allocSerial;

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
    Pair_004db000 value;   // +0xc  the free block: offset, length
    int color;             // +0x14
};

class FreeBlockIter {
public:
    Node_004dacf0* ptr;

    FreeBlockIter() {}
    FreeBlockIter(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const FreeBlockIter& o) const { return ptr == o.ptr; }
    bool operator!=(const FreeBlockIter& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    FreeBlockIter Previous(int); // operator--(int)
    FreeBlockIter Next(int); // operator++(int)
};

class MapInsertResult {
public:
    FreeBlockIter first;
    unsigned char second;
    MapInsertResult() {}
};

class BlockInfo {
public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    BlockInfo(int a, int b, int c, int d, const char* e);
};

class BlockMap { public: MapInsertResult Insert(const BlockInfo& v); };

class FreeBlockMap {
public:
    char unknown_0[4];     // +0x0
    Node_004dacf0* head;   // +0x4
    unsigned char rebuild; // +0x8
    unsigned int count;    // +0xc
    unsigned int total;    // +0x10

    FreeBlockIter begin() { return Begin(); }
    FreeBlockIter end() { return FreeBlockIter(head); }
    unsigned int size() const { return count; }
    unsigned int TakeFreeBlock(unsigned int bytes);
    void AddFreeBlock(Pair_004db000);
    FreeBlockIter lower_bound(const Pair_004db000& k);
    FreeBlockIter Begin();
    FreeBlockIter EraseCopyIter(FreeBlockIter it);
    MapInsertResult InsertOrFind(const Pair_004db000& v);
    bool GrowReservation(unsigned int);
};

CRITICAL_SECTION* GetAllocLock();
unsigned int __cdecl RoundUpToDoublePage(unsigned int size);
unsigned int __cdecl RoundUpToPage(unsigned int size);
FreeBlockMap* GetFreeBlockSet();
BlockMap* GetBlockMap();
void __cdecl CountAlloc(unsigned int size);
char IsBackAlign();
int GetDebugFillPattern();
void __cdecl FillPattern(void* at, int value, unsigned int count);

// The allocator's alloc() (0x4db1c0): find a free block
// of `bytes`, preferring the one the last allocation came from, and hand back
// the leftovers around the request as new free blocks.
// Needs `inline`: MSVC 5 does not inline the recursive member on its own.
inline unsigned int FreeBlockMap::TakeFreeBlock(unsigned int bytes)
{
    if (size() > 0) {
        Pair_004db000 k;
        k.offset = g_lastAllocOffset;
        k.length = 0;
        FreeBlockIter lb = lower_bound(k);
        if (lb != begin()) {
            FreeBlockIter it = lb;
            it.Previous(0);
            if (g_lastAllocOffset >= it->offset && g_lastAllocOffset + bytes <= it->offset + it->length)
                lb = it;
        }
        FreeBlockIter cur = lb;
        // Declared just before the loop, not at the top: it changes how zeros are allocated.
        int tries = 0;
        do {
            if (cur == end()) {
                cur = begin();
                g_lastAllocOffset = 0;
                g_freeBlockWraps++;
                tries++;
            }
            if (cur->length >= bytes) {
                // Copies the free block's value: its fields share one 8-byte local.
                Pair_004db000 b = *cur;
                EraseCopyIter(cur);
                if (g_lastAllocOffset == 0)
                    g_lastAllocOffset = b.offset;
                // The clamp reads g_lastAllocOffset directly; going through mark shifts registers.
                unsigned int mark;
                if (g_lastAllocOffset >= b.offset && g_lastAllocOffset + bytes <= b.offset + b.length)
                    mark = g_lastAllocOffset;
                else
                    mark = b.offset;
                if (mark > b.offset)
                    InsertOrFind(Pair_004db000(b.offset, mark - b.offset));
                unsigned int end = mark + bytes;
                // Length written as offset - mark + length - bytes: the other form is shorter.
                if (end < b.offset + b.length)
                    InsertOrFind(Pair_004db000(end, b.offset - mark + b.length - bytes));
                g_lastAllocOffset = end;
                return mark;
            }
            cur.Next(0);
        } while (tries < 2);
    }
    if (GrowReservation(bytes))
        return TakeFreeBlock(bytes);
    return 0;
}

// FUNCTION: 0x4dacf0
unsigned int __cdecl AllocDebugBlock(unsigned int n, unsigned int arg2)
{
    CRITICAL_SECTION* lock = GetAllocLock();
    EnterCriticalSection(lock);
    unsigned int size = n;
    unsigned int res = 0;
    if (size == 0)
        size = 1;
    unsigned int need = RoundUpToPage(size);
    unsigned int want = RoundUpToDoublePage(size);
    if (want < need) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int base = GetFreeBlockSet()->TakeFreeBlock(want);
    if (base != 0) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT, PAGE_READWRITE);
        if (res == 0) {
            FreeBlockMap* pool = GetFreeBlockSet();
            pool->AddFreeBlock(Pair_004db000(base, want));
        }
    }
    if (res == 0) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int pad = (0 - (n & 0xfff)) & 0xfff;
    if (IsBackAlign()) {
        FillPattern((void*)res, GetDebugFillPattern(), pad);
        res += pad;
    } else {
        FillPattern((void*)(res + n), GetDebugFillPattern(), pad);
    }
    BlockInfo rec(res, n, g_allocSerial, arg2, 0);
    BlockMap* blocks = GetBlockMap();
    blocks->Insert(rec);
    CountAlloc(n);
    g_committedBytes += (n + 0xfff) & 0xfffff000;
    if (g_committedBytes > g_committedBytesPeak)
        g_committedBytesPeak = g_committedBytes;
    LeaveCriticalSection(lock);
    return res;
}
