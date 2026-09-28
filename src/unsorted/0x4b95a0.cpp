// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Blends two 2-byte source pixels per destination pixel: each source pixel is
// a pair of palette indices looked up in the app's 256x256 colour-blend table
// at +0xc0, and the two results are blended again. The source rows for output
// row y are y and 2y+1, and the destination is one byte per pixel.
//
// Partial, check.py 81.1%. Every field offset, the two nested for loops, the
// strength-reduced (row*2+1) stride counter and the inlined lookups match; the
// remaining differences are MSVC 5 register allocation:
//   * the first source index is computed into eax and the +0xc0 load sinks
//     below the pointer lea, where the original computes the index in ebx
//     (imul ebx,esi) and loads the table above the lea;
//   * in the second lookup the original reads q[1] before q[0] and keeps
//     q[0]<<8 in eax (esi holds q[1]); ours reads q[0] first;
//   * at the destination store the original adds x to the row offset before
//     loading dst->data, ours folds dst->data in first and indexes with x.
// <memory.h> is included only because MSVC 5's allocator and operand order
// depend on it; without it check.py drops to 80.5%.
#include <memory.h>

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

Palette_004b95a0* FUN_004b6220();

// FUNCTION: 0x4b95a0
void __stdcall FUN_004b95a0(Image_004b95a0* src, Image_004b95a0* dst)
{
    Palette_004b95a0* pal = FUN_004b6220();
    int row = 0;
    for (; row < dst->height; row++) {
        for (int x = 0; x < dst->width; x++) {
            int w = src->width;
            unsigned char* data = src->data;
            unsigned char* p = data + (row * w + x) * 2;
            unsigned char* table = pal->table;
            unsigned char* q = data + (row * 2 + 1) * w + x * 2;
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
