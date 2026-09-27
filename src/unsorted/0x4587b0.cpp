// Decompiled by Opus. Names are provisional.
// Halves a bitmap into the destination: copies every other byte of every
// other source row (the source is twice the destination's size).

struct Bitmap_004587b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[0x10];
    unsigned char* data;               // +0x14
};

// FUNCTION: 0x4587b0
void __stdcall FUN_004587b0(Bitmap_004587b0* src, Bitmap_004587b0* dst)
{
    unsigned char* d = dst->data;
    if (d == 0)
        return;
    unsigned char* s = src->data;
    for (int y = 0; y < dst->height; y++) {
        unsigned int n = dst->width;
        while (n--) {
            unsigned int c = *s;
            *d++ = c;
            s += 2;
        }
        s += src->width;
    }
}
