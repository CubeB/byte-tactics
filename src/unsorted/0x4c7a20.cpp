// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Partial (62.8%). Everything from the mask switch onward matches once the
// stack offsets line up; the only real difference is the prologue frame size:
// the original is `sub esp,0x1c` (7 locals), ours is `sub esp,0x20` (8),
// because MSVC keeps the value of span[0] live across the three idivs and
// spills it to an extra slot (stores eax=span[0] to [esp+0x24] right after the
// span[1] load). The original instead discards it and reloads `[ebx]` at the
// left-clip test (`mov edx,[ebx]; test edx,edx`), so span[0] never gets a
// second slot. Both are semantically identical; the +4 shifts every later
// offset and flips the register picks in the mask loops (e.g. the 0x80 body
// computes the source index in eax instead of edx, so `and edx,0xffffff80`
// becomes the two byte `and al,0x80`, and `imul eax,[esp+0x30]` becomes
// `imul edx,eax`). Tried and no change: reusing `width` vs a fresh `count`;
// writing the divisor `(span[1]-span[0])` inline three times (MSVC still CSEs
// the span[0] load); declaring `src` after the step computations (worse,
// 61.9%). Next idea: force the reload, e.g. an intermediate that clobbers the
// value's memory expression, so the frame drops to 0x1c.
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
void __stdcall FUN_004c7a20(int row, int* span, Surf_4c7a20* surf, Info_4c7a20* info)
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
        span[6] -= dstep * span[0];
        span[0] = 0;
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
