// Decompiled by Opus. Names are provisional.
// Returns whether the name of menu entry `index` contains `text`.
#include <string.h>

struct Entry_0045ba60;

void __stdcall GetGadgetName(Entry_0045ba60* entries, char* name, int index);

// FUNCTION: 0x419630
int __stdcall MenuEntryNameContains(Entry_0045ba60* entries, char* text, int index)
{
    char name[32];
    GetGadgetName(entries, name, index);
    return strstr(name, text) != 0;
}
