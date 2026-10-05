// Decompiled by Opus. Names are provisional.
// __stdcall wrapper around the CRT's _chdir, like 0x4bbc30 (_rmdir).
#include <direct.h>

// FUNCTION: 0x4bc360
void __stdcall ChangeDirectory(const char* path)
{
    _chdir(path);
}
