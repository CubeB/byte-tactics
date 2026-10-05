// Decompiled by deepseek-v4.1. Names are provisional.
// Sibling of 0x4dab10: under the allocator lock, walks the 0x30-byte block
// records from the container's last element down to its first and appends the
// info line of every block whose range covers `address`, separating records
// with a newline. `found` shares the dead `buf` argument home and the walk is
// one `&&` loop condition: `p != end && n > 100` gives the original's single
// top test plus the bottom pointer test, where a do/while with a break
// duplicates the size test.
#include <windows.h>
#include <string.h>

class BlockInfo {
public:
    int field_0;                 // +0x00
    int field_4;                 // +0x04
    int field_8;                 // +0x08
    char unknown_c[0x24];        // +0x0c
    BlockInfo(void);
};

struct Container_004da9f0 {
    int field_0;                 // +0x0
    void* field_4;               // +0x4
    void* field_8;               // +0x8
};

extern Container_004da9f0* GetFreedBlockRing();
extern char IsMemFussy();
extern CRITICAL_SECTION* FUN_004da780();
extern void __cdecl FormatBlockInfo(BlockInfo info, char* buf, int unused);

// FUNCTION: 0x4dabb0
char __cdecl FUN_004dabb0(unsigned int address, char* buf, unsigned int n)
{
    CRITICAL_SECTION* cs = FUN_004da780();
    EnterCriticalSection(cs);
    *buf = 0;
    if (IsMemFussy()) {
        BlockInfo* p = (BlockInfo*)GetFreedBlockRing()->field_8;
        char found = 0;
        while (p != (BlockInfo*)GetFreedBlockRing()->field_4 && n > 100) {
            p = (BlockInfo*)((char*)p - 0x30);
            BlockInfo info = *p;
            bool inRange = address >= (unsigned int)info.field_0
                        && address < (unsigned int)info.field_4 + (unsigned int)info.field_0;
            if (inRange) {
                if (found)
                    strcat(buf, "\n");
                FormatBlockInfo(info, buf + strlen(buf), n);
                unsigned int len = strlen(buf);
                found = 1;
                buf += len;
                n -= len;
            }
        }
        LeaveCriticalSection(cs);
        return found;
    }
    LeaveCriticalSection(cs);
    return 0;
}
