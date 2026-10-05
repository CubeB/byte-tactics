// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the pixel rows of a bitmap into the file opened by
// Class_004cb7f0::Open, bottom-up, each row padded to 4 bytes.
// Suspected original bug: the seek offset subtracts `n` (the image height the
// caller just passed to Open, which stores it in bmp.height), so
// `bmp.height - n` is always 0 and the seek always lands on dataOffset.
#include <stdio.h>

struct Image_004cb9e0 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
};

class Class_004cb7f0 {
public:
    int width;                         // +0x0
    int height;                        // +0x4
    int dataOffset;                    // +0x8
    FILE* file;                        // +0xc

    bool Open(char* name, int width, int height);
};

// FUNCTION: 0x4cb9e0
int __stdcall SaveBmp(char* name, Image_004cb9e0* image)
{
    Class_004cb7f0 bmp;
    bool ok;
    bmp.file = 0;
    if (!bmp.Open(name, image->width, image->height)) {
        if (bmp.file) fclose(bmp.file);
        return 0;
    }
    {
        int stride = (image->width + 3) & ~3;
        int n = image->height;
        if (fseek(bmp.file, (bmp.height - n) * stride + bmp.dataOffset, SEEK_SET) != 0) {
            ok = false;
            goto done;
        }
        for (int i = n - 1; i >= 0; i--) {
            if (fwrite(image->pixels + i * image->pitch, stride, 1, bmp.file) != 1) {
                ok = false;
                goto done;
            }
        }
        ok = true;
    }
done:
    if (!ok) {
        if (bmp.file) fclose(bmp.file);
        return 0;
    }
    if (bmp.file) fclose(bmp.file);
    return 1;
}
