// Decompiled by Opus. Names are provisional.
// Allocates a two-plane w*h bitmap from the arena (0x437a30): a 0x18-byte
// header, then a plane filled with 1 and a plane cleared to 0. The plane
// pointer is taken before the header stores (it keeps the zero out of eax).
#include <string.h>

struct Bitmap_00437be0 {
    short width;                       // +0x0
    short height;                      // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char field_8;                      // +0x8
    char field_9;                      // +0x9
    char field_a;                      // +0xa
    char field_b;                      // +0xb
    int field_c;                       // +0xc
    unsigned char* plane1;             // +0x10
    unsigned char* plane2;             // +0x14
};

class CMemoryCache {
public:
    int AllocHandle(void** handle, int size);
    int AllocTwoPlaneBitmap(Bitmap_00437be0** handle, int w, int h);
};

// FUNCTION: 0x437be0
int CMemoryCache::AllocTwoPlaneBitmap(Bitmap_00437be0** handle, int w, int h)
{
    int n = w * h;
    if (!AllocHandle((void**)handle, n * 2 + 0x18))
        return 0;
    Bitmap_00437be0* b = *handle;
    if (!b)
        return 0;
    unsigned char* p = (unsigned char*)(b + 1);
    b->field_4 = 0;
    b->field_6 = 0;
    b->field_a = 0;
    b->field_b = 0;
    b->field_c = 0;
    b->field_9 = 0;
    b->width = w;
    b->height = h;
    b->plane1 = p;
    b->plane2 = p + n;
    memset(b->plane2, 0, n);
    b->field_8 = 1;
    memset(b->plane1, 1, n);
    return 1;
}
