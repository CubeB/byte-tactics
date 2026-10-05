// Decompiled by space-bunny-free. Names are provisional.
// Advances the directory search 0x4bc4b0 opened and fills in one _finddata_t
// per call, returning -1 when the search is exhausted. A negative state means
// the handle is still a raw _findfirst search, so entries come straight out of
// _findnext until it runs dry; after that the search walks the path groups the
// game registered, matching each group's entries against the stored pattern.
// Attribute 0x11 with a zero size marks a directory, 1 with the node's size a
// file, and flag bit 2 hides an entry from the walk entirely.
//
// Two source details are load-bearing for the byte match. The outer loop is a
// `for` whose increment clause holds both latch statements, which is what makes
// MSVC rotate it and reload the group index in the body instead of keeping the
// guard's value in a register. Inside the inner loop the index has to sit in a
// local (`int i = f->index`): with the field read inline, the commutative add
// that forms the entry address takes its operands the other way round and the
// address lands in edx instead of edi, which moves three instructions and the
// whole register allocation of the found path with them.
#include <io.h>
#include <string.h>

// One entry of a group's list: the name, the node that carries the size, and a
// flag byte (1 for a directory, 2 to skip the entry).
#pragma pack(push, 1)
struct ArchiveEntry {
    char* name;              // +0
    void* node;              // +4
    char flags;              // +8
};

struct ArchiveDirectory {
    int count;               // +0
    ArchiveEntry* entries; // +4, 9 bytes each
};
#pragma pack(pop)

struct Path_004bc640 {
    char pad_0[0x10];
    ArchiveDirectory* list;  // +0x10
};

struct Group_004bc640 {
    char pad_0[8];
    Path_004bc640* path;     // +8
};

// The path group list: one entry per path and how many there are.
struct Groups_004bc640 {
    char pad_0[0x618];
    Group_004bc640** group;  // +0x618
    int count;               // +0x61c
};

// The search 0x4bc4b0 allocates: the path being walked, the pattern to match,
// the group being read and the entry index inside that group.
#pragma pack(push, 1)
struct FindFiles {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200
    char recursive;          // +0x204
    long handle;             // +0x205
    int index;               // +0x209
};
#pragma pack(pop)

int GetDisplay(void);
int __stdcall MatchWildcard(const char* str, const char* pat);
int __stdcall HAPI_FindDirectory(ArchiveDirectory* list, FindFiles* f);

// The state +0x205 doubles as the raw _findnext handle and as the group list
// the walk is reading, so it only holds a group list once the state is >= 0.
// FUNCTION: 0x4bc640
int __stdcall HAPI_FindNext(FindFiles* f, struct _finddata_t* fd)
{
    if (f == (FindFiles*)-1 || f == 0)
        return -1;
    if (f->state < 0) {
        if (_findnext(f->handle, fd) != -1)
            return 0;
        _findclose(f->handle);
        f->state = 0;
        f->index = -1;
        if (!f->recursive)
            return -1;
    }
    Groups_004bc640* g = (Groups_004bc640*)GetDisplay();
    for (; f->state < g->count; f->index = -1, f->state++) {
        if (f->index < 0) {
            f->handle = (long)HAPI_FindDirectory(((Path_004bc640*)((Group_004bc640*)g->group[f->state])->path)->list, f);
            if (f->handle == 0)
                continue;
        }
        while (++f->index < ((ArchiveDirectory*)f->handle)->count) {
            int i = f->index;
            ArchiveEntry* e = &((ArchiveDirectory*)f->handle)->entries[i];
            if (MatchWildcard(e->name, f->pattern) && !(e->flags & 2)) {
                if (e->flags & 1) {
                    fd->attrib = 0x11;
                    fd->size = 0;
                } else {
                    fd->attrib = 1;
                    fd->size = *(unsigned long*)((char*)e->node + 4);
                }
                fd->time_create = 0;
                fd->time_access = 0;
                fd->time_write = 0;
                strcpy(fd->name, e->name);
                return 0;
            }
        }
        if (!f->recursive)
            return -1;
    }
    return -1;
}
