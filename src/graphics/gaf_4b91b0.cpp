// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-opus-5-5, re-verified by GPT-6, finished by Claude Opus 5.5. Names are provisional.
//
// Builds the "lens" displacement frame that 0x420620 asks for with
// (22, 22, 8): a GAF-style frame header with two w*h buffers of 16-bit cells.
// Each cell inside radius w/4 of the centre holds the offset (in cells) to
// the source pixel of a magnifying lens; the others hold 0x7d00 (no
// displacement).
#include <math.h>

struct Bitmap_004b8e00 {
    unsigned short width;           // +0x0
    unsigned short height;          // +0x2
    short x;                        // +0x4
    short y;                        // +0x6
    char unknown_8;                 // +0x8
    char unknown_9;                 // +0x9
    char unknown_a;                 // +0xa
    char unknown_b;                 // +0xb
    int unknown_c;                  // +0xc
    unsigned char* plane0;          // +0x10
    unsigned char* plane1;          // +0x14
};

extern void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

Bitmap_004b8e00* __stdcall AllocDepthFrame(const char* name, int width, int height)
{
    int size = height * width;
    Bitmap_004b8e00* b = (Bitmap_004b8e00*)FUN_004d83b0(name, size * 2 + sizeof(Bitmap_004b8e00));
    unsigned char* p = (unsigned char*)(b + 1);
    b->width = width;
    b->plane0 = p;
    b->height = height;
    p += size;
    b->plane1 = p;
    b->x = 0;
    b->y = 0;
    b->unknown_9 = 0;
    b->unknown_a = 0;
    b->unknown_b = 0;
    return b;
}

// FUNCTION: 0x4b91b0
void* __stdcall BuildLensFrame(int w, int h, int lens)
{
    double scale = lens;
    Bitmap_004b8e00* f = AllocDepthFrame("LensFrame", w * 2, h);
    if (!f)
        return 0;
    f->width /= 2;
    unsigned short* p = (unsigned short*)f->plane0;
    // hw and hh stay short: the int w / 2 lives in the dead lens slot.
    short hw = w / 2;
    f->x = hw;
    short hh = h / 2;
    f->y = hh;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int dx = x - hw;
            int dy = y - hh;
            double dist = sqrt((double)(dx * dx + dy * dy));
            if ((int)dist >= w / 4) {
                p[y * w + x] = 0x7d00;
            } else {
                // Computed before the gain: keeps the index one shared value.
                int i = y * w + x;
                // The gain is written out at both uses, not named.
                int v = (int)(dx / ((w / 2 - dist) / scale))
                        + (w * ((int)(dy / ((w / 2 - dist) / scale)) + hh) + hw);
                p[i] = v - i;
            }
        }
    }
    return f;
}
