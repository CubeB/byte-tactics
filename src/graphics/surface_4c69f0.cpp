// Decompiled by Opus. Names are provisional.
// Allocates an image of width x height bytes with its header (pixels follow
// the 0x30-byte header); the header set-up is the body of InitSurface,
// inlined.

struct Rect_004c6a60 {
    int left;
    int top;
    int right;
    int bottom;
    Rect_004c6a60() {}
    Rect_004c6a60(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

struct Class_004c6a60 {
    int width;                         // +0x0
    int height;                        // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    short field_18;                    // +0x18
    short field_1a;                    // +0x1a
    Rect_004c6a60 clip;                // +0x1c
    unsigned int flag0 : 1;            // +0x2c bit 0
    unsigned int flag1 : 1;            // +0x2c bit 1
    unsigned char pixels[1];           // +0x30
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

static inline void Init(Class_004c6a60* s, int width, int height, int a, int b)
{
    s->width = width;
    s->height = height;
    s->field_8 = a;
    s->field_c = b;
    s->field_18 = 0;
    s->field_1a = 0;
    s->field_10 = 10000;
    s->field_14 = -1;
    s->flag0 = 1;
    s->flag1 = 0;
    s->clip = Rect_004c6a60(0, 0, width - 1, height - 1);
}

// FUNCTION: 0x4c69f0
Class_004c6a60* __stdcall AllocSurface(char* name, int width, int height)
{
    Class_004c6a60* s = (Class_004c6a60*)FUN_004d83b0(name, height * width + 0x30);
    Init(s, width, height, width, (int)s->pixels);
    return s;
}
