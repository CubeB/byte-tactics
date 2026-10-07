// Decompiled by deepseek-v4.1-flash, space-bunny-free, Claude Opus 5.5, Opus, muse-spark-1.3-free, Sonnet, Haiku, Space Bunny Free, GPT-6.1-sol, deepseek-v4.1, mimo-v2.6-pro, DeepSeek V4.1 Flash, GPT-6, Sonnet 5.5, claude-sonnet-5-5 and opus. Names are provisional.
// The HAPI archive API, part 2: the directory search, the path helpers, the
// package reader and writer, and the list of open archives. The gap function,
// the vector insert and the function the compiler would inline into its caller
// stay in their own files (hpi_4bc800.cpp, hpi_4be6c0.cpp and
// hpi_4be320.cpp).
#include <string.h>
#include <io.h>

// A directory search: the directory part, the pattern to match, the state and
// the raw _findfirst handle.
#pragma pack(push, 1)
struct FindFiles {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200, negative while a _findfirst handle is open
    char recursive;          // +0x204
    long handle;             // +0x205, the _findfirst handle or the current directory list
    int index;               // +0x209
};
#pragma pack(pop)

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall HAPI_FindNext(FindFiles* f, struct _finddata_t* fd);

// Strips the directory from a path in place ("a\\b\\c.txt" -> "c.txt").
static char* StripDir(char* path)
{
    int len = (int)strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '\\')
        i--;
    int j = i + 1;
    int k = 0;
    do {
        path[k] = path[j];
        k++;
    } while (path[j++] != 0);
    return path;
}

// FUNCTION: 0x4bc4b0
int __stdcall HAPI_FindFirst(const char* path, void* fd, int state, char recursive)
{
    FindFiles* f = (FindFiles*)FUN_004d83b0("Find Files structure", 0x20d);
    strcpy(f->dir, path);
    for (int i = strlen(f->dir); i >= 0; i--) {
        if (f->dir[i] == '\\') {
            f->dir[i + 1] = '\0';
            break;
        }
    }
    strcpy(f->pattern, path);
    StripDir(f->pattern);
    if (strcmp(f->pattern, "*.*") == 0)
        strcpy(f->pattern, "*");
    f->recursive = recursive;
    f->state = state;
    if (state == -1) {
        f->handle = _findfirst(path, (struct _finddata_t*)fd);
        if (f->handle != -1)
            return (int)f;
        if (f->recursive == 0)
            goto fail;
        f->state = 0;
    }
    f->index = -1;
    if (HAPI_FindNext(f, (struct _finddata_t*)fd) != -1)
        return (int)f;
fail:
    FUN_004d85a0(f);
    return -1;
}

#include <stdio.h>

// An archive's directory tree: entries are packed at 9 bytes each. While the
// archive is being built the first two fields are offsets into the package;
// once it is loaded they are pointers, so the code that walks the loaded tree
// casts them.
#pragma pack(push, 1)
struct ArchiveEntry {
    int name;                          // +0x0
    int data;                          // +0x4
    union {
        unsigned char flags;           // +0x8, bit 0: a directory, bit 1: shadowed
        struct {
            unsigned char isDir : 1;
            unsigned char spare : 7;
        };
    };
};
#pragma pack(pop)

struct ArchiveDirectory {
    int count;                         // +0x0
    ArchiveEntry* entries;             // +0x4
};

// An archive's header: the key at +0xc and the root directory at +0x10.
struct Header {
    char magic[4];                     // +0x0
    unsigned char version[4];          // +0x4
    unsigned int size;                 // +0x8
    unsigned char obfuscate;           // +0xc
    char unknown_d[3];
    ArchiveDirectory* list;            // +0x10
};

// An open archive: the file, the read position, its header, the open count
// and the mode it was opened in.
struct OPENHAPIFILE {
    FILE* fp;                          // +0x0
    int pos;                           // +0x4
    Header* node;                      // +0x8
    int count;                         // +0xc, handles open on this item
    int mode;                          // +0x10
    char name[0x104];                  // +0x14
};

// The display context the archives and the path helpers hang from: the list
// of open archives at +0x618, the start directory at +0x628 and the last
// directory set at +0x728.
struct Display_004be0b0 {
    char unknown_0[0x618];
    OPENHAPIFILE** files;              // +0x618
    int count;                         // +0x61c
    char unknown_620[8];
    char cwd[0x100];                   // +0x628
    char lastDir[0x100];               // +0x728
};

Display_004be0b0* GetDisplay();
int __stdcall MatchWildcard(const char* str, const char* pat);
ArchiveDirectory* __stdcall HAPI_FindDirectory(ArchiveDirectory* list, char* path);

// Advances the directory search 0x4bc4b0 opened and fills in one _finddata_t
// per call, returning -1 when the search is exhausted. A negative state means
// the handle is still a raw _findfirst search, so entries come straight out of
// _findnext until it runs dry; after that the search walks the path groups the
// game registered, matching each group's entries against the stored pattern.
// Attribute 0x11 with a zero size marks a directory, 1 with the node's size a
// file, and flag bit 2 hides an entry from the walk entirely.
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
    Display_004be0b0* g = GetDisplay();
    // Both latch statements stay in the increment clause.
    for (; f->state < g->count; f->index = -1, f->state++) {
        if (f->index < 0) {
            f->handle = (long)HAPI_FindDirectory(g->files[f->state]->node->list, (char*)f);
            if (f->handle == 0)
                continue;
        }
        while (++f->index < ((ArchiveDirectory*)f->handle)->count) {
            // The index sits in a local: read inline, the entry address operands swap.
            int i = f->index;
            ArchiveEntry* e = &((ArchiveDirectory*)f->handle)->entries[i];
            if (MatchWildcard((char*)e->name, f->pattern) && !(e->flags & 2)) {
                if (e->flags & 1) {
                    fd->attrib = 0x11;
                    fd->size = 0;
                } else {
                    fd->attrib = 1;
                    fd->size = *(unsigned long*)((char*)e->data + 4);
                }
                fd->time_create = 0;
                fd->time_access = 0;
                fd->time_write = 0;
                strcpy(fd->name, (char*)e->name);
                return 0;
            }
        }
        if (!f->recursive)
            return -1;
    }
    return -1;
}

// Closes the search handle (if open) and frees the search; -1 for no search.
// FUNCTION: 0x4bc8d0
int __stdcall HAPI_FindClose(FindFiles* f)
{
    if (f == (FindFiles*)-1 || f == 0)
        return -1;
    int result;
    if (f->state < 0)
        result = _findclose(f->handle);
    else
        result = 0;
    FUN_004d85a0(f);
    return result;
}

// FUNCTION: 0x4bc930
int __stdcall CountDirectoryEntries(const char* path, int flag)
{
    struct _finddata_t fd;
    int count = 0;
    int handle = HAPI_FindFirst(path, &fd, -1, 1);

    if (handle != -1) {
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0
                && (flag == 0 || (fd.attrib & 0x10))) {
                count++;
            }
        } while (HAPI_FindNext((FindFiles*)handle, &fd) != -1);
        if (handle != 0) {
            FindFiles* h = (FindFiles*)handle;
            if (h->state < 0)
                _findclose(h->handle);
            FUN_004d85a0(h);
        }
    }
    return count;
}

// The caller's std::vector<Class_004c91a0>, written by hand so that insert
// (0x4be6c0, which has its own file) stays an out-of-line call under its real
// name.
class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ReleaseRef(); }
};

namespace std {
template<class T> class allocator;
template<class T, class A = allocator<T> > class vector {
public:
    char allocator_;                   // +0x0
    T* _First;                         // +0x4
    T* _Last;                          // +0x8
    T* _End;                           // +0xc
    void insert(T* pos, unsigned int n, const T& x);
};
}
typedef std::vector<Class_004c91a0> Class_004be6c0;

// FUNCTION: 0x4bca30
void __stdcall ListDirectory(const char* path, int param_2, Class_004be6c0* param_3)
{
    struct _finddata_t fd;
    int h = HAPI_FindFirst(path, &fd, -1, 1);
    if (h != -1) {
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0
                && (param_2 == 0 || (fd.attrib & 0x10) != 0)) {
                Class_004c91b0 key(fd.name);
                param_3->insert(param_3->_Last, 1, key);
            }
        } while (HAPI_FindNext((FindFiles*)h, &fd) != -1);
        if (h != 0) {
            FindFiles* f = (FindFiles*)h;
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}

static inline int Next(int h, struct _finddata_t* fd) { int r = HAPI_FindNext((FindFiles*)h, fd); return r; }

// Recursive directory walk: opens path with the search 0x4bc4b0 and, for every
// entry whose name is neither "." nor "..", either descends into the
// subdirectory or, when the name matches the wildcard pattern, appends
// "path\\name" to the vector passed in. The search handle is closed at the end
// (the inlined body of 0x4bc8d0).
// FUNCTION: 0x4bcb50
void __stdcall FindFilesRecursive(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive)
{
    char buf[0x100];
    struct _finddata_t fd;

    sprintf(buf, "%s\\*", path);
    int h = HAPI_FindFirst(buf, &fd, state, recursive);
    if (h != -1) {
        // Never read: the extra live value demotes the handle from ebx to ebp.
        int r;
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                if ((fd.attrib & 0x10) != 0) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    FindFilesRecursive(buf, pat, tree, ((FindFiles*)h)->state, 0);
                } else if (MatchWildcard(fd.name, pat)) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    Class_004c91b0 key(buf);
                    tree->insert(tree->_Last, 1, key);
                }
            }
        } while ((r = HAPI_FindNext((FindFiles*)h, &fd)) != -1);
        // A second variable, cast from h: the -1 test is dropped if written on h.
        FindFiles* f = (FindFiles*)h;
        if (f != (FindFiles*)-1 && f != 0) {
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}

// Walks the search opened by HAPI_FindFirst and stops on the `index`-th entry
// that is neither "." nor ".." (skipping entries that fail the flag test when
// `flag` is set). The close of the search is HAPI_FindClose inlined.
// FUNCTION: 0x4bcd10
void __stdcall GetDirectoryEntry(struct _finddata_t* fd, int index, const char* pattern, int flag)
{
    int count = 0;
    FindFiles* handle = (FindFiles*)HAPI_FindFirst(pattern, fd, -1, 1);
    if (handle != (FindFiles*)-1) {
        do {
            if (strcmp(fd->name, ".") != 0 && strcmp(fd->name, "..") != 0) {
                if (flag == 0 || (fd->attrib & 0x10) != 0) {
                    if (count == index)
                        break;
                    count++;
                }
            }
        } while (HAPI_FindNext(handle, fd) != -1);
        if (handle != 0) {
            if (handle->state < 0)
                _findclose(handle->handle);
            FUN_004d85a0(handle);
        }
    }
}

#include <direct.h>
#include <stddef.h>

// Stores the current directory of the current drive. The drive number is
// converted on each path separately (`n = d - '@'` in both branches); a
// single `d - '@'` after the if picks edx and interleaves the subtraction
// with the argument pushes.
// FUNCTION: 0x4bce10
void SaveStartDirectory()
{
    Display_004be0b0* g = GetDisplay();
    char buf[12];
    char d = _getdrive() + '@';
    char* p = g->cwd;
    int n;
    if (buf != NULL)
        n = d - '@';
    else
        n = (char)(_getdrive() + '@') - '@';
    _getdcwd(n, p, 0x100);
}

// FUNCTION: 0x4bce60
int __stdcall SetLastDirectory(char* path)
{
    Display_004be0b0* base = GetDisplay();
    int result = _chdir(path);
    if (result == 0) {
        strncpy(base->lastDir, path, 0x100);
    }
    return result;
}

// FUNCTION: 0x4bcea0
void __stdcall GetLastDirectory(char* dst)
{
    Display_004be0b0* s = GetDisplay();
    strncpy(dst, s->lastDir, 0x100);
}

// FUNCTION: 0x4bcec0
void __stdcall GetStartDirectory(char* param)
{
    Display_004be0b0* p = GetDisplay();
    char* src = p->cwd;
    strncpy(param, src, 0x100);
}

// FUNCTION: 0x4bcee0
void RestoreStartDirectory() {
    Display_004be0b0* p = GetDisplay();
    _chdir(p->cwd);
}

// Creates every directory along a path (like "mkdir -p").
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

// A file's entry data: the offset, the size and whether the data is
// compressed.
#pragma pack(push, 1)
struct Info {
    long offset;                       // +0x0
    unsigned int size;                 // +0x4
    unsigned char compressed;          // +0x8
};
#pragma pack(pop)

// A file handle, loose or inside an archive.
struct FileHandle {
    FILE* fp;                          // +0x0
    OPENHAPIFILE* shared;              // +0x4
    Info* info;                        // +0x8
    unsigned int pos;                  // +0xc
    int* buffer;                       // +0x10, the block sizes
    unsigned char* buffer2;            // +0x14, the current block
    char name[0x100];                  // +0x18
};

FileHandle* __stdcall HAPI_OpenFile(char* filename, const char* mode);
long __stdcall HAPI_SeekFile(FileHandle* file, long pos);
int __stdcall HAPI_readfromfile(FileHandle* file, unsigned char* buf, int size);

// Drops one reference on a handle, closing and freeing whatever it still holds.
static void Close_004bcf80(FileHandle* f)
{
    if (f->shared) {
        f->shared->count--;
        if (f->shared->count == 0 && f->shared->mode == 0) {
            fclose(f->shared->fp);
            f->shared->fp = 0;
        }
    } else {
        fclose(f->fp);
    }
    if (f->buffer)
        FUN_004d85a0(f->buffer);
    if (f->buffer2)
        FUN_004d85a0(f->buffer2);
    FUN_004d85a0(f);
}

// Copies a file into an already open destination handle, 0x19000 bytes at a
// time, and returns the number of bytes copied (0 on failure). The size to
// copy comes from the source handle: the cached block size when it is one of
// the game's packed items, else _filelength of the real file, else 0, and an
// empty source is a failure. The destination has to be a real file: when it
// is a packed item itself the write is refused by counting -1 bytes written,
// which never matches the chunk size.
// FUNCTION: 0x4bcf80
int __stdcall HAPI_CopyIntoFile(FileHandle* dst, char* name)
{
    int len;
    int left;
    int got;
    int written;
    void* buf = 0;
    FileHandle* f = HAPI_OpenFile(name, "rb");
    if (f == 0)
        return 0;
    if (f->shared)
        len = f->info->size;
    else if (f->fp)
        len = _filelength(f->fp->_file);
    else
        len = 0;
    if (len == 0)
        goto fail;
    if (HAPI_SeekFile(f, 0) == -1)
        goto fail;
    buf = FUN_004d83b0("COPY BUFFER", 0x19000);
    left = len;
    while (left != 0) {
        got = HAPI_readfromfile(f, (unsigned char*)buf, 0x19000);
        if (got <= 0)
            goto fail;
        if (dst->shared)
            written = -1;
        else
            written = (int)fwrite(buf, 1, got, dst->fp);
        if (written != got)
            goto fail;
        left -= got;
    }
    Close_004bcf80(f);
    // Returns len, kept separate from the loop counter left; not dst.
    return len;
fail:
    if (buf)
        FUN_004d85a0(buf);
    Close_004bcf80(f);
    return 0;
}

// FUNCTION: 0x4bd150
int __stdcall FUN_004bd150(int, int)
{
    return 0;
}

// The package buffer 0x4bd160 and 0x4bd3b0 grow: the current size and the
// block of data.
struct HapiBuf {
    unsigned int size;
    char* buf;
};

// The 20-byte "HAPI" header written at the front of a package.
struct Hapi_004bd160 {
    char magic[4];
    char a4;
    char a5;
    char a6;
    char a7;
    unsigned int size;
    unsigned char key;
    char ad;
    unsigned short ae;
    int extra;
};

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall HAPI_WriteArchiveData(char* path, char* base, int off, FILE* f,
                           void (__cdecl* cb)(unsigned), unsigned extra,
                           int key, int flags);
unsigned int __stdcall HAPI_BuildArchiveDirectory(char* path, HapiBuf* out, int* total);

static inline unsigned int Grow(HapiBuf* b, unsigned int n)
{
    unsigned int old = b->size;
    b->size += n;
    b->buf = (char*)FUN_004d84a0(b->buf, "Package Data", b->size);
    return old;
}

// Writes an HPI package: a 20-byte "HAPI"
// header in a growing buffer {size, buf}, the directory built by HAPI_BuildArchiveDirectory,
// the file data from HAPI_WriteArchiveData, the directory encrypted with the key, and
// a copyright line at the end.
#include <time.h>

// FUNCTION: 0x4bd160
int __stdcall HAPI_PackDirectory(char* srcname, char* dstname, void (__cdecl* cb)(int),
                           unsigned int key, int flags)
{
    char* extra = 0;
    char year[8];
    char copyright[0x40];
    int off;
    FILE* f;
    int i, n;
    unsigned char* p;

    if (cb)
        cb(0);
    struct HapiBuf sb;
    memset(&sb, 0, sizeof(sb));
    Grow(&sb, 20);
    off = HAPI_BuildArchiveDirectory(srcname, &sb, (int*)&extra);

    {
        struct Hapi_004bd160* h = (struct Hapi_004bd160*)sb.buf;
        unsigned int m;
        unsigned int t;
        unsigned char k;
        strncpy(sb.buf, "HAPI", 4);
        h->a4 = 0;
        h->a5 = 0;
        h->a6 = 1;
        h->a7 = 0;
        h->size = sb.size;
        m = key & 0xff;
        if ((unsigned char)key == 0)
            k = 0;
        else {
            t = (m >> 2) | (m << 6);
            k = ~t;
        }
        h->key = k;
        h->ad = 0;
        h->ae = 0;
        h->extra = off;
    }

    f = fopen(dstname, "wb");
    if (!f) {
        if (sb.buf)
            FUN_004d85a0(sb.buf);
        return 0;
    }
    fwrite(sb.buf, sb.size, 1, f);
    if (cb)
        cb(5);
    HAPI_WriteArchiveData(srcname, sb.buf, off, f, (void (__cdecl*)(unsigned))cb, (unsigned)extra, key, flags);
    if (cb)
        cb(0x5f);
    n = (int)sb.size - 20;
    p = (unsigned char*)sb.buf + 20;
    if ((unsigned char)key) {
        for (i = 0; i < n; i++)
            p[i] = (char)~((unsigned char)(i + 20) ^ (unsigned char)key ^ p[i]);
    }
    rewind(f);
    fwrite(sb.buf, sb.size, 1, f);
    {
    time_t now;
    struct tm* t;
    now = time(0);
    t = localtime(&now);
    sprintf(year, "%i", t->tm_year + 1900);
    strcpy(copyright, "Copyright 0000 Cavedog Entertainment");
    strncpy(strstr(copyright, "0000"), year, 4);
    fseek(f, 0, SEEK_END);
    fprintf(f, copyright);
    fclose(f);
    if (sb.buf)
        FUN_004d85a0(sb.buf);
    }
    return 1;
}

#pragma pack(push, 1)
// A file's record in a package directory: the offset of its data and its size.
struct Node_004bd3b0 {
    unsigned int offset; // +0
    unsigned int size;   // +4
    unsigned char flags; // +8
};
#pragma pack(pop)

extern char DAT_0050a56c[];  // "Package Data"
extern char DAT_0050a57c[];  // "\\*"
extern char DAT_0050372c[];  // "*"
extern char DAT_0050a548[];  // ".."
extern char DAT_00502910[];  // "."
extern char DAT_00503374[];  // "\\"

// Builds one package directory in the growing buffer `out` (size,
// pointer): a header {count, entries offset}, then one 9-byte entry per file
// or subdirectory (name offset, data offset, directory bit), recursing into
// subdirectories and adding a 9-byte file record per file. Returns the
// header's offset; *total collects the file sizes.
// FUNCTION: 0x4bd3b0
unsigned int __stdcall HAPI_BuildArchiveDirectory(char* path, HapiBuf* out, int* total)
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
            FUN_004d85a0((FindFiles*)h);
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
            FUN_004d85a0((FindFiles*)h);
        }
    }
    return root;
}

#pragma pack(push, 1)
struct Node_004bd830 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};
#pragma pack(pop)

int __stdcall SquashPack(void* chunk, int* chunkSize, char* data,
                           int size, int method, int encrypt);
unsigned int __stdcall SquashMaxPackedSize(unsigned int value, int mode);
// This forward declaration, Node_004bd830 and the casts in nblocks all stay.
void __stdcall HAPI_WriteArchiveData(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags);

// The handle's file size, from the same translation unit (its own file is
// src/util/hpi_4bbd00.cpp), so it is inlined below.
long __stdcall HAPI_FileLength(FileHandle* file)
{
    if (file->shared != 0)
        return file->info->size;
    if (file->fp != 0)
        return _filelength(file->fp->_file);
    return 0;
}

// Whole 64K blocks of a byte size, rounded up.
static inline int nblocks(unsigned w)
{
    return (0 != (int)w % 65536) + (int)w / 65536;
}

// Writes the file data of one package directory, recursing into
// subdirectories. For each file entry it records the data offset, size and
// compression flag in the directory buffer, then copies the file, either as
// 64K blocks packed by SquashPack behind a table of block sizes, or raw in
// 4K pieces, scrambling the bytes when a key is set.
// FUNCTION: 0x4bd830
void __stdcall HAPI_WriteArchiveData(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags)
{
    Info* info;
    int len;
    ArchiveEntry* e;
    unsigned size;
    char name[260];
    char full[260];
    unsigned char buffer[0x1000];
    int n, * table;
    int clen;
    unsigned remaining, i;
    FileHandle* file;
    unsigned char* pack;
    int j;
    unsigned char* data;
    unsigned packlen;

    strcpy(name, path);
    if (name[strlen(name) - 1] != '\\')
        strcat(name, "\\");

    long pos2;
    long pos;
    for (i = 0; i < *(unsigned*)(off + base); i++) {
        e = (ArchiveEntry*)(base + *(int*)(off + base + 4)) + i;
        strcpy(full, name);
        strcat(full, base + e->name);
        if ((e->flags & 1) != 0) {
            HAPI_WriteArchiveData(full, base, e->data, f, cb, extra, key, flags);
        } else {
            file = HAPI_OpenFile(full, "rb");
            info = (Info*)(base + e->data);
            info->offset = ftell(f);
            info->size = HAPI_FileLength(file);
            info->compressed = (char)flags;
            if ((char)flags) {
                int blocks = nblocks(info->size);
                table = (int*)FUN_004d83b0("Block Sizes", blocks * 4);
                fwrite(table, blocks, 4, f);
                packlen = SquashMaxPackedSize(0x10000, flags & 0xff);
                pack = (unsigned char*)FUN_004d83b0("Pack Buffer", packlen);
                data = (unsigned char*)FUN_004d83b0("Data Buffer", 0x10000);
                remaining = info->size;
                // remaining is decremented in the for-increment, not as the last body statement.
                for (n = 0; n < blocks; n++, remaining -= 0x10000) {
                    int chunk;
                    chunk = remaining >= 0x10000 ? 0x10000 : remaining;
                    HAPI_readfromfile(file, data, chunk);
                    clen = packlen;
                    SquashPack(pack, &clen, (char*)data, chunk, flags & 0xff, 1);
                    table[n] = clen;
                    pos = ftell(f);
                    len = clen;
                    if ((char)key) {
                        for (j = 0; j < len; j++)
                            pack[j] = (unsigned char)~((char)j + (char)pos ^ (char)key ^ pack[j]);
                    }
                    fwrite(pack, len, 1, f);
                }
                fseek(f, info->offset, SEEK_SET);
                pos2 = ftell(f);
                if ((char)key) {
                    for (j = 0; j < blocks * 4; j++)
                        ((unsigned char*)table)[j] = (unsigned char)~((char)j
                            + ((char)pos2) ^ (char)key ^ ((unsigned char*)table)[j]);
                }
                fwrite(table, blocks, 4, f);
                fseek(f, 0, SEEK_END);
                FUN_004d85a0(table);
                FUN_004d85a0(pack);
                FUN_004d85a0(data);
            } else {
                size = info->size;
                while (size > 0) {
                    int chunk = size >= 0x1000 ? 0x1000 : size;
                    HAPI_readfromfile(file, buffer, chunk);
                    pos = ftell(f);
                    if ((char)key) {
                        for (j = 0; j < chunk; j++)
                            buffer[j] = (unsigned char)~(buffer[j] ^ ((char)pos + (char)j
                                            ^ ((char)key)));
                    }
                    fwrite(buffer, chunk, 1, f);
                    size -= chunk;
                }
            }
            if (file->shared != 0) {
                --file->shared->count;
                if (file->shared->count == 0 && file->shared->mode == 0) {
                    fclose(file->shared->fp);
                    file->shared->fp = 0;
                }
            } else {
                fclose(file->fp);
            }
            if (file->buffer != 0)
                FUN_004d85a0(file->buffer);
            if (file->buffer2 != 0)
                FUN_004d85a0(file->buffer2);
            FUN_004d85a0(file);
            if (cb != 0)
                cb(5 + (unsigned)(90 * info->offset - *(int*)(8 + base) * 90) / extra);
        }
    }
}

#include <windows.h>

extern char g_hapiCopyright[];         // "Copyright 0000 Cavedog Entertainment"

// 4bdd70 passes the entry's data offset to the definition (0x4be010), which
// takes it as the directory pointer, so it keeps its own declaration.
void __stdcall HAPI_RelocateDirectory(ArchiveDirectory* h, int delta);
void __stdcall HAPI_RelocateDirectory(int name, int base);

static inline int Bad_004bdd70(FILE* f, Header* hdr, char* copyright)
{
    if (strncmp(hdr->magic, "HAPI", 4) != 0 || hdr->version[0] != 0 || hdr->version[1] != 0 ||
        hdr->version[2] != 1 || hdr->version[3] != 0)
        return 1;
    int len = strlen(g_hapiCopyright);
    fseek(f, -len, 2);
    fread(copyright, 1, len, f);
    copyright[len] = 0;
    strncpy(copyright + (strstr(g_hapiCopyright, "0000") - g_hapiCopyright), "0000", 4);
    if (strcmp(copyright, g_hapiCopyright) == 0)
        return 0;
    return 1;
}

// FUNCTION: 0x4bdd70
OPENHAPIFILE* __stdcall HAPI_OpenArchive(const char* name, int mode)
{
    FILE* f = fopen(name, "rb");
    if (f == 0)
        return 0;
    OPENHAPIFILE* h = (OPENHAPIFILE*)FUN_004d83b0("OPENHAPIFILE structure", 0x118);
    char* filePart;
    h->fp = f;
    h->pos = -1;
    h->count = 0;
    h->mode = mode;
    GetFullPathNameA(name, 0x104, h->name, &filePart);
    Header hdr;
    fread(&hdr, 0x14, 1, f);
    char copyright[0x40];
    if (Bad_004bdd70(f, &hdr, copyright)) {
        fclose(f);
        FUN_004d85a0(h);
        return 0;
    }
    h->node = (Header*)FUN_004d83b0("HAPIFILE header", hdr.size);
    rewind(f);
    fread(h->node, hdr.size, 1, f);
    {
        Header* base = h->node;
        // Key derivation: a byte key for the condition, an unsigned w for the split rotate,
        // and a second byte local r receiving the result.
        unsigned char key = base->obfuscate;
        unsigned int w = key;
        unsigned char r = key;
        if (key) { unsigned int hi = w >> 6; w = w << 2; hi = hi | w; r = (unsigned char)~hi; }
        base->obfuscate = r;
        hdr.obfuscate = h->node->obfuscate;
        {
        unsigned char k;
        int i;
        int n;
        unsigned char* p;
        p = (unsigned char*)h->node;
        p += 0x14;
        k = hdr.obfuscate;
        n = hdr.size - 0x14;
        if (k != 0) {
            for (i = 0; i < n; i++)
                p[i] = (unsigned char)((i + 0x14) ^ k ^ ~p[i]);
        }
        }
        base = h->node;
        base->list = (ArchiveDirectory*)((char*)base->list + (int)base);
        Header* b = h->node;
        ArchiveDirectory* t = b->list;
        t->entries = (ArchiveEntry*)((char*)t->entries + (int)b);
        for (int i = t->count - 1; i >= 0; i--) {
            ArchiveEntry* e = &t->entries[i];
            e->name += (int)b;
            e->data += (int)b;
            if (e->flags & 1)
                HAPI_RelocateDirectory(e->data, (int)b);
        }
    }
    if (mode == 0) {
        fclose(f);
        h->fp = 0;
    }
    return h;
}

// FUNCTION: 0x4be010
void __stdcall HAPI_RelocateDirectory(ArchiveDirectory* h, int delta)
{
    h->entries = (ArchiveEntry*)((int)h->entries + delta);
    for (int i = h->count - 1; i >= 0; i--) {
        ArchiveEntry* p = (ArchiveEntry*)((int)h->entries + i * 9);
        p->name += delta;
        p->data += delta;
        if (p->flags & 1)
            HAPI_RelocateDirectory((ArchiveDirectory*)p->data, delta);
    }
}

// Closes a file record: closes its FILE (if open), then frees its buffer at
// +0x8 and the record itself.
// FUNCTION: 0x4be070
void __stdcall HAPI_CloseArchive(OPENHAPIFILE* f)
{
    if (f != 0) {
        if (f->fp != 0)
            fclose(f->fp);
        FUN_004d85a0(f->node);
        FUN_004d85a0(f);
    }
}

extern char DAT_0050a5c0[];            // "HAPIFILE array"

// FUNCTION: 0x4be0b0
void* __stdcall HAPI_AddArchive(LPCSTR param_1, int param_2)
{
    Display_004be0b0* display = GetDisplay();
    char fullPath[0x100];
    char* filePart;
    GetFullPathNameA(param_1, 0x100, fullPath, &filePart);

    for (int i = 0; i < display->count; i++) {
        if (_strcmpi(fullPath, (char*)display->files[i] + 0x14) == 0)
            return 0;
    }

    void* file = HAPI_OpenArchive(param_1, param_2);
    if (!file)
        return 0;

    display->files = (OPENHAPIFILE**)FUN_004d84a0(display->files, DAT_0050a5c0, display->count * 4 + 4);
    display->files[display->count] = (OPENHAPIFILE*)file;
    display->count++;
    return file;
}

static void FreeRecord_004be180(OPENHAPIFILE* p)
{
    if (p != 0) {
        if (p->fp != 0)
            fclose(p->fp);
        FUN_004d85a0(p->node);
        FUN_004d85a0(p);
    }
}

// Walks the record list and probes each record whose file is not open yet:
// opens the record's filename in "rb" and, if it can be opened, closes it
// again (the record only checks that the file exists). If the open fails the
// record is freed and removed from the list, shifting the tail down.
// FUNCTION: 0x4be180
void HAPI_DropMissingArchives(void)
{
    Display_004be0b0* state = GetDisplay();
    int i;
    for (i = 0; i < state->count; i++) {
        if (state->files[i]->fp == 0) {
            state->files[i]->fp = fopen(state->files[i]->name, "rb");
            OPENHAPIFILE* rec = state->files[i];
            if (rec->fp == 0) {
                // Passed state->files[i], not rec: keeps the redundant file test.
                FreeRecord_004be180(state->files[i]);
                state->files[i] = 0;
                for (int j = i; j < state->count - 1; j++)
                    state->files[j] = state->files[j + 1];
                state->count--;
                i--;
            } else {
                fclose(state->files[i]->fp);
                state->files[i]->fp = 0;
            }
        }
    }
}

// Takes a record out of the state's list: closes the record's file, frees its
// buffer and the record itself, then shifts the tail of the list down over the
// hole and shrinks the count. A record that is not in the list is left alone.
// FUNCTION: 0x4be270
void __stdcall HAPI_RemoveArchive(OPENHAPIFILE* pRecord)
{
    Display_004be0b0* state = GetDisplay();
    int i;
    for (i = 0; i < state->count; i++) {
        if (state->files[i] == pRecord) {
            OPENHAPIFILE* p = state->files[i];
            if (p != 0) {
                if (p->fp != 0)
                    fclose(p->fp);
                FUN_004d85a0(p->node);
                FUN_004d85a0(p);
            }
            state->files[i] = 0;
            for (int j = i; j < state->count - 1; j++) {
                state->files[j] = state->files[j + 1];
            }
            state->count--;
            return;
        }
    }
}

// FUNCTION: 0x4be3b0
void __stdcall HAPI_ClearShadowFlags(ArchiveDirectory* list)
{
    for (int i = list->count - 1; i >= 0; i--) {
        list->entries[i].flags &= ~2;
        if (list->entries[i].flags & 1)
            HAPI_ClearShadowFlags((ArchiveDirectory*)list->entries[i].data);
    }
}

ArchiveEntry* __stdcall HAPI_FindEntry(ArchiveDirectory* table, char* name);

// Walks a directory tree (the search 0x4bc4b0 allocates, the same one 0x4bcb50
// uses) and, for every plain file, marks the matching entry of every open
// HAPI archive: the entry named "path + file name" is looked up with
// HAPI_FindEntry in each archive's directory (from the search's current archive
// index on) and gets bit 2 set unless bit 1 is already set. Sub directories
// other than "." and ".." are entered with the path extended by "name\\".
// FUNCTION: 0x4be400
void __stdcall HAPI_MarkShadowedFiles(char* path, int state, int recursive)
{
    Display_004be0b0* d = GetDisplay();
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
                ArchiveEntry* e = HAPI_FindEntry(d->files[i]->node->list, buf);
                if (e && !(e->flags & 1))
                    e->flags |= 2;
            }
        }
    } while (HAPI_FindNext((FindFiles*)h, &fd) != -1);
    FindFiles* f = (FindFiles*)h;
    if (f) {
        if (f->state < 0)
            _findclose(f->handle);
        FUN_004d85a0(f);
    }
}
