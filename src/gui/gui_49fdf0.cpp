// Decompiled by Opus. Names are provisional.
// Finds the gadget entry (from 1) whose name matches; returns its index or
// -1. The gadget type argument is not used (compare 0x49ff10).
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049fdf0 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x49fdf0
int __stdcall FindGadgetIndex(Entry_0049fdf0* entries, char* name, int type)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}
