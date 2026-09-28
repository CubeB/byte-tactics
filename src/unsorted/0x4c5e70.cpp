// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Locks the DirectDraw surface and fills a 0x30-byte surface descriptor,
// registering it in the screen lock stack (DAT_0051fe00 entries at
// DAT_0051fe08) so FUN_004c5df0 can unlock it later. When the display is
// already locked (+0xdc) or using the cached descriptor (+0x44) it copies
// the cached block instead.
#include <ddraw.h>

#pragma pack(push, 1)
struct LockEntry_004c5e70 {
    void* surface;                   // +0x0
    char flag;                       // +0x4
};
#pragma pack(pop)

struct Vec16_004c5e70 {
    int x;                           // +0x00
    int y;                           // +0x04
    int z;                           // +0x08
    int w;                           // +0x0c
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

struct Display_004c5e70 {
    char unknown_0[0x44];
    int field_44;                    // +0x44
    char unknown_48[0x8];
    Out_004c5e70 cached;             // +0x50
    char unknown_80[0xc];
    IDirectDrawSurface* surface;     // +0x8c
    char unknown_90[0x10];
    Vec16_004c5e70 vec;              // +0xa0
    char unknown_b0[0xc];
    Out_004c5e70* field_bc;          // +0xbc
    char unknown_c0[0x14];
    int field_d4;                    // +0xd4
    int field_d8;                    // +0xd8
    int field_dc;                    // +0xdc
};

extern int DAT_0051fe00;
extern LockEntry_004c5e70 DAT_0051fe08[];

Display_004c5e70* FUN_004b6220(void);

// Still differs (97.8%): MSVC schedules the `desc.dwSize = sizeof(desc)`
// store and the vtable load `mov ecx,[eax]` before the argument pushes; the
// original keeps the store and the vtable load after `push 0x801`, and only
// then reloads d->surface. Function size and every other instruction match.
// FUNCTION: 0x4c5e70
int __stdcall FUN_004c5e70(Out_004c5e70* out)
{
    Display_004c5e70* d = FUN_004b6220();
    if (d->field_dc != 0) {
        *out = *d->field_bc;
        return 1;
    }
    if (d->field_44 != 0) {
        *out = d->cached;
        return 1;
    }
    if (d->surface == 0)
        return 0;
    DDSURFACEDESC desc;
    desc.dwSize = sizeof(desc);
    if (d->surface->Lock(0, &desc, 0x801, 0) != 0)
        return 0;
    out->field_0 = d->field_d4;
    out->field_4 = d->field_d8;
    out->field_8 = desc.lPitch;
    out->field_c = (int)desc.lpSurface;
    out->field_18 = 0;
    out->field_1a = 0;
    out->field_10 = 10000;
    out->field_14 = -1;
    out->field_2c &= ~1;
    out->vec = d->vec;
    if (DAT_0051fe00 < 10) {
        DAT_0051fe08[DAT_0051fe00].surface = out;
        DAT_0051fe08[DAT_0051fe00].flag = 0;
    }
    return 1;
}
