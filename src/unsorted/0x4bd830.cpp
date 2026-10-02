// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free: 66.7% -> 75.3% (1353 -> 1345 bytes; the original is 1332).
// Two changes won the points:
// (1) A plain int copy of the compressor's output length, taken after the
// ftell: `int len = clen;` with the scramble and the fwrite both using len.
// That is the difference between the loop bound living in a frame slot
// (reloaded into eax every iteration, which also clobbers al) and living in
// ebp across the loop, which is what the original does (0x4bdaf4) and reuses
// for the fwrite count (0x4bdb30). Worth 5 points on its own.
// (2) The entry pointer through a static inline helper, `entryAt(base,
// nameoff, recoff)`. Nothing about the expression changes, but the inlining
// boundary moves the register allocation: the nblocks sequence then matches
// the original instruction for instruction (the mod part into esi first), and
// the callback's *dataptr * 90 chain starts before the *(int*)(base + 8) one.
// Worth 4 points. This is the "missing piece is usually a helper that was
// inlined" lever; the same treatment for the scramble loops, the size ternary,
// the progress value, the record setup, the teardown and the path join all
// compile to identical code (75.3%, no change) or worse.
// What still differs, with the evidence:
// (1) The frame-slot map. Ours: data +0x10, pos +0x14, dataptr +0x18,
// remaining +0x1c, nameoff +0x20, tp +0x24, clen +0x28, table +0x2c, n +0x30,
// i +0x34, file +0x38, count +0x3c, packlen/pack +0x40. The original's:
// dataptr +0x10, remaining +0x14, data +0x18, tp +0x1c, nameoff +0x20,
// table/pos2 +0x24, clen +0x28, file +0x2c, n +0x30, i +0x34, pos +0x38,
// recarr +0x3c, pack +0x40, so only nameoff, clen, n, i and pack agree. Both
// have 13 scalar slots, so the frame size and every buffer reference are
// right; it is the membership that differs: we spend the slot recarr needs on
// the count local. I measured the map by tracking esp through the object
// disassembly (build/scratch/0x4bd830/smap.py) and it is NOT the declaration
// order: reversing all fourteen scalar declarations changes not one slot. The
// original's map IS its declaration order, so its source declares them
// dataptr, remaining, data, tp, nameoff, table, clen, file, n, i, pos, recarr,
// pack. Dropping our count local so the loop test re-reads *(int*)(base+off),
// as the original does at 0x4bdd4e, does free a slot, but the frame drops to
// 0x1238: MSVC keeps rematerialising recoff as a folded [off+base+4] load
// instead of giving it the slot, so every buffer reference shifts by four and
// the score falls to 61.4%.
// (2) The base/entry register mirror at the loop head: base ebp and the entry
// ebx in the original, base ebx and the entry ebp here, which also reverses
// the order of the entry arithmetic (lea ebx,[ebp+ecx]; add ebx,[recarr] there,
// a folded load plus two adds here). The loop bottom is the other half of the
// same tie: the original reloads base and re-reads the count from [base+off]
// (0x4bdd2c-0x4bdd4e), we keep the count in a slot. A redundant conditional
// re-assignment that mentions base or the entry (the 0x4b3c60 trick) does not
// flip it: 69.8% and 69.5%.
// (3) The three scramble loops. The original keeps the key byte in the
// register its test used (al in the pack loop, dl in the table loop) and folds
// the data byte into the xor (`xor dl, al; xor dl, [ecx+edi]`), and delays the
// store past the loop test. We reload the key and the data byte into al every
// iteration, which is 14 bytes larger over the three loops. That matters more
// than it looks: check.py compares in-function branch targets literally, so
// every je/jne after the first difference is a diff line while the size differs.
// Every spelling of the expression I tried (operand order either way, the key
// in a local, the buffer pointer in a local, a shared inline helper, the index
// declared outside the loop, a reversed while loop) is byte-identical.
// (4) Two store shapes that the original's disassembly shows and that make the
// score worse when reproduced, so they are recorded as leads, not adopted: the
// original stores the Pack Buffer pointer into the clen slot just before the
// FUN_004d1820 call (0x4bda5e, 0x4bdac8, 0x4bdad4) and re-reads remaining from
// *(int*)(dataptr+1) after the two FUN_004d83b0 calls (0x4bda86). `clen =
// (int)pack` scores 67.0% and `remaining = *(int*)(dataptr+1)` 58.6%, both
// because the store shape changes which values stay live across the calls.
// (5) The callback: the original computes *(int*)(base+8) * 90 into ecx, then
// *dataptr * 90 into eax, then sub eax, ecx; we interleave the two chains and
// use edx. A temporary for either term, or for the whole span, is folded away
// before allocation and changes nothing.
// BUG: the max-output-size argument handed to FUN_004d1820 is a heap address.
// The original stores the result of FUN_004d83b0("Pack Buffer", packlen) into
// the very slot whose address it passes as that argument (0x4bda5e stores it,
// 0x4bdac8 reloads it into eax, 0x4bdad4 stores eax again, and 0x4bdacd's
// lea edx,[esp+0x34] is the same slot 0x28). 0x4d1820 (MATCH) uses that slot
// as the limit, `if ((length + 0x13) > *chunkSize) return 5;`, so the "does it
// fit" test compares a length against a pointer, always passes, and nothing
// checks the result against the FUN_004d1aa0(0x10000, flags) size the buffer
// was allocated with. The two other callers of FUN_004d1820 (0x4b3c60, 0x4b39c0,
// both MATCH) pass a real size.
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

// One record of the package's file table, at the row the running offset names.
static inline Entry_004bd830* entryAt(char* base, int nameoff, int* rec)
{
    return (Entry_004bd830*)(base + nameoff + *rec);
}

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
            Entry_004bd830* e = entryAt(base, nameoff, recoff);
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
                            int len = clen;
                            if ((char)key != 0) {
                                for (int j = 0; j < len; j++)
                                    pack[j] = (unsigned char)~((char)pos + (char)j
                                              ^ (char)key ^ pack[j]);
                            }
                            fwrite(pack, len, 1, f);
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
