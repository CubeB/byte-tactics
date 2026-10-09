// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by opus. Names are provisional.
// Stays in its own file: joined into hpi.cpp, its register allocation changes
// (the entry-name strlen takes another register with the join's symbol ids),
// and the bytes only match with this file's declarations.
// Writes the file data of one package directory, recursing into
// subdirectories. For each file entry it records the data offset, size and
// compression flag in the directory buffer, then copies the file, either as
// 64K blocks packed by SquashPack behind a table of block sizes, or raw in
// 4K pieces, scrambling the bytes when a key is set.
#include <stdio.h>
#include <string.h>
#include <io.h>
#include "archive_entry.h"

#pragma pack(push, 1)
struct Node_004bd830 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};

struct OPENHAPIFILE {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4
    Node_004bd830* node;                 // +0x8
    int count;                           // +0xc
    int unknown_10;                      // +0x10
    char name[0x100];                    // +0x14
};

struct Info {
    long offset;                         // +0x0
    unsigned int size;                   // +0x4
    unsigned char compressed;            // +0x8
};
#pragma pack(pop)

#include "file_handle.h"

struct ArchiveDirectory;

FileHandle* __stdcall HAPI_OpenFile(char* filename, const char* mode);
int __stdcall HAPI_readfromfile(FileHandle* file, unsigned char* buf, int size);
int __stdcall SquashPack(void* chunk, int* chunkSize, char* data,
                           int size, int method, int encrypt);
unsigned int __stdcall SquashMaxPackedSize(unsigned int value, int mode);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
// This forward declaration, Node_004bd830 and the casts in nblocks all stay.
void __stdcall HAPI_WriteArchiveData(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags);

// The handle's file size, copied from hpi.cpp (0x4bbd00) so it is inlined
// below.
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
                table = (int*)GameAllocIgnoreTag("Block Sizes", blocks * 4);
                fwrite(table, blocks, 4, f);
                packlen = SquashMaxPackedSize(0x10000, flags & 0xff);
                pack = (unsigned char*)GameAllocIgnoreTag("Pack Buffer", packlen);
                data = (unsigned char*)GameAllocIgnoreTag("Data Buffer", 0x10000);
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
                GameFreeThunk(table);
                GameFreeThunk(pack);
                GameFreeThunk(data);
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
                if (file->shared->count == 0 && file->shared->unknown_10 == 0) {
                    fclose(file->shared->fp);
                    file->shared->fp = 0;
                }
            } else {
                fclose(file->fp);
            }
            if (file->buffer != 0)
                GameFreeThunk(file->buffer);
            if (file->buffer2 != 0)
                GameFreeThunk(file->buffer2);
            GameFreeThunk(file);
            if (cb != 0)
                cb(5 + (unsigned)(90 * info->offset - *(int*)(8 + base) * 90) / extra);
        }
    }
}
