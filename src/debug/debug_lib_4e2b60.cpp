// Decompiled by Opus. Names are provisional.
// Same allocator as 0x4dddf0 for a second free list.
// Pool allocator for the DAT_00529e58 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece.
#include <windows.h>

extern void* DAT_00529e58;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

// A method that ignores `this`: its callers (0x4e17c0, 0x4e2620) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x208).
class Class_004e2b60 {
public:
    void* FUN_004e2b60(unsigned int n);
};

// FUNCTION: 0x4e2b60
void* Class_004e2b60::FUN_004e2b60(unsigned int n)
{
    if (DAT_00529e58 == 0) {
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
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)DAT_00529e58;
    return p;
}
