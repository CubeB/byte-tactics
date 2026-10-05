// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049fe60 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// Like the lookup in 0x49ff10, but matches entries whose name contains the
// given text; returns the entry index or -1.
// FUNCTION: 0x49fe60
int __stdcall FindGadgetIndexBySubstring(Entry_0049fe60* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strstr(entries[i].name, name)) {
            return i;
        }
    }
    return -1;
}
