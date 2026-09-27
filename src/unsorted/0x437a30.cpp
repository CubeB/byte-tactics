// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Memory cache allocation: grows or reuses the handle in *p, splitting the
// block found at `cur` (or wrapping to the base) into an allocated chunk plus
// a leftover free chunk. Chunks are {owner handle, size} and the handle points
// just past the 8-byte header.
// <windows.h> decides two operand orders: `lea ebp,[ebx+eax]` (size+cur, not
// cur+size) and the wrap-loop reload of this->base before this->cap.
#include <windows.h>

struct Chunk_00437a30 {
    void** owner;                      // +0x0
    int size;                          // +0x4
};

class Class_00437a30 {
public:
    int cap;                           // +0x0
    int base;                          // +0x4
    int cur;                           // +0x8

    int FUN_00437a30(void** p, int need);
};

// FUNCTION: 0x437a30
int Class_00437a30::FUN_00437a30(void** p, int need)
{
    int total = 0;
    if (need <= 0) {
        *p = 0;
        return 0;
    }
    int size = need + 8;
    int existing = (int)*p;
    if (existing != 0)
        existing = *(int*)(existing - 4);
    if (existing >= size)
        return 1;
    if (size > cap) {
        *p = 0;
        return 0;
    }
    int start;
    int end = base + cap;
    if (cur + size > end) {
        int q = cur;
        start = base;
        while (q < end) {
            void** owner = *(void***)q;
            if (owner != 0 && (int)*owner == q + 8)
                *owner = 0;
            q += *(int*)(q + 4);
            end = base + cap;
        }
    } else {
        start = cur;
    }
    int q = start;
    while (total < size) {
        total += *(int*)(q + 4);
        void** owner = *(void***)q;
        if (owner != 0 && (int)*owner == q + 8)
            *owner = 0;
        q += *(int*)(q + 4);
    }
    if (total - size > 8u) {
        Chunk_00437a30* nc = (Chunk_00437a30*)(start + size);
        nc->owner = 0;
        nc->size = total - size;
        cur = (int)nc;
    } else {
        size = total;
        int e = start + total;
        if (e >= base + cap)
            e = base;
        cur = e;
    }
    ((Chunk_00437a30*)start)->owner = p;
    ((Chunk_00437a30*)start)->size = size;
    *p = (void*)(start + 8);
    return 1;
}
