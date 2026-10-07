// Decompiled by space-bunny-free. Names are provisional.
// Copies `src` to `dst` at (x, y) with a transparent colour through the blitter
// at 0x4cbcd5, or, when `dst` is null, onto the screen: lock it with
// LockScreen, blit, then unlock. The unlock is UnlockScreen inlined.
#include <ddraw.h>

struct Surface {
    int unknown_0[6];                  // +0x00
    short field_18;                    // +0x18
    short field_1a;                    // +0x1a
    int unknown_1c[5];                 // +0x1c
};

struct Screen_004c6c50 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c6c50 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6c50 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display_004c6c50* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
void __cdecl BlitSurfaceKeyed(Surface* dst, Surface* src, int x, int y, int color);

// 0x4c5fa0, inlined here.
static inline void UnlockScreenInline(Surface* s)
{
    Display_004c6c50* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
}

// FUNCTION: 0x4c6c50
void __stdcall DrawSurfaceKeyed(Surface* dst, Surface* src, int x, int y, int color)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            BlitSurfaceKeyed(&screen, src, x - src->field_18, y - src->field_1a, color);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitSurfaceKeyed(dst, src, x - src->field_18, y - src->field_1a, color);
    }
}
