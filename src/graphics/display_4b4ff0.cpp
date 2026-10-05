// Decompiled by Opus. Names are provisional.
// Releases the display's DirectDraw objects (palette, surfaces, then the
// DirectDraw object itself) and clears the pointers.
#include <windows.h>
#include <ddraw.h>

struct Display_004b4ff0 {
    char unknown_0[0x84];
    IDirectDraw* ddraw;                // +0x84
    IDirectDrawSurface* primary;       // +0x88
    IDirectDrawSurface* back;          // +0x8c
    IDirectDrawSurface* field_90;      // +0x90
    IDirectDrawPalette* palette;       // +0x94
};

// FUNCTION: 0x4b4ff0
void __stdcall FUN_004b4ff0(Display_004b4ff0* d)
{
    if (d->ddraw) {
        if (d->palette) {
            d->palette->Release();
            d->palette = 0;
        }
        if (d->back) {
            d->back->Release();
            d->back = 0;
        }
        if (d->primary) {
            d->primary->Release();
            d->primary = 0;
        }
        if (d->field_90) {
            d->field_90->Release();
            d->field_90 = 0;
        }
        d->ddraw->Release();
        d->ddraw = 0;
    }
}
