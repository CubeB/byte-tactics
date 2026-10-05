// Decompiled by Opus. Names are provisional.
// Unlocks the screen surface locked by LockScreen (see 0x4c6d20, which
// inlines this function). The Unlock call goes through a method of the
// embedded screen struct, which is why the surface pointer is re-read.
#include <ddraw.h>

struct Surface {
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

extern int g_screenLockCount;

Display_004c5fa0* GetDisplay(void);

// FUNCTION: 0x4c5fa0
int __stdcall UnlockScreen(Surface* s)
{
    Display_004c5fa0* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}
