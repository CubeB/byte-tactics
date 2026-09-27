// Decompiled by Opus. Names are provisional.
// __stdcall wrapper around the CRT's _rmdir, like its neighbour 0x4bbc10
// (rename).
#include <direct.h>

// FUNCTION: 0x4bbc30
void __stdcall FUN_004bbc30(const char* path)
{
    _rmdir(path);
}
