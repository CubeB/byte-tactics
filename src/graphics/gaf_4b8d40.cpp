// Decompiled by Opus. Names are provisional.
// Looks up a GAF entry by name (case-insensitive); returns 0 when the file is
// missing or no entry has that name.
#include <string.h>

struct GafEntry_004b8d40 {
    char unknown_0[8];
    char name[1];                      // +0x8
};

struct Gaf_004b8d40 {
    char unknown_0[4];
    short count;                       // +0x4
    char unknown_6[6];
    GafEntry_004b8d40* entries[1];     // +0xc
};

// FUNCTION: 0x4b8d40
GafEntry_004b8d40* __stdcall FindGafEntry(Gaf_004b8d40* gaf, const char* name)
{
    if (gaf) {
        GafEntry_004b8d40** p = gaf->entries;
        for (int i = 0; i < gaf->count; i++, p++) {
            if (_strcmpi((*p)->name, name) == 0)
                return *p;
        }
    }
    return 0;
}
