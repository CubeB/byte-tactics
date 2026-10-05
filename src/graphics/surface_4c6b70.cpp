// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Blits a bitmap with the hand-written routine BlitSurface. If `dst` is
// null the screen is locked with LockScreen and used as the destination;
// if `bmp` is null the locked screen is used as the source instead. When one
// of the two pointers is null the blit is offset by the bitmap's half width
// and half height (the shorts at +0x18 and +0x1a). The unlock is UnlockScreen
// inlined; its Unlock call goes through a method of the embedded screen
// struct, which is why the tested surface pointer is copied.
//
// The source shape that matches is two flat top-level `if`s (dst == 0, then
// bmp == 0) that each blit and then tail-call `UnlockScreenInline(0); return;`.
// MSVC cross-jumps the two blits onto one shared `call` and the two unlocks
// onto one shared block. Writing it as an `if/else if/else` with a single
// trailing unlock compiles to the same outline but rotates the scratch
// registers of the second branch by one (eax/ecx/edx instead of
// edx/eax/ecx), which is what left it at 91.2%.
#include <ddraw.h>

// One layout for both the locked surface and the bitmap: BlitSurface takes
// a source that is either a bitmap (the half sizes at +0x18 and +0x1a) or a
// locked surface.
struct Image_004c6b70 {
    char unknown_0[0x18];
    short half_width;                  // +0x18
    short half_height;                 // +0x1a
    char unknown_1c[0x30 - 0x1c];
};

struct Screen_004c6b70 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c6b70 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6b70 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;

Display_004c6b70* GetDisplay(void);
int __stdcall LockScreen(Image_004c6b70* out);
void __cdecl BlitSurface(Image_004c6b70* dst, Image_004c6b70* src, int x, int y);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Image_004c6b70* s)
{
    Display_004c6b70* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c6b70
void __stdcall DrawSurface(Image_004c6b70* dst, Image_004c6b70* bmp, int x, int y)
{
    if (dst == 0) {
        if (bmp == 0)
            return;
        Image_004c6b70 screen;
        if (LockScreen(&screen) == 0)
            return;
        BlitSurface(&screen, bmp, x - bmp->half_width, y - bmp->half_height);
        UnlockScreenInline(0);
        return;
    }
    if (bmp == 0) {
        Image_004c6b70 screen;
        if (LockScreen(&screen) == 0)
            return;
        BlitSurface(dst, &screen, x, y);
        UnlockScreenInline(0);
        return;
    }
    BlitSurface(dst, bmp, x - bmp->half_width, y - bmp->half_height);
}
