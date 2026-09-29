// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial: the depth-buffer path (mask != 0) has five duplicated inline span
// loops plus a default; the no-depth path dispatches to FUN_004cd8xx helpers.
// Still differs: overall register roles and slot reuse in the prologue.
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

void FUN_004cd896(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void FUN_004cd8da(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void FUN_004cd91e(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);
void FUN_004cd962(unsigned char* dest, unsigned char* src, int width, int y, int x, int rowstep, int colstep);

// FUNCTION: 0x4c7a20
void __stdcall FUN_004c7a20(int row, int* r, Surf_4c7a20* surf, Info_4c7a20* info)
{
    unsigned char* mask = surf->mask;
    unsigned char* dest = surf->pixels;
    unsigned char* src = info->data;
    int dx = r[1] - r[0];
    int rowstep = (r[4] - r[2]) / dx;
    int colstep = (r[5] - r[3]) / dx;
    int dstep = (r[7] - r[6]) / dx;

    if (r[0] < 0) {
        r[2] -= rowstep * r[0];
        r[3] -= colstep * r[0];
        r[6] -= dstep * r[0];
        r[0] = 0;
    }
    {
        int limit = (int)surf->pitch - 1;
        if (r[1] > limit)
            r[1] = limit;
    }
    {
        int width = r[1] - r[0];
        if (width > 0) {
            int y = r[2];
            int x = r[3];
            int w = r[6];

            dest += surf->pitch * row + r[0];
            if (mask != 0) {
                unsigned char* m = mask + surf->pitch * row + r[0];
                switch (info->bits) {
                case 0x80: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 9) & ~0x7f)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
                        y += rowstep;
                    } while (--n);
                    return; }
                case 0x40: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 10) & ~0x3f)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
                        y += rowstep;
                    } while (--n);
                    return; }
                case 0x20: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 11) & ~0x1f)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
                        y += rowstep;
                    } while (--n);
                    return; }
                case 0x10: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 12) & ~0xf)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
                        y += rowstep;
                    } while (--n);
                    return; }
                case 8: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 13) & ~7)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
                        y += rowstep;
                    } while (--n);
                    return; }
                default: {
                    int n = width;
                    do {
                        if (*m <= (unsigned char)(w >> 16)) {
                            *dest = src[(y >> 16) + ((x >> 16) * info->bits)];
                            *m = (unsigned char)(w >> 16);
                        }
                        x += colstep;
                        w += dstep;
                        dest++;
                        m++;
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
}
