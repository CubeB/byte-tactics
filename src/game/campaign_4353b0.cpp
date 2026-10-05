// Decompiled by Opus. Names are provisional.
// Stores a name in slot `index`; for slot 1 also records whether a file of
// that name exists (HAPI_FileLengthByName), or 0 when the name is empty.
#include <string.h>

int __stdcall HAPI_FileLengthByName(char* path);

class Class_004353b0 {
public:
    char unknown_0[0x104];
    char names[9][0x100];              // +0x104
    int exists;                        // +0xa04

    void FUN_004353b0(int index, char* text);
};

// FUNCTION: 0x4353b0
void Class_004353b0::FUN_004353b0(int index, char* text)
{
    strcpy(names[index], text);
    if (index == 1) {
        if (strlen(text) != 0)
            exists = HAPI_FileLengthByName(text);
        else
            exists = 0;
    }
}
