// Decompiled by Claude Opus 5.5. Names are provisional.
// Stays in its own file: it is gap code (0x4bc800), and tools/gapcheck.py
// sizes a region's functions by the next annotation in the file, so a file
// that also holds functions outside the region cannot be its source.
#include <malloc.h>
#include <string.h>

// An archive's directory tree (0x4bc640.cpp names the same structs).
struct ArchiveDirectory;

#pragma pack(push, 1)
struct ArchiveEntry {
    char* name;                          // +0x0
    ArchiveDirectory* child;             // +0x4, the directory's own list
    unsigned char flags;                 // +0x8, bit 0: a directory
};
#pragma pack(pop)

struct ArchiveDirectory {
    int count;                           // +0x0
    ArchiveEntry* entries;               // +0x4
};

// The list of the directory a backslash-separated path ends in (everything up
// to its last backslash), walking down from `list`; 0 when a part of it is
// missing or is a file.
// FUNCTION: 0x4bc800
ArchiveDirectory* __stdcall HAPI_FindDirectory(ArchiveDirectory* list, char* path)
{
    char* sep;
    while ((sep = strchr(path, '\\')) != 0) {
        // Declared before len: operand order of the add.
        char* name;
        int len = sep - path;
        name = (char*)_alloca(len + 1);
        strncpy(name, path, len);
        name[len] = 0;
        int i = list->count - 1;
        // Endless loop that tests i at the top, not a for loop.
        for (;;) {
            if (i < 0)
                return 0;
            if (_strcmpi(name, list->entries[i].name) == 0)
                break;
            i--;
        }
        if (!(list->entries[i].flags & 1))
            return 0;
        // path advances before list: otherwise ecx and edx swap.
        path = sep + 1;
        list = list->entries[i].child;
    }
    return list;
}
