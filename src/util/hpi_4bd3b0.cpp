// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by opus. Names are provisional.
// Builds one package directory in the growing buffer `out` (size,
// pointer): a header {count, entries offset}, then one 9-byte entry per file
// or subdirectory (name offset, data offset, directory bit), recursing into
// subdirectories and adding a 9-byte file record per file. Returns the
// header's offset; *total collects the file sizes.
#include <io.h>
#include <string.h>

#pragma pack(push, 1)
struct FindFiles {
    char dir[0x100];     // +0x000
    char name[0x100];    // +0x100
    int state;           // +0x200
    char recursive;      // +0x204
    long handle;         // +0x205
    int index;           // +0x209
};

struct ArchiveEntry {
    unsigned int name;   // +0
    unsigned int data;   // +4
    unsigned char isDir : 1; // +8
    unsigned char spare : 7;
};

struct Node_004bd3b0 {
    unsigned int offset; // +0
    unsigned int size;   // +4
    unsigned char flags; // +8
};
#pragma pack(pop)

struct HapiBuf_004bd3b0 {
    unsigned int size;
    char* buf;
};

extern char DAT_0050a56c[];  // "Package Data"
extern char DAT_0050a57c[];  // "\\*"
extern char DAT_0050372c[];  // "*"
extern char DAT_0050a548[];  // ".."
extern char DAT_00502910[];  // "."
extern char DAT_00503374[];  // "\\"

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall HAPI_FindFirst(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall HAPI_FindNext(FindFiles* f, struct _finddata_t* fd);
unsigned int __stdcall HAPI_BuildArchiveDirectory(char* path, HapiBuf_004bd3b0* out, int* total);

// Grows the buffer by n bytes and returns the offset of the new space.
static inline unsigned int Grow(HapiBuf_004bd3b0* b, unsigned int n)
{
    unsigned int old = b->size;
    b->size += n;
    b->buf = (char*)FUN_004d84a0(b->buf, DAT_0050a56c, b->size);
    return old;
}

// FUNCTION: 0x4bd3b0
unsigned int __stdcall HAPI_BuildArchiveDirectory(char* path, HapiBuf_004bd3b0* out, int* total)
{
    char buf[0x104];
    struct _finddata_t fd;
    char* base;
    int h;
    int trailing;
    unsigned int root;
    unsigned int entries;
    int k;

    // Written out, not through Grow: Grow loads out->size into the wrong register.
    unsigned int nsize = out->size;
    root = nsize;
    nsize += 8;
    out->size = nsize;
    base = (char*)FUN_004d84a0(out->buf, DAT_0050a56c, nsize);
    out->buf = base;
    *(unsigned int*)(base + root) = 0;

    strcpy(buf, path);
    if (buf[strlen(buf) - 1] != '\\') {
        strcat(buf, DAT_0050a57c);
        trailing = 0;
    } else {
        strcat(buf, DAT_0050372c);
        trailing = 1;
    }

    h = HAPI_FindFirst(buf, &fd, -1, 1);
    if (h != -1) {
        do {
            if (strcmp(fd.name, DAT_00502910) != 0 && strcmp(fd.name, DAT_0050a548) != 0)
                ++(*(unsigned int*)(base + root));
        } while (HAPI_FindNext((FindFiles*)h, &fd) == 0);
        if (h != 0) {
            if (((FindFiles*)h)->state < 0)
                _findclose(((FindFiles*)h)->handle);
            FUN_004d85a0((void*)h);
        }
    }

    entries = Grow(out, *(unsigned int*)(base + root) * 9);
    *(unsigned int*)(out->buf + root + 4) = entries;

    h = HAPI_FindFirst(buf, &fd, -1, 1);
    if (h != -1) {
        k = 0;
        do {
            if (strcmp(fd.name, DAT_00502910) != 0 && strcmp(fd.name, DAT_0050a548) != 0) {
                unsigned int nameOff = Grow(out, strlen(fd.name) + 1);
                strcpy(out->buf + nameOff, fd.name);
                // Two steps: a one-expression pointer gets regrouped as (entries + k) + buf.
                ArchiveEntry* e = (ArchiveEntry*)(out->buf + entries);
                e += k;
                e->name = nameOff;
                e->spare = 0;
                if (fd.attrib & 0x10) {
                    e->isDir = 1;
                    strcpy(buf, path);
                    if (!trailing)
                        strcat(buf, DAT_00503374);
                    strcat(buf, fd.name);
                    unsigned int sub = HAPI_BuildArchiveDirectory(buf, out, total);
                    e = (ArchiveEntry*)(out->buf + entries);
                    e += k;
                    e->data = sub;
                } else {
                    e->isDir = 0;
                    unsigned int nodeOff = Grow(out, 9);
                    e = (ArchiveEntry*)(out->buf + entries);
                    e += k;
                    e->data = nodeOff;
                    // Addressed through e->data: this second use of e keeps the store unfolded.
                    Node_004bd3b0* node = (Node_004bd3b0*)(out->buf + e->data);
                    node->size = fd.size;
                    node->offset = 0;
                    node->flags = 0;
                    *total += node->size;
                }
                k++;
            }
        } while (HAPI_FindNext((FindFiles*)h, &fd) == 0);
        if (h != 0) {
            if (((FindFiles*)h)->state < 0)
                _findclose(((FindFiles*)h)->handle);
            FUN_004d85a0((void*)h);
        }
    }
    return root;
}
