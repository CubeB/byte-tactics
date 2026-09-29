// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// Run of deepseek-v4.1-flash: 77.0%, 820 of 820 bytes. The only source change
// from the 75.7% version is the width/height block, which now takes pointer
// locals to the two fields and stores through them:
//     int h = d->startHeight;
//     int w = d->startWidth;
//     int* pw = &d->width;
//     int* ph = &d->height;
//     *pw = w;
//     *ph = h;
// That moves w out of the incoming-argument spill and into a register (still
// eax, not the original's edi) and buys 1.3 points, but as a side effect the
// two updates of the flag word at +0xf0 fold into one `and eax, 0xf3ff` where
// the original clears bit 11 in place with `and word [esi+0xf0], 0xf7ff` and
// reloads. Net +1.3.
//
// The single upstream cause stays the same: edi is allocated to the wc base
// address, so w and h cannot both have edi/ebp and the ATOM stays in ax.
// Tried and did NOT move edi off the wc base (all 75.7% or worse): breaking
// the &d->wc CSE with a (char*)d + 0x18 cast, with a second WNDCLASSA* local
// before/after the call, with a static inline accessor, and with a helper
// that fills all the WNDCLASSA fields; using d->width/d->height or
// d->startWidth/d->startHeight directly in CreateWindowExA; declaring w/h
// before the items stores or before the gdi test; inline W/H getters. A
// diagnostic that moved the RegisterClassA argument to &d->hwnd just moved
// the edi base to +0x40, so it is the address-of-a-member value itself that
// wins edi, not the particular member.
// Brings the application up: reads the work area and the physical memory
// size, resets the frame timer, the mouse-event queue and the video-mode
// struct, runs the five per-object initialisers selected by the flag word at
// +0xf0, then, when the GDI bit is set, registers the window class, creates
// and shows the main window and asks for the display mode. Returns 1 on
// success; on failure it tears the GDI objects down again, shows the
// "Environment Initialization Failed" box and returns 0.
//
// 75.7%, 820 of 820 bytes (size exact). Fixes since the 71.5% version:
// removed a spurious early `d->unknown_80 = 0` (the original only zeroes
// +0x80 in the GDI block, and the extra store shifted the videoFlags load),
// and GetStockObject takes BLACK_BRUSH (4), not WHITE_BRUSH (0).
//
// What is still different, all downstream of one allocation decision:
// the width local lands in ecx spilled through [esp+0x4c] instead of edi,
// so the height spill, the five flag-test byte registers (dl/al/cl/dx/al
// here vs cl/dl/al/cx/dl there), the hoisted `lea edi, [esi+0x18]`
// WNDCLASSA base (original addresses wc through esi and recomputes the
// address just for RegisterClassA), the mode-copy pointer roles, the menuId
// zero extension (ecx here, eax there), the ATOM kept in ax and tested with
// `cmp ax, bx` (original stores the zero-extended word to [esp+0x4c] and
// tests the `and` flags), the CreateWindowExA reload of w from the stack,
// and the tail `test eax, eax` (original `cmp eax, ebx`). Tried without
// effect: both w/h load orders, both store orders, loads before items and
// flags, items after the loads, stores before items and flags, unsigned
// w/h, `int cls`, mode copy before the zero stores, `value &= 0xf7ff` for
// the bit 11 clear. A single long-lived local always takes ebp and the
// second always spills, whatever the order, so edi stays reserved for the
// wc base; the original must reach width through a tree that pins edi first.
// Brings the application up: reads the work area and the physical memory
// size, resets the frame timer, the mouse-event queue and the video-mode
// struct, runs the five per-object initialisers selected by the flag word at
// +0xf0, then, when the GDI bit is set, registers the window class, creates
// and shows the main window and asks for the display mode. Returns 1 on
// success; on failure it tears the GDI objects down again, shows the
// "Environment Initialization Failed" box and returns 0.
//
// 71.5%, 816 of 820 bytes. What is still different, all of it in the window
// creation half of the function:
//
//  * The local handed to FUN_004c2360 is a 24 byte object whose FIRST three
//    dwords are the ones zeroed, and the pointer passed is the object's own
//    base, not the base plus 4. Getting that wrong costs a store of a fourth
//    dword and puts the three stores at +0x14..+0x1c instead of
//    +0x10..+0x18.
//  * dwExStyle is 0x40000, that is WS_EX_APPWINDOW, not WS_EX_TOPMOST (0x8).
//  * `int w`/`int h` must be declared in the order height, width, or the
//    allocator hands ebp to the width and spills the height through the
//    incoming argument slot at [esp+0x4c].
//
//  * +0x0e the two flag statements are merged. The original clears bit 11
//    with an in-place `and word [esi+0xf0], 0xf7ff`, then reloads the word
//    for the bit 10 update (`and eax, 0xfbff / shl ecx,1 / or eax,ecx`);
//    ours folds the two read-modify-writes into one `and eax, 0xf3ff` and
//    sinks the `d->hwnd = 0` store between the load and the store. Every
//    spelling tried folds: `&= 0xf7ff` on `.value`, `= .value & 0xf7ff`, a
//    bitfield write for the clear, `d->hwnd = 0` before or after the clear,
//    `d->flags.value = d->flags.value & 0xfbff` as a separate statement from
//    the `|=`, a named local holding the masked word, a named local holding
//    `(d->videoFlags & 0x200) << 1`, `d->flags.bits.sound_opt = (... != 0)`
//    and a read through a second `unsigned short*` local. All score 71.0% or
//    below, so this is MSVC 5 forwarding the bitfield store's value into the
//    later read, and the original must have reached the same flag word by a
//    tree MSVC cannot forward through.
//  * +0x9f the original holds width in edi and height in ebp from 0x4b5aa1
//    to the CreateWindowExA call. Ours still gives ebp to the width and
//    spills the height through the incoming argument slot at [esp+0x4c]
//    (and reloads it for nHeight), because edi is already taken (next item).
//    Declaring the two locals at the top of the function instead of next to
//    their stores makes it much worse (51.9%), and declaring them before the
//    `d->items = 0` pair instead of after it costs 3 points (66.8%), because
//    both move the frame layout. Declaring height before width is the only
//    ordering that matches at all and is worth 0.5 points.
//  * because one callee-saved register is free, ours hoists
//    `lea edi, [esi+0x18]` (the WNDCLASSA address, used for the
//    `wc.style = 8` store and for RegisterClassA) to the top of the block,
//    where the original materialises it just before the call at 0x4b5bd6 and
//    stores the style through esi. At the style store every other register is
//    live in the original too, which is why it can only use esi. Freeing edi
//    for the width is the single upstream cause of this item, of the height
//    spill above and of the ATOM store below.
//  * +0x205 ours scalar-replaces the ATOM returned by RegisterClassA and
//    tests it with `cmp ax, bx`; the original stores the zero-extended word
//    to the incoming argument slot at [esp+0x4c] and tests that. A dead store
//    to a home-area local that MSVC 5 evidently does not eliminate.
//  * +0x14d the two of the five per-object flag tests that read the low byte
//    land in dl and al here and in cl there, and the load of
//    `d->videoFlags` for the second flag update sits three instructions
//    earlier. Scheduling.
//
// The struct needs `#pragma pack(2)`: the mode struct at +0x1ea and the two
// ints after it sit at 0x1ea, 0x1fa and 0x1fe, and videoFlags is a word at
// +0x202. WNDCLASSA has to be padded to +0x18 by hand for the same reason.
// IDC_ARROW is passed as the raw Win16 value 103 (0x67); the SDK header in
// this toolchain defines it as MAKEINTRESOURCE(32512), the same value
// IDI_APPLICATION has, which is what the LoadIconA call really uses.
//
// The local at -56 from the frame pointer is 24 bytes but only its first
// three dwords are written, and the pointer handed to
// FUN_004c2360 is `&view`; that callee copies 24 bytes, so it reads 12
// bytes past the initialised part, which lands in the MEMORYSTATUS directly
// above it. Declaring the local as a plain `int[3]` gives a 0x2c frame and
// puts every [esp+N] reference in the function two or four bytes out, so the
// 24 byte shape is what makes the frame, the MEMORYSTATUS at -32 and the
// three stores at [esp+0x10] to [esp+0x18] line up.
//
// Suspected original bug: the work area rectangle fetched with
// SystemParametersInfoA(SPI_GETWORKAREA) at +0xec overlaps the flag word at
// +0xf0, which is the rectangle's `top`, and the flag word is overwritten
// three instructions later. The fetch is therefore pointless, and the
// matching SPI_SETWORKAREA call passes a null rectangle, so the work area
// Windows had is left in place. 0x4b6110 shows the mirror image of the same
// confusion, passing the rectangle's `left` as the uiParam of
// SPI_SETWORKAREA.
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

int FUN_004bce10(void);
void __stdcall FUN_004c2360(int* p);
void __stdcall FUN_004c1a60(int size);
void __stdcall FUN_004c2bd0(int count, int start);
void __stdcall FUN_004ba610(App_4b5980* d);
void __stdcall FUN_004ba5c0(App_4b5980* d);
void __stdcall FUN_004ba660(App_4b5980* d);
void __stdcall FUN_004ba6b0(App_4b5980* d);
void __stdcall FUN_004ba700(App_4b5980* d);
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
    d->flags.value = (d->flags.value & 0xfbff) | ((d->videoFlags & 0x200) << 1);
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
    d->flags.value = (d->flags.value & 0xfc03) | ((d->videoFlags & 0x1fe) << 1) | 1;
    int h = d->startHeight;
    int w = d->startWidth;
    int* pw = &d->width;
    int* ph = &d->height;
    *pw = w;
    *ph = h;
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
        ATOM cls = RegisterClassA(&d->wc);
        if (cls != 0) {
            d->hwnd = CreateWindowExA(WS_EX_APPWINDOW, d->className, d->title,
                                      WS_POPUP | WS_VISIBLE | WS_SYSMENU,
                                      CW_USEDEFAULT, CW_USEDEFAULT, w, h,
                                      0, 0, d->hInstance, 0);
            if (d->hwnd != 0) {
                ShowWindow(d->hwnd, d->nCmdShow);
                UpdateWindow(d->hwnd);
                if (FUN_004b5510(d->videoFlags & 1) != 0) {
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
