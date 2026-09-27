// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x4c6d20 / 0x4c6e70, drawing with the cdecl blitter at 0x4cbe70
// (which takes one extra byte argument, the transparent colour). Draws into
// `dst`, or, when `dst` is null, into the screen: lock it with FUN_004c5e70,
// draw, then unlock. The unlock is FUN_004c5fa0 inlined; its Unlock call goes
// through a method of the embedded screen struct, which is why the tested
// surface pointer is copied.
#include <ddraw.h>

struct Surface_004c6dc0 {
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

extern int DAT_0051fe00;

Display_004c6dc0* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Surface_004c6dc0* out);
void __cdecl FUN_004cbe70(Surface_004c6dc0* dst, Surface_004c6dc0* src, Rect_004c6dc0* rect, Point_004c6dc0* pos, unsigned char transparent);

// 0x4c5fa0, inlined here.
static inline int UnlockScreen(Surface_004c6dc0* s)
{
    Display_004c6dc0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
}

// FUNCTION: 0x4c6dc0
void __stdcall FUN_004c6dc0(Surface_004c6dc0* dst, Surface_004c6dc0* src, Rect_004c6dc0* rect, Point_004c6dc0* pos, unsigned char transparent)
{
    if (dst == 0) {
        Surface_004c6dc0 screen;
        if (FUN_004c5e70(&screen)) {
            FUN_004cbe70(&screen, src, rect, pos, transparent);
            UnlockScreen(&screen);
        }
    } else {
        FUN_004cbe70(dst, src, rect, pos, transparent);
    }
}
