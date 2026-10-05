// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-sonnet-5-5, finished by DeepSeek V4.1 Flash, finished by opus. Names are provisional.
// MATCH. Writes the file data of one package directory, recursing into
// subdirectories. For each file entry it records the data offset, size and
// compression flag in the directory buffer, then copies the file, either as
// 64K blocks packed by FUN_004d1820 behind a table of block sizes, or raw in
// 4K pieces, scrambling the bytes when a key is set.
// What made it match, after sessions stuck between 66 and 81 percent:
//  - FUN_004bbd00 (the handle's file size) is defined in this file, as in the
//    original translation unit, and /Ob2 inlines it. Its result goes straight
//    into info->size, so the size lives in a scratch register (ecx) and every
//    later use re-reads info->size: the block loop's remaining count is
//    reloaded from [info+4] after the allocations. A `size` local put it in a
//    callee-saved register instead.
//  - Both loops are plain indexed for-loops that MSVC strength-reduces. The
//    block loop writes table[n], which becomes a walking pointer plus a
//    down-counter, and the directory loop indexes the entry array with i,
//    which becomes the hidden i*9 offset ([esp+0x20]) and the hoisted record
//    array pointer ([esp+0x3c]); the count is re-read from the directory
//    header on every test. Writing those pointers and counters by hand, as
//    earlier versions did, always left one slot wrong. Together with the
//    previous item: 80.8 -> 98.4.
//  - `remaining -= 0x10000` belongs in the for-increment after n++. As the
//    last body statement it swaps the frame slots of remaining and the table
//    pointer (98.4 -> MATCH).
//  - Declarations that emit nothing still decide the last few bytes (the
//    compiler-state effect of docs/field-notes.md section 10): the forward
//    declaration of this function, the Node struct behind Shared::node and
//    the casts in nblocks are each needed, and dropping any one of them
//    scores 94 to 97 percent (1334 bytes).
// FUN_004bb5d0 (the handle close) is not inlined by MSVC 5 even when defined
// here, so the close sequence near the end is written out by hand.
// Earlier notes called the FUN_004d1820 size argument a bug (a heap address
// passed as the limit). It is not: 0x4bda5e stores FUN_004d1aa0's result
// (packlen) before the "Pack Buffer" call, and the loop copies packlen into
// clen before passing &clen.
#include <stdio.h>
#include <string.h>
#include <io.h>

#pragma pack(push, 1)
struct Node_004bd830 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};

struct Shared_004bd830 {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4
    Node_004bd830* node;                 // +0x8
    int count;                           // +0xc
    int unknown_10;                      // +0x10
    char name[0x100];                    // +0x14
};

struct Info_004bd830 {
    long offset;                         // +0x0
    unsigned int size;                   // +0x4
    unsigned char compressed;            // +0x8
};

struct File_004bd830 {
    FILE* fp;                            // +0x0
    Shared_004bd830* shared;             // +0x4
    Info_004bd830* info;                 // +0x8
    unsigned int pos;                    // +0xc
    int* buffer;                         // +0x10
    unsigned char* buffer2;              // +0x14
    char name[0x100];                    // +0x18
};

struct Entry_004bd830 {
    int name;                            // +0x0, offset of the name string
    int offset;                          // +0x4, offset of the record data
    unsigned char flags;                 // +0x8
};
#pragma pack(pop)

File_004bd830* __stdcall FUN_004bb2e0(char* filename, const char* mode);
int __stdcall FUN_004bb7c0(File_004bd830* file, unsigned char* buf, int size);
int __stdcall FUN_004d1820(void* chunk, int* chunkSize, char* data,
                           int size, int method, int encrypt);
unsigned int __stdcall FUN_004d1aa0(unsigned int value, int mode);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004bd830(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags);

// The handle's file size, from the same translation unit (its own file is
// src/util/hpi_4bbd00.cpp), so it is inlined below.
long __stdcall FUN_004bbd00(File_004bd830* file)
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
void __stdcall FUN_004bd830(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags)
{
    Info_004bd830* info;
    int len;
    Entry_004bd830* e;
    unsigned size;
    char name[260];
    char full[260];
    unsigned char buffer[0x1000];
    int n, * table;
    int clen;
    unsigned remaining, i;
    File_004bd830* file;
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
        e = (Entry_004bd830*)(base + *(int*)(off + base + 4)) + i;
        strcpy(full, name);
        strcat(full, base + e->name);
        if ((e->flags & 1) != 0) {
            FUN_004bd830(full, base, e->offset, f, cb, extra, key, flags);
        } else {
            file = FUN_004bb2e0(full, "rb");
            info = (Info_004bd830*)(base + e->offset);
            info->offset = ftell(f);
            info->size = FUN_004bbd00(file);
            info->compressed = (char)flags;
            if ((char)flags) {
                int blocks = nblocks(info->size);
                table = (int*)FUN_004d83b0("Block Sizes", blocks * 4);
                fwrite(table, blocks, 4, f);
                packlen = FUN_004d1aa0(0x10000, flags & 0xff);
                pack = (unsigned char*)FUN_004d83b0("Pack Buffer", packlen);
                data = (unsigned char*)FUN_004d83b0("Data Buffer", 0x10000);
                remaining = info->size;
                for (n = 0; n < blocks; n++, remaining -= 0x10000) {
                    int chunk;
                    chunk = remaining >= 0x10000 ? 0x10000 : remaining;
                    FUN_004bb7c0(file, data, chunk);
                    clen = packlen;
                    FUN_004d1820(pack, &clen, (char*)data, chunk, flags & 0xff, 1);
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
                    FUN_004bb7c0(file, buffer, chunk);
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
                FUN_004d85a0(file->buffer);
            if (file->buffer2 != 0)
                FUN_004d85a0(file->buffer2);
            FUN_004d85a0(file);
            if (cb != 0)
                cb(5 + (unsigned)(90 * info->offset - *(int*)(8 + base) * 90) / extra);
        }
    }
}
