// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Entry_004287d0 {
    void* surface;                     // +0x00
    int* data;                         // +0x04
    char name[0x20];                   // +0x08
};

extern Entry_004287d0 DAT_005120b8[10];

// Looks an entry of the table cleared by FreePictureCache up by name.
// FUNCTION: 0x4287d0
int* __stdcall FindCachedPicturePalette(const char* name)
{
    if (name == 0)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (strcmp(DAT_005120b8[i].name, name) == 0)
            return DAT_005120b8[i].data;
    }
    return 0;
}
