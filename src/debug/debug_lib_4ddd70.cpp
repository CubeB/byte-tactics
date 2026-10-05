// Decompiled by Opus. Names are provisional.
// Pool allocator for the DAT_00528a10 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Identical to 0x4dddf0 apart from the
// free list.
#include <windows.h>

extern void* DAT_00528a10;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

// A method that ignores `this`: its callers (0x4db610, 0x4dce60) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x18).
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// FUNCTION: 0x4ddd70
void* Class_004ddd70::FUN_004ddd70(unsigned int n)
{
    if (DAT_00528a10 == 0) {
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
            *(void**)block = DAT_00528a10;
            DAT_00528a10 = block;
            block += n;
        }
    }
    void* p = DAT_00528a10;
    DAT_00528a10 = *(void**)DAT_00528a10;
    return p;
}
