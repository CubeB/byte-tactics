// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash, retried by Claude Fable 5.1, finished by DeepSeek V4.1 Flash, worked on by Space Bunny Free. Names are provisional.
// Space Bunny Free: 89.2% (827 bytes) -> 93.6% (815 bytes). Two changes, both
// found by sweeping the statement order of the block that writes width,
// height and the flag word, and the one spelling of the bit-5 test:
//  * `d->width = w; d->height = h;` written directly (the `int* pw/ph` locals
//    earlier attempts needed are gone: they are what forced MSVC to pick
//    videoFlags=edx/flags=ecx in the flag block), and the flag-word update
//    moved AFTER those two stores. That fixes the instruction schedule of the
//    whole block: all 17 instructions now match the original one for one
//    (videoFlags=ecx, flags=eax, `or al,1`, `shr al,6`, and the height store
//    sunk past the test), which removes the `mov al,[esi+0xf0]` reload and the
//    two-byte-longer `and ecx,imm32` / `or ecx,1` forms. It also lets the
//    remaining bit tests fall into the original's cl/dl/al/cx/dl cycle.
//  * `d->wc.style = 8;` moved to just after `d->wc.lpfnWndProc = FUN_004b5cc0;`
//    instead of just before RegisterClassA. On its own that made MSVC keep
//    `&d->wc` in edi (a 3-byte lea, a spilled startHeight, a 0x3c frame,
//    835 bytes); with the schedule above it does not, so the store lands at
//    the original's place as `mov dword ptr [esi + 0x18], 8`.
// What still differs (11 of the 14 remaining diff lines are jump targets that
// only moved, so the shape is 98.7%):
//  * the bit-5 test is written `d->flags.value & 0x20`, which MSVC folds to
//    `test byte ptr [esi + 0xf0], 0x20` (7 bytes). The original loads the
//    byte into a register: `mov cl,[esi+0xf0]; shr cl,5; test cl,1`, which
//    only a `Flags_4b5980::bits.has_c0` bitfield produces. This spelling is
//    load-bearing: with the bitfield the test takes the register the allocator
//    has left (dl instead of cl), which rotates every later test and puts the
//    `&d->wc` materialisation back (826 bytes, frame 0x3c, 69.2%). The other
//    four tests still are bitfields and do match. All 64 spellings of the six
//    tests (bitfield / mask) were compiled: `!= 0` and `& 1` on the bitfield
//    give the same memory test as the mask, and no combination gets past 93.6%.
//  * the `mov cx, word ptr [esi + 0x202]` of the first flag block sits after
//    `mov dword ptr [esi + 0x40], ebx` instead of between the scratch[0] and
//    scratch[1] stores. Reading `d->videoFlags` into an `unsigned short`
//    before the scratch stores does move that load to the original's offset,
//    but it also swaps the two values' registers in that block (videoFlags
//    gets eax, the flag word ecx), which costs more than it gains (91.1%).
// Leads from earlier attempts, all still true:
//  * `Flags_4b5980* fl = &d->flags;` declared just before the no_video clear
//    is what keeps MSVC from folding the bit-11 clear and the bit-10 update
//    into one `and eax,0xf3ff`; it pins the in-place
//    `and word ptr [esi + 0xf0], 0xf7ff` and the reload after it.
//  * `unsigned int cls = RegisterClassA(...)` (not `ATOM`) is what gives the
//    `and eax,0xffff` and the home-slot store at `[esp + 0x4c]`, and
//    `int r = FUN_004b5510(...); if (r != 0)` gives the `cmp eax,ebx; jne`.
//  * `int w` must be declared before `int h`, or edi and ebp swap.
//  * About 450 check.py runs in this round and 2 permuter runs on the older
//    file (15 and 45 minutes, both flat at 89.2%). Inert: the `int* pw/ph`
//    pointer locals (any one of them flips the allocator the other way),
//    address-taken locals for every other field, reading the bit tests through
//    `fl` instead of `d` (that one does give the original's cl/dl/al/cx/dl,
//    but it puts videoFlags back in edx and the frame back to 0x3c), the
//    `&d->wc` pointer/reference/cast spellings of the style store, a
//    field-by-field mode copy, the `int`/`unsigned int`/`unsigned char`
//    bitfield types, 16 single-bit fields, byte and word shift expressions
//    instead of bitfields, inline helpers for the flag update and the size
//    update, every statement position of the flag update, of the width/height
//    declaration and of the wc style store, and the `MEMORYSTATUS`/`view`/
//    early-store orderings.
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
    Flags_4b5980* fl = &d->flags;
    fl->bits.no_video = 0;
    d->hwnd = 0;
    fl->value = (fl->value & 0xfbff) | ((d->videoFlags & 0x200) << 1);
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
    d->flags.value = (d->flags.value & 0xfc03) | ((d->videoFlags & 0x1fe) << 1) | 1;
    if (d->flags.bits.has_c4) {
        FUN_004ba610(d);
    } else {
        d->obj_c4 = 0;
    }
    if (d->flags.value & 0x20) {
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
