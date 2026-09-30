// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 56.4%. Frame size and the name/full/buffer offsets now match the
// original (0x123c, esp+0x44/0x148/0x24c). What still differs is stack-slot
// coloring and register allocation in the entry loop: the original keeps
// nameoff at +0x20 and i at +0x34 and recoff as a real slot +0x3c, while MSVC
// here colors nameoff to +0x28 and i to +0x38 and rematerialises *recoff as
// *(base+off+4); consequently base lands in ebx (original ebp) and the entry
// pointer in ebp (original ebx), and all downstream register choices follow.
// Tried: branch order (fixed, see below), for-loop with an inline count deref
// (exact 1332-byte size but 51.8%, shifts the frame to 0x1238), count as a
// local with do-while (best 56.4%), assignment order i/nameoff/recoff swapped
// (no change). The entry loop has nameoff += 9 and i++ in the latch, the
// count is the inline deref *(unsigned*)(base + off).
// Body structure (path strcpy/strcat, entry loop, uncompressed 0x1000 chunk
// copy, compressed block table with FUN_004d1820, table re-obfuscation,
// fclose/free, progress callback) matches. nblocks(size) inline and the entry
// pointer reused as `long* dataptr` are in place. The flags test puts the
// compressed path first (if (flags) { compressed } else { uncompressed }),
// which is what made the original fall through into compression.
#include <stdio.h>
#include <string.h>
#include <io.h>

#pragma pack(push, 1)
struct Node_004bd830 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};

struct Item_004bd830 {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4
    Node_004bd830* node;                 // +0x8
    int count;                           // +0xc
    int unknown_10;                      // +0x10
    char name[0x100];                    // +0x14
};

struct Info_004bd830 {
    int offset;                          // +0x0
    int size;                            // +0x4
    unsigned char compressed;            // +0x8
};

struct File_004bd830 {
    FILE* fp;                            // +0x0
    Item_004bd830* shared;               // +0x4
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
int __stdcall FUN_004d1820(void* chunk, unsigned int* chunkSize, char* data,
                           int size, int method, int encrypt);
unsigned int __stdcall FUN_004d1aa0(unsigned int value, int mode);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004bd830(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags);

// Whole 64K blocks of a byte size, rounded up.
static inline int nblocks(unsigned w)
{
    return w / 65536 + (w % 65536 != 0);
}

// FUNCTION: 0x4bd830
void __stdcall FUN_004bd830(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags)
{
    char name[260];
    char full[260];
    unsigned char buffer[0x1000];
    long* dataptr;
    unsigned remaining;
    unsigned char* data;
    int* tp;
    int nameoff;
    int* table;
    unsigned clen;
    File_004bd830* file;
    int n;
    unsigned i;
    long pos;
    int* recoff;
    unsigned packlen;

    strcpy(name, path);
    if (name[strlen(name) - 1] != '\\')
        strcat(name, "\\");

    unsigned count = *(unsigned*)(base + off);
    if (count != 0) {
        i = 0;
        nameoff = 0;
        recoff = (int*)(base + off + 4);
        do {
            Entry_004bd830* e = (Entry_004bd830*)(base + nameoff + *recoff);
            strcpy(full, name);
            strcat(full, (char*)(base + e->name));
            if ((e->flags & 1) == 0) {
                file = FUN_004bb2e0(full, "rb");
                dataptr = (long*)(base + e->offset);
                *dataptr = ftell(f);
                unsigned size;
                if (file->shared == 0) {
                    if (file->fp == 0)
                        size = 0;
                    else
                        size = _filelength(_fileno(file->fp));
                } else {
                    size = file->info->size;
                }
                *(unsigned*)(dataptr + 1) = size;
                *((char*)dataptr + 8) = (char)flags;
                if ((char)flags != 0) {
                    int blocks = nblocks(size);
                    table = (int*)FUN_004d83b0("Block Sizes", blocks * 4);
                    fwrite(table, blocks, 4, f);
                    packlen = FUN_004d1aa0(0x10000, flags & 0xff);
                    unsigned char* pack = (unsigned char*)FUN_004d83b0("Pack Buffer", packlen);
                    data = (unsigned char*)FUN_004d83b0("Data Buffer", 0x10000);
                    remaining = size;
                    n = blocks;
                    tp = table;
                    if (blocks > 0) {
                        do {
                            unsigned chunk = remaining < 0x10000 ? remaining : 0x10000;
                            FUN_004bb7c0(file, data, chunk);
                            clen = packlen;
                            FUN_004d1820(pack, &clen, (char*)data, chunk, flags & 0xff, 1);
                            *tp = clen;
                            pos = ftell(f);
                            if ((char)key != 0) {
                                for (unsigned j = 0; j < clen; j++)
                                    pack[j] = (unsigned char)~((char)pos + (char)j
                                              ^ (char)key ^ pack[j]);
                            }
                            fwrite(pack, clen, 1, f);
                            tp++;
                            remaining -= 0x10000;
                            n--;
                        } while (n != 0);
                    }
                    fseek(f, *dataptr, 0);
                    pos = ftell(f);
                    if ((char)key != 0) {
                        for (unsigned j = 0; j < (unsigned)(blocks * 4); j++)
                            ((unsigned char*)table)[j] = (unsigned char)~((char)pos
                                + (char)j ^ (char)key ^ ((unsigned char*)table)[j]);
                    }
                    fwrite(table, blocks, 4, f);
                    fseek(f, 0, 2);
                    FUN_004d85a0(table);
                    FUN_004d85a0(pack);
                    FUN_004d85a0(data);
} else {
                    while (size != 0) {
                        unsigned chunk = size < 0x1000 ? size : 0x1000;
                        FUN_004bb7c0(file, buffer, chunk);
                        pos = ftell(f);
                        if ((char)key != 0) {
                            for (unsigned j = 0; j < chunk; j++)
                                buffer[j] = (unsigned char)~((char)pos + (char)j
                                            ^ (char)key ^ buffer[j]);
                        }
                        fwrite(buffer, chunk, 1, f);
                        size -= chunk;
                    }
}
                if (file->shared == 0) {
                    fclose(file->fp);
                } else {
                    file->shared->count--;
                    if (file->shared->count == 0 && file->shared->unknown_10 == 0) {
                        fclose(file->shared->fp);
                        file->shared->fp = 0;
                    }
                }
                if (file->buffer != 0)
                    FUN_004d85a0(file->buffer);
                if (file->buffer2 != 0)
                    FUN_004d85a0(file->buffer2);
                FUN_004d85a0(file);
                if (cb != 0)
                    cb((unsigned)(*dataptr * 90 - *(int*)(base + 8) * 90) / extra + 5);
            } else {
                FUN_004bd830(full, base, e->offset, f, cb, extra, key, flags);
            }
            nameoff += 9;
            i++;
        } while (i < count);
    }
}
