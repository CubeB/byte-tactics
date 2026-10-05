// Decompiled by Opus. Names are provisional.
#include <stdio.h>

// Memory block description, passed by value (0x30 bytes).
struct BlockInfo_004d8790 {
    void* address;                   // +0x00
    long size;                       // +0x04
    int allocNumber;                 // +0x08
    char name[0x24];                 // +0x0c
};

// FUNCTION: 0x4d8790
void __cdecl FormatBlockInfo(BlockInfo_004d8790 info, char* buf, int unused)
{
    if (info.name[0] != 0) {
        sprintf(buf, "\tBlock info: %ld bytes at %08lX - Alloc #%d - '%s'",
                info.size, info.address, info.allocNumber, info.name);
        return;
    }
    sprintf(buf, "\tBlock info: %ld bytes at %08lX - Alloc #%d",
            info.size, info.address, info.allocNumber);
}
