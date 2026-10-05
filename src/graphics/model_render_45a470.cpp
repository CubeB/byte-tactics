// Decompiled by Opus. Names are provisional.
// Copies an 8-bit image (header, pixels and the optional second plane) into
// the image at +0x10, clears its non-key pixels (FUN_004b96a0) and returns it.
#include <string.h>

struct Image_0045a470 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    short field_4;                      // +0x4
    short field_6;                      // +0x6
    char colorKey;                      // +0x8
    char unknown_9[7];
    char* data;                         // +0x10
    char* data2;                        // +0x14
};

void __stdcall FUN_004b96a0(Image_0045a470* image);

class Class_0045a470 {
public:
    char unknown_0[0x10];
    Image_0045a470* image;              // +0x10
    Image_0045a470* FUN_0045a470(Image_0045a470* src);
};

// FUNCTION: 0x45a470
Image_0045a470* Class_0045a470::FUN_0045a470(Image_0045a470* src)
{
    image->width = src->width;
    image->height = src->height;
    image->field_4 = src->field_4;
    image->field_6 = src->field_6;
    image->colorKey = src->colorKey;
    memcpy(image->data, src->data, src->width * src->height);
    if (src->data2)
        memcpy(image->data2, src->data2, src->width * src->height);
    FUN_004b96a0(image);
    return image;
}
