// Decompiled by GPT-5.6-Terra. Names are provisional.
// PARTIAL 97.2% after 8 completed checks: everything matches except the scheduling of the Lock setup.
// The original emits `mov eax,[esi+0x88]`, `push edi`, `lea edx,[esp+0xc]`,
// `push 1`, then `mov [esp+0x10],0x6c` and `mov ecx,[eax]`. Ours emits the
// `desc.dwSize` store first and the vtable load before `push 1`. The null
// check, argument pushes and call target all agree. Tried a local surface
// pointer (gives the right store/vtable order but turns the null check into a
// register compare and elides the reload), comma expressions, inline helpers,
// constructor, array/struct wrappers, reference bindings, cast forms, header
// sets and /G5 and /G6; the scheduler keeps hoisting the store.
//
// Fills a 0x30-byte surface descriptor. When the GDI offscreen path is active
// (field 0x44 of the display object is set) it copies the cached descriptor at
// +0x50; otherwise it locks the primary DirectDraw surface, builds the
// descriptor from the locked DDSURFACEDESC and from the block at +0xa0, and
// clears bit 0 of the flag at +0x2c.
#include <string.h>
#include <ddraw.h>

struct Block_004c5ff0 {
    int a;                           // +0x0
    int b;                           // +0x4
    int c;                           // +0x8
    int d;                           // +0xc
};

struct Out_004c5ff0 {
    int field_0;                     // +0x00
    int field_4;                     // +0x04
    int field_8;                     // +0x08
    int field_c;                     // +0x0c
    int field_10;                    // +0x10
    int field_14;                    // +0x14
    short field_18;                  // +0x18
    short field_1a;                  // +0x1a
    Block_004c5ff0 block;            // +0x1c
    int field_2c;                    // +0x2c
};

struct Display_004c5ff0 {
    char unknown_0[0x44];
    int field_44;                    // +0x44
    char unknown_48[0x8];
    Out_004c5ff0 cached;             // +0x50
    char unknown_80[0x8];
    IDirectDrawSurface* primary;     // +0x88
    char unknown_8c[0x14];
    Block_004c5ff0 block;            // +0xa0
    char unknown_b0[0x24];
    int field_d4;                    // +0xd4
    int field_d8;                    // +0xd8
};

Display_004c5ff0* FUN_004b6220(void);

// FUNCTION: 0x4c5ff0
int __stdcall FUN_004c5ff0(Out_004c5ff0* out)
{
    Display_004c5ff0* d = FUN_004b6220();
    if (d->field_44 != 0) {
        *out = d->cached;
        return 1;
    }
    if (d->primary == 0)
        return 0;
    DDSURFACEDESC desc;
    desc.dwSize = sizeof(desc);
    if (d->primary->Lock(0, &desc, DDLOCK_WAIT, 0) != 0)
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
    out->block = d->block;
    return 1;
}
