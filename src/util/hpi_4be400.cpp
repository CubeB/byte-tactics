// Decompiled by Sonnet 5.5, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// Walks a directory tree (the search 0x4bc4b0 allocates, the same one 0x4bcb50
// uses) and, for every plain file, marks the matching entry of every open
// HAPI archive: the entry named "path + file name" is looked up with
// HAPI_FindEntry in each archive's directory (from the search's current archive
// index on) and gets bit 2 set unless bit 1 is already set. Sub directories
// other than "." and ".." are entered with the path extended by "name\\".

#include <io.h>
#include <string.h>

#pragma pack(push, 1)
struct FindFiles {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200
    char recursive;          // +0x204
    long handle;             // +0x205
    int index;               // +0x209
};

struct ArchiveEntry {                  // 9 bytes
    int field_0;
    int field_4;
    unsigned char flags;               // +0x8
};

struct ArchiveDirectory {
    int count;
    ArchiveEntry* entries;
};

struct Header_004be400 {
    char unknown_0[0x10];
    ArchiveDirectory* table;           // +0x10
};

struct OPENHAPIFILE {
    char unknown_0[8];
    Header_004be400* header;           // +0x8
};

struct Display_004be400 {
    char unknown_0[0x618];
    OPENHAPIFILE** files;              // +0x618
    int count;                         // +0x61c
};
#pragma pack(pop)

Display_004be400* GetDisplay();
ArchiveEntry* __stdcall HAPI_FindEntry(ArchiveDirectory* table, char* name);
int __stdcall HAPI_FindFirst(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall HAPI_FindNext(int handle, struct _finddata_t* fd);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4be400
void __stdcall HAPI_MarkShadowedFiles(char* path, int state, int recursive)
{
    Display_004be400* d = GetDisplay();
    char buf[0x100];
    struct _finddata_t fd;
    int i;

    strcpy(buf, path);
    strcat(buf, "*");
    int h = HAPI_FindFirst(buf, &fd, state, recursive);
    if (h == -1)
        return;
    do {
        if (fd.attrib & 0x10) {
            // Two nested ifs, not one &&.
            if (strcmp(fd.name, ".") != 0) {
                if (strcmp(fd.name, "..") != 0) {
                    strcpy(buf, path);
                    strcat(buf, fd.name);
                    strcat(buf, "\\");
                    HAPI_MarkShadowedFiles(buf, ((FindFiles*)h)->state, 0);
                }
            }
        } else {
            i = ((FindFiles*)h)->state;
            if (i < 0)
                i = 0;
            else
                i++;
            for (; i < d->count; i++) {
                strcpy(buf, path);
                strcat(buf, fd.name);
                ArchiveEntry* e = HAPI_FindEntry(d->files[i]->header->table, buf);
                if (e && !(e->flags & 1))
                    e->flags |= 2;
            }
        }
    } while (HAPI_FindNext(h, &fd) != -1);
    FindFiles* f = (FindFiles*)h;
    if (f) {
        if (f->state < 0)
            _findclose(f->handle);
        FUN_004d85a0(f);
    }
}
