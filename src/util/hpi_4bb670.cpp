// Decompiled by Opus. Names are provisional.
#include <string.h>

class FileHandle {
public:
    char unknown_0[0x18];
    char name[0x100];                  // +0x18

    void SetFileName(const char* text);
};

// FUNCTION: 0x4bb670
void FileHandle::SetFileName(const char* text)
{
    strncpy(name, text, sizeof(name));
    name[sizeof(name) - 1] = 0;
}
