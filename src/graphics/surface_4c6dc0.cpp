// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x4c6d20 / 0x4c6e70, drawing with the cdecl blitter at 0x4cbe70
// (which takes one extra byte argument, the transparent colour). Draws into
// `dst`, or, when `dst` is null, into the screen: lock it with LockScreen,
// draw, then unlock. The unlock is UnlockScreen inlined; its Unlock call goes
// through a method of the embedded screen struct, which is why the tested
// surface pointer is copied.
#include <ddraw.h>

struct Surface {
    int data[12];
};

struct Rect_004c6dc0;
struct Point_004c6dc0;

struct Screen_004c6dc0 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c6dc0 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6dc0 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display_004c6dc0* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
void __cdecl FUN_004cbe70(Surface* dst, Surface* src, Rect_004c6dc0* rect, Point_004c6dc0* pos, unsigned char transparent);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface* s)
{
    Display_004c6dc0* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c6dc0
void __stdcall CopySurfaceRectKeyed(Surface* dst, Surface* src, Rect_004c6dc0* rect, Point_004c6dc0* pos, unsigned char transparent)
{
    if (dst == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            FUN_004cbe70(&screen, src, rect, pos, transparent);
            UnlockScreenInline(&screen);
        }
    } else {
        FUN_004cbe70(dst, src, rect, pos, transparent);
    }
}
