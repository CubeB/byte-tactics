// Decompiled by Opus. Names are provisional.
// Returns whether the name of menu entry `index` contains `text`.
#include <string.h>

struct Entry_0045ba60;

void __stdcall FUN_0049fed0(Entry_0045ba60* entries, char* name, int index);

// FUNCTION: 0x419630
int __stdcall FUN_00419630(Entry_0045ba60* entries, char* text, int index)
{
    char name[32];
    FUN_0049fed0(entries, name, index);
    return strstr(name, text) != 0;
}
