// Decompiled by Claude Opus 5.5. Names are provisional.
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
//
// The copy of a path component goes into its own `buf` and only then into
// `name`. Written straight into `name` (as the sibling 0x4bc800 does), `path`
// outranks `name` in C2's priorities and the two swap ebx and edi; the extra
// local makes `name` a copy with few references of its own, so `buf` and `i`
// are coloured before `path`.
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
            char* buf = (char*)_alloca(len + 1);
            strncpy(buf, path, len);
            buf[len] = 0;
            name = buf;
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
