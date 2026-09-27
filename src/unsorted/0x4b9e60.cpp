// Decompiled by Opus. Names are provisional.
// Compresses an image row by row with FUN_004ba000 (which skips runs of the
// transparent key colour). Each row is preceded by its 16-bit compressed
// length; with a null destination it only measures. Returns the total size.

struct Image_004b9e60 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
    unsigned short field_4;
    unsigned short field_6;
    unsigned char key;                  // +0x8
    char unknown_9[7];
    unsigned char* data;                // +0x10
};

int __stdcall FUN_004ba000(unsigned char* dest, unsigned char* src, int width, unsigned char key);

// FUNCTION: 0x4b9e60
int __stdcall FUN_004b9e60(unsigned char* dest, Image_004b9e60* img)
{
    int total = 0;
    unsigned char* src = img->data;
    int width = img->width;
    int height = img->height;
    while (height-- > 0) {
        unsigned short* len;
        if (dest) {
            len = (unsigned short*)dest;
            dest += 2;
        }
        int n = FUN_004ba000(dest, src, width, img->key);
        src += width;
        total += n + 2;
        if (dest) {
            dest += n;
            *len = n;
        }
    }
    return total;
}
