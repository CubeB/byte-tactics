// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Window procedure of the main application window: translates the custom
// display messages and forwards the rest to the default handler.
//
// PARTIAL, 84.5% (original 1104 bytes, ours 1136). What still differs:
//  * Control flow and all struct offsets are right. The 32 extra bytes are in
//    the two button-event arms (0x201/0x202/0x204/0x205 and 0x203/0x206): the
//    original ends each arm with only the flag store and a `jmp` to a shared
//    tail (lea &e / store msg / push / call FUN_004c2e30), while ours emits
//    the whole tail in both arms. Reordering the field stores (flag before
//    message, or after) did not make MSVC 5 merge the two suffixes.
//  * The 0x219/0x3b9 handlers load DAT_0051fbd0 into eax then the callback
//    field into eax, where the original uses ecx for DAT (0x219) / edx (0x3b9).
//  * The 0x30f/0x311 palette arms use eax/ecx/edx in the opposite roles to the
//    original (original: edx = DAT in 0x30f, ecx = DAT in 0x311).

#include <windows.h>
#include <ddraw.h>

// 24 byte event packet shared with FUN_004c2360 and FUN_004c2e30.
struct Event_4b5cc0 {
    int x;          // +0x00
    int y;          // +0x04
    int buttons;    // +0x08
    int time;       // +0x0c
    int message;    // +0x10
    int flag;       // +0x14
};

// +0xf0: display state flags, a 16 bit bitfield so a single bit test loads
// just the byte holding the bit.
union Flags_4b5cc0 {
    unsigned short value;
    struct {
        unsigned short opt : 1;    // bit 0
        unsigned short bit1 : 1;   // bit 1
        unsigned short gdi : 1;    // bit 2
        unsigned short bit3 : 1;
        unsigned short bit4 : 1;
        unsigned short rest : 11;
    } bits;
};

struct App_4b5cc0 {
    char unknown_0[0x40];
    HWND hwnd;                                 // +0x40
    char unknown_44[0x4c - 0x44];
    HPALETTE hpalette;                         // +0x4c
    char unknown_50[0x80 - 0x50];
    void (__cdecl *callback)(int, int, int);   // +0x80
    char unknown_84[0x88 - 0x84];
    IDirectDrawSurface* primary;               // +0x88
    char unknown_8c[0x94 - 0x8c];
    IDirectDrawPalette* palette;               // +0x94
    char unknown_98[0xe0 - 0x98];
    int active;                                // +0xe0
    char unknown_e4[0xe8 - 0xe4];
    int tickScale;                             // +0xe8
    char unknown_ec[0xf0 - 0xec];
    Flags_4b5cc0 flags;                        // +0xf0
};

extern App_4b5cc0* DAT_0051fbd0;
extern void (*DAT_0051fc78)(int);
extern int DAT_0051fc7c;

void __stdcall FUN_004b5510(int param);
void __stdcall FUN_004c1b20(int v);
void __stdcall FUN_004c1d50(int v, int flag);
void __stdcall FUN_004c2360(int* p);
void __stdcall FUN_004c2e30(Event_4b5cc0* ev);

// FUNCTION: 0x4b5cc0
long __stdcall FUN_004b5cc0(HWND hwnd, unsigned int msg, unsigned int wparam,
                            unsigned int lparam)
{
    switch (msg) {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        if (DAT_0051fbd0->flags.bits.bit1)
            FUN_004b5510(0);
        PostQuitMessage(0);
        return 0;
    case WM_ACTIVATE:
        if ((unsigned short)wparam == 0)
            DAT_0051fbd0->active = 0;
        else
            DAT_0051fbd0->active = 1;
        return 0;
    case WM_CLOSE:
        if (DAT_0051fc78 != 0) {
            DAT_0051fc78(DAT_0051fc7c);
            return 0;
        }
        DestroyWindow(hwnd);
        return 0;
    case WM_KEYDOWN:
        FUN_004c1d50(wparam, 0);
        return 0;
    case WM_CHAR:
        FUN_004c1b20(wparam);
        return 0;
    case WM_SYSKEYDOWN:
        FUN_004c1d50(wparam, 1);
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_SYSCOMMAND:
        if (wparam == 0xf100)
            return 0;
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_MOUSEMOVE: {
        Event_4b5cc0 e;
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 0;
        e.message = msg;
        FUN_004c2360((int*)&e);
        return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP: {
        Event_4b5cc0 e;
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 0;
        e.message = msg;
        FUN_004c2e30(&e);
        return 0;
    }
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK: {
        Event_4b5cc0 e;
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 1;
        e.message = msg;
        FUN_004c2e30(&e);
        return 0;
    }
    case 0x219:
        if (DAT_0051fbd0->callback != 0)
            DAT_0051fbd0->callback(0x219, wparam, lparam);
        return 1;
    case 0x30f: {
        if (DAT_0051fbd0->hpalette) {
            HDC dc = GetDC(DAT_0051fbd0->hwnd);
            SelectPalette(dc, DAT_0051fbd0->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(DAT_0051fbd0->hwnd, dc);
            return 1;
        }
        HRESULT hr = E_FAIL;
        if (DAT_0051fbd0->primary && DAT_0051fbd0->palette)
            hr = DAT_0051fbd0->primary->SetPalette(DAT_0051fbd0->palette);
        return hr == DD_OK ? 1 : 0;
    }
    case 0x311:
        if (DAT_0051fbd0->hwnd == (HWND)wparam)
            return 0;
        if (DAT_0051fbd0->hpalette) {
            HDC dc = GetDC(DAT_0051fbd0->hwnd);
            SelectPalette(dc, DAT_0051fbd0->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(DAT_0051fbd0->hwnd, dc);
            return 0;
        }
        {
            HRESULT hr2 = E_FAIL;
            if (DAT_0051fbd0->primary && DAT_0051fbd0->palette)
                hr2 = DAT_0051fbd0->primary->SetPalette(DAT_0051fbd0->palette);
        }
        return 0;
    case 0x3b9:
        if (DAT_0051fbd0->callback != 0)
            DAT_0051fbd0->callback(0x3b9, wparam, lparam);
        return 1;
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}
