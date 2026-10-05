// Decompiled by Opus. Names are provisional.
// Unlocks the screen surface locked by FUN_004c5e70 (see 0x4c6d20, which
// inlines this function). The Unlock call goes through a method of the
// embedded screen struct, which is why the surface pointer is re-read.
#include <ddraw.h>

struct Surface_004c5fa0 {
    int data[12];
};

struct Screen_004c5fa0 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c5fa0 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c5fa0 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int DAT_0051fe00;

Display_004c5fa0* FUN_004b6220(void);

// FUNCTION: 0x4c5fa0
int __stdcall FUN_004c5fa0(Surface_004c5fa0* s)
{
    Display_004c5fa0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
}
