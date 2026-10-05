// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by GPT-6. Names are provisional.
// MATCH. Preserve the native 0x80 case fallthrough into the 0x40 scaler.

#include <string.h>
#include <stddef.h>

struct Info_4c7a20 {
    unsigned short bits;          // +0x00
    char gap_2[0xe];
    unsigned char* data;          // +0x10
};

struct Surf_4c7a20 {
    unsigned short pitch;         // +0x00
    char gap_2[0xe];
    unsigned char* pixels;        // +0x10
    unsigned char* mask;          // +0x14
};

void __cdecl FUN_004cd896(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl FUN_004cd8da(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl FUN_004cd91e(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void __cdecl FUN_004cd962(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);

// FUNCTION: 0x4c7a20
void __stdcall DrawTexturedSpan(int row, int* span, Surf_4c7a20* surf, Info_4c7a20* info)
{
    unsigned char* mask = surf->mask;
    unsigned char* dest = surf->pixels;
    unsigned char* src = info->data;
    int width = span[1] - span[0];
    int rowstep = (span[4] - span[2]) / width;
    int colstep = (span[5] - span[3]) / width;
    int dstep = (span[7] - span[6]) / width;

    if (span[0] < 0) {
        span[2] -= rowstep * span[0];
        span[3] -= colstep * span[0];
        int clippedDepth = span[6] - dstep * span[0];
        span[0] = 0;
        span[6] = clippedDepth;
    }
    if (span[1] > surf->pitch - 1)
        span[1] = surf->pitch - 1;
    width = span[1] - span[0];
    if (width > 0) {
        int y = span[2];
        int x = span[3];
        int w = span[6];

        dest += surf->pitch * row + span[0];
        if (mask != 0) {
            mask += surf->pitch * row + span[0];
            switch (info->bits) {
            case 0x80: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 9) & ~0x7f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x40: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 10) & ~0x3f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x20: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 11) & ~0x1f)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 0x10: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 12) & ~0xf)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            case 8: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 13) & ~7)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            default: {
                int n = width;
                do {
                    if (*mask <= (unsigned char)(w >> 16)) {
                        *dest = src[(y >> 16) + ((x >> 16) * info->bits)];
                        *mask = (unsigned char)(w >> 16);
                    }
                    x += colstep;
                    w += dstep;
                    dest++;
                    mask++;
                    y += rowstep;
                } while (--n);
                return; }
            }
        }
        switch (info->bits) {
        case 0x80:
            FUN_004cd896(dest, src, width, y, x, rowstep, colstep);
        case 0x40:
            FUN_004cd8da(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x20:
            FUN_004cd91e(dest, src, width, y, x, rowstep, colstep);
            return;
        case 0x10:
            FUN_004cd962(dest, src, width, y, x, rowstep, colstep);
            return;
        case 8: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + ((x >> 13) & ~7)];
                y += rowstep;
                x += colstep;
            } while (--n);
            return; }
        default: {
            int n = width;
            do {
                *dest++ = src[(y >> 16) + ((x >> 16) * info->bits)];
                y += rowstep;
                x += colstep;
            } while (--n);
            return; }
        }
    }
}

