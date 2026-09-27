// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH yet (91.2%, 224 of 224 bytes, every block and jump identical).
// What still differs: six instructions in the second branch (the one that
// locks the screen and then blits `dst` from it, 0x4c6bce..0x4c6bdd) pick a
// rotated set of registers. Original: `lea edx,[esp+4]` for the lock
// argument, `mov eax,[esp+0x44]` (y), `mov ecx,[esp+0x40]` (x),
// `lea edx,[esp+8]`. Mine: eax, ecx, edx, eax. The register allocator
// starts one step earlier here, so the EAX/ECX/EDX cycle is off by one.
// Tried and rejected: every branch shape that keeps this block layout
// (if/else-if/else is the only one), the unlock helper's form, the local's
// declaration (one or two locals), the callee declarations (return and
// argument types, reference parameters), packed structs, and moving the
// bitmap half-size reads into temporaries: all leave the same six
// instructions different.
// A second pass also ruled out, all still 91.2%: copying the arguments into
// locals just before the call (`int px = x; int py = y;` in either order, and
// with a local for `&screen` or for all four arguments), taking a reference
// to x and y, writing `x + 0` / `y + 0`, hoisting the lock result into a local,
// declaring the single `screen` local at function scope instead of per block,
// nesting the else-if as a plain else with a nested if, writing the lock test
// as `!FUN_004c5e70(...)`, and spelling branch 1 with locals for the two
// computed coordinates or for the two half sizes. The push order and the
// pushed addresses already agree; only the choice of eax/ecx/edx differs, so
// this is the register rotation the guide calls out, with nothing left in the
// block to change its live-value count.
// Blits a bitmap with the hand-written routine FUN_004cbbe0. If `dst` is
// null the screen is locked with FUN_004c5e70 and used as the destination;
// if `bmp` is null the locked screen is used as the source instead. When one
// of the two pointers is null the blit is offset by the bitmap's half width
// and half height (the shorts at +0x18 and +0x1a). The unlock is FUN_004c5fa0
// inlined; its Unlock call goes through a method of the embedded screen
// struct, which is why the tested surface pointer is copied.
#include <ddraw.h>

// One layout for both the locked surface and the bitmap: FUN_004cbbe0 takes
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

extern int DAT_0051fe00;

Display_004c6b70* FUN_004b6220(void);
int __stdcall FUN_004c5e70(Image_004c6b70* out);
void __cdecl FUN_004cbbe0(Image_004c6b70* dst, Image_004c6b70* src, int x, int y);

// 0x4c5fa0, inlined here.
static inline int UnlockScreen(Image_004c6b70* s)
{
    Display_004c6b70* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
}

// FUNCTION: 0x4c6b70
void __stdcall FUN_004c6b70(Image_004c6b70* dst, Image_004c6b70* bmp, int x, int y)
{
    if (dst == 0) {
        if (bmp == 0)
            return;
        Image_004c6b70 screen;
        if (FUN_004c5e70(&screen) == 0)
            return;
        FUN_004cbbe0(&screen, bmp, x - bmp->half_width, y - bmp->half_height);
    } else if (bmp == 0) {
        Image_004c6b70 screen;
        if (FUN_004c5e70(&screen) == 0)
            return;
        FUN_004cbbe0(dst, &screen, x, y);
    } else {
        FUN_004cbbe0(dst, bmp, x - bmp->half_width, y - bmp->half_height);
        return;
    }
    UnlockScreen(0);
}
