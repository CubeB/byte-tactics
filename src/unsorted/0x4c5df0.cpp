// Decompiled by Opus. Names are provisional.
// Pops every entry of the screen lock stack (count DAT_0051fe00, entries
// written by FUN_004c5e70 at DAT_0051fe08), unlocking each one. The screen
// unlock is FUN_004c5fa0 inlined (see 0x4c6d20.cpp).
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

extern int DAT_0051fe00;
extern LockEntry_004c5df0 DAT_0051fe08[];

Display_004c5df0* FUN_004b6220(void);
int __stdcall FUN_004c60d0(Surface_004c5df0* s, int a, int b);

// 0x4c5fa0, inlined here.
static inline int UnlockScreen(Surface_004c5df0* s)
{
    Display_004c5df0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->screen.surface == 0)
            return 0;
        d->screen.UnlockSurface();
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
}

// FUNCTION: 0x4c5df0
void FUN_004c5df0(void)
{
    while (DAT_0051fe00 > 0) {
        int i = DAT_0051fe00;
        if (DAT_0051fe08[DAT_0051fe00].flag)
            FUN_004c60d0(DAT_0051fe08[DAT_0051fe00 - 1].surface, 0, 0);
        else
            UnlockScreen(DAT_0051fe08[DAT_0051fe00 - 1].surface);
        if (i == DAT_0051fe00)
            DAT_0051fe00--;
    }
}
