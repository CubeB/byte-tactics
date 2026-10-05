// Decompiled by Opus. Names are provisional.

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
    int field_14;             // +0x14
    unsigned char pixels[1];  // +0x18
};

void* __cdecl FUN_004d83b0(unsigned int param_1, unsigned int param_2);

// FUNCTION: 0x4b8da0
Class_004b8da0* __stdcall FUN_004b8da0(unsigned int param_1, int width, int height)
{
    Class_004b8da0* p = (Class_004b8da0*)FUN_004d83b0(param_1, height * width + 0x18);
    if (p == 0) {
        return 0;
    }
    p->width = width;
    p->height = height;
    p->data = p->pixels;
    p->field_14 = 0;
    p->field_4 = 0;
    p->field_6 = 0;
    p->field_9 = 0;
    p->field_a = 0;
    p->field_b = 0;
    return p;
}
