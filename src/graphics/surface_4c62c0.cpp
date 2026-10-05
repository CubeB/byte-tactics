// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Frame recovery for a lost DirectDraw surface: when bit 1 of the display
// flags at +0xf0 is set, check whether the surface at +0x88 reports
// DDERR_SURFACELOST, restore the two surfaces at screen+0x8 and screen+0xc,
// lock the screen with LockScreen, blit the object at +0x98 onto it with
// BlitSurface, then unlock with the +0x80 method (UnlockScreen inlined). The
// screen surface at +0x8c is saved to +0xb4 and +0xdc is always cleared.
// The unlock is a local inline helper because MSVC then keeps the tested
// surface pointer in ecx and copies it to eax for the call, as in 0x4c5fa0.
#include <ddraw.h>

struct Surface_004c62c0 {
    int data[12];                      // +0x00, filled by LockScreen
};

struct Screen_004c62c0 {
    char unknown_0[0x8];
    IDirectDrawSurface* field_88;      // +0x8
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

#pragma pack(push, 1)
struct Display_004c62c0 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c62c0 screen;            // +0x80
    char unknown_90[0x98 - 0x90];
    void* field_98;                    // +0x98
    char unknown_9c[0xb4 - 0x9c];
    IDirectDrawSurface* field_b4;      // +0xb4
    char unknown_b8[0xdc - 0xb8];
    int field_dc;                      // +0xdc
    char unknown_e0[0xf0 - 0xe0];
    unsigned short bit0 : 1;           // +0xf0 bit 0
    unsigned short flag1 : 1;          // +0xf0 bit 1
    unsigned short bit2_15 : 14;
};
#pragma pack(pop)

extern int g_screenLockCount;

Display_004c62c0* GetDisplay(void);
int __stdcall LockScreen(Surface_004c62c0* out);
void __cdecl BlitSurface(void* dst, void* src, int x, int y);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline()
{
    Display_004c62c0* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c62c0
void RestoreScreen(void)
{
    Display_004c62c0* d = GetDisplay();
    if (d->flag1) {
        if (d->screen.field_88->IsLost() == DDERR_SURFACELOST) {
            Display_004c62c0* d2 = GetDisplay();
            if (d2->field_44 == 0) {
                if (d->screen.field_88->Restore() == 0) {
                    if (d->screen.surface->Restore() == 0) {
                        Surface_004c62c0 screen;
                        LockScreen(&screen);
                        BlitSurface(&screen, d2->field_98, 0, 0);
                        UnlockScreenInline();
                    }
                }
            }
        }
        d->field_b4 = d->screen.surface;
    }
    d->field_dc = 0;
}
