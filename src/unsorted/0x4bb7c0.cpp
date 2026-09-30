// Decompiled by Sonnet 5.5, finished by deepseek-v4.1-flash, GPT-6, and GPT-6.1-sol. Names are provisional.
// Retry #1764: GPT-6.1-sol confirmed 98.6%; deepseek-v4.1-flash retry #1749 fixed
// one of the two diffs: 99.2% (1042 bytes, exact size), one hunk left.
// Fix for the old hunk 2: the `Info* info = file->info` local kept `esi`
// occupied and made MSVC accumulate the non-compressed `off` in ebx
// (mov ebx,[esi]; add ebx,eax). Removing the local and writing `file->info->`
// inline at every use lets ebx hold `off` and eax accumulate, matching the
// original (add eax,[esi]; mov ebx,eax).
// Fix for the last hunk (MATCH): `remaining = n;` was written before the
// `blocks` and `tableSize` computations, so n's live range ended inside the
// block-count expression and the allocator coalesced the n reload straight
// into the `lea` destination (esi), then into edx once the use was extended
// past the lea. Moving `remaining = n;` after `tableSize = ...` keeps n live
// across the tableSize `cdq`, so neither esi nor edx is available for the
// reload and it lands in ebx exactly as the original:
//   mov ebx,[esp+0x14]; mov [esp+0x1c],ebx; lea esi,[ebx+eax-1].
// The same MATCH is reached with `remaining = n;` after `dst = buf;` or with
// `i = 0;` moved with it; the position of the last use of n, not the
// expression, decides the register. Flat before this: swapped commutative
// operand, all 128 headers.py sets, every local-declaration permutation (guide
// 1793), `int n = size` init, inline/nested declarations of n/remaining/i, a
// function-scope `off`, and an inline Sum2 helper. Removing the `info` local
// is what moved the previous hunk.
// The block-count/table-size order is load bearing: writing tableSize as
// ((size % 65536 != 0) + size / 65536) * 4 (modulo first) makes blocks land in
// esi and tableSize in edi as the original does. The helper form
// size / 65536 + (size % 65536 != 0) puts them the other way round (83.5%).
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

static inline int BlockOffset(File_004bb7c0* file, int& counter, int b, int tableSize)
{
    int off = tableSize;
    for (counter = 0; counter < b; ++counter)
        off += file->buffer[counter];
    return off;
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
        n = size;
        if (file->info->size - (int)file->pos < n)
            n = file->info->size - (int)file->pos;
        if (file->info->compressed != 0) {
            i = 0;
            blocks = (((n + file->pos - 1) & 0xffff0000) - (file->pos & 0xffff0000) >> 16) + 1;
            tableSize = ((file->info->size % 65536 != 0) + file->info->size / 65536) * 4;
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
                    int err = FUN_004d1970(file->buffer2, comp);
                    if (err) {
                        char msg[1000];
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
            goto done;
        }
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
