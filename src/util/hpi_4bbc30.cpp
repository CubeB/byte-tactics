// Decompiled by Opus. Names are provisional.
// __stdcall wrapper around the CRT's remove (delete a file), like its
// neighbour 0x4bbc10 (rename). remove and _rmdir are the same code apart from
// the function they import (DeleteFileA, RemoveDirectoryA); the original
// calls remove at 0x4e7990.
#include <stdio.h>

// FUNCTION: 0x4bbc30
void __stdcall RemoveFile(const char* path)
{
    remove(path);
}
