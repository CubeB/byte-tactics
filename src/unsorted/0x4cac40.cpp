// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// Best score is 91.2%. The dword locals (total, rows, n, row, p) now live in a
// Locs_004cac40 struct, which reproduced their original slots exactly
// (S+0x18/0x1c/0x20/0x24/0x28); grouping them as one struct was the lever the
// flat declaration sweeps never found. The outer-run count byte is a separate
// `ocnt` local, which fixed the counted-run writer and lifted 86.2 to 90.2.
// Still differing (best first):
//   1. The byte slots. Original: curmem S+0x12, t S+0x13, rep S+0x14,
//      next/outer count S+0x15, inner cnt S+0x16, lit S+0x17. Here: curmem
//      0x12, t 0x13, cnt 0x14, ocnt 0x15, lit 0x16, next 0x17, rep 0x11.
//      The original needs seven roles in six slots because the outer count
//      shares `next`'s slot; our compiler does not coalesce them, so rep gets
//      an extra slot at 0x11 and next is pushed to 0x17. Reusing `next` for
//      the count in source (51.1%) or aliasing it in a union (47.5%) changes
//      the global register allocation (width/height swap out of ebx), so the
//      sharing has to come from coalescing, which I did not crack.
//   2. The tail. Original keeps a redundant `if (file)` test at 0x4cae8d and
//      two separate epilogues (return 1 at 0x4caeb1, return 0 at 0x4cae99);
//      ours folds the two closes into one. MSVC proves `file` non-null after
//      the early return, so both `if (file)` and an unguarded call merge.
//   3. `mov esi,1`/`mov al,bl` swap after the inner flush (scheduler tie).
// Tried this session: flat byte/dword declaration permutations (all neutral),
// whole-frame struct including the header (36.0), dword-only struct (91.2),
// byte-only union shared with next (47.5), block-scoped ocnt (88.2), sharing
// the outer count with inner cnt (87.2), scoping total/n to their loops
// (neutral).
//
// Best score was 86.2%. The row guard uses
// rows = height - 1; if (rows >= 0) { ++rows; do ... while (--rows); } to
// reproduce the original signed dec/test/jl/inc sequence. Remaining codegen
// differences are documented below; no MATCH was reached.
// Retry in issue-2304: five checks kept 86.2%. Reordering the total/row locals
// or byte locals did not change codegen; aliasing the literal byte with `next`
// fell to 52.1%, and an outer non-null guard fell to 85.6%. The best source was
// restored. Remaining differences are local slots, row/total spills, and the
// tail-merged close path described below.
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

struct Locs_004cac40 {
    int total;
    int rows;
    int n;
    unsigned char* row;
    unsigned char* p;
};

// FUNCTION: 0x4cac40
int __stdcall FUN_004cac40(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    Class_004bbbe0* file = (Class_004bbbe0*)FUN_004bb2c0(filename);
    Locs_004cac40 L;
    int run;
    int wrote;
    unsigned char cur;
    unsigned char curmem;
    unsigned char t;
    unsigned char rep;
    unsigned char next;
    unsigned char cnt;
    unsigned char lit;
    unsigned char ocnt;

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

    L.rows = height - 1;
    L.row = data;
    if (L.rows >= 0) {
        ++L.rows;
        do {
            cur = *L.row;
            run = 1;
            L.p = L.row + 1;
            L.total = 0;
            curmem = cur;
            if (width > 1) {
                L.n = width - 1;
                do {
                    next = *L.p++;
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
                        L.total += wrote;
                        cur = next;
                        curmem = next;
                        run = 1;
                    }
                } while (--L.n);
            }
            if (run == 1 && (cur & 0xc0) != 0xc0) {
                t = cur;
                FUN_004bbbe0(file, &t, 1);
            } else {
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    ocnt = (unsigned char)(chunk | 0xc0);
                    FUN_004bbbe0(file, &ocnt, 1);
                    rep = curmem;
                    FUN_004bbbe0(file, &rep, 1);
                    run -= chunk;
                }
            }
            L.row += width;
        } while (--L.rows);
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
// 2. The dword slot order, same cause. Original: L.total=S+0x18, L.rows=S+0x1c,
//    L.n=S+0x20, L.row=S+0x24, L.p=S+0x28. Here `L.row` and `L.total` are swapped.
//    Only two instructions differ (`mov ecx,[esp+0x24]` and `mov [esp+0x24],ecx`
//    against `mov ecx,[esp+0x18]` and `mov [esp+0x18],ecx`) but they also drag
//    the `L.total` reload at 0x4cadb1 and the `L.total = 0` at the L.row top with them.
// 3. The outer loop guard. Original 0x4cacfd-0x4cad0b is `dec eax / test / jl /
//    inc eax / mov [S+0x1c],eax`, that is MSVC's canonicalisation of a signed
//    `>` (as `x > 0` becomes `x - 1 >= 0`), and it stores the L.row pointer
//    before the test. Neither `if (L.rows > 0)` with `L.rows = height` one line
//    earlier nor `L.row = data; if (height > 0) { L.rows = height; ... }` gives it:
//    both compile to `test eax,eax / jle` (tried, both 76.6%).
// 4. The tail. The original re-tests the file pointer at 0x4cae8d
//    (`xor edx,edx / cmp ebp,edx / je`) before the failure-path FUN_004bb5d0,
//    and keeps two separate epilogues (return 1 at 0x4caeb1, return 0 at
//    0x4cae99). MSVC proves the pointer non-null and tail-merges the two
//    closes into one. Both `if (file)` and a plain unguarded call give the
//    same merged code. Those 8 bytes are the entire size difference.
