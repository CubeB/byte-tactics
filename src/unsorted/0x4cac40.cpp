// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6.1-sol. Names are provisional.
// Best score is 86.2%. The row guard uses
// rows = height - 1; if (rows >= 0) { ++rows; do ... while (--rows); } to
// reproduce the original signed dec/test/jl/inc sequence. Remaining codegen
// differences are documented below; no MATCH was reached.
// GPT-6.1-sol retry in #1928: 5 additional checks kept 86.2%. Reordering
// the row/total declarations produced no code change. Remaining byte scratch
// mapping, row/total spills, outer guard and duplicated cleanup are noted below.
// Refinement in issue-1928-r1: five worker checks and one local variant check
// kept 86.2%. Expanding the chunk-size ternary to an if/else produced the same
// score but changed unrelated stack-slot and epilogue choices, so the best
// source was restored. No MATCH was reached.
//
// What the function does. It opens `filename` through FUN_004bb2c0, writes a
// 128 byte fixed header, run length compresses `height` rows of `width` bytes
// from `data`, then writes a single 0x0c byte and the caller's 0x300 byte
// `block` (whose first 0x30 bytes also go into the header as a palette).
// It returns 1 only if both trailing writes returned their full length.
//
// The compression: inside a row, equal bytes accumulate in `run`. When the byte
// changes the run is flushed. A run of exactly 1 whose value has no 0xc0 bits
// goes out as one literal byte; every other run goes out as 0xc0|count followed
// by the run's value, split into 0x3f sized chunks.
//
// The one non-obvious construct, and it is worth 20 points: the byte currently
// being accumulated exists TWICE, as `cur` (a register copy) and `curmem` (a
// memory copy that is its call-spill home). The counted-run writer must send
// the run's VALUE, not its count, so it reads `curmem` rather than `cur`: `cur`
// lives in al, which the chunk loop has just reused as its own temporary, so
// reading the value back out of its home at [esp+0x12] into dl and storing that
// into `rep` is what the original does at 0x4cad95/0x4cad9b and 0x4cae2c/0x4cae32.
// Writing `rep = cnt` instead (a copy of the count byte) was the 56% version.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;
    int error;
};

extern void* __stdcall FUN_004bb2c0(void* thing);
extern void __stdcall FUN_004bb5d0(Class_004bbbe0* file);
extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* buf, unsigned int size);

struct Header_004cac40 {
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
    unsigned short e;
    unsigned short f;
    short g;
    short h;
    short i;
    short j;
    unsigned char palette[0x30];
    unsigned char k;
    unsigned char l;
    unsigned short m;
    unsigned short n;
    unsigned char rest[0x3a];
};

// FUNCTION: 0x4cac40
int __stdcall FUN_004cac40(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    Class_004bbbe0* file = (Class_004bbbe0*)FUN_004bb2c0(filename);
    int total;
    unsigned char* row;
    unsigned char* p;
    int rows;
    int run;
    int n;
    int wrote;
    unsigned char cur;
    unsigned char curmem;
    unsigned char t;
    unsigned char rep;
    unsigned char next;
    unsigned char cnt;
    unsigned char lit;

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

    rows = height - 1;
    row = data;
    if (rows >= 0) {
        ++rows;
        do {
            cur = *row;
            run = 1;
            p = row + 1;
            total = 0;
            curmem = cur;
            if (width > 1) {
                n = width - 1;
                do {
                    next = *p++;
                    if (cur == next) {
                        run++;
                    } else {
                        wrote = 0;
                        if (run == 1 && (cur & 0xc0) != 0xc0) {
                            lit = cur;
                            FUN_004bbbe0(file, &lit, 1);
                            wrote = run;
                        } else {
                            while (run > 0) {
                                int chunk = run > 0x3f ? 0x3f : run;
                                cnt = (unsigned char)(chunk | 0xc0);
                                FUN_004bbbe0(file, &cnt, 1);
                                rep = curmem;
                                FUN_004bbbe0(file, &rep, 1);
                                run -= chunk;
                                wrote += 2;
                            }
                        }
                        total += wrote;
                        cur = next;
                        curmem = next;
                        run = 1;
                    }
                } while (--n);
            }
            if (run == 1 && (cur & 0xc0) != 0xc0) {
                t = cur;
                FUN_004bbbe0(file, &t, 1);
            } else {
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    cnt = (unsigned char)(chunk | 0xc0);
                    FUN_004bbbe0(file, &cnt, 1);
                    rep = curmem;
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
// 1. The byte slot order. Original: curmem=S+0x12, t=S+0x13, rep=S+0x14,
//    next=S+0x15, cnt(inner)=S+0x16, lit=S+0x17. Here: curmem=S+0x12,
//    rep=S+0x13, cnt=S+0x14, t=S+0x15, next=S+0x16, lit=S+0x17. The same
//    declaration list in the order curmem, t, rep, next, cnt, lit does NOT
//    reproduce it, and permuting the whole declaration block (including the
//    pointers and ints) moves the byte slots as well and also swaps which
//    parameter lands in ebx, so the layout is tied to the declaration order in
//    a way I did not crack. Note the original needs SEVEN byte roles in SIX
//    slots: the outer flush's count byte is at S+0x15, the same slot as `next`,
//    so those two are probably one source variable.
// 2. The dword slot order, same cause. Original: total=S+0x18, rows=S+0x1c,
//    n=S+0x20, row=S+0x24, p=S+0x28. Here `row` and `total` are swapped.
//    Only two instructions differ (`mov ecx,[esp+0x24]` and `mov [esp+0x24],ecx`
//    against `mov ecx,[esp+0x18]` and `mov [esp+0x18],ecx`) but they also drag
//    the `total` reload at 0x4cadb1 and the `total = 0` at the row top with them.
// 3. The outer loop guard. Original 0x4cacfd-0x4cad0b is `dec eax / test / jl /
//    inc eax / mov [S+0x1c],eax`, that is MSVC's canonicalisation of a signed
//    `>` (as `x > 0` becomes `x - 1 >= 0`), and it stores the row pointer
//    before the test. Neither `if (rows > 0)` with `rows = height` one line
//    earlier nor `row = data; if (height > 0) { rows = height; ... }` gives it:
//    both compile to `test eax,eax / jle` (tried, both 76.6%).
// 4. The tail. The original re-tests the file pointer at 0x4cae8d
//    (`xor edx,edx / cmp ebp,edx / je`) before the failure-path FUN_004bb5d0,
//    and keeps two separate epilogues (return 1 at 0x4caeb1, return 0 at
//    0x4cae99). MSVC proves the pointer non-null and tail-merges the two
//    closes into one. Both `if (file)` and a plain unguarded call give the
//    same merged code. Those 8 bytes are the entire size difference.
