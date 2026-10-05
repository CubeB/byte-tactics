// Decompiled by Sonnet. Names are provisional.

#include <ddraw.h>

struct SurfaceWrapper {
    char unknown_0[4];
    LPDIRECTDRAWSURFACE surface; // +4
};

class Class_0047c2f0 {
public:
    char unknown_0[0x544];
    SurfaceWrapper* wrapper; // +0x544

    void FUN_0047c2f0();
};

// FUNCTION: 0x47c2f0
void Class_0047c2f0::FUN_0047c2f0()
{
    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;
    wrapper->surface->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}
