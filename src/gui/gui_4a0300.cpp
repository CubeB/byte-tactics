// Decompiled by Opus. Names are provisional.
// Returns 1 when GUI entry i exists and is named `name` (compare 0x4a0280).
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a0300 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0x15b - 0x12];
};
#pragma pack(pop)

// FUNCTION: 0x4a0300
int __stdcall IsGadgetNamed(Entry_4a0300* entries, int i, char* name)
{
    if (i == -1) {
        return 0;
    }
    return strncmp(entries[i].name, name, 0x10) == 0;
}
