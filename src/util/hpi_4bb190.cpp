// Decompiled by Opus, space-bunny-free, Claude Opus 5.5, Haiku, Sonnet 5.5, deepseek-v4.1-flash, GPT-6, GPT-6.1-sol, deepseek-v4.1 and GPT-5.6-Terra. Names are provisional.
// The HAPI file API: the CD drive helpers, the archive directory tree, the
// file handles and the path helpers around them.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <io.h>
#include <direct.h>
#include <stddef.h>

// Returns the letter of the first CD-ROM drive after the given drive letter
// (from 'A' when the letter is not a valid drive), or 0 when there is none.
// FUNCTION: 0x4bb190
char __stdcall FindNextCdDrive(char drive)
{
    char letter = toupper(drive);
    if (letter >= 'A' && letter <= 'Z')
        letter++;
    else
        letter = 'A';
    for (; letter <= 'Z'; letter++) {
        char root[4];
        sprintf(root, "%c:\\", letter);
        if (GetDriveTypeA(root) == DRIVE_CDROM)
            return letter;
    }
    return 0;
}

// FUNCTION: 0x4bb200
DWORD __stdcall GetVolumeNameAndSerial(char drive, LPSTR volumeName, DWORD volumeNameSize)
{
    char root[4];
    DWORD serial;
    DWORD flags;
    DWORD maxComponentLength;
    char fileSystemName[64];

    serial = 0;
    sprintf(root, "%c:\\", drive);
    GetVolumeInformationA(root, volumeName, volumeNameSize, &serial,
                          &maxComponentLength, &flags, fileSystemName, 64);
    return serial;
}

// FUNCTION: 0x4bb260
DWORD __stdcall GetVolumeSerial(char drive)
{
    char root[4];
    char volumeName[64];
    DWORD serial;
    DWORD maxComponentLength;
    DWORD flags;
    char fileSystemName[64];

    serial = 0;
    sprintf(root, "%c:\\", drive);
    GetVolumeInformationA(root, volumeName, 64, &serial, &maxComponentLength,
                          &flags, fileSystemName, 64);
    return serial;
}

// The archive's directory tree: entries are packed at 9 bytes each.
#pragma pack(push, 1)
struct ArchiveEntry {
    char* name;                          // +0x0
    void* child;                         // +0x4, a directory's list or a file's data
    unsigned char flags;                 // +0x8, bit 0: a directory
};
#pragma pack(pop)

struct ArchiveDirectory {
    int count;                           // +0x0
    ArchiveEntry* entries;               // +0x4
};

// An archive's header: the key at +0xc and the root directory at +0x10.
struct Node_004bb2e0 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
    ArchiveDirectory* list;              // +0x10
};

// A file's entry data: the block's offset, its 16.16 size and whether the
// data is compressed.
struct Tex_004bb2e0 {
    int offset;                          // +0x0
    int size;                            // +0x4
    unsigned char compressed;            // +0x8
};

// An open archive: the file, the read position, its header and the open count.
struct OPENHAPIFILE {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4, read position
    Node_004bb2e0* node;                 // +0x8
    int refCount;                        // +0xc
    int field_10;                        // +0x10
    char name[0x100];                    // +0x14
};

// The display context the open archives hang from.
struct State_004bb2e0 {
    char unknown_0[0x618];
    OPENHAPIFILE** items;                // +0x618
    int itemCount;                       // +0x61c
};

// A file the game has open, loose or inside an archive.
struct FileHandle {
    FILE* fp;                            // +0x0
    OPENHAPIFILE* shared;                // +0x4
    Tex_004bb2e0* info;                  // +0x8
    unsigned int pos;                    // +0xc
    int* buffer;                         // +0x10, the block sizes
    unsigned char* buffer2;              // +0x14, the current block
    char name[0x100];                    // +0x18

    void SetFileName(const char* text);
};

FileHandle* __stdcall HAPI_OpenFile(char* filename, const char* mode);

// Opens a file in "a+b" mode through HAPI_OpenFile (sibling of 0x4bb5b0).
// FUNCTION: 0x4bb2c0
void __stdcall HAPI_OpenFileAppend(void* param1)
{
    HAPI_OpenFile((char*)param1, "a+b");
}

State_004bb2e0* GetDisplay(void);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
// HAPI_FindEntry (0x4bb4e0) is gap code and stays in src/util/hpi_4bb4e0.cpp:
// tools/gapcheck.py sizes a region's functions by the next annotation in the
// file, so a file that also holds functions outside the region cannot be its
// source.
ArchiveEntry* __stdcall HAPI_FindEntry(ArchiveDirectory* list, char* path);

// Whole pixels of a 16.16 size, rounded up.
// Written as w / 65536 + (w % 65536 != 0) to expand to the original's code.
static inline int nblocks(int w)
{
    return w / 65536 + (w % 65536 != 0);
}

// Opens a file for reading (the mode comes from the caller: "rb" from 0x4bb5b0,
// "a+b" from 0x4bb2c0). If the file cannot be opened it falls back to the first
// already loaded item whose name tree has an entry without bit 0 of its flags
// set, reads that entry's texture block into a fresh "Block Sizes" buffer and
// un-obfuscates it in place.
// The block size is the 16.16 field rounded up to whole pixels, times 4.
// FUNCTION: 0x4bb2e0
FileHandle* __stdcall HAPI_OpenFile(char* filename, const char* mode)
{
    // Declared here in the order size, off, k, p: it fixes the operand order of the sums.
    int size;
    int off;
    int k;
    unsigned char* p;
    State_004bb2e0* state = GetDisplay();
    FileHandle* h = (FileHandle*)FUN_004d83b0("File Handle", 0x118);
    memset(h, 0, 0x118);
    strncpy(h->name, filename, 0x100);
    h->name[0xff] = 0;
    h->fp = fopen(filename, mode);
    if (h->fp) {
        h->shared = 0;
        return h;
    }
    for (int i = 0; i < state->itemCount; i++) {
        ArchiveEntry* e = HAPI_FindEntry(state->items[i]->node->list, filename);
        if (e == 0 || (e->flags & 1))
            continue;
        if (state->items[i]->fp == 0) {
            state->items[i]->fp = fopen(state->items[i]->name, "rb");
            if (state->items[i]->fp == 0) {
                FUN_004d85a0(h);
                return 0;
            }
            state->items[i]->pos = 0;
        }
        state->items[i]->refCount++;
        h->info = (Tex_004bb2e0*)e->child;
        h->shared = state->items[i];
        h->pos = 0;
        if (h->info->compressed) {
            size = nblocks(h->info->size) * 4;
            h->buffer = (int*)FUN_004d83b0("Block Sizes", size);
            off = h->info->offset;
            fseek(h->shared->fp, off, 0);
            fread(h->buffer, 1, size, h->shared->fp);
            h->shared->pos = off + size;
            p = (unsigned char*)h->buffer;
            unsigned char key = h->shared->node->obfuscate;
            if (key) {
                for (k = 0; k < size; k++)
                    p[k] = (unsigned char)(p[k] ^ 0xff ^ (unsigned char)(k + off) ^ key);
            }
        }
        return h;
    }
    FUN_004d85a0(h);
    return 0;
}

// FUNCTION: 0x4bb5b0
void __stdcall HAPI_OpenFileRead(void* param1)
{
    HAPI_OpenFile((char*)param1, "rb");
}

// FUNCTION: 0x4bb5d0
int __stdcall HAPI_CloseFile(FileHandle* file)
{
    int result;
    if (file->shared != 0) {
        result = 0;
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            result = fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        result = fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return result;
}

// FUNCTION: 0x4bb650
bool __stdcall HAPI_IsInArchive(FileHandle* obj)
{
    return obj->shared != 0;
}

// FUNCTION: 0x4bb670
void FileHandle::SetFileName(const char* text)
{
    strncpy(name, text, sizeof(name));
    name[sizeof(name) - 1] = 0;
}

// Creates a file for writing ("w+b") and wraps it in a zeroed "File Handle"
// that remembers the file name.
// FUNCTION: 0x4bb6a0
FileHandle* __stdcall HAPI_CreateFile(char* path)
{
    FILE* fp = fopen(path, "w+b");
    if (fp) {
        FileHandle* file = (FileHandle*)FUN_004d83b0("File Handle", sizeof(FileHandle));
        memset(file, 0, sizeof(FileHandle));
        strncpy(file->name, path, sizeof(file->name));
        file->name[sizeof(file->name) - 1] = 0;
        file->fp = fp;
        file->shared = 0;
        return file;
    }
    return 0;
}

// FUNCTION: 0x4bb710
long __stdcall HAPI_SeekFile(FileHandle* file, long pos)
{
    if (file->shared != 0) {
        unsigned int old = file->pos;
        if (pos == -1) {
            file->pos = file->info->size;
        } else {
            file->pos = pos;
        }
        if (((old ^ file->pos) & 0xffff0000) != 0 && file->buffer2 != 0) {
            FUN_004d85a0(file->buffer2);
            file->buffer2 = 0;
        }
        return 0;
    }
    int result;
    if (pos == -1) {
        result = fseek(file->fp, 0, SEEK_END);
    } else {
        result = fseek(file->fp, pos, SEEK_SET);
    }
    if (result != 0) {
        return -1;
    }
    return ftell(file->fp);
}

// FUNCTION: 0x4bb7a0
long __stdcall HAPI_TellFile(FileHandle* param_1)
{
    if (param_1->shared != 0) {
        return param_1->pos;
    }
    return ftell(param_1->fp);
}

int __stdcall SquashUnpack(unsigned char* dst, unsigned char* src);
char* __stdcall SquashErrorString(int code);
void __stdcall FatalError(char* text);

static inline int BlockOffset(FileHandle* file, int& counter, int b, int tableSize)
{
    int off = tableSize;
    for (counter = 0; counter < b; ++counter)
        off += file->buffer[counter];
    return off;
}

// FUNCTION: 0x4bb7c0
int __stdcall HAPI_readfromfile(FileHandle* file, unsigned char* buf, int size)
{
    int n;
    unsigned char* dst;
    int remaining;
    unsigned char* comp;
    int i;
    int blocks;
    int tableSize;
    int b;
    OPENHAPIFILE* item = file->shared;
    if (item != 0) {
        Tex_004bb2e0* info = file->info;
        n = size;
        if (info->size - (int)file->pos < n)
            n = info->size - (int)file->pos;
        if (info->compressed != 0) {
            i = 0;
            // Statement order i, blocks, tableSize, remaining decides where n is reloaded.
            blocks = (((n + file->pos - 1) & 0xffff0000) - (file->pos & 0xffff0000) >> 16) + 1;
            tableSize = ((info->size % 65536 != 0) + info->size / 65536) * 4;
            remaining = n;
            dst = buf;
            while (i < blocks) {
                b = file->pos >> 16;
                if (file->buffer2 == 0) {
                    int k;
                    int off = BlockOffset(file,k,b,tableSize);
                    off += file->info->offset;
                    fseek(file->shared->fp, off, 0);
                    file->shared->pos = off;
                    file->buffer2 = (unsigned char*)FUN_004d83b0("Uncompressed Block", 0x10000);
                    comp = (unsigned char*)FUN_004d83b0("Compressed Buffer", file->buffer[k]);
                    int got = fread(comp, 1, file->buffer[k], file->shared->fp);
                    if (got != file->buffer[k]) {
                        FUN_004d85a0(file->buffer2);
                        file->buffer2 = 0;
                        FUN_004d85a0(comp);
                        return -1;
                    }
                    file->shared->pos += got;
                    unsigned char key = file->shared->node->obfuscate;
                    if (key) {
                        for (int k = 0; k < got; k++)
                            comp[k] = (unsigned char)(comp[k] ^ 0xff ^ (unsigned char)(k + off) ^ key);
                    }
                    int err = SquashUnpack(file->buffer2, comp);
                    if (err) {
                        char msg[1000];
                        sprintf(msg, "[HAPI_readfromfile] Decompression Error: %s\n", SquashErrorString(err));
                        sprintf(msg + strlen(msg), "block %d of %d\n", i, blocks);
                        sprintf(msg + strlen(msg), "base name '%s'\n", file->shared->name);
                        sprintf(msg + strlen(msg), "length = %d\n", size);
                        sprintf(msg + strlen(msg), "name = '%s'\n", file->name);
                        FatalError(msg);
                    }
                    FUN_004d85a0(comp);
                }
                int chunk = ((b + 1) << 16) - file->pos;
                if (chunk > remaining)
                    chunk = remaining;
                int o = file->pos & 0xffff;
                if (chunk == 1)
                    *dst = file->buffer2[o];
                else
                    memcpy(dst, file->buffer2 + o, chunk);
                dst += chunk;
                HAPI_SeekFile(file, file->pos + chunk);
                remaining -= chunk;
                i++;
            }
            goto done;
        }
        // Read through file->info, not the cached info local.
        int off = file->pos + file->info->offset;
        if (off != item->pos) {
            fseek(item->fp, off, 0);
            file->shared->pos = off;
        }
        n = fread(buf, 1, n, file->shared->fp);
        unsigned char key = file->shared->node->obfuscate;
        if (key) {
            for (int i = 0; i < n; i++)
                buf[i] = (unsigned char)(buf[i] ^ 0xff ^ (unsigned char)(i + off) ^ key);
        }
        if (n < 0)
            goto done;
        file->pos += n;
        file->shared->pos += n;
        goto done;
    }
    n = fread(buf, 1, size, file->fp);
done:
    return n;
}

// FUNCTION: 0x4bbbe0
unsigned int __stdcall HAPI_WriteFile(FileHandle* param_1, void* param_2, unsigned int param_3)
{
    if (param_1->shared != 0)
        return 0xffffffff;
    return fwrite(param_2, 1, param_3, param_1->fp);
}

// FUNCTION: 0x4bbc10
void __stdcall RenameFile(const char* param_1, const char* param_2)
{
    rename(param_1, param_2);
}

// __stdcall wrapper around the CRT's remove (delete a file), like its
// neighbour 0x4bbc10 (rename). remove and _rmdir are the same code apart from
// the function they import (DeleteFileA, RemoveDirectoryA); the original
// calls remove at 0x4e7990.
// FUNCTION: 0x4bbc30
void __stdcall RemoveFile(const char* path)
{
    remove(path);
}

// Closes a file handle opened by HAPI_OpenFile and returns its size.
// Sibling of 0x4bbd00, which reports the size without closing.
// FUNCTION: 0x4bbc40
long __stdcall HAPI_FileLengthByName(char* param_1)
{
    FileHandle* h = (FileHandle*)HAPI_OpenFile(param_1, "rb");
    if (h == 0)
        return 0;

    long len;
    if (h->shared != 0)
        len = h->info->size;
    else if (h->fp != 0)
        len = _filelength(h->fp->_file);
    else
        len = 0;

    if (h->shared != 0) {
        h->shared->refCount--;
        if (h->shared->refCount == 0 && h->shared->field_10 == 0) {
            fclose(h->shared->fp);
            h->shared->fp = 0;
        }
    } else {
        fclose(h->fp);
    }

    if (h->buffer != 0)
        FUN_004d85a0((void*)h->buffer);
    if (h->buffer2 != 0)
        FUN_004d85a0((void*)h->buffer2);
    FUN_004d85a0(h);

    return len;
}

// FUNCTION: 0x4bbd00
long __stdcall HAPI_FileLength(FileHandle* param_1)
{
    if (param_1->shared != 0)
        return param_1->info->size;
    if (param_1->fp != 0)
        return _filelength(param_1->fp->_file);
    return 0;
}

// Opens a file, seeks to an offset and reads a block into the caller's
// buffer; on success the buffer is returned, otherwise null. The file handle
// is closed (and its shared refcount dropped) on both paths. The refcount,
// fclose and buffer-free blocks are the inlined body of 0x4bb5d0.
// FUNCTION: 0x4bbd30
void* __stdcall HAPI_ReadFileAt(char* name, void* buffer, long pos, unsigned int size)
{
    FileHandle* file = HAPI_OpenFile(name, "rb");
    if (file == 0) {
        return 0;
    }
    if (HAPI_SeekFile(file, pos) == -1) {
        goto fail;
    }
    if (HAPI_readfromfile(file, (unsigned char*)buffer, size) <= 0) {
        goto fail;
    }
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return buffer;

fail:
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
            fclose(file->shared->fp);
            file->shared->fp = 0;
        }
    } else {
        fclose(file->fp);
    }
    if (file->buffer != 0) {
        FUN_004d85a0(file->buffer);
    }
    if (file->buffer2 != 0) {
        FUN_004d85a0(file->buffer2);
    }
    FUN_004d85a0((int*)file);
    return 0;
}

// Loads a whole file into a named heap block: opens the name (falling back to
// an already loaded item through HAPI_OpenFile), takes the length from the
// shared item's info block or from the file descriptor, rewinds, copies the
// name into a local buffer, strips the directory from it (the same in-place
// strip as 0x4bb150, so the leaf name is what names the block), allocates the
// block, reads into it and stores the length through `size`. `size` is set to
// -1 first, and again after the open, so a caller that only wanted the size
// sees a failure. The tail is HAPI_CloseFile (close, release the two block
// pointers, free the handle) written out inline, with its fclose results dead
// because the return value is the block.
// FUNCTION: 0x4bbe50
char* __stdcall HAPI_LoadFile(char* name, int* size)
{
    char buf[0x100];
    char* data = 0;
    FileHandle* f;
    int len;

    if (size) {
        *size = -1;
    }
    f = HAPI_OpenFile(name, "rb");
    if (!f) {
        return 0;
    }
    if (size) {
        *size = -1;
    }
    if (f->shared) {
        len = f->info->size;
    } else if (f->fp) {
        len = _filelength(_fileno(f->fp));
    } else {
        len = 0;
    }
    // An if / else if / else chain with the failure arms first, not gotos or success-first.
    if (len <= 0) {
        data = 0;
    } else if (HAPI_SeekFile(f, 0) == -1) {
        data = 0;
    } else {
        strcpy(buf, name);
        {
            // Two indices into the same array: pointers would peel the first iteration.
            int n = (int)strlen(buf);
            int i = n - 1;
            while (i >= 0 && buf[i] != '\\') {
                i--;
            }
            int j = i + 1;
            int k = 0;
            do {
                buf[k] = buf[j];
                k++;
            } while (buf[j++] != 0);
        }
        data = (char*)FUN_004d83b0(buf, len);
        if (HAPI_readfromfile(f, (unsigned char*)data, len) <= 0) {
            FUN_004d85a0(data);
            data = 0;
            goto close;
        }
        if (size) {
            *size = len;
        }
    }
close:
    if (f->shared != 0) {
        f->shared->refCount--;
        if (f->shared->refCount == 0 && f->shared->field_10 == 0) {
            fclose(f->shared->fp);
            f->shared->fp = 0;
        }
    } else {
        fclose(f->fp);
    }
    if (f->buffer != 0) {
        FUN_004d85a0(f->buffer);
    }
    if (f->buffer2 != 0) {
        FUN_004d85a0(f->buffer2);
    }
    FUN_004d85a0(f);
    return data;
}

// Loads a whole file into a freshly allocated buffer. It takes the length from
// the file handle (cached size when the handle is shared, else _filelength),
// rejects an empty file, seeks to the start, copies the path into a local
// buffer, strips the directory with a backward scan for '\\', allocates that
// many bytes tagged with the base name, reads the file into them and returns
// the buffer. The size is written back through the third argument, which is
// preset to -1 so a failure leaves it at -1.
// FUNCTION: 0x4bbff0
void* __stdcall HAPI_LoadOpenFile(char* name, FileHandle* file, unsigned int* outSize)
{
    long len;
    char buf[256];
    void* data;
    int n;
    int i;
    int j;
    int k;

    if (outSize != 0) {
        *outSize = 0xffffffff;
    }
    if (file->shared != 0) {
        len = file->info->size;
    } else if (file->fp != 0) {
        len = _filelength(file->fp->_file);
    } else {
        len = 0;
    }
    if (len <= 0) {
        return 0;
    }
    if (HAPI_SeekFile(file, 0) == -1) {
        return 0;
    }
    strcpy(buf, name);
    n = strlen(buf);
    i = n - 1;
    while (i >= 0 && buf[i] != '\\') {
        i--;
    }
    j = i + 1;
    k = 0;
    // Two indices into the same array, not pointers.
    do {
        buf[k] = buf[j];
        k++;
    } while (buf[j++] != 0);
    data = FUN_004d83b0(buf, len);
    if (HAPI_readfromfile(file, (unsigned char*)data, len) <= 0) {
        FUN_004d85a0(data);
        return 0;
    }
    if (outSize != 0) {
        *outSize = len;
    }
    return data;
}

// Reads a whole file into the caller's buffer: open "rb", take its size, seek
// to the start and read that many bytes. The buffer is returned when the read
// succeeds; a read of zero bytes frees it and returns null. On any failure the
// file is closed and null is returned.
// FUNCTION: 0x4bc120
int* __stdcall HAPI_LoadFileInto(char* param_1, int* param_2)
{
    FileHandle* file = HAPI_OpenFile(param_1, "rb");
    long size;
    if (file == 0)
        goto fail;
    if (file->shared != 0)
        size = file->info->size;
    else if (file->fp != 0)
        size = _filelength(file->fp->_file);
    else
        size = 0;
    if (size <= 0)
        goto fail;
    if (HAPI_SeekFile(file, 0) < 0)
        goto fail;
    size = HAPI_readfromfile(file, (unsigned char*)param_2, size);
    if (size < 0)
        goto fail;
    if (file->shared != 0) {
        file->shared->refCount--;
        if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
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
    if (size <= 0) {
        FUN_004d85a0(param_2);
        return 0;
    }
    return param_2;
fail:
    if (file != 0) {
        if (file->shared != 0) {
            file->shared->refCount--;
            if (file->shared->refCount == 0 && file->shared->field_10 == 0) {
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
    }
    return 0;
}

// Writes a buffer to a new file; returns the number of bytes written or -1.
// FUNCTION: 0x4bc290
int __stdcall WriteBufferToFile(char* filename, void* data, int size)
{
    FILE* f = fopen(filename, "wb");
    if (f == NULL) {
        return -1;
    }
    int written = fwrite(data, 1, size, f);
    fclose(f);
    return written;
}

// Writes the current drive letter ("C") into buf as a string.
// FUNCTION: 0x4bc2e0
void __stdcall GetCurrentDriveLetter(char* buf)
{
    buf[0] = _getdrive() + '@';
    buf[1] = 0;
}

// FUNCTION: 0x4bc300
int __stdcall ChangeDrive(char* param_1)
{
    if (param_1 == 0) {
        return -1;
    }
    return _chdrive(*param_1 - 0x40);
}

// Gets the current directory of a drive ("C:..." or the current drive when
// null) into buf and returns buf. The drive number is computed on each path
// (`- '@'` in both branches, tail-merged into one `sub`); a single `d - '@'`
// after the if gives `add eax, -0x40` scheduled after the first push.
// FUNCTION: 0x4bc320
char* __stdcall GetDriveDirectory(char* drive, char* buf, int size)
{
    int n;
    if (drive == NULL)
        n = (char)(_getdrive() + '@') - '@';
    else
        n = *drive - '@';
    _getdcwd(n, buf, size);
    return buf;
}

// __stdcall wrapper around the CRT's _chdir, like 0x4bbc30 (_rmdir).
// FUNCTION: 0x4bc360
void __stdcall ChangeDirectory(const char* path)
{
    _chdir(path);
}

// Case-insensitive wildcard match of a string against a pattern that may
// contain '?' (any one character) and '*' (zero or more characters). The
// pattern positions still alive after each input character are kept in a
// stack of at most 100 entries, so a '*' backtracks in linear time.
// Needed for the operand order of the pat[stack[i]] load.
// FUNCTION: 0x4bc370
int __stdcall MatchWildcard(const char* str, const char* pat)
{
    int stack[100];
    int n;
    int i;
    char c;

    stack[0] = 0;
    n = 1;
    while ((c = toupper(*str++)) != 0) {
        for (i = 0; i < n; i++) {
            int idx = stack[i];
            // Subscripted by stack[i], not idx: the element must be read from memory.
            char pc = toupper(pat[stack[i]]);
            if (pc == '?' || pc == c) {
                stack[i] = idx + 1;
            } else if (pc == '*') {
                if (n < 100)
                    stack[n++] = idx + 1;
            } else {
                if (--n == 0)
                    return 0;
                stack[i] = stack[n];
                i--;
            }
        }
    }
    for (i = 0; i < n; i++) {
        char pc = pat[stack[i]];
        if (pc == 0)
            return 1;
        if (pc == '*' && pat[stack[i] + 1] == 0)
            return 1;
    }
    return 0;
}

// The path helpers before 0x4bb190 (0x4bb0a0 to 0x4bb150, in their own files):
// the symbol ids these declarations take keep 0x4bb2e0's allocation
// (docs/c2-regalloc.md).
int __stdcall HasExtension(char* name);
char* __stdcall StripExtension(char* name);
char* __stdcall StripFileName(char* name);
char* __stdcall StripPath(char* name);
