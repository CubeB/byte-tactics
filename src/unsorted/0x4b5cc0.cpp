// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Window procedure of the main application window: translates the custom
// display messages and forwards the rest to the default handler.
//
// PARTIAL, 89.3% (original 1104 bytes, ours 1112). Control flow, every struct
// offset, every call and the whole jump table now match instruction for
// instruction except for the two points listed below.
//
//  * The two FUN_004c2e30 arms are written as two case bodies that `break` out
//    of the switch into a shared tail (e.message = msg; FUN_004c2e30(&e)).
//    That is what makes the compiler duplicate their common prefix and
//    tail-merge the suffix: 0x4b5cc0+0x23c jumps to the tail and the
//    0x203/0x206 arm falls straight into it, exactly as the original does.
//    A `default: return DefWindowProcA(...)` clause is required: without it the
//    switch's default call is dead-eliminated and the function loses 29 bytes.
//  * The 0x30f arm's E_FAIL has to be declared BEFORE the hpalette `if` so that
//    `hr` is live across that branch. That extra liveness is what demotes
//    DAT_0051fbd0 from edx to ecx and the surface pointer into edx, matching
//    the original's 0x4b60c4 block. Declaring it after the `if` (the obvious
//    spelling) gives the right code shape but the wrong registers.
//
// What still differs, 2 items, 8 bytes:
//
//  1. `je` at 0x4b5f9ad and 0x4b5fd3 (the "callback == 0" jumps out of the
//     0x219 and 0x3b9 arms) are 6 bytes here and 2 bytes in the original,
//     because the shared "return 1" block they target is laid out after the
//     0x30f arm instead of immediately after the 0x3b9 arm. Both layouts have
//     the same three copies of `mov eax,1 / pop esi / add esp,0x18 / ret 0x10`;
//     MSVC 5 just picks a different representative to absorb the two
//     duplicates. Reordering the case labels, spelling the 0x219/0x3b9 returns
//     as a `break` into a shared `return 1` after the switch, and using `goto`
//     all leave the representative at the end of the function. Those 8 bytes
//     are the whole size difference.
//  2. `mov eax, 0x80004005` (E_FAIL) is hoisted to the top of the 0x30f arm
//     (ours 0x4b60c3) instead of into the primary/palette block (original
//     0x4b60ca, after `mov edx, [ecx+0x88]`). Net zero bytes: the 0x30f
//     primary block is otherwise byte identical. Moving the initialiser back
//     after the hpalette `if` puts the constant in the right place and breaks
//     the register allocation again (87.8%).

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
    Event_4b5cc0 e;
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
    case WM_MOUSEMOVE:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 0;
        e.message = msg;
        FUN_004c2360((int*)&e);
        return 0;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 0;
        break;
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        e.x = lparam & 0xffff;
        e.y = (lparam >> 16) & 0xffff;
        e.buttons = wparam;
        e.time = GetTickCount() * DAT_0051fbd0->tickScale / 1000;
        e.flag = 1;
        break;
    case 0x219:
        if (DAT_0051fbd0->callback != 0)
            DAT_0051fbd0->callback(0x219, wparam, lparam);
        return 1;
    case 0x30f: {
        HRESULT hr = E_FAIL;
        if (DAT_0051fbd0->hpalette) {
            HDC dc = GetDC(DAT_0051fbd0->hwnd);
            SelectPalette(dc, DAT_0051fbd0->hpalette, FALSE);
            RealizePalette(dc);
            ReleaseDC(DAT_0051fbd0->hwnd, dc);
            return 1;
        }
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
        if (DAT_0051fbd0->primary && DAT_0051fbd0->palette)
            DAT_0051fbd0->primary->SetPalette(DAT_0051fbd0->palette);
        return 0;
    case 0x3b9:
        if (DAT_0051fbd0->callback != 0)
            DAT_0051fbd0->callback(0x3b9, wparam, lparam);
        return 1;
    default:
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    e.message = msg;
    FUN_004c2e30(&e);
    return 0;
}
