// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// Window procedure of the main application window: translates the custom
// display messages and forwards the rest to the default handler.
#include <windows.h>
#include <ddraw.h>

// 24 byte event packet shared with SetCurrentMouseEvent and PushMouseEvent.
struct Event_4b5cc0 {
    int x;          // +0x00
    int y;          // +0x04
    int buttons;    // +0x08
    int time;       // +0x0c
    int message;    // +0x10
    int flag;       // +0x14
};

// +0xf0: display state flags, a 16 bit bitfield so the bit 1 test loads just
// the byte holding the bit.
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

extern App_4b5cc0* g_display;
extern void (__cdecl *g_closeHandler)(int);
extern int g_closeHandlerArg;

void __stdcall SetFullScreen(int param);
void __stdcall PushKeyCode(int v);
void __stdcall HandleVirtualKey(int v, int flag);
void __stdcall SetCurrentMouseEvent(int* p);
void __stdcall PushMouseEvent(Event_4b5cc0* ev);

// FUNCTION: 0x4b5cc0
long __stdcall WindowProc(HWND hwnd, unsigned int msg, unsigned int wparam,
                            unsigned int lparam)
{
    Event_4b5cc0 e;
    switch (msg) {
    case WM_CREATE:
        return 0;
    case WM_DESTROY:
        if (g_display->flags.bits.bit1)
            SetFullScreen(0);
        PostQuitMessage(0);
        return 0;
    case WM_ACTIVATE:
        if ((unsigned short)wparam == 0)
            g_display->active = 0;
        else
            g_display->active = 1;
        return 0;
    case WM_CLOSE:
        if (g_closeHandler != 0) {
            g_closeHandler(g_closeHandlerArg);
            return 0;
        }
        DestroyWindow(hwnd);
        return 0;
    case WM_KEYDOWN:
        HandleVirtualKey(wparam, 0);
        return 0;
    case WM_CHAR:
        PushKeyCode(wparam);
        return 0;
    case WM_SYSKEYDOWN:
        HandleVirtualKey(wparam, 1);
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_SYSCOMMAND:
        if (wparam == 0xf100)
            return 0;
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    case WM_MOUSEMOVE:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 0;
        e.message = msg;
        SetCurrentMouseEvent((int*)&e);
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 0;
        break;
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * g_display->tickScale / 1000;
        e.flag = 1;
        break;
    case 0x219:
        if (g_display->callback != 0)
            g_display->callback(0x219, wparam, lparam);
        return 1;
    case 0x30f: {
        // Single result local and one `return ok;`: keeps the return path folded.
        long ok;
        if (g_display->hpalette) {
            HDC dc = GetDC(g_display->hwnd);
            SelectPalette(dc, g_display->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(g_display->hwnd, dc);
            ok = 1;
        }
        else {
            HRESULT hr = E_FAIL;
            if (g_display->primary && g_display->palette)
                hr = g_display->primary->SetPalette(g_display->palette);
            ok = hr == DD_OK ? 1 : 0;
        }
        return ok;
    }
    case 0x311:
        if (g_display->hwnd == (HWND)wparam)
            return 0;
        if (g_display->hpalette) {
            HDC dc = GetDC(g_display->hwnd);
            SelectPalette(dc, g_display->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(g_display->hwnd, dc);
            return 0;
        }
        if (g_display->primary && g_display->palette)
            g_display->primary->SetPalette(g_display->palette);
        return 0;
    case 0x3b9:
        if (g_display->callback != 0)
            g_display->callback(0x3b9, wparam, lparam);
        return 1;
    default:
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    e.message = msg;
    PushMouseEvent(&e);
    return 0;
}
