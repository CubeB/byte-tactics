// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by claude-opus-5-5. re-verified by GPT-6. Names are provisional.
//
// Builds the "lens" displacement frame that 0x420620 asks for with
// (22, 22, 8): a GAF-style frame header with two w*h buffers of 16-bit cells.
// Each cell inside radius w/4 of the centre holds the offset (in cells) to
// the source pixel of a magnifying lens; the others hold 0x7d00 (no
// displacement).
//
// claude-opus-5-5 (#4733): 55.9% -> 99.3% (423 of 423 bytes). The earlier
// "register family" notes were all symptoms of four source-shape errors:
//  - The header set-up is the real FUN_004b8e00 inlined (alloc w*h*2 cells
//    plus a 0x18 header, width, plane0/plane1, height, zeroed x/y/flags).
//    It has no null check of its own, which is why the stores come before
//    the null test. The copy below (width, plane0, height, plane1) MATCHES
//    0x4b8e00 out of line as well; the order in 0x4b8e00's own file
//    (plane0, height, plane1, width) also matches there but costs 1.4
//    points here, because inlined it stores height before width.
//  - The cells pointer is a `p` local read from f->plane0 (MSVC forwards
//    the helper's store), and the cell index is `y * w + x`, not a base
//    accumulator: MSVC strength-reduces y * w itself into the esi induction
//    variable (initialised after the h > 0 test, as in the original), so
//    there is no walking pointer and no folding of the 0x18 header into the
//    index. A base accumulator gave either a walking pointer or a
//    base-starts-at-12 fold in every spelling.
//  - The falloff branch computes `int i = y * w + x;` FIRST, before g. That
//    is what keeps the index as one value used by both the store address
//    and the subtraction (`add esi, edi` straight after the fisubr).
//  - hw and hh are `short` locals and the falloff uses `w / 2 - dist`: the
//    int w/2 lives in the dead `lens` slot (fisubr dword) while the shorts
//    are sign-extended at each use. With an int hw, f and w/2 swap the two
//    dead parameter slots.
//
// GPT-6 recheck: no improvement from an inline Centered(y, hh) helper, an
// inline HalfValue helper for hw/hh, `hh = hh`, or a `register short hh`
// hint (all byte-identical at 99.3%). A named row = y * w local changes
// index lowering and falls to 59.5%, so keep the direct y * w + x form.
// What still differs: one instruction pair. After the inner loop the
// original restores the outer-loop registers in the order hw (ebx),
// hh (edx), y (eax), f (ecx); this file restores hh last
// (ebx, eax, ecx, edx). Measured, none of which gives the original's order
// (each keeps 99.3% or loses points): every order of the six set-up
// statements, int or short hw/hh with and without (short) casts, w / 2 or
// hw in the falloff and the f->x store, h / 2 or hh in the f->y store,
// declaring f/p/hw/hh/y up front in all 120 orders, dy as a variable or
// repeated, and a second pointer variable for the alloc result (that one
// reverses the restore order to edx, ecx, eax, ebx and loses 50 points).
// Also flat: loop and compare spellings, the value expression in ten
// groupings, y * w + x operand orders, /Gi (95.9%), and a 15-minute permuter
// run from this file (8531 candidates, no gain). The order looks like the
// outer-region allocation priority: writing the halving as
// `f->width = (unsigned short)(w * 2) / 2` (one f reference fewer) moves f
// to the end (ebx, eax, edx, ecx) but breaks the prologue (80.3%). So the
// original needs f a little lower and hh a little higher than here; the
// construct that does that is the lead for the next attempt.
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

Bitmap_004b8e00* __stdcall FUN_004b8e00(const char* name, int width, int height)
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
void* __stdcall FUN_004b91b0(int w, int h, int lens)
{
    double scale = lens;
    Bitmap_004b8e00* f = FUN_004b8e00("LensFrame", w * 2, h);
    if (!f)
        return 0;
    f->width /= 2;
    unsigned short* p = (unsigned short*)f->plane0;
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
                int i = y * w + x;
                double g = (w / 2 - dist) / scale;
                int v = (int)(dx / g) + (w * ((int)(dy / g) + hh) + hw);
                p[i] = v - i;
            }
        }
    }
    return f;
}
