// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Loads a PCX file whose header starts with 0x0a 0x05, allocates an image of
// the header's width x height, decodes the body through DecodePcx and copies
// the pixels into the image. When the caller passes a palette buffer it is
// filled with 256 PALETTEENTRY quads (peFlags = 0) expanded from the file's
// 0x300-byte colour map. Returns the image, or 0 when the file is missing, is
// not the expected PCX variant, or the image allocation fails.
#include <windows.h>
#include <string.h>

struct PCX_004caf30 {
    unsigned char* data;     // +0x0
    unsigned char* palette;  // +0x4
    int width;               // +0x8
    int height;              // +0xc
};

struct Image_004caf30 {
    int width;               // +0x0
    int height;              // +0x4
    int field_8;             // +0x8
    unsigned char* pixels;   // +0xc
};

void* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
void __stdcall HAPI_SeekFile(void* file, int pos);
void __stdcall HAPI_CloseFile(void* file);
int __stdcall DecodePcx(void* file, PCX_004caf30* pcx);
Image_004caf30* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* p);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4caf30
Image_004caf30* __stdcall LoadPcx(char* path, unsigned char* outPalette)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0)
        return 0;
    int ok;
    PCX_004caf30 pcx;
    unsigned char header[0x80];
    if (HAPI_readfromfile(file, header, 0x80) != 0x80 || header[0] != 0x0a || header[1] != 5) {
        HAPI_CloseFile(file);
        return 0;
    }
    int width = *(unsigned short*)(header + 8) - *(unsigned short*)(header + 4) + 1;
    int height = *(unsigned short*)(header + 10) - *(unsigned short*)(header + 6) + 1;
    Image_004caf30* image = AllocSurface(path, width, height);
    if (image == 0) {
        HAPI_CloseFile(file);
        return 0;
    }
    HAPI_SeekFile(file, 0);
    ok = DecodePcx(file, &pcx);
    HAPI_CloseFile(file);
    if (ok) {
        memcpy(image->pixels, pcx.data, height * width);
    }
    if (outPalette != 0) {
        unsigned char* s = pcx.palette;
        for (int i = 0; i < 0x100; i++) {
            outPalette[0] = s[0];
            outPalette[1] = s[1];
            outPalette[2] = s[2];
            outPalette[3] = 0;
            outPalette += 4;
            s += 3;
        }
    }
    FUN_004d85a0(pcx.palette);
    FUN_004d85a0(pcx.data);
    if (!ok) {
        FreeSurface(image);
    }
    return image;
}
