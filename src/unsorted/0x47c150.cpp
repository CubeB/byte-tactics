// Decompiled by Space Bunny Free. Names are provisional.
// Asks the movie's output surface for its pixel format and returns the blit
// mode matching it, or 0 when the format is not one of the four 16-bit RGB
// layouts Smacker can blit to.
#include <windows.h>
#include <ddraw.h>

typedef void (__stdcall *GetPixelFormatFn)(void* self, DDPIXELFORMAT* format);

struct SmackerSurface {
    GetPixelFormatFn* methods;      // +0x00, slot 21 is the pixel format query
};

struct SmackerSurfaces {
    char unknown_0[4];
    SmackerSurface* surface;        // +0x04
};

class Class_0047c150 {
public:
    char unknown_0[0x544];
    SmackerSurfaces* surfaces;      // +0x544

    int FUN_0047c150(SmackerSurface* unused);
};

// FUNCTION: 0x47c150
int Class_0047c150::FUN_0047c150(SmackerSurface* unused)
{
    DDPIXELFORMAT format;
    format.dwSize = sizeof(format);
    format.dwFlags = DDPF_RGB;
    SmackerSurface* surface = surfaces->surface;
    surface->methods[21](surface, &format);
    if (format.dwRGBBitCount == 8)
        return 0;
    if (format.dwRBitMask == 0xf800 && format.dwGBitMask == 0x7e0 && format.dwBBitMask == 0x1f)
        return 0xc0000000;
    if (format.dwRBitMask == 0x7c00 && format.dwGBitMask == 0x3e0 && format.dwBBitMask == 0x1f)
        return 0x80000000;
    if (format.dwRBitMask == 0xf800 && format.dwGBitMask == 0x7c0 && format.dwBBitMask == 0x3f)
        return 0xa0000000;
    if (format.dwRBitMask == 0xfc00 && format.dwGBitMask == 0x3e0 && format.dwBBitMask == 0x1f)
        return 0xe0000000;
    MessageBoxA(0, "Unsupported pixel format.", "Smacker Error", 0);
    return 0;
}
