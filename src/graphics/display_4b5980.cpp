// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash, retried by Claude Fable 5.1, finished by DeepSeek V4.1 Flash, worked on by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Space Bunny Free (89.2% -> 93.6%): `d->width = w; d->height = h;` written
// directly and the flag-word update after them fixed the schedule of that
// block; `int w` must be declared before `int h`, or edi and ebp swap.
//
// Claude Opus 5.5 (93.6% -> MATCH), two changes:
//  * The first flag update is three plain statements on the bitfields,
//    `no_video = 0; hwnd = 0; sound_opt = (videoFlags >> 9) & 1;`. Written as
//    a whole-word expression it needed a `Flags*` alias to keep MSVC from
//    folding the two masks, and the alias also stopped the scheduler from
//    hoisting the videoFlags load above the in-memory bit-11 clear.
//  * Every bit test is a bitfield test, as in the original
//    (`mov cl,[esi+0xf0]; shr cl,5; test cl,1`). What made the bit-5 test
//    misbehave before was the scratch-register rotation: MSVC 5 hands out
//    eax/ecx/edx in turn, and the original has one more temporary in the
//    second flag update than a single expression gives. Naming the shifted
//    videoFlags bits as an `int` local (`int t = ...; flags = ... | t | 1;`)
//    adds it at no code cost, which moves the bit-5 test to cl and every
//    later test to the original's register. Without it the bitfield test
//    lands on dl and the allocator CSEs `&d->wc` into edi and spills the
//    height (826 bytes); the old mask spelling `flags.value & 0x20` only hid
//    that by folding the test into memory.
// The struct needs `#pragma pack(2)`: the mode struct at +0x1ea and the two
// ints after it sit at 0x1ea, 0x1fa and 0x1fe, and videoFlags is a word at
// +0x202. WNDCLASSA has to be padded to +0x18 by hand for the same reason.
// IDC_ARROW is passed as the raw Win16 value 103 (0x67). `push 0x7f00` at the
// top of the GDI block is MSVC hoisting LoadIconA's IDI_APPLICATION (0x7f00
// in this SDK), not a source statement.
//
// Suspected original bug: the work area rectangle fetched with
// SystemParametersInfoA(SPI_GETWORKAREA) at +0xec overlaps the flag word at
// +0xf0, which is the rectangle's `top`, and the flag word is overwritten
// three instructions later, so the fetch is pointless.
#include <windows.h>

#pragma pack(push, 2)

struct View_4b5980 {
    int x;
    int y;
    int z;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
};

struct Mode_4b5980 {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
};

// +0xf0: the word that holds the display object's state flags. The work area
// rectangle fetched from Windows starts four bytes earlier, so its `top` word
// is the flag word and both are written through this one field.
union Flags_4b5980 {
    unsigned short value;
    struct {
        unsigned short opt : 1;        // bit 0
        unsigned short bit1 : 1;       // bit 1
        unsigned short gdi : 1;        // bit 2
        unsigned short bit3 : 1;       // bit 3
        unsigned short bit4 : 1;       // bit 4
        unsigned short has_c0 : 1;     // bit 5
        unsigned short has_c4 : 1;     // bit 6
        unsigned short has_c8 : 1;     // bit 7
        unsigned short has_cc : 1;     // bit 8
        unsigned short has_d0 : 1;     // bit 9
        unsigned short sound_opt : 1;  // bit 10
        unsigned short no_video : 1;   // bit 11
        unsigned short bit12 : 4;      // bits 12 to 15
    } bits;
};

struct App_4b5980 {
    HINSTANCE hInstance;                 // +0x00
    int nCmdShow;                        // +0x04
    char* className;                     // +0x08
    char* title;                         // +0x0c
    WNDPROC wndProc;                     // +0x10
    unsigned short menuId;               // +0x14
    char unknown_16[2];
    WNDCLASSA wc;                        // +0x18
    HWND hwnd;                           // +0x40
    HGDIOBJ oldPalette;                  // +0x44
    HDC dc;                              // +0x48
    HGDIOBJ bitmap;                      // +0x4c
    char unknown_50[0x80 - 0x50];
    int unknown_80;                      // +0x80
    int scratch[6];                      // +0x84
    char unknown_9c[0xa0 - 0x9c];
    Mode_4b5980 mode;                    // +0xa0
    char unknown_b0[0xc4 - 0xb0];
    int obj_c4;                          // +0xc4
    int obj_c8;                          // +0xc8
    char unknown_cc[0xd4 - 0xcc];
    int width;                           // +0xd4
    int height;                          // +0xd8
    int unknown_dc;                      // +0xdc
    char unknown_e0[0xec - 0xe0];
    int wa_left;                         // +0xec, work area left
    Flags_4b5980 flags;                  // +0xf0, work area top
    char wa_rest[0xfc - 0xf2];
    char unknown_fc[0x1da - 0xfc];
    int accum;                           // +0x1da
    int lastTick;                        // +0x1de
    int unknown_1e2;                     // +0x1e2
    int unknown_1e6;                     // +0x1e6
    Mode_4b5980 startMode;               // +0x1ea
    int startWidth;                      // +0x1fa
    int startHeight;                     // +0x1fe
    unsigned short videoFlags;           // +0x202
    char unknown_204[0x618 - 0x204];
    void** items;                        // +0x618
    int itemCount;                       // +0x61c
    int availPhys;                       // +0x620
    int unknown_624;                     // +0x624
    char unknown_628[0x728 - 0x628];
    unsigned char unknown_728;           // +0x728
};

#pragma pack(pop)

extern App_4b5980* DAT_0051fbd0;

void FUN_004bce10(void);
void __stdcall FUN_004c2360(int* p);
void __stdcall FUN_004c1a60(int size);
void __stdcall FUN_004c2bd0(int count, int start);
int __stdcall FUN_004ba610(App_4b5980* d);
int __stdcall FUN_004ba5c0(App_4b5980* d);
int __stdcall FUN_004ba660(App_4b5980* d);
int __stdcall FUN_004ba6b0(App_4b5980* d);
int __stdcall FUN_004ba700(App_4b5980* d);
void __stdcall FUN_004b4ff0(App_4b5980* d);
int __stdcall FUN_004b5510(int param);
long __stdcall FUN_004b5cc0(HWND hwnd, unsigned int msg, unsigned int wparam, long lparam);

// FUNCTION: 0x4b5980
int __stdcall FUN_004b5980(App_4b5980* d)
{
    DAT_0051fbd0 = d;
    MEMORYSTATUS mem;
    mem.dwLength = 0x20;
    GlobalMemoryStatus(&mem);
    d->availPhys = mem.dwTotalPhys;
    SystemParametersInfoA(0x5e, 0, (LPRECT)&DAT_0051fbd0->wa_left, TRUE);
    SystemParametersInfoA(0x5d, 0, 0, TRUE);
    d->accum = 0;
    d->lastTick = GetTickCount();
    d->unknown_1e2 = 0;
    d->unknown_1e6 = 0;
    d->scratch[0] = 0;
    d->scratch[1] = 0;
    d->scratch[2] = 0;
    d->scratch[3] = 0;
    d->scratch[4] = 0;
    d->scratch[5] = 0;
    d->unknown_624 = 0;
    d->unknown_dc = 0;
    d->flags.bits.no_video = 0;
    d->hwnd = 0;
    d->flags.bits.sound_opt = (d->videoFlags >> 9) & 1;
    FUN_004c1a60(0x1e);
    FUN_004c2bd0(0x14, d->flags.bits.sound_opt);
    View_4b5980 view;
    view.x = 0;
    view.y = 0;
    view.z = 0;
    d->unknown_728 = 0;
    FUN_004bce10();
    FUN_004c2360((int *)&view);

    d->items = 0;
    d->itemCount = 0;
    int w = d->startWidth;
    int h = d->startHeight;
    d->width = w;
    d->height = h;
    // bits 1..8 of the video flags become bits 2..9 of the flag word
    int subsys = (d->videoFlags & 0x1fe) << 1;
    d->flags.value = (d->flags.value & 0xfc03) | subsys | 1;
    if (d->flags.bits.has_c4) {
        FUN_004ba610(d);
    } else {
        d->obj_c4 = 0;
    }
    if (d->flags.bits.has_c0) {
        FUN_004ba5c0(d);
    }
    if (d->flags.bits.has_c8) {
        FUN_004ba660(d);
    } else {
        d->obj_c8 = 0;
    }
    if (d->flags.bits.has_cc) {
        FUN_004ba6b0(d);
    }
    if (d->flags.bits.has_d0) {
        FUN_004ba700(d);
    }
    if (d->flags.bits.gdi) {
        d->dc = 0;
        d->bitmap = 0;
        d->oldPalette = 0;
        d->unknown_80 = 0;
        d->mode = d->startMode;
        d->wc.lpfnWndProc = FUN_004b5cc0;
        d->wc.style = 8;
        d->wc.hInstance = d->hInstance;
        d->wc.lpszClassName = d->className;
        d->wc.hIcon = LoadIconA(d->hInstance, IDI_APPLICATION);
        d->wc.hCursor = LoadCursorA(d->hInstance, (LPCSTR)103);
        d->wc.lpszMenuName = (LPSTR)d->menuId;
        d->wc.cbClsExtra = 0;
        d->wc.cbWndExtra = 0;
        d->wc.hbrBackground = GetStockObject(BLACK_BRUSH);
        unsigned int cls = RegisterClassA(&d->wc);
        if (cls != 0) {
            d->hwnd = CreateWindowExA(WS_EX_APPWINDOW, d->className, d->title,
                                      WS_POPUP | WS_VISIBLE | WS_SYSMENU,
                                      CW_USEDEFAULT, CW_USEDEFAULT, w, h,
                                      0, 0, d->hInstance, 0);
            if (d->hwnd != 0) {
                ShowWindow(d->hwnd, d->nCmdShow);
                UpdateWindow(d->hwnd);
                int r = FUN_004b5510(d->videoFlags & 1);
                if (r != 0) {
                    return 1;
                }
            }
        }
        FUN_004b4ff0(d);
        if (d->dc) {
            DeleteDC(d->dc);
        }
        if (d->bitmap) {
            DeleteObject(d->bitmap);
        }
        if (d->oldPalette) {
            DeleteObject(d->oldPalette);
        }
        d->dc = 0;
        d->bitmap = 0;
        d->oldPalette = 0;
        MessageBoxA(d->hwnd, "Error:  Environment Initialization Failed!\nCheck your DirectX setup", d->title, 0);
        DestroyWindow(d->hwnd);
        return 0;
    }
    return 1;
}
