// Decompiled by Sonnet 5.5, finished by deepseek-v4.1-flash, GPT-6, GPT-6.1-sol, and deepseek-v4.1. Names are provisional.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Node_004bb7c0 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
};

struct OPENHAPIFILE {
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

struct FileHandle {
    FILE* fp;                            // +0x0
    OPENHAPIFILE* shared;                // +0x4
    Info_004bb7c0* info;                 // +0x8
    unsigned int pos;                    // +0xc
    int* buffer;                         // +0x10, block sizes
    unsigned char* buffer2;              // +0x14, the current block
    char name[0x100];                    // +0x18
};
#pragma pack(pop)

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
long __stdcall HAPI_SeekFile(FileHandle* file, long pos);
int __stdcall SquashUnpack(unsigned char* dst, unsigned char* src);
char* __stdcall SquashErrorString(int code);
void __stdcall FatalError(char* text);

static inline int nblocks(int w)
{
    return w / 65536 + (w % 65536 != 0);
}

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
        Info_004bb7c0* info = file->info;
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
