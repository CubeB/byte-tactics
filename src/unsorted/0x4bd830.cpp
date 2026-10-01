// Decompiled by DeepSeek V4.1 Flash, finished by deepseek-v4.1-flash. Names are provisional.
// Session 7 (deepseek-v4.1-flash): renaming clen (name-hash slot theory) is
// byte-flat at 61.9, so the stack-slot colour is not name-keyed either; the
// layout is decided by something internal to the allocator, not by
// declaration order, not by identifier spelling, not by use order in the
// source. Timebox fired; this remains the best scored version.
// Session 6 (deepseek-v4.1-flash): best remains 61.9 percent (this file).
// What still differs: the stack-slot map is a permutation (original nameoff
// +0x20, i +0x34, recarr +0x3c, dataptr +0x10; ours nameoff +0x28, i +0x38),
// so base sits in ebx and the entry pointer in ebp here (mirror image of the
// original) and every slot offset in the entry loop drifts. Probed this
// session (all scored with --sym): dropping the count local and inlining
// *(unsigned*)(base + off) in the guard AND latch = 54.8; count in the guard
// with the inline deref only in the latch = 54.8 (byte-identical to the
// previous), so the inline latch deref itself is the -7 point lever and the
// count local is load-bearing for the 61.9 shape. Declaration order of the 13
// scalar locals is byte-flat (reverse order 61.9, loop-vars-first 61.9), so
// declaration order is NOT a slot-colouring lever in MSVC 5.0. The original
// latch stores nameoff (+0x20) before i (+0x34) and then compares i against
// the inline memory compare cmp eax,[edx+ebp] with off reloaded from the arg
// slot, and the callback computes both *90 terms as lea *5 / lea *9 / shl 1
// with *(int*)(base + 8) as the subtracted term (confirmed at 0x4bdce9).
// Session 5 (deepseek-v4.1-flash): probes on top of 61.9: explicit
// `((base + nameoff) + *recoff)` parens (flat) and `int ro = *recoff;` before
// the entry address (flat), so the folded recoff deref is not an association
// or temp lever, and the rewrite `if (size > 0) do {...} while (size != 0)`
// with signed xor counters is byte-flat, so the file now keeps that shape.
// Best remains 61.9 percent, 1334 bytes; still open: base in
// ebp / entry pointer in ebx (mirror image) and the slot map permutation.
// Session 4 (deepseek-v4.1-flash), current best 61.9 percent (1334 vs 1332 bytes):
// flipping the entry test to `if ((e->flags & 1) != 0) { recursive } else { leaf }`
// puts the recursive call in the fall-through and the leaf at the jump target,
// matching the original's `test byte [x+8],1 / je leaf` at 0x4bd93c: 56.4 -> 61.9.
// On top of that, dropping the count local and writing the guard and latch as
// `i < *(unsigned*)(base + off)` drops to 54.8, and keeping the count local with
// `i = 0; if (i < count)` is byte-flat 61.9, so the latch count deref is not a
// standalone lever. Remaining diff: base lives in ebx and the entry pointer in
// ebp (original: base ebp, entry pointer ebx, the mirror image), and the scalar
// slot map is still a permutation (ours nameoff +0x28, i +0x38, count +0x40).
// NOTE from the second session (timebox fired): still 56.4% best (this file);
// a count-inline-only variant (build/scratch/0x4bd830/v1.cpp) printed 51.9%,
// so removing the count local alone is NOT the fix and is worse. /Fa listing of
// this file shows the whole slot map is a permutation, not just nameoff/i:
// ours: clen +0x10, pos +0x14, data +0x18, remaining +0x1c, dataptr +0x20,
// tp +0x24, nameoff +0x28, n +0x2c, file +0x30, table +0x34, i +0x38,
// packlen +0x3c, count +0x40; original: dataptr +0x10, remain +0x14,
// databuf +0x18, tp +0x1c, nameoff +0x20, table +0x24, clen +0x28,
// file +0x2c, n +0x30, i +0x34, pos +0x38, recarr +0x3c, packlen +0x40.
// recoff/size/blocks/pack/chunk/j take no slots at all here (registers or
// rematerialised). Tried this session: count as inline deref in the if and the
// latch only (v1, 51.9%, slots unchanged: nameoff +0x28, i +0x38). Not tried
// before the timebox: declaring each scalar at its point of use to reshuffle
// the allocator's processing order, and forcing recarr to be a real slot.
// Partial, 56.4% (best actually printed by check.py; this file is that best
// version, kept as is because the rewrite below was never scored when the
// timebox fired). What still differs: stack-slot assignment in the entry loop
// (original nameoff +0x20, i +0x34, recarr +0x3c; ours nameoff +0x28,
// i +0x38) and the register choices that follow from it (base in ebp, entry
// pointer in ebx in the original).
// This session's disassembly analysis (not yet compiled): the loop latch at
// 0x4bdd33 re-derefs the count inline (cmp eax,[edx+ebp] with off reloaded
// from the arg slot), so count must NOT be a local; the precheck at 0x4bd8bc
// compares i against the count before nameoff/recarr stores (they are
// initialised inside the guard: if (i < *(unsigned*)(base+off)) { nameoff=0;
// recarr=base+off+4; do {...} while (i < *(unsigned*)(base+off)); } with
// nameoff+=9; i++ as the last body statements in that order). The entry
// flags test at 0x4bd93c is test byte [ebx+8],1 / je leaf, i.e. the TRUE
// branch (e->flags & 1) is the recursive call laid out inline first and the
// leaf is the jump target, opposite of the branch order in this file. The
// uncompressed size loop precheck is test/jbe (unsigned > 0) while its latch
// is sub/jne, so likely while (size > 0) with unsigned size. An untested
// rewrite with all of these lives at build/scratch/0x4bd830/variant.cpp;
// it also makes clen and the xor loops signed (jl/jle in the original) and
// declares the 13 scalar slots in the order dataptr, remain, databuf, tp,
// nameoff, table, clen, file, n, i, pos, recarr, packlen. Slot map recovered
// from the original (frame slots at esp+0x10..+0x40): +0x10 dataptr,
// +0x14 remain, +0x18 databuf, +0x1c tp, +0x20 nameoff, +0x24 table base,
// +0x28 clen, +0x2c file, +0x30 n, +0x34 i, +0x38 pos, +0x3c recarr,
// +0x40 packlen (the +0x24 "unused" dword is in fact table, read at 0x4bdb62).
// Older notes follow.
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
                    pos = ftell(f);
                    if ((char)key != 0) {
                        for (int j = 0; j < (int)(blocks * 4); j++)
                            ((unsigned char*)table)[j] = (unsigned char)~((char)pos
                                + (char)j ^ (char)key ^ ((unsigned char*)table)[j]);
                    }
                    fwrite(table, blocks, 4, f);
                    fseek(f, 0, 2);
                    FUN_004d85a0(table);
                    FUN_004d85a0(pack);
                    FUN_004d85a0(data);
} else {
                    if (size > 0) do {
                        unsigned chunk = size < 0x1000 ? size : 0x1000;
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
            }
            nameoff += 9;
            i++;
        } while (i < count);
    }
}
