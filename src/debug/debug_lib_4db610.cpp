// Decompiled by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// The allocator singleton getter, with the constructor of the class it builds
// inlined. Class_004db000 is a std::map<unsigned int, int> of free blocks
// (keyed by base offset, valued by length) whose allocator hands out its 0x18
// byte nodes from the pool at 0x4ddd70, plus a running total of reserved bytes
// at +0x10. The constructor is the real std::map one: the two one-byte loads
// from uninitialised stack slots at the start are the default `_Pr()` and
// `_A()` temporaries it copies into the empty key_compare and allocator
// members (no volatile needed), and the lock block is `_Tree::_Init()`
// creating the shared _Nil node (DAT_00528a54, counted in DAT_00528a58) and the
// head node. The class's own operator new is GlobalAlloc, which is why the
// constructor is skipped when it fails and why both exits store the result
// through eax: `DAT_00528a40 = new Class_004db000;` is a conditional
// expression yielding the pointer or 0.
//
// The constructor body reserves up to four address-space regions of 256 MB,
// halving the size (down to 64 KB) whenever VirtualAlloc fails or returns a
// block that ends above 2 GB, and records each one with the map's insert
// (0x4db000, taking the 8-byte pair by value).
//
// check.py shows std::_Tree<...>::_Nil and ::_Nilrefs (the real template's
// statics) at 0x528a54 and 0x528a58, the addresses symbols.csv calls
// DAT_00528a54 and DAT_00528a58; the tree template's real names for them
// belong in data/symbols.csv.
#include <map>
#include <windows.h>

class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// The map's allocator: std::allocator whose node allocation (_Charalloc, the
// hook <xtree>'s _Buynode uses) is the pool at 0x4ddd70.
class PoolAlloc_004db610 : public std::allocator<int> {
public:
    char* _Charalloc(size_t n)
    {
        return (char*)((Class_004ddd70*)this)->FUN_004ddd70(n);
    }
};

struct Pair_004db610 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

typedef std::map<unsigned int, int, std::less<unsigned int>, PoolAlloc_004db610>
    Map_004db610;

class Class_004db000 {
public:
    Map_004db610 freeMap;              // +0x0
    int total;                         // +0x10

    void FUN_004db000(Pair_004db610 p);

    void* operator new(size_t n) { return GlobalAlloc(0, n); }

    Class_004db000() : total(0)
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
            FUN_004db000(pair);
        next:
            ;
        } while (--n);
    }
};

extern Class_004db000* DAT_00528a40;

// FUNCTION: 0x4db610
Class_004db000* FUN_004db610()
{
    if (DAT_00528a40 == 0)
        DAT_00528a40 = new Class_004db000;
    return DAT_00528a40;
}