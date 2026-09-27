// Decompiled by Opus. Names are provisional.
// Allocates a one-plane w*h bitmap from the arena (0x437a30): a 0x18-byte
// header, then the pixels, filled with 1. One-plane version of 0x437be0.
#include <string.h>

struct Bitmap_00437b50 {
    short width;                       // +0x0
    short height;                      // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char field_8;                      // +0x8
    char field_9;                      // +0x9
    char field_a;                      // +0xa
    char field_b;                      // +0xb
    int field_c;                       // +0xc
    unsigned char* pixels;             // +0x10
    int field_14;                      // +0x14
};

class Class_00437a30 {
public:
    int FUN_00437a30(void** handle, int size);
    int FUN_00437b50(Bitmap_00437b50** handle, int w, int h);
};

// FUNCTION: 0x437b50
int Class_00437a30::FUN_00437b50(Bitmap_00437b50** handle, int w, int h)
{
    int n = w * h;
    if (!FUN_00437a30((void**)handle, n + 0x18))
        return 0;
    Bitmap_00437b50* b = *handle;
    if (!b)
        return 0;
    b->field_14 = 0;
    b->field_4 = 0;
    b->field_6 = 0;
    b->field_a = 0;
    b->field_b = 0;
    b->field_c = 0;
    b->field_9 = 0;
    b->width = w;
    b->height = h;
    b->pixels = (unsigned char*)(b + 1);
    b->field_8 = 1;
    memset(b->pixels, 1, n);
    return 1;
}
