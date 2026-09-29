// Decompiled by space-bunny-free. Names are provisional.
// NOT MATCHING: 56% (see the notes at the bottom for what still differs).
//
// What the function does. It opens `filename` through FUN_004bb2c0, writes a
// 128 byte fixed header, run length compresses a `height` rows of `width` bytes
// from `data`, then writes a single 0x0c byte and the caller's 0x300 byte
// `block` (the first 0x30 bytes of which also go into the header as a palette).
// It returns 1 only if both trailing writes returned their full length.
//
// The compression: inside a row, equal bytes accumulate in `run`. When the byte
// changes, the run is flushed. A run of exactly 1 whose value has no 0xc0 bits
// goes out as one literal byte; every other run goes out as 0xc0|count followed
// by one more byte, split into 0x3f sized chunks.
//
// Suspected original bug: the second byte of a counted run is a COPY OF THE
// COUNT BYTE, not the run's value. 0x4cad95 loads [S+0x16] (the byte just
// stored as 0xc0|count at 0x4cad86) and 0x4cad9b stores it into [S+0x14] for
// the second FUN_004bbbe0; the flush copy at 0x4cae2c/0x4cae32 does the same
// with [S+0x15]. So every counted run is written as 0xc0|n, 0xc0|n instead of
// 0xc0|n, value. A reader/writer asymmetry worth checking against the
// decompressor.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

extern void* __stdcall FUN_004bb2c0(void* thing);
extern void __stdcall FUN_004bb5d0(Class_004bbbe0* file);
extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* buf, unsigned int size);

// 0x80 bytes, all zeroed first. Offsets are from the disassembly's stack
// offsets; the trailing 0x3a bytes are never written and rely on the memset.
struct Header_004cac40 {
    unsigned char a;                   // +0x00, 10
    unsigned char b;                   // +0x01, 5
    unsigned char c;                   // +0x02, 1
    unsigned char d;                   // +0x03, 8
    unsigned short e;                  // +0x04, 0
    unsigned short f;                  // +0x06, 0
    short g;                           // +0x08, width - 1
    short h;                           // +0x0a, height - 1
    short i;                           // +0x0c, width
    short j;                           // +0x0e, height
    unsigned char palette[0x30];       // +0x10, copy of block[0..0x2f]
    unsigned char k;                   // +0x40, 0 (memset only)
    unsigned char l;                   // +0x41, 1
    unsigned short m;                  // +0x42, width
    unsigned short n;                  // +0x44, 0
    unsigned char rest[0x3a];          // +0x46
};

// FUNCTION: 0x4cac40
// The five arguments, in the order the incoming stack slots are read:
//   [E+4]  filename, passed straight to FUN_004bb2c0 and never used again
//   [E+8]  data,     the height*width byte image
//   [E+c]  width,    kept in ebx for the whole function (0x4cac60)
//   [E+10] height,   the outer loop counter (0x4cacef)
//   [E+14] block,    0x300 bytes; its first 0x30 are the header palette
int __stdcall FUN_004cac40(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    Class_004bbbe0* file = (Class_004bbbe0*)FUN_004bb2c0(filename);
    unsigned char* row;
    int rows;
    unsigned char c;
    unsigned char t;
    unsigned char rep;
    unsigned char next;
    unsigned char cnt;
    unsigned char lit;
    int total;

    if (file == 0)
        return 0;

    memset(&hdr, 0, 0x80);
    hdr.a = 10;
    hdr.b = 5;
    hdr.c = 1;
    hdr.d = 8;
    hdr.e = 0;
    hdr.f = 0;
    hdr.g = (short)(width - 1);
    hdr.h = (short)(height - 1);
    hdr.i = (short)width;
    hdr.j = (short)height;
    memcpy(hdr.palette, block, 0x30);
    hdr.n = 0;
    hdr.l = 1;
    hdr.m = (unsigned short)width;
    if (FUN_004bbbe0(file, &hdr, 0x80) != 0x80)
        goto out;

    // Initialising `rows` and `row` here rather than at the top is worth about
    // 14 points: it keeps FUN_004bb2c0's argument in eax for its `push` and
    // leaves the two loop-invariant loads to the header's own block.
    rows = height;
    row = data;
    if (rows > 0) {
        do {
            int run = 1;
            unsigned char* p = row + 1;
            c = *row;
            total = 0;
            if (width > 1) {
                int n = width - 1;
                do {
                    next = *p++;
                    if (c == next) {
                        run++;
                    } else {
                        int wrote = 0;
                        if (run == 1 && (c & 0xc0) != 0xc0) {
                            lit = c;
                            FUN_004bbbe0(file, &lit, run);
                            wrote = run;
                        } else {
                            while (run > 0) {
                                int chunk = run > 0x3f ? 0x3f : run;
                                cnt = (unsigned char)(chunk | 0xc0);
                                rep = cnt;
                                FUN_004bbbe0(file, &cnt, 1);
                                FUN_004bbbe0(file, &rep, 1);
                                run -= chunk;
                                wrote += 2;
                            }
                        }
                        total += wrote;
                        c = next;
                        run = 1;
                    }
                } while (--n);
            }
            if (run == 1 && (c & 0xc0) != 0xc0) {
                t = c;
                FUN_004bbbe0(file, &t, run);
            } else {
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    cnt = (unsigned char)(chunk | 0xc0);
                    rep = cnt;
                    FUN_004bbbe0(file, &cnt, 1);
                    FUN_004bbbe0(file, &rep, 1);
                    run -= chunk;
                }
            }
            row += width;
        } while (--rows);
    }

    t = 0x0c;
    FUN_004bbbe0(file, &t, 1);
    if (FUN_004bbbe0(file, block, 0x300) == 0x300) {
        FUN_004bb5d0(file);
        return 1;
    }
out:
    if (file)
        FUN_004bb5d0(file);
    return 0;
}

// Still differing, best first, with the evidence:
//
// 1. The `total` accumulation. This version matches the original's instruction
//    for instruction in the inner loop; the difference is that the original
//    reuses `c`'s stack slot S+0x12 as a scratch for the inner `cnt`/`rep`
//    pair, which forces its `total` (S+0x18) to be reloaded before the add
//    (0x4cadb1, 0x4cadb5) where this version has it live. Related: the
//    original mirrors `c` into S+0x12 (0x4cad21, 0x4cadbb) and never reads it
//    back, a dead byte store this version does not produce. Forcing that spill
//    is what shifts the char slots up by one and is the single biggest
//    remaining blocker.
// 2. The byte slot order. Original: c=S+0x12, t=S+0x13, rep=S+0x14,
//    next=S+0x15, cnt=S+0x16, lit=S+0x17. Here: t=S+0x13, cnt=S+0x14,
//    rep=S+0x15, lit=S+0x16, next=S+0x17. Making `cnt` and `lit` block scoped
//    and hoisting `rep` above `next` does NOT reproduce it (tried, drops to
//    40.6%), so MSVC's slot assignment here is not following declaration order
//    and the order above is a consequence, not a cause.
// 3. The scan pointer. The original advances ecx straight from the row
//    pointer, so the row base stays in S+0x24 and ecx becomes the scan pointer
//    (0x4cad16, 0x4cad39); this version gives the row base ecx and the scan
//    pointer edx, costing a `lea edx,[ecx+1]` and shifting the loop body.
// 4. The outer loop guard. Original 0x4cacfd-0x4cad0b is `dec eax / test / jl /
//    inc eax / mov [S+0x1c],eax`, that is a test of `height - 1`, while
//    `if (rows > 0)` compiles here to `test eax,eax / jle`. Spelling the guard
//    `if (height - 1 >= 0)` does not reproduce it either (tried, 40.6%).
// 5. The tail. The original re-tests the file pointer at 0x4cae8d
//    (`xor edx,edx / cmp ebp,edx / je`) before the failure-path
//    FUN_004bb5d0; MSVC proves the pointer non-null here and drops the test.
//    Both `if (file)` and a plain unguarded call produce the same code.
