// Decompiled by Opus. Names are provisional.
// Fills every pixel of an 8-bit image whose mask value is at most `level`
// with the image's colour key.

struct Palette_004ba1b0;

struct Image_004ba1b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    char unknown_4[4];                 // +0x4
    unsigned char colorKey;            // +0x8
    char unknown_9[7];                 // +0x9
    unsigned char* data;               // +0x10
    unsigned char* mask;               // +0x14
};

Palette_004ba1b0* FUN_004b6220();

// FUNCTION: 0x4ba1b0
void __stdcall FUN_004ba1b0(Image_004ba1b0* image, unsigned char level)
{
    unsigned char* p = image->data;
    unsigned char* m = image->mask;
    int count = image->height * image->width;
    Palette_004ba1b0* pal = FUN_004b6220();
    while (count--) {
        if (*m <= level) {
            *p = image->colorKey;
        }
        p++;
        m++;
    }
}
