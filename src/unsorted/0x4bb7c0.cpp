// Decompiled by Sonnet 5.5. Names are provisional.
// Reads up to `size` bytes at the file's position into `buf` and returns the
// count. A plain file is one fread. A file inside a loaded archive item is read
// from the item's stream: an uncompressed entry with a single fread (after a
// seek when the item's stream is elsewhere) and the archive's obfuscation
// undone in place; a compressed entry in 64 KiB blocks, each block read into a
// "Compressed Buffer" (offset = the block size table plus the sizes of the
// blocks before it), un-obfuscated, decompressed by FUN_004d1970 into the
// "Uncompressed Block" buffer at +0x14 and then copied out (the buffer is kept
// until the position leaves the block). A decompression error is reported
// through FUN_004b6290 with a multi-line message. A short read of a
// compressed block returns -1.
//
// NOT MATCHED: 46.0%, 1008 of 1042 bytes. The code follows the original's
// shape (the shared-item test first, compressed path before the uncompressed
// one, the block loop, the five appended error lines) but the register
// assignment and frame differ: the original keeps the item in edi and the
// entry info in esi (here esi and ebx), its frame is 0x40c with the locals at
// [esp+0x13] (a byte copy of the obfuscation key), 0x14 (n), 0x18 (dst), 0x1c
// (remaining), 0x20 (compressed buffer), 0x24 (block counter), 0x28 (block
// count), 0x2c (size table bytes) and 0x30 (block index) followed by the
// message buffer; declaring locals in that order did not reproduce it.
// File.pos has to be unsigned (the block index is `shr 16`) but the room test
// `info->size - pos < n` is signed.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Node_004bb7c0 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};

struct Item_004bb7c0 {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4
    Node_004bb7c0* node;                 // +0x8
    int count;                           // +0xc
    char unknown_10[4];
    char name[0x100];                    // +0x14
};

struct Info_004bb7c0 {
    int offset;                          // +0x0
    int size;                            // +0x4
    unsigned char compressed;            // +0x8
};

struct File_004bb7c0 {
    FILE* fp;                            // +0x0
    Item_004bb7c0* shared;               // +0x4
    Info_004bb7c0* info;                 // +0x8
    unsigned int pos;                    // +0xc
    int* buffer;                         // +0x10, block sizes
    unsigned char* buffer2;              // +0x14, the current block
    char name[0x100];                    // +0x18
};
#pragma pack(pop)

void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);
long __stdcall FUN_004bb710(File_004bb7c0* file, long pos);
int __stdcall FUN_004d1970(unsigned char* dst, unsigned char* src);
char* __stdcall FUN_004d1c60(int code);
void __stdcall FUN_004b6290(char* text);

static inline int nblocks(int w)
{
    return w / 65536 + (w % 65536 != 0);
}

// FUNCTION: 0x4bb7c0
int __stdcall FUN_004bb7c0(File_004bb7c0* file, unsigned char* buf, int size)
{
    int n;
    unsigned char* dst;
    int remaining;
    unsigned char* comp;
    int i;
    int blocks;
    int tableSize;
    int b;
    Item_004bb7c0* item = file->shared;
    if (item != 0) {
        Info_004bb7c0* info = file->info;
        n = size;
        if (info->size - (int)file->pos < n)
            n = info->size - (int)file->pos;
        if (info->compressed != 0) {
            remaining = n;
            i = 0;
            blocks = (((n + file->pos - 1) & 0xffff0000) - (file->pos & 0xffff0000) >> 16) + 1;
            tableSize = nblocks(info->size) * 4;
            dst = buf;
            while (i < blocks) {
                b = file->pos >> 16;
                if (file->buffer2 == 0) {
                    int off = tableSize;
                    for (int k = 0; k < b; k++)
                        off += file->buffer[k];
                    off += file->info->offset;
                    fseek(file->shared->fp, off, 0);
                    file->shared->pos = off;
                    file->buffer2 = (unsigned char*)FUN_004d83b0("Uncompressed Block", 0x10000);
                    comp = (unsigned char*)FUN_004d83b0("Compressed Buffer", file->buffer[b]);
                    int got = fread(comp, 1, file->buffer[b], file->shared->fp);
                    if (got != file->buffer[b]) {
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
                    int err = FUN_004d1970(file->buffer2, comp);
                    if (err) {
                        char msg[0x3f0];
                        sprintf(msg, "[HAPI_readfromfile] Decompression Error: %s\n", FUN_004d1c60(err));
                        sprintf(msg + strlen(msg), "block %d of %d\n", i, blocks);
                        sprintf(msg + strlen(msg), "base name '%s'\n", file->shared->name);
                        sprintf(msg + strlen(msg), "length = %d\n", size);
                        sprintf(msg + strlen(msg), "name = '%s'\n", file->name);
                        FUN_004b6290(msg);
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
                FUN_004bb710(file, file->pos + chunk);
                remaining -= chunk;
                i++;
            }
            return n;
        }
        int off = file->pos + info->offset;
        if (off != item->pos) {
            fseek(item->fp, off, 0);
            item->pos = off;
        }
        n = fread(buf, 1, n, item->fp);
        unsigned char key = file->shared->node->obfuscate;
        if (key) {
            for (int i = 0; i < n; i++)
                buf[i] = (unsigned char)(buf[i] ^ 0xff ^ (unsigned char)(i + off) ^ key);
        }
        if (n < 0)
            return n;
        file->pos += n;
        file->shared->pos += n;
        return n;
    }
    return fread(buf, 1, size, file->fp);
}
