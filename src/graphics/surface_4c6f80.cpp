// Decompiled by space-bunny-free. Names are provisional.
// Reads back a surface written by 0x4c6f10: an 8-byte header of width and
// height, then one row per scan line into a freshly allocated surface.
// The allocation and the header set-up (the body of 0x4c6a60) are one inlined
// helper, so its width and height arguments live in callee-saved registers
// across the FUN_004d83b0 call.
// <ddraw.h> decides the operand order of the i * s->pitch multiply in the row
// loop (compiler state, not header content; see 0x4c6f10.cpp).
#include <ddraw.h>

struct Rect_004c6f80 {
    int left;
    int top;
    int right;
    int bottom;
    Rect_004c6f80() {}
    Rect_004c6f80(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

struct Surface_004c6f80 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    char* data;                        // +0xc
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    short field_18;                    // +0x18
    short field_1a;                    // +0x1a
    Rect_004c6f80 clip;                // +0x1c
    unsigned int flag0 : 1;            // +0x2c bit 0
    unsigned int flag1 : 1;            // +0x2c bit 1
    unsigned char pixels[1];           // +0x30
};

class HapiBank {
public:
    void SeekBox(int pos);
    int ReadBox(void* dst, int len);
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* ptr);

static inline Surface_004c6f80* NewSurface(char* name, int w, int h)
{
    Surface_004c6f80* s = (Surface_004c6f80*)FUN_004d83b0(name, h * w + 0x30);
    s->width = w;
    s->pitch = w;
    s->height = h;
    s->field_18 = 0;
    s->data = (char*)s->pixels;
    s->flag0 = 1;
    s->flag1 = 0;
    s->field_1a = 0;
    s->field_10 = 10000;
    s->field_14 = -1;
    s->clip = Rect_004c6f80(0, 0, w - 1, h - 1);
    return s;
}

// FUNCTION: 0x4c6f80
Surface_004c6f80* __stdcall LoadSurface(void* file)
{
    ((HapiBank*)file)->SeekBox(0);
    int header[2];
    if (((HapiBank*)file)->ReadBox(header, 8) < 8u) {
        return 0;
    }
    Surface_004c6f80* s = NewSurface("Loaded Surface", header[0], header[1]);
    for (int i = 0; i < header[1]; i++) {
        if (((HapiBank*)file)->ReadBox(s->data + i * s->pitch, header[0]) < header[0]) {
            FUN_004d85a0(s);
            return 0;
        }
    }
    return s;
}
