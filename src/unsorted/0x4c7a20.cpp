// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// Partial (62.8%). GPT-6.1-sol retry: a reference alias for the initial span[0] produced the same score. No better variant found. Everything from the mask switch onward is structurally
// identical to the original; the only real difference is the prologue frame
// size: the original is `sub esp,0x1c` (7 frame slots), ours is `sub esp,0x20`
// (8), because MSVC keeps the value of span[0] live from the `span[1]-span[0]`
// width computation all the way to the left-clip test and spills it to an extra
// slot (an extra `mov [esp+0x24],eax` right after the span[2] load, and the
// clip test then reads `mov edx,[esp+0x24]` instead of `mov edx,[ebx]`). The
// original clearly has three separate span[0] values (dead after `sub edi,eax`,
// one for the three `imul`s inside the clip block, one reloaded at 0x4c7ad0 for
// `dest`/`mask`), and only the last of them owns slot [esp+0x24] (which the
// original shares with the span[2] temp, so 7 slots in total). Our source merges
// all of them into one CSE temp. The +4 shifts every later offset and also flips
// two register picks that are probably the same allocator state: in the mask
// loop bodies the source index is in eax instead of edx, so `and edx,0xffffff80`
// becomes the byte `and al,0x80` and the row term goes through `add ebp,edx`
// while the index goes in the addressing mode; and `imul eax,[esp+0x30]` becomes
// `imul edx,eax` with an extra `mov`.
// Tried, all 62.8% (frame stays 0x20, prologue spill store stays):
//   - the index written first in the subscript, `src[((x>>9)&~0x7f)+(y>>16)]`
//   - the full dword masks 0xffffff80/0xc0/0xe0/0xf0/0xf8 instead of ~0x7f...
//   - a named local for the index inside the 0x80 case
//   - `int width = span[1] - *span`, and a second `const int*` alias for span
//     (VNT evidently still unifies the two loads, so the spill store remains)
//   - the clip test as `!(span[0] >= 0)` and as `0 > span[0]`
//   - a named local copy of span[0] used by the three `imul`s in the clip block
//   - a named local x0 for the post-clamp span[0] used by dest and mask, both
//     declared inside and before the `if (width > 0)` block
//   - reordering the three `span[k] -= step * span[0]` statements (59.6%)
//   - the three steps declared and assigned in a different order (41.2%)
// Moving `span[0] = 0;` to the top of the clip block does drop the frame to
// `sub esp,0x18` (the store kills the CSE temp), but then the three `-=` read
// the zeroed element and MSVC folds the whole block away: 43.9%, 1481 bytes.
// So the remaining puzzle is one value: the original's prologue span[0] temp
// must not be merged with the clip test's.
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

