// Decompiled by space-bunny-free. Names are provisional.
// Copies `src` to `dst` at (x, y) with a transparent colour through the blitter
// at 0x4cbcd5, or, when `dst` is null, onto the screen: lock it with
// FUN_004c5e70, blit, then unlock. The unlock is FUN_004c5fa0 inlined.
#include <ddraw.h>

struct Surface_004c6c50 {
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

extern int DAT_0051fe00;

Display_004c6c50* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Surface_004c6c50* out);
void __cdecl FUN_004cbcd5(Surface_004c6c50* dst, Surface_004c6c50* src, int x, int y, int color);

// 0x4c5fa0, inlined here.
static inline void UnlockScreen(Surface_004c6c50* s)
{
    Display_004c6c50* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
}

// FUNCTION: 0x4c6c50
void __stdcall FUN_004c6c50(Surface_004c6c50* dst, Surface_004c6c50* src, int x, int y, int color)
{
    if (dst == 0) {
        Surface_004c6c50 screen;
        if (FUN_004c5e70(&screen)) {
            FUN_004cbcd5(&screen, src, x - src->field_18, y - src->field_1a, color);
            UnlockScreen(&screen);
        }
    } else {
        FUN_004cbcd5(dst, src, x - src->field_18, y - src->field_1a, color);
    }
}
