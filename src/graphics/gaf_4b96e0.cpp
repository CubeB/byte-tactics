// Decompiled by Opus. Names are provisional.
// Remaps every pixel of an 8-bit image whose mask value is at most `level`
// (and that is not the colour key) through a table of the current palette.

struct Palette_004b96e0 {
    char unknown_0[0xd0];
    unsigned char* remap;              // +0xd0
};

struct Image_004b96e0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[4];                 // +0x4
    unsigned char colorKey;            // +0x8
    char unknown_9[7];                 // +0x9
    unsigned char* data;               // +0x10
    unsigned char* mask;               // +0x14
};

Palette_004b96e0* FUN_004b6220();

// FUNCTION: 0x4b96e0
void __stdcall FUN_004b96e0(Image_004b96e0* image, unsigned char level)
{
    unsigned char* p = image->data;
    unsigned char* m = image->mask;
    int count = image->height * image->width;
    Palette_004b96e0* pal = FUN_004b6220();
    while (count--) {
        if (*m <= level && *p != image->colorKey) {
            *p = pal->remap[*p];
        }
        p++;
        m++;
    }
}
