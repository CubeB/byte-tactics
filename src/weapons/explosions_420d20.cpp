// Decompiled by Claude Opus 5.5. Names are provisional.
#include <math.h>
#include <stdlib.h>

// The aligned frame (`and esp, -8`) comes from the default flags here, no /Op:
// MSVC 5 aligns the frame itself once enough doubles live in stack slots.

struct Class_004b8da0 {
    short width;              // +0x0
    short height;             // +0x2
    short field_4;            // +0x4
    short field_6;            // +0x6
    unsigned char field_8;    // +0x8
    unsigned char field_9;    // +0x9
    unsigned char field_a;    // +0xa
    unsigned char field_b;    // +0xb
    int field_c;              // +0xc
    unsigned char* data;      // +0x10
};

Class_004b8da0* __stdcall FUN_004b8da0(const char* name, int width, int height);

// A size x size explosion frame: a noisy, slightly squashed radial gradient
// from the centre outwards, transparent (0xff) outside the circle.
// FUNCTION: 0x420d20
Class_004b8da0* FUN_00420d20(int size)
{
    double half = size / 2;
    Class_004b8da0* img = FUN_004b8da0("ExplosionFrame", size, size);
    img->field_6 = img->field_4 = (short)half;
    for (int y = 0; y < size; y++) {
        double dy = half - y;
        double dy2 = dy * dy * 1.33;
        for (int x = 0; x < size; x++) {
            double dx = half - x;
            double d = (double)(int)(rand() * (__int64)10 / 0x8000) + sqrt(dx * dx + dy2);
            unsigned char c = 32 - (unsigned char)(int)(d / half * 32.0);
            if (c >= 0x22)
                img->data[y * size + x] = 0xff;
            else if (c >= 0x20)
                img->data[y * size + x] = 0x6e;
            else
                img->data[y * size + x] = c + 0x4f;
        }
    }
    img->field_8 = 0xff;
    return img;
}
