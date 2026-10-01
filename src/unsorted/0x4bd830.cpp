// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
// Session 13b (mimo-v2.6-pro): best is now 66.7 percent (this file, 1353 bytes).
// Session 13b change that won 2 points: the table-obfuscation position is a
// separate local pos2 (declared beside pos). MSVC coalesces pos and pos2 into
// one slot (+0x24) and the frame-slot map then lands several entries on the
// original's offsets (nameoff +0x20, n +0x30, i +0x34, packlen +0x40 agree).
// Also verified from the original's slots: table (+0x24) and the second
// obfuscation position share a slot, and the third (uncompressed path)
// position never gets a slot at all (the ftell result stays in eax).
// What still differs and what I tried in this session:
// (1) The base/entry register mirror is still wrong (base ebx and entry ebp
// here, base ebp and entry ebx in the original), and with it the loop head:
// the original loads nameoff and recarr from slots and computes
// e = (base + nameoff) + *recarr in ebx, while this file rematerializes
// *recoff as a folded [off + base + 4] load into ebp. recarr is a real slot
// local (+0x3c) in the original; our recoff never gets a slot.
// (2) The obfuscation loops: the original keeps key in the byte register its
// test used (al in the pack loop, dl in the table and buffer loops) and the
// accumulator takes the other byte register, folding the buffer read as a
// memory xor operand; this file reloads key each iteration and uses al as a
// rolling scratch. Root cause seen in the listing: our loop bound clen is
// reloaded into eax every iteration (clobbering al), while the original holds
// clen in ebp across the loop. The buffer loop in the original even keeps the
// ftell result in eax across the loop (pos read as al). Tried dropping the
// count local and inlining the loop condition (57.7 percent, the frame
// shrinks to 0x1238 because recoff stays rematerialized), and declaring the
// three positions as scoped locals at their point of use (still 66.7).
// (3) nblocks: all three spellings of w / 65536 + (w % 65536 != 0) compile
// identically out of line (the mod part into esi first, as in the original),
// so the interleaved div-first order in this file is inline register pressure
// (size sits in ebp here, ecx in the original), not the operand order.
// (4) The callback still interleaves both *90 chains; the original computes
// the subtracted *(int*)(base + 8) * 90 term fully into ecx first, then
// *dataptr * 90 into eax, then sub eax, ecx.
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
int __stdcall FUN_004d1820(void* chunk, int* chunkSize, char* data,
                           int size, int method, int encrypt);
unsigned int __stdcall FUN_004d1aa0(unsigned int value, int mode);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004bd830(char* path, char* base, int off, FILE* f,
                            void (__cdecl* cb)(unsigned), unsigned extra,
                            int key, int flags);

// Whole 64K blocks of a byte size, rounded up.
static inline int nblocks(int w)
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
    int clen;
    File_004bd830* file;
    int n;
    unsigned i;
    long pos;
    long pos2;
    int* recoff;
    unsigned packlen;

    strcpy(name, path);
    if (name[strlen(name) - 1] != '\\')
        strcat(name, "\\");

    unsigned count = *(unsigned*)(base + off);
    i = 0;
    if (i < count) {
        nameoff = 0;
        recoff = (int*)(base + off + 4);
        do {
            Entry_004bd830* e = (Entry_004bd830*)(base + nameoff + *recoff);
            strcpy(full, name);
            strcat(full, (char*)(base + e->name));
            if ((e->flags & 1) != 0) {
                FUN_004bd830(full, base, e->offset, f, cb, extra, key, flags);
            } else {
                file = FUN_004bb2e0(full, "rb");
                dataptr = (long*)(base + e->offset);
                *dataptr = ftell(f);
                unsigned size;
                if (file->shared != 0) {
                    size = file->info->size;
                } else if (file->fp != 0) {
                    size = _filelength(_fileno(file->fp));
                } else {
                    size = 0;
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
                            int chunk = remaining >= 0x10000 ? 0x10000 : remaining;
                            FUN_004bb7c0(file, data, chunk);
                            clen = packlen;
                            FUN_004d1820(pack, &clen, (char*)data, chunk, flags & 0xff, 1);
                            *tp = clen;
                            pos = ftell(f);
                            if ((char)key != 0) {
                                for (int j = 0; j < clen; j++)
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
                    pos2 = ftell(f);
                    if ((char)key != 0) {
                        for (int j = 0; j < (int)(blocks * 4); j++)
                            ((unsigned char*)table)[j] = (unsigned char)~((char)pos2
                                + (char)j ^ (char)key ^ ((unsigned char*)table)[j]);
                    }
                    fwrite(table, blocks, 4, f);
                    fseek(f, 0, 2);
                    FUN_004d85a0(table);
                    FUN_004d85a0(pack);
                    FUN_004d85a0(data);
} else {
                    if (size > 0) do {
                        int chunk = size >= 0x1000 ? 0x1000 : size;
                        FUN_004bb7c0(file, buffer, chunk);
                        pos = ftell(f);
                        if ((char)key != 0) {
                            for (int j = 0; j < chunk; j++)
                                buffer[j] = (unsigned char)~((char)pos + (char)j
                                            ^ (char)key ^ buffer[j]);
                        }
                        fwrite(buffer, chunk, 1, f);
                        size -= chunk;
                    } while (size != 0);
}
                if (file->shared != 0) {
                    file->shared->count--;
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
                    cb((unsigned)(*dataptr * 90 - *(int*)(base + 8) * 90) / extra + 5);
            }
            nameoff += 9;
            i++;
        } while (i < count);
    }
}
