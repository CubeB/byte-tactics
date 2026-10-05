// Decompiled by Opus. Names are provisional.
// Pops every entry of the screen lock stack (count g_screenLockCount, entries
// written by LockScreen at g_screenLocks), unlocking each one. The screen
// unlock is UnlockScreen inlined (see 0x4c6d20.cpp).
#include <ddraw.h>

struct Surface_004c5df0 {
    int data[12];
};

#pragma pack(push, 1)
struct LockEntry_004c5df0 {
    Surface_004c5df0* surface;         // +0x0
    char flag;                         // +0x4
};
#pragma pack(pop)

struct Screen_004c5df0 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockSurface() { surface->Unlock(0); }
};

struct Display_004c5df0 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c5df0 screen;            // +0x80
    char unknown_90[0xdc - 0x90];
    int field_dc;                      // +0xdc
};

extern int g_screenLockCount;
extern LockEntry_004c5df0 g_screenLocks[];

Display_004c5df0* GetDisplay(void);
int __stdcall UnlockPrimary(Surface_004c5df0* s, int a, int b);

// 0x4c5fa0, inlined here.
static inline int UnlockScreenInline(Surface_004c5df0* s)
{
    Display_004c5df0* d = GetDisplay();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (g_screenLockCount > 0)
            g_screenLockCount--;
    }
    return 1;
}

// FUNCTION: 0x4c5df0
void UnlockAllScreens(void)
{
    while (g_screenLockCount > 0) {
        int i = g_screenLockCount;
        if (g_screenLocks[g_screenLockCount].flag)
            UnlockPrimary(g_screenLocks[g_screenLockCount - 1].surface, 0, 0);
        else
            UnlockScreenInline(g_screenLocks[g_screenLockCount - 1].surface);
        if (i == g_screenLockCount)
            g_screenLockCount--;
    }
}
