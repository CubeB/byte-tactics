// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by deepseek-v4.1. Names are provisional.
// PARTIAL 81.0%. Frame is right (0xf4: pt@0x10 reused by held/bmp, rect@0x18,
// src@0x28, out@0x38, screen@0x68, desc@0x98, all [esp+N]). The tail loop now has
// the original's shape: `int hr; for(;;){ hr = Blt(...); if (hr==0) return; if (hr
// != 0x887601c2) continue; dd = FUN_004b6220(); if (dd->field_44 == 0) { hr =
// field_88->Restore(); if (hr==0) { hr = surface->Restore(); if (hr==0) { ...;
// UnlockScreen(); } } } else { hr = 0; } if (hr != 0) continue; return; }` keeps
// hr in edi and dd in ebx exactly as the original (0x4c6722-0x4c67aa), and that
// alone took the file from 74.7 to 81.0.
// What still differs (ours 1129 bytes vs the original 1051):
//  - the two inlined Lock()/Unlock(held) pairs are not merged. The original keeps
//    ONE Unlock body (0x4c663f) and branch 1 jumps into it (mov eax,[esp+0x10];
//    test eax,eax; jmp 0x4c6639), and its bmp-path Lock() loads ebp=InterlockedExchange,
//    ebx=WaitForSingleObject with held ending in ebx; ours loads ebx=InterlockedExchange,
//    ebp=WaitForSingleObject and keeps held in ebp, so the two Unlock bodies differ
//    (call ebp vs call ebx) and MSVC emits both (~0x2c + 0x20 bytes).
//  - ours also repeats the whole Blt block once at the loop tail (the trailing
//    `if (hr != 0) continue;` back edge is not folded onto the loop top).
//  - branch 1's jne displacements are 4 bytes wider because of those extra bytes.
// The zero register now matches (xor ebp,ebp / cmp ecx,ebp in the field_dc test),
// but that did not move the bmp-path Lock's (iel,wfso,held) trio off (ebx,ebp,ebp).
// Tested this pass: making the Unlock ONE shared statement (if / else if / else
// plus a single trailing Unlock(held)) DOES unify the bmp Lock to the original's
// (ebp,ebx,ebx), which shows the original's shared Unlock body really is one
// source-level statement; but then held needs a function-wide memory home at
// [esp+0x10], the field_bc temp moves to 0x14 and the frame becomes 0xf8: 67.6%.
// Hoisting a single `LONG held;` while keeping the two Unlock call sites is
// byte-identical to this file. A do-while form of the Blt loop scores 41.3%.
// The locals must stay scoped as they are (desc/out inside the field_dc!=0 block,
// rect/pt plus the Blt loop in a nested block, screen at function scope): declaring
// them at function scope makes the frame 0xf8 and shifts every [esp+N] by 4.
// Session deepseek-v4.1-flash: no code change, 2 check runs, still 81.0%.
// New evidence from the 81.0% diff: our bmp-path Unlock emits `push ebp` and
// `mov [DAT_0052a4ec], ebp` (the register holding held, proven 0 by the test)
// where the original emits `push 0` / `mov [DAT_0052a4ec], 0`. The original's
// shared body also shows the `test held,held` duplicated into each predecessor
// (test eax,eax; jmp shared_jne vs test ebx,ebx; fall into shared_jne), which
// only happens when held lives in different places in the two branches (memory
// at [esp+0x10] in branch 1, ebx in branch 2) and the Unlock is ONE source
// statement after the if/else chain: the value propagation that turns the 0
// immediates into the held register then cannot fire, since the shared body
// has no single name for held. So the remaining shape is confirmed to be a
// single trailing if (held == 0) { unlock } over a function-scope held, but
// that spelling gives held a whole-variable home and pushes bmp to 0x14 (frame
// 0xf8). Not tried this session: a struct home (as in matched 0x4b5510's
// `struct { int lockResult; HDC* dcSlot; } setup`) to force held and bmp into
// one shared slot pair, or splitting held's live range some other way.

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
    int pad[8];
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
static inline int UnlockScreen()
{
    Display_004c63a0* d = FUN_004b6220();
    if (d->field_44 == 0 && d->field_dc == 0) {
        if (d->surface == 0)
            return 0;
        d->surface->Unlock(0);
        if (DAT_0051fe00 > 0)
            DAT_0051fe00--;
    }
    return 1;
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

    Surface_004c63a0 screen;

    if (d->field_dc != 0) {
        Desc desc;
        Surface_004c63a0 out;
        Surface_004c63a0* bmp = d->field_bc;
        if (bmp->data[0] != FUN_004b6700())
            return;
        if (bmp->data[1] != FUN_004b6710())
            return;

        LONG held = Lock();
        desc.dwSize = sizeof(desc);
        unsigned long lr = d->field_88->Lock(0, (DDSURFACEDESC*)&desc, 1, 0);
        if (lr == 0) {
            out.data[0] = d->field_d4;
            out.data[1] = d->field_d8;
            out.data[2] = desc.lPitch;
            out.data[3] = (int)desc.lpSurface;
            FUN_004c67c0(d, bmp);
            FUN_004cbbe0(&out, bmp, 0, 0);
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
    }

    if (d->field_9c != 0 && (flags & 1) != 0) {
        d->field_88->Flip(0, 1);
        return;
    }

    {
    RECT rect;
    POINT pt;
    GetClientRect(d->hwnd, &rect);
    pt.x = 0;
    pt.y = 0;
    RECT src = rect;
    ClientToScreen(d->hwnd, &pt);
    OffsetRect(&rect, pt.x, pt.y);

    int hr;
    for (;;) {
        hr = d->field_88->Blt(&rect, d->surface, &src, 0x1000000, 0);
        if (hr == 0)
            return;
        if (hr != 0x887601c2)
            continue;
        Display_004c63a0* dd = FUN_004b6220();
        if (dd->field_44 == 0) {
            hr = d->field_88->Restore();
            if (hr == 0) {
                hr = d->surface->Restore();
                if (hr == 0) {
                    FUN_004c5e70(&screen);
                    FUN_004cbbe0(&screen, dd->field_98, 0, 0);
                    UnlockScreen();
                }
            }
        } else {
            hr = 0;
        }
        if (hr != 0)
            continue;
        return;
    }
    }
}
