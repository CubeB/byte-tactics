// Decompiled by Claude Opus 5.5. Names are provisional.
//
// Best so far 85.2%: the code is the original's but for one register swap.
// The original keeps `path` (and later the scaled index i * 9) in ebx and
// `name` in edi; this compiles to the other way round. Tried: every order of
// the five locals' declarations (120 variants), the walk as tail recursion,
// as an endless loop and with gotos, the search loop rotated or not, a local
// copy of `path`, `last` inverted, the else branch first, `name` defaulting
// to `path`, an Entry pointer for the found entry, /Gi, and every set of the
// common headers (tools/headers.py). The sibling 0x4bc800, written the same
// way, matches with the registers this one has.
#include <malloc.h>
#include <string.h>

// An archive's directory tree (0x4bb2e0.cpp names the same structs).
struct List_004bb2e0;

#pragma pack(push, 1)
struct Entry_004bb2e0 {
    char* name;                          // +0x0
    List_004bb2e0* child;                // +0x4, the directory's own list
    unsigned char flags;                 // +0x8, bit 0: a directory
};
#pragma pack(pop)

struct List_004bb2e0 {
    int count;                           // +0x0
    Entry_004bb2e0* entries;             // +0x4
};

// The entry a backslash-separated path names, walking down from `list`, or 0.
// FUNCTION: 0x4bb4e0
Entry_004bb2e0* __stdcall FUN_004bb4e0(List_004bb2e0* list, char* path)
{
    int len;
    char* name;
    int last;
    char* sep;
    for (;;) {
        sep = strchr(path, '\\');
        if (sep) {
            len = sep - path;
            name = (char*)_alloca(len + 1);
            strncpy(name, path, len);
            name[len] = 0;
            last = 0;
        } else {
            name = path;
            last = 1;
        }
        int i = list->count - 1;
        for (;;) {
            if (i < 0)
                return 0;
            if (_strcmpi(name, list->entries[i].name) == 0)
                break;
            i--;
        }
        if (last)
            return &list->entries[i];
        if (!(list->entries[i].flags & 1))
            return 0;
        path = sep + 1;
        list = list->entries[i].child;
    }
}
