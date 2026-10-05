// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads a PCX file whose header starts with 0x0a 0x05, opens its body through
// DecodePcx (which allocates the pixel data and the 256-entry packed RGB
// palette), expands the palette to 256 PALETTEENTRY quads (peFlags = 0), copies
// them into the caller's buffer and frees the PCX buffers. Returns 1 on
// success, 0 when the file is missing or is not the expected PCX variant.
#include <windows.h>
#include <string.h>

struct PCX_004cb080 {
    unsigned char* data;     // +0x0
    unsigned char* palette;  // +0x4
    int width;               // +0x8
    int height;              // +0xc
};

void* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb5d0(void* file);
void __stdcall DecodePcx(void* file, PCX_004cb080* pcx);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4cb080
int __stdcall LoadPcxPalette(char* path, unsigned int* out)
{
    void* file = FUN_004bb5b0(path);
    if (file == 0)
        return 0;
    PCX_004cb080 pcx;
    unsigned char header[0x80];
    PALETTEENTRY pal[0x100];
    if (FUN_004bb7c0(file, header, 0x80) != 0x80) {
        FUN_004bb5d0(file);
        return 0;
    }
    if (header[0] != 0x0a || header[1] != 5) {
        FUN_004bb5d0(file);
        return 0;
    }
    FUN_004bb710(file, 0);
    DecodePcx(file, &pcx);
    FUN_004bb5d0(file);
    unsigned char* s = pcx.palette;
    for (int i = 0; i < 0x100; i++) {
        pal[i].peRed = s[0];
        pal[i].peGreen = s[1];
        pal[i].peBlue = s[2];
        pal[i].peFlags = 0;
        s += 3;
    }
    memcpy(out, pal, 0x400);
    FUN_004d85a0(pcx.palette);
    FUN_004d85a0(pcx.data);
    return 1;
}
