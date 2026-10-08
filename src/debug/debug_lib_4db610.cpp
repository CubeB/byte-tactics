// Decompiled by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// FreeBlockMap is a std::map<unsigned int, int> of free blocks
// (keyed by base offset, valued by length) whose allocator hands out its 0x18
// byte nodes from the pool at 0x4ddd70, plus a running total of reserved bytes
// at +0x10.
//
// The constructor body reserves up to four address-space regions of 256 MB,
// halving the size (down to 64 KB) whenever VirtualAlloc fails or returns a
// block that ends above 2 GB, and records each one with the map's insert
// (0x4db000, taking the 8-byte pair by value).
#include <map>
#include <windows.h>

class FreeBlockAllocator {
public:
    void* Allocate(unsigned int n);
};

// The map's allocator: std::allocator whose node allocation (_Charalloc, the
// hook <xtree>'s _Buynode uses) is the pool at 0x4ddd70.
class PoolAlloc_004db610 : public std::allocator<int> {
public:
    char* _Charalloc(size_t n)
    {
        return (char*)((FreeBlockAllocator*)this)->Allocate(n);
    }
};

struct Pair_004db610 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

typedef std::map<unsigned int, int, std::less<unsigned int>, PoolAlloc_004db610>
    Map_004db610;

class FreeBlockMap {
public:
    Map_004db610 freeMap;              // +0x0
    int total;                         // +0x10

    void AddFreeBlock(Pair_004db610 p);

    void* operator new(size_t n) { return GlobalAlloc(0, n); }

    FreeBlockMap() : total(0)
    {
        int n = 4;
        do {
            unsigned int len = 0x10000000;
            void* m = VirtualAlloc(0, len + 0x2000, MEM_RESERVE, PAGE_READWRITE);
            for (;;) {
                if (m != 0 && (unsigned int)m + len <= 0x80000000u)
                    break;
                if (m != 0)
                    VirtualFree(m, len + 0x2000, MEM_RELEASE);
                len = (len >> 1) & 0x7fffe000;
                if (len < 0x10000)
                    goto next;
                m = VirtualAlloc(0, len + 0x2000, MEM_RESERVE, PAGE_READWRITE);
            }
            total = total + len;
            Pair_004db610 pair;
            pair.offset = (unsigned int)m;
            pair.length = len;
            AddFreeBlock(pair);
        next:
            ;
        } while (--n);
    }
};

extern FreeBlockMap* g_freeBlockSet;

// FUNCTION: 0x4db610
FreeBlockMap* GetFreeBlockSet()
{
    if (g_freeBlockSet == 0)
        g_freeBlockSet = new FreeBlockMap;
    return g_freeBlockSet;
}