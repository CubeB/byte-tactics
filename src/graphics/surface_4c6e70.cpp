// Decompiled by Opus. Names are provisional.
// Sibling of 0x4c6d20 using the blitter at 0x4cbef1 (hand-written assembly
// in the gap region starting at 0x4cbbe0): draws into `dst`, or, when `dst`
// is null, into the screen: lock it with LockScreen, draw, then unlock.
#include <ddraw.h>

struct Surface_004c6e70 {
    int data[12];
};

struct Rect_004c6e70;
struct Point_004c6e70;

struct Screen_004c6e70 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    // Unlock through a method of this struct: the tested pointer gets copied.
    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c6e70 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6e70 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display_004c6e70* GetDisplay(void);
int __stdcall LockScreen(Surface_004c6e70* out);
void __cdecl BlitTile32x32(Surface_004c6e70* dst, Surface_004c6e70* src, Rect_004c6e70* rect, Point_004c6e70* pos);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface_004c6e70* s)
{
    Display_004c6e70* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c6e70
void __stdcall DrawTile(Surface_004c6e70* dst, Surface_004c6e70* src, Rect_004c6e70* rect, Point_004c6e70* pos)
{
    if (dst == 0) {
        Surface_004c6e70 screen;
        if (LockScreen(&screen)) {
            BlitTile32x32(&screen, src, rect, pos);
            UnlockScreenInline(&screen);
        }
    } else {
        BlitTile32x32(dst, src, rect, pos);
    }
}
