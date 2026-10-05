// Decompiled by Sonnet. Names are provisional.
#include <string.h>

int GetDisplay(void);
extern "C" int __cdecl _chdir(const char* path);

// FUNCTION: 0x4bce60
int __stdcall FUN_004bce60(char* path)
{
    int base = GetDisplay();
    int result = _chdir(path);
    if (result == 0) {
        strncpy((char*)(base + 0x728), path, 0x100);
    }
    return result;
}
