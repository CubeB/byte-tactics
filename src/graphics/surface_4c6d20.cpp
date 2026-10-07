// Decompiled by Opus. Names are provisional.
// Draws through the blitter at 0x4cbdd1 (hand-written assembly in a gap
// region) into `dst`, or, when `dst` is null, into the screen: lock it with
// LockScreen, draw, then unlock.
#include <ddraw.h>

struct Surface {
    int data[12];
};

struct Rect_004c6d20;
struct Point_004c6d20;

struct Screen_004c6d20 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    // Unlock through a method of this struct: the tested pointer gets copied.
    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c6d20 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6d20 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display_004c6d20* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
void __cdecl FUN_004cbdd1(Surface* dst, Surface* src, Rect_004c6d20* rect, Point_004c6d20* pos);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface* s)
{
    Display_004c6d20* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c6d20
void __stdcall CopySurfaceRect(Surface* dst, Surface* src, Rect_004c6d20* rect, Point_004c6d20* pos)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            FUN_004cbdd1(&screen, src, rect, pos);
            UnlockScreenInline(&screen);
        }
    } else {
        FUN_004cbdd1(dst, src, rect, pos);
    }
}
