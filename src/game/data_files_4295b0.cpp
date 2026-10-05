// Decompiled by Space Bunny Free. Names are provisional.
// Loads a picture from `path`. It reads a 0x40-byte header; if bit 0 of the
// byte at header+0x2c is set it seeks to the offset stored at header+0x28,
// reads width and height (two dwords), allocates a RADARPIC bitmap and reads
// width*height pixels into it. Otherwise it returns null. The two dwords at
// header+4 and header+8 are reported through the output pointers.
//
// The local frame is a single struct: the pixel dimensions live at +0/+4, a
// never-read dword sits at +8 (it is what makes the frame 0x4c bytes), and the
// 0x40-byte header starts at +0xc.

struct Bitmap_004b8da0 {
    short width;              // +0x0
    short height;             // +0x2
    short field_4;            // +0x4
    short field_6;            // +0x6
    unsigned char field_8;    // +0x8
    unsigned char field_9;    // +0x9
    unsigned char field_a;    // +0xa
    unsigned char field_b;    // +0xb
    int field_c;              // +0xc
    unsigned char* data;      // +0x10
    int field_14;             // +0x14
    unsigned char pixels[1];  // +0x18
};

struct Pic_004295b0 {
    int w;               // +0x0
    int h;               // +0x4
    int unknown;         // +0x8
    char header[0x40];   // +0xc
};

void* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_FileLength(void* file);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_CloseFile(void* file);
Bitmap_004b8da0* __stdcall AllocFrame(char* name, int width, int height);

// FUNCTION: 0x4295b0
Bitmap_004b8da0* __stdcall FUN_004295b0(char* path, int* outX, int* outY)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0) {
        return 0;
    }
    HAPI_FileLength(file);
    Pic_004295b0 pic;
    HAPI_readfromfile(file, pic.header, 0x40);
    Bitmap_004b8da0* bmp;
    if (pic.header[0x2c] & 1) {
        HAPI_SeekFile(file, *(int*)(pic.header + 0x28));
        HAPI_readfromfile(file, &pic.w, 8);
        bmp = AllocFrame("RADARPIC", pic.w, pic.h);
        HAPI_readfromfile(file, bmp->data, pic.w * pic.h);
    } else {
        bmp = 0;
    }
    if (outX != 0 && outY != 0) {
        *outX = *(int*)(pic.header + 4);
        *outY = *(int*)(pic.header + 8);
    }
    HAPI_CloseFile(file);
    return bmp;
}
