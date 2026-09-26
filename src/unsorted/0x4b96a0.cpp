// Decompiled by Opus. Names are provisional.
// Clears every pixel of an 8-bit image that is not the colour key.

struct Image_004b96a0 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    char unknown_4[4];      // +0x4
    char colorKey;          // +0x8
    char unknown_9[7];      // +0x9
    char* data;             // +0x10
};

// FUNCTION: 0x4b96a0
void __stdcall FUN_004b96a0(Image_004b96a0* image)
{
    int count = image->height * image->width;
    char* p = image->data;
    while (count--) {
        if (*p != image->colorKey) {
            *p = 0;
        }
        p++;
    }
}
