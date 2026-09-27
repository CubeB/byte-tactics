// Decompiled by Opus. Names are provisional.
// Pool allocator for the DAT_005289e0 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Same shape as 0x4e2b60; 0x4ddce0 has
// an inlined copy for 0x40-byte nodes.
#include <windows.h>

extern void* DAT_005289e0;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

// A method that ignores `this`: its callers (0x4da8d0, 0x4dd430) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x40).
class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

// FUNCTION: 0x4dddf0
void* Class_004dddf0::FUN_004dddf0(unsigned int n)
{
    if (DAT_005289e0 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_005289e0;
            DAT_005289e0 = block;
            block += n;
        }
    }
    void* p = DAT_005289e0;
    DAT_005289e0 = *(void**)DAT_005289e0;
    return p;
}
