// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
//
// Writes a PCX file: opens `filename` through FUN_004bb2c0, writes the 128
// byte header (manufacturer 10, version 5, RLE encoding, 8 bits per pixel,
// the first 0x30 bytes of `block` as the 16-colour palette, one plane),
// run length compresses `height` rows of `width` bytes from `data`, then
// writes the 0x0c marker byte and the 0x300 byte VGA palette `block`.
// Returns 1 only if the header and the palette went out in full.
//
// What matched it (#5018, after many passes stuck at 95-96% on the byte
// slots), all of it about MSVC 5's frame layout rather than the code:
// * One `cur` variable, not a register copy plus a memory copy. MSVC splits
//   it by itself: al in the column loop, a home at [esp+0x12] that the chunk
//   loops reload. That split variable carries one more reference than its
//   four visible memory accesses, which is what puts it first in the frame.
// * The final 0x0c byte is its own variable in its own block, and the outer
//   literal byte `t` is declared in the row loop. That makes the two share
//   [esp+0x13] while the outer run count, declared in the else block, shares
//   `next`'s [esp+0x15] instead of joining them.
// * Plain locals for total/rows/n/row/p: the earlier Locs struct was only
//   there to pin a layout that these declarations give anyway.
// * `total = 0` before `cur = *row` keeps cur's spill store after it.
// The layout model (C2.EXE's slot sorting) is written up in the pull request.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;
    int error;
};

extern void* __stdcall FUN_004bb2c0(void* thing);
extern void __stdcall FUN_004bb5d0(Class_004bbbe0* file);
extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* buf, unsigned int size);

// PCX file header (128 bytes).
struct Header_004cac40 {
    unsigned char a;            // manufacturer
    unsigned char b;            // version
    unsigned char c;            // encoding
    unsigned char d;            // bits per pixel
    unsigned short e;           // xmin
    unsigned short f;           // ymin
    short g;                    // xmax
    short h;                    // ymax
    short i;                    // horizontal resolution
    short j;                    // vertical resolution
    unsigned char palette[0x30];
    unsigned char k;            // reserved
    unsigned char l;            // planes
    unsigned short m;           // bytes per line
    unsigned short n;           // palette info
    unsigned char rest[0x3a];
};

// FUNCTION: 0x4cac40
int __stdcall FUN_004cac40(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    Class_004bbbe0* file = (Class_004bbbe0*)FUN_004bb2c0(filename);
    int total;
    int rows;
    int n;
    unsigned char* row;
    unsigned char* p;
    int run;
    int wrote;
    unsigned char cur;
    unsigned char next;
    unsigned char rep;
    unsigned char lit;
    unsigned char cnt;

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

    rows = height - 1;
    row = data;
    if (rows >= 0) {
        ++rows;
        do {
            unsigned char t;
            total = 0;
            cur = *row;
            run = 1;
            p = row + 1;
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
                                rep = cur;
                                FUN_004bbbe0(file, &rep, 1);
                                run -= chunk;
                                wrote += 2;
                            }
                        }
                        total += wrote;
                        cur = next;
                        run = 1;
                    }
                } while (--n);
            }
            if (run == 1 && (cur & 0xc0) != 0xc0) {
                t = cur;
                FUN_004bbbe0(file, &t, 1);
            } else {
                unsigned char ocnt;
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    ocnt = (unsigned char)(chunk | 0xc0);
                    FUN_004bbbe0(file, &ocnt, 1);
                    rep = cur;
                    FUN_004bbbe0(file, &rep, 1);
                    run -= chunk;
                }
            }
            row += width;
        } while (--rows);
    }

    {
        unsigned char marker = 0x0c;
        FUN_004bbbe0(file, &marker, 1);
    }
    if (FUN_004bbbe0(file, block, 0x300) != 0x300)
        goto out;
    FUN_004bb5d0(file);
    return 1;

out:
    if (file)
        FUN_004bb5d0(file);
    return 0;
}
