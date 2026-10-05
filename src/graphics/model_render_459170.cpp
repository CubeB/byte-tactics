// Decompiled by Opus. Names are provisional.
// Copies an image (size, three header fields, pixels and optional mask) into
// the object's own image, whose buffers are already allocated.
#include <string.h>

struct Image_00459170 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char field_8;                      // +0x8
    char unknown_9[7];
    void* pixels;                      // +0x10
    void* mask;                        // +0x14
};

class CMemoryCache {
public:
    char unknown_0[0x10];
    Image_00459170* image;             // +0x10

    void CopyPicture(Image_00459170* source);
};

// FUNCTION: 0x459170
void CMemoryCache::CopyPicture(Image_00459170* source)
{
    image->width = source->width;
    image->height = source->height;
    image->field_4 = source->field_4;
    image->field_6 = source->field_6;
    image->field_8 = source->field_8;
    memcpy(image->pixels, source->pixels, source->width * source->height);
    if (source->mask)
        memcpy(image->mask, source->mask, source->width * source->height);
}
