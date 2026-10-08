// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Stays in its own file: the merged gaf.cpp cannot give it the header state
// its pixel read order needs.
// Blends two 2-byte source pixels per destination pixel: each source pixel is
// a pair of palette indices looked up in the app's 256x256 colour-blend table
// at +0xc0, and the two results are blended again. The source rows for output
// row y are y and 2y+1, and the destination is one byte per pixel.
// Must include <stdlib.h>: the header state sets the pixel read order.
#include <stdlib.h>

struct Image_004b95a0 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    char unknown_4[4];      // +0x4
    unsigned char colorKey; // +0x8
    char unknown_9[7];      // +0x9
    unsigned char* data;    // +0x10
    unsigned char* mask;    // +0x14
};

struct Palette_004b95a0 {
    char unknown_0[0xc0];
    unsigned char* table;   // +0xc0
};

Palette_004b95a0* GetDisplay();

// FUNCTION: 0x4b95a0
void __stdcall DownsampleFrame(Image_004b95a0* src, Image_004b95a0* dst)
{
    Palette_004b95a0* pal = GetDisplay();
    int row = 0;
    for (; row < dst->height; row++) {
        for (int x = 0; x < dst->width; x++) {
            // src->width and src->data stay inline in both expressions, not cached.
            unsigned char* p = src->data + (row * src->width + x) * 2;
            unsigned char* table = pal->table;
            unsigned char* q = src->data + (row * 2 + 1) * src->width + x * 2;
            int a = p[0];
            int b = p[1];
            int p1 = table[(a << 8) + b];
            int c = q[0];
            int d = q[1];
            int p2 = table[(c << 8) + d];
            dst->data[row * dst->width + x] = table[(p1 << 8) + p2];
        }
    }
}
