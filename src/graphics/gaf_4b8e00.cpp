// Decompiled by Opus. Names are provisional.

struct Bitmap_004b8e00 {
    unsigned short width;           // +0x0
    unsigned short height;          // +0x2
    short unknown_4;                // +0x4
    short unknown_6;                // +0x6
    char unknown_8;                 // +0x8
    char unknown_9;                 // +0x9
    char unknown_a;                 // +0xa
    char unknown_b;                 // +0xb
    int unknown_c;                  // +0xc
    unsigned char* plane0;          // +0x10
    unsigned char* plane1;          // +0x14
};

void* __cdecl FUN_004d83b0(unsigned int param_1, unsigned int param_2);

// FUNCTION: 0x4b8e00
Bitmap_004b8e00* __stdcall AllocDepthFrame(unsigned int heap, int width, int height)
{
    int size = height * width;
    Bitmap_004b8e00* b = (Bitmap_004b8e00*)FUN_004d83b0(heap, size * 2 + sizeof(Bitmap_004b8e00));
    unsigned char* p = (unsigned char*)(b + 1);
    b->plane0 = p;
    b->height = height;
    p += size;
    b->plane1 = p;
    b->width = width;
    b->unknown_4 = 0;
    b->unknown_6 = 0;
    b->unknown_9 = 0;
    b->unknown_a = 0;
    b->unknown_b = 0;
    return b;
}
