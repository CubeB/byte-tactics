// Decompiled by GPT-5.6-Terra, finished by Sonnet 5.5. Names are provisional.
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
    // Lock in a one-expression method: gives the separate surface reload.
    struct Screen {
        char unknown_0[0x8];
        IDirectDrawSurface* primary;
        int Lock(DDSURFACEDESC* p) { return primary->Lock(0, p, DDLOCK_WAIT, 0); }
    } screen;
    char unknown_8c[0x14];
    Block_004c5ff0 block;            // +0xa0
    char unknown_b0[0x24];
    int field_d4;                    // +0xd4
    int field_d8;                    // +0xd8

    // Method of the display object: this is the call result.
    int LockMe(Out_004c5ff0* out)
    {
        if (field_44 != 0) {
            *out = cached;
            return 1;
        }
        if (screen.primary == 0)
            return 0;
        DDSURFACEDESC desc;
        desc.dwSize = sizeof(desc);
        // One-case switch: tests the result with test eax, eax.
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
        out->block = block;
        return 1;
    }
};

Display_004c5ff0* GetDisplay(void);


// FUNCTION: 0x4c5ff0
int __stdcall LockPrimary(Out_004c5ff0* out)
{
    return GetDisplay()->LockMe(out);
}
