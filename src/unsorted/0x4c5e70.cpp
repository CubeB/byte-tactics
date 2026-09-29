// Decompiled by GPT-5.6-Terra, finished by Sonnet 5.5. Names are provisional.
// Locks the DirectDraw surface and fills a 0x30-byte surface descriptor,
// registering it in the screen lock stack (DAT_0051fe00 entries at
// DAT_0051fe08) so FUN_004c5df0 can unlock it later. When the display is
// already locked (+0xdc) or using the cached descriptor (+0x44) it copies
// the cached block instead.
//
// How it matches (earlier versions stopped at 97.8%): the body is a method of
// the display object so `this` is the call result and MSVC does not order the
// desc.dwSize store before the surface load; the Lock call sits in a
// one-expression method of the embedded screen struct (+0x80), which gives the
// separate reload of the surface after the null test (see UnlockSurface in
// 0x4c5fa0); a one-case switch on the result gives `test eax, eax`. Same
// recipe as 0x4c5ff0.
#include <ddraw.h>

#pragma pack(push, 1)
struct LockEntry_004c5e70 {
    void* surface;                   // +0x0
    char flag;                       // +0x4
};
#pragma pack(pop)

struct Vec16_004c5e70 {
    int x;
    int y;
    int z;
    int w;
};

struct Out_004c5e70 {
    int field_0;                     // +0x00
    int field_4;                     // +0x04
    int field_8;                     // +0x08
    int field_c;                     // +0x0c
    int field_10;                    // +0x10
    int field_14;                    // +0x14
    short field_18;                  // +0x18
    short field_1a;                  // +0x1a
    Vec16_004c5e70 vec;              // +0x1c
    int field_2c;                    // +0x2c
};

extern int DAT_0051fe00;
extern LockEntry_004c5e70 DAT_0051fe08[];

struct Display_004c5e70 {
    char unknown_0[0x44];
    int field_44;                    // +0x44
    char unknown_48[0x8];
    Out_004c5e70 cached;             // +0x50
    struct Screen {
        char unknown_0[0xc];
        IDirectDrawSurface* surface; // +0x8c
        int Lock(DDSURFACEDESC* p) { return surface->Lock(0, p, 0x801, 0); }
    } screen;                        // +0x80
    char unknown_90[0x10];
    Vec16_004c5e70 vec;              // +0xa0
    char unknown_b0[0xc];
    Out_004c5e70* field_bc;          // +0xbc
    char unknown_c0[0x14];
    int field_d4;                    // +0xd4
    int field_d8;                    // +0xd8
    int field_dc;                    // +0xdc

    int LockMe(Out_004c5e70* out)
    {
        if (field_dc != 0) {
            *out = *field_bc;
            return 1;
        }
        if (field_44 != 0) {
            *out = cached;
            return 1;
        }
        if (screen.surface == 0)
            return 0;
        DDSURFACEDESC desc;
        desc.dwSize = sizeof(desc);
        switch (screen.Lock(&desc)) { case 0: break; default: return 0; }
        out->field_0 = field_d4;
        out->field_4 = field_d8;
        out->field_8 = desc.lPitch;
        out->field_c = (int)desc.lpSurface;
        out->field_18 = 0;
        out->field_1a = 0;
        out->field_10 = 10000;
        out->field_14 = -1;
        out->field_2c &= ~1;
        out->vec = vec;
        if (DAT_0051fe00 < 10) {
            DAT_0051fe08[DAT_0051fe00].surface = out;
            DAT_0051fe08[DAT_0051fe00].flag = 0;
        }
        return 1;
    }
};

Display_004c5e70* FUN_004b6220(void);

// FUNCTION: 0x4c5e70
int __stdcall FUN_004c5e70(Out_004c5e70* out)
{
    return FUN_004b6220()->LockMe(out);
}
