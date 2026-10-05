// Decompiled by Opus. Names are provisional.
// Copies the 16-character name of entry `index` (0x15b-byte entries, see
// 0x49ff10) into `name` and terminates it.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0049fed0 {
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0x15b - 0x12];
};
#pragma pack(pop)

// FUNCTION: 0x49fed0
void __stdcall GetGadgetName(Entry_0049fed0* entries, char* name, int index)
{
    strncpy(name, entries[index].name, 0x10);
    name[0x10] = 0;
}
