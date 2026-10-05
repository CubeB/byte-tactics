// Decompiled by Opus. Names are provisional.
// Initialises a drawing surface description from a source record: width,
// height, pitch (= width) and bits, then resets its clip rectangle through
// ResetClipRect.

struct Src_004b8a80 {
    unsigned short width;           // +0x0
    unsigned short height;          // +0x2
    unsigned short x;               // +0x4
    unsigned short y;               // +0x6
    char unknown_8[8];
    int bits;                       // +0x10
};

struct Surface {
    int width;                      // +0x0
    int height;                     // +0x4
    int pitch;                      // +0x8
    int bits;                       // +0xc
    int field_10;                   // +0x10
    int field_14;                   // +0x14
    unsigned short x;               // +0x18
    unsigned short y;               // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;         // +0x2c bit 0
    unsigned int flag1 : 1;         // +0x2c bit 1
};

void __stdcall ResetClipRect(int* param_1);

// FUNCTION: 0x4b8a80
void __stdcall SurfaceFromFrame(Surface* dst, Src_004b8a80* src)
{
    dst->width = src->width;
    dst->height = src->height;
    dst->pitch = src->width;
    dst->bits = src->bits;
    dst->field_10 = 10000;
    dst->field_14 = -1;
    dst->x = src->x;
    dst->y = src->y;
    dst->flag1 = 0;
    dst->flag0 = 1;
    ResetClipRect((int*)dst);
}
