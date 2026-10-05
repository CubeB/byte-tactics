// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

// Memory block description, 0x30 bytes. Same layout as the type used by
// FormatBlockInfo and FindBlocksAroundAddress.
class BlockInfo {
public:
    int field_0;                 // +0x00
    int field_4;                 // +0x04
    int field_8;                 // +0x08
    char unknown_c[0x24];        // +0x0c
    BlockInfo(void);
};

extern void __cdecl FindBlocksAroundAddress(unsigned int address, BlockInfo* a, BlockInfo* b);
extern void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);

// FUNCTION: 0x4dab10
void __cdecl DescribeAddress(unsigned int address, char* buf, int unused)
{
    BlockInfo local1;
    BlockInfo local2;
    FindBlocksAroundAddress(address, &local1, &local2);
    if (local1.field_0 != 0) {
        bool inRange = address >= (unsigned int)local1.field_0
                    && address < (unsigned int)local1.field_4 + (unsigned int)local1.field_0;
        if (inRange) {
            FormatBlockInfo(local1, buf, unused);
            return;
        }
    }
    strcpy(buf, "Address is not within an allocated block.");
}
