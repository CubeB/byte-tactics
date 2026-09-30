// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 74.2% (was 50.7%). Frame now matches the original (sub esp, 0xf4; desc ends exactly at [esp+0x104]).
// Fixed here: (1) branch ORDER. The original lays the (flags&2)==0 BitBlt path first (falling into a
//     tail-merged Unlock), then the field_dc!=0 bmp path, then the field_dc==0 flip/Blt path out of line.
//     Writing it as if(!(flags&2)){...} else if(field_dc!=0){...} else{...} was worth +22 points.
// (2) The Blt-path locals must live in an ELSE block so they overlay the earlier held/bmp slot at
//     [esp+0x10]; at function scope they pushed everything +4 (frame 0xf8).
// (3) The Blt retry loop needs one reused result variable r (restores assign into it) to get a single
//     back-edge; the direct-test form rotated the loop and emitted the Blt call twice.
// Remaining: a register permutation. The original keeps 0 in ebp and the Lock result/bmp in ebx for the
// whole field_dc!=0 branch (so the inlined Lock uses ebp=InterlockedExchange, ebx=WaitForSingleObject,
// result ebx). Ours uses ebx as the 0, which swaps the Lock imports (ebx=Interlocked, ebp=Wait) and lands
// the result in ebp, pushing dd to edi and the restore result to eax. Downstream that flips every
// cmp/test/zero operand from ebp to ebx. This is one upstream fact, not many.
// Surface locals are 48 bytes and the native descriptor is 108 bytes (0x6c).

#include <windows.h>
#include <ddraw.h>

struct Surface_004c63a0 {
    int data[12];
};

struct Out_004c63a0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    short field_18;                    // +0x18
    short field_1a;                    // +0x1a
    int vec[4];                        // +0x1c
    int field_2c;                      // +0x2c
};

#pragma pack(push, 1)
struct Display_004c63a0 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    int field_44;                      // +0x44
    HDC srcDC;                         // +0x48
    HPALETTE palette;                  // +0x4c
    Out_004c63a0 cached;               // +0x50
    char unknown_80[0x8];
    IDirectDrawSurface* field_88;      // +0x88
    IDirectDrawSurface* surface;       // +0x8c
    char unknown_90[0x98 - 0x90];
    Surface_004c63a0* field_98;        // +0x98
    int field_9c;                      // +0x9c
    char unknown_a0[0xbc - 0xa0];
    Surface_004c63a0* field_bc;        // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    int field_d4;                      // +0xd4
    int field_d8;                      // +0xd8
    int field_dc;                      // +0xdc
    char unknown_e0[0xf0 - 0xe0];
    unsigned short flags;              // +0xf0
    char unknown_f2[0x196 - 0xf2];
    int field_196;                     // +0x196
    char unknown_19a[0x1b2 - 0x19a];
    Surface_004c63a0* field_1b2;       // +0x1b2
    int field_1b6;                     // +0x1b6
    int field_1ba;                     // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int field_1ce;                     // +0x1ce
    int field_1d2;                     // +0x1d2
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern int DAT_0051fe00;

Display_004c63a0* FUN_004b6220(void);
int FUN_004b6700(void);
int FUN_004b6710(void);
int __stdcall FUN_004c5e70(Surface_004c63a0* out);
void __stdcall FUN_004c6b70(Surface_004c63a0* dst, Surface_004c63a0* bmp, int x, int y);
void __stdcall FUN_004c67c0(Display_004c63a0* obj, void* dst);
void __cdecl FUN_004cbbe0(Surface_004c63a0* dst, Surface_004c63a0* src, int x, int y);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
}

// 0x4c5fa0, inlined here.
static inline void UnlockScreen()
{
    Display_004c63a0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->surface == 0)
            return;
        d->surface->Unlock(0);
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
}

struct Desc {
        DWORD dwSize, dwFlags, height, width;
        LONG lPitch;
        DWORD backbuffers, mipmaps, alpha, reserved;
        void* lpSurface;
        char fields[68];
};
// FUNCTION: 0x4c63a0
void FUN_004c63a0(void)
{
    Surface_004c63a0 screen;
    Out_004c63a0 out;
    Desc desc;
    Display_004c63a0* d = FUN_004b6220();
    unsigned short flags = d->flags;

    if ((flags & 2) == 0) {
        LONG held = Lock();
        Out_004c63a0* p = &d->cached;
        FUN_004c6b70((Surface_004c63a0*)p, d->field_bc, 0, 0);
        FUN_004c67c0(d, p);
        HDC hdc = GetDC(d->hwnd);
        SelectPalette(hdc, d->palette, 0);
        RealizePalette(hdc);
        BitBlt(hdc, 0, 0, p->field_0, d->cached.field_4, d->srcDC, 0, 0, SRCCOPY);
        ReleaseDC(d->hwnd, hdc);
        Unlock(held);
        return;
    }

    if (d->field_dc != 0) {
        Surface_004c63a0* bmp = d->field_bc;
        if (bmp->data[0] != FUN_004b6700())
            return;
        if (bmp->data[1] != FUN_004b6710())
            return;

        LONG held = Lock();

        desc.dwSize = sizeof(desc);
        unsigned long lr = d->field_88->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {

            out.field_0 = d->field_d4;
            out.field_4 = d->field_d8;
            out.field_8 = desc.lPitch;
            out.field_c = (int)desc.lpSurface;
            FUN_004c67c0(d, bmp);
            FUN_004cbbe0((Surface_004c63a0*)&out, bmp, 0, 0);
            if (d->field_1ce != 0 && d->field_1d2 != 0)
                FUN_004c6b70(bmp, (Surface_004c63a0*)d->field_1be, d->field_1b6, d->field_1ba);
            d->field_88->Unlock(0);
        } else if (lr == 0x887601c2) {
            Display_004c63a0* dd = FUN_004b6220();
            if (dd->field_44 == 0) {
                if (d->field_88->Restore() == 0) {
                    if (d->surface->Restore() == 0) {

                        FUN_004c5e70(&screen);
                        FUN_004cbbe0(&screen, dd->field_98, 0, 0);
                        UnlockScreen();
                    }
                }
            }
        }
        Unlock(held);
        return;
    } else {
        if (d->field_9c != 0 && (flags & 1) != 0) {
            d->field_88->Flip(0, 1);
            return;
        }
        RECT rect;
        POINT pt;
        GetClientRect(d->hwnd, &rect);
        pt.x = 0;
        pt.y = 0;
        RECT src = rect;
        ClientToScreen(d->hwnd, &pt);
        OffsetRect(&rect, pt.x, pt.y);

        LONG r;
        for (;;) {
            r = d->field_88->Blt(&rect, d->surface, &src, 0x1000000, 0);
            if (r == 0) break;
            if (r != 0x887601c2) continue;
            Display_004c63a0* dd = FUN_004b6220();
            if (dd->field_44 != 0) {
                r = 0;
            } else {
                r = d->field_88->Restore();
                if (r != 0) continue;
                r = d->surface->Restore();
                if (r != 0) continue;
                FUN_004c5e70(&screen);
                FUN_004cbbe0(&screen, dd->field_98, 0, 0);
                UnlockScreen();
            }
            if (r != 0) continue;
        }
    }
}
