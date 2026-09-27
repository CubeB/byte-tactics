// Decompiled by Opus. Names are provisional.
#include <windows.h>

// smackw32.dll ordinal 18 (SmackClose), called through its import slot.
extern "C" __declspec(dllimport) void __stdcall SmackClose(void* smack);

struct Surfaces_0047bf20 {
    IUnknown* surface_0;             // +0x0
    IUnknown* surface_4;             // +0x4
    char unknown_8[8];
    IUnknown* surface_10;            // +0x10
};

class Class_0047bf20 {
public:
    void* smack;                     // +0x0
    char unknown_4[0x414 - 4];
    int hasSurfaces;                 // +0x414
    char unknown_418[0x544 - 0x418];
    Surfaces_0047bf20* surfaces;     // +0x544

    void FUN_0047bf20();
};

// FUNCTION: 0x47bf20
void Class_0047bf20::FUN_0047bf20()
{
    SmackClose(smack);
    if (hasSurfaces) {
        if (surfaces->surface_10)
            surfaces->surface_10->Release();
        if (surfaces->surface_4)
            surfaces->surface_4->Release();
        if (surfaces->surface_0)
            surfaces->surface_0->Release();
    }
}
