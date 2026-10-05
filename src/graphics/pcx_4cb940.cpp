// Decompiled by Opus. Names are provisional.
// Bitmap file writer: seeks to the file row for `y` and writes `height` rows
// of the image, bottom-up, each padded to a multiple of 4 bytes. Returns
// false when the seek or a write fails.
#include <stdio.h>

struct Image_004cb940 {
    int width;                         // +0x0
    int unknown_4;
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
};

class Class_004cb940 {
public:
    int unknown_0;
    int height;                        // +0x4
    int dataOffset;                    // +0x8
    FILE* file;                        // +0xc

    bool FUN_004cb940(Image_004cb940* image, int x, int rows, int unused_4, int y,
                      int unused_6, int srcY);
};

// FUNCTION: 0x4cb940
bool Class_004cb940::FUN_004cb940(Image_004cb940* image, int x, int rows, int unused_4,
                                  int y, int unused_6, int srcY)
{
    int stride = (image->width + 3) & ~3;
    if (fseek(file, (height - rows - y) * stride + dataOffset, SEEK_SET) != 0) {
        return false;
    }
    for (int i = rows - 1; i >= 0; i--) {
        if (fwrite(image->pixels + image->pitch * (i + srcY), stride, 1, file) != 1) {
            return false;
        }
    }
    return true;
}
