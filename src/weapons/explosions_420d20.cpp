// Decompiled by Claude Opus 5.5. Names are provisional.
#include <math.h>
#include <stdlib.h>

// The aligned frame (`and esp, -8`) comes from the default flags here, no /Op:
// MSVC 5 aligns the frame itself once enough doubles live in stack slots.

struct GafFrame {
    short width;              // +0x0
    short height;             // +0x2
    short xOffset;            // +0x4
    short yOffset;            // +0x6
    unsigned char transparency;  // +0x8
    unsigned char compressed;  // +0x9
    unsigned char layers;     // +0xa
    unsigned char blend;      // +0xb
    int reserved;             // +0xc
    unsigned char* data;      // +0x10
};

GafFrame* __stdcall AllocFrame(const char* name, int width, int height);

// A size x size explosion frame: a noisy, slightly squashed radial gradient
// from the centre outwards, transparent (0xff) outside the circle.
// FUNCTION: 0x420d20
GafFrame* BuildExplosionFrame(int size)
{
    double half = size / 2;
    GafFrame* img = AllocFrame("ExplosionFrame", size, size);
    img->yOffset = img->xOffset = (short)half;
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
    img->transparency = 0xff;
    return img;
}
