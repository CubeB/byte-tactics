// Decompiled by Opus. Names are provisional.
// Creates every directory along a path (like "mkdir -p").
#include <string.h>
#include <direct.h>

// FUNCTION: 0x4bcf00
void __stdcall MakeDirectoryPath(char* path)
{
    char buf[260];
    strcpy(buf, path);
    for (char* p = buf; *p; p++) {
        if (*p == '\\' || *p == '/') {
            *p = 0;
            _mkdir(buf);
            *p = '\\';
        }
    }
    _mkdir(buf);
}
