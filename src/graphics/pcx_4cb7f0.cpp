// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens a 256-colour BMP file for writing and emits the BMP file header,
// the BITMAPINFOHEADER and the palette (the game's RGB palette bytes are
// stored red, green, blue and written out reversed as blue, green, red).
// width/height are kept in the object, plus the offset of the pixel data
// (ftell right after the header) and the FILE*. Returns false if the file
// cannot be opened or either header write is short.
#include <stdio.h>

#pragma pack(push, 2)
struct BmpFileHeader {
    unsigned short bfType;      // +0x0
    unsigned int bfSize;        // +0x2
    unsigned short bfReserved1; // +0x6
    unsigned short bfReserved2; // +0x8
    unsigned int bfOffBits;     // +0xa
};
#pragma pack(pop)

struct BmpInfoHeader {
    unsigned int biSize;        // +0x0
    int biWidth;                // +0x4
    int biHeight;               // +0x8
    unsigned short biPlanes;    // +0xc
    unsigned short biBitCount;  // +0xe
    unsigned int biCompression; // +0x10
    unsigned int biSizeImage;   // +0x14
    int biXPelsPerMeter;        // +0x18
    int biYPelsPerMeter;        // +0x1c
    unsigned int biClrUsed;     // +0x20
    unsigned int biClrImportant;// +0x24
};

struct RgbQuad {
    unsigned char blue;         // +0x0
    unsigned char green;        // +0x1
    unsigned char red;          // +0x2
    unsigned char reserved;     // +0x3
};

// Header plus all 256 palette entries (0x28 + 256 * 4 = 0x428 bytes).
struct BmpInfo {
    BmpInfoHeader header;
    RgbQuad colors[256];
};

void* GetDisplay();

class Class_004cb7f0 {
public:
    int width;                  // +0x0
    int height;                 // +0x4
    int dataOffset;             // +0x8
    FILE* file;                 // +0xc
    bool Open(const char* name, int w, int h);
};

// FUNCTION: 0x4cb7f0
bool Class_004cb7f0::Open(const char* name, int w, int h)
{
    void* palbase = GetDisplay();

    width = w;
    height = h;
    file = fopen(name, "wb");
    if (file == NULL) {
        return false;
    }
    BmpFileHeader fh;
    fh.bfType = 0x4d42;
    fh.bfSize = 0;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = 0x436;
    if (fwrite(&fh, sizeof(fh), 1, file) != 1) {
        return false;
    }
    BmpInfo info;
    info.header.biSize = 0x28;
    info.header.biWidth = w;
    info.header.biHeight = h;
    info.header.biPlanes = 1;
    info.header.biBitCount = 8;
    info.header.biCompression = 0;
    info.header.biSizeImage = 0;
    info.header.biXPelsPerMeter = 3000;
    info.header.biYPelsPerMeter = 3000;
    info.header.biClrUsed = 0x100;
    info.header.biClrImportant = 0x100;
    unsigned char* dp = (unsigned char*)info.colors + 1;
    unsigned char* sp = (unsigned char*)palbase + 0x215;
    for (int i = 0; i < 256; i++) {
        dp[1] = sp[-1];
        dp[0] = sp[0];
        dp[-1] = sp[1];
        dp += 4;
        sp += 4;
    }
    if (fwrite(&info, sizeof(info), 1, file) != 1) {
        return false;
    }
    dataOffset = ftell(file);
    return true;
}
