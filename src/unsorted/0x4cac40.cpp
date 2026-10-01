// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// Best score is 94.5% (this session), up from 91.2%. The dword locals (total,
// rows, n, row, p) live in a Locs_004cac40 struct, which reproduced their
// original slots exactly (S+0x18/0x1c/0x20/0x24/0x28). The outer-run count byte
// is a separate `ocnt` local.
//
// The tail FIXED this session: change the entry null test to `if (file == 0)
// goto out;` (not `return 0`) and the final test to `if (FUN(...) != 0x300)
// goto out;` with the success close as the fall-through. That made MSVC 5
// keep the redundant `if (file)` test at the out label (the value is unknown
// on the out edge) and emit both epilogues, matching the original byte for
// byte (640 bytes). Score 90.5 with only the entry goto, 94.5 with both.
//
// Still differing (best first):
//   1. The byte slots. Original: curmem S+0x12, t S+0x13, rep S+0x14,
//      next/outer count S+0x15, inner cnt S+0x16, lit S+0x17. Here: curmem
//      0x12, t 0x13, cnt 0x14, ocnt 0x15, lit 0x16, next 0x17, rep 0x11.
//      The original needs seven roles in six slots because the outer count
//      shares `next`'s slot (its lifetime ends inside the inner loop, so MSVC
//      5 reuses the slot: the original writes the outer count at 0x4cae1d to
//      S+0x15, the same slot `next` is stored to at 0x4cad35 and read from at
//      0x4cadad). Our compiler gives all seven roles their own slot.
//   2. `mov al,bl` / `mov esi,1` swap after the inner flush (scheduler tie).
//
// Measured this session about the byte-slot allocation (read straight out of
// the /Fa listing, `_<name>$ = -<off>`, all free scratch variants):
//   * The pool order is NOT declaration order and NOT name order. Declaring
//     the seven byte locals in three different orders, and swapping two names,
//     all give the identical permutation (rep 0x11, curmem 0x12, t 0x13,
//     cnt 0x14, ocnt 0x15, lit 0x16, next 0x17). So the order is a function
//     of the IR alone, which is why no amount of reshuffling the declarations
//     helps.
//   * Reusing `next` as the outer count byte does drop to six slots, and the
//     order becomes next 0x12, t 0x13, rep 0x14, curmem 0x15, lit 0x16,
//     cnt 0x17, but it costs 52.7%: the code grows to 641 bytes and the
//     width/height arguments swap out of ebx. So the six-slot layout is not
//     reachable this way.
//   * Swapping just the two literal roles (inner literal and row-end/0xc byte
//     exchange variables) gives next 0x12, t 0x13, rep 0x14, curmem 0x15,
//     cnt 0x16, lit 0x17, so `rep` and `cnt` reach their original slots. What
//     is left is exactly the curmem/next pair at 0x12 and 0x15. Also 52.7%.
//   * A single `unsigned char b[6]` (or byte struct) holding all six buffers
//     would pin every slot by index, and `&b[k]` folds to the same lea
//     displacement the original uses, but it is 52.2%: the array base lands at
//     S+0x10 (so two pad elements are needed to reach 0x12) and width/height
//     swap ebx/eax again. Confirms the earlier byte-struct measurement.
//   * Cheap perturbations that do NOT move the pool at all: `wrote = 1` instead
//     of `wrote = run`, a shared function-scope `chunk`, `char` instead of
//     `unsigned char` (that one spills `cur`), `(unsigned char)cur` casts, the
//     `&&` order in the literal test, and moving `curmem = cur` to the top of
//     the row. Inlining the `chunk` ternary does move it (next 0x13, rep 0x14,
//     t 0x15, cnt 0x16, lit 0x17) but loses `curmem`, so it is not a lead.
// #3417 (deepseek-v4.1-flash): two more merge experiments, both measured at
// exactly 640 bytes: collapsing the outer flush count into the inner one
// (`ocnt` removed, both flushes write `cnt`) scores 90.5, and collapsing the
// final 0x0c writer into the literal temp (`t` removed, `lit` used in the tail
// too) scores 71.1. So the original does have separate byte variables for
// those roles; only the pool order remains wrong.
// Next step for whoever retries: the pool order looks like it follows the
// expression trees assigned to each byte, so try reshapes of those (for
// example making the row-start `curmem = cur` and the flush `rep = curmem`
// load through a second pointer local) rather than the declarations.
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
        goto out;

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
    if (FUN_004bbbe0(file, block, 0x300) != 0x300)
        goto out;
    FUN_004bb5d0(file);
    return 1;
out:
    if (file)
        FUN_004bb5d0(file);
    return 0;
}

// The dword slot order (L.total=S+0x18, L.rows=S+0x1c, L.n=S+0x20,
// L.row=S+0x24, L.p=S+0x28) and the outer loop guard (dec/test/jl/inc as the
// canonicalisation of `rows = height - 1; if (rows >= 0) { ++rows; do ...
// while (--rows); }`) both match now. Only two things still differ:
//
// 1. The byte slot order. Original: curmem=S+0x12, t=S+0x13, rep=S+0x14,
//    next=S+0x15, cnt(inner)=S+0x16, lit=S+0x17, with the outer flush count
//    sharing next's S+0x15. Here: curmem=0x12, t=0x13, cnt=0x14, ocnt=0x15,
//    lit=0x16, next=0x17, rep=0x11. Seven slots instead of six. The desired
//    mapping is exactly the declaration order curmem, t, rep, next, cnt, lit
//    low-to-high, but MSVC does not pack ours that way while `ocnt` is a
//    seventh variable. Making `ocnt` a separate variable keeps seven slots;
//    reusing `next` collapses the source roles and wrecks register allocation
//    (52.7%); a byte struct forces memory traffic (52.2%).
// 2. After the inner flush the original schedules `mov esi,1` before
//    `mov al,bl`; ours emits them the other way round (scheduler tie).
