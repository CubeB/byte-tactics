// Decompiled by space-bunny-free, improved by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
//
// Writes a PCX file: opens `filename` through HAPI_OpenFileAppend, writes the 128
// byte header (manufacturer 10, version 5, RLE encoding, 8 bits per pixel,
// the first 0x30 bytes of `block` as the 16-colour palette, one plane),
// run length compresses `height` rows of `width` bytes from `data`, then
// writes the 0x0c marker byte and the 0x300 byte VGA palette `block`.
// Returns 1 only if the header and the palette went out in full.
#include <stdio.h>
#include <string.h>

struct FileHandle {
    FILE* file;
    int error;
};

extern void* __stdcall HAPI_OpenFileAppend(void* thing);
extern void __stdcall HAPI_CloseFile(FileHandle* file);
extern unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* buf, unsigned int size);

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
int __stdcall WritePcx(void* filename, unsigned char* data, int width, int height, unsigned char* block)
{
    Header_004cac40 hdr;
    FileHandle* file = (FileHandle*)HAPI_OpenFileAppend(filename);
    int total;
    int rows;
    int n;
    unsigned char* row;
    unsigned char* p;
    int run;
    int wrote;
    // One cur variable, no separate register and memory copies.
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
    if (HAPI_WriteFile(file, &hdr, 0x80) != 0x80)
        goto out;

    rows = height - 1;
    row = data;
    if (rows >= 0) {
        ++rows;
        do {
            // t is declared in the row loop, not at function scope: shares its slot.
            unsigned char t;
            // total = 0 comes before cur = *row: keeps cur's spill store after it.
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
                            HAPI_WriteFile(file, &lit, 1);
                            wrote = run;
                        } else {
                            while (run > 0) {
                                int chunk = run > 0x3f ? 0x3f : run;
                                cnt = (unsigned char)(chunk | 0xc0);
                                HAPI_WriteFile(file, &cnt, 1);
                                rep = cur;
                                HAPI_WriteFile(file, &rep, 1);
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
                HAPI_WriteFile(file, &t, 1);
            } else {
                // Declared in this else block: shares a slot with next.
                unsigned char ocnt;
                while (run > 0) {
                    int chunk = run > 0x3f ? 0x3f : run;
                    ocnt = (unsigned char)(chunk | 0xc0);
                    HAPI_WriteFile(file, &ocnt, 1);
                    rep = cur;
                    HAPI_WriteFile(file, &rep, 1);
                    run -= chunk;
                }
            }
            row += width;
        } while (--rows);
    }

    {
        // Own variable in its own block: shares a slot with t.
        unsigned char marker = 0x0c;
        HAPI_WriteFile(file, &marker, 1);
    }
    if (HAPI_WriteFile(file, block, 0x300) != 0x300)
        goto out;
    HAPI_CloseFile(file);
    return 1;

out:
    if (file)
        HAPI_CloseFile(file);
    return 0;
}
