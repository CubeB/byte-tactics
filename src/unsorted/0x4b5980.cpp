// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry 2: seven more variants, all below 89.2%.
// Two findings for the next attempt. (1) Block 0: the original's early
// `mov cx,[esi+0x202]` position IS reachable, put `unsigned short vf =
// d->videoFlags;` in the source between the scratch[0] and scratch[1] stores and
// use `vf` in the block-0 expression; the load then lands exactly where the
// original has it, but the allocator swaps the block-0 roles (videoFlags to eax,
// flag word to ecx/eax) so the file stays 827 bytes at 86.7%. (2) Block 2: an
// `unsigned short`/union local for the new flag word makes that block byte-exact
// (`mov ax`/`and eax,0xfc03`/`or al,1`/no al reload) and drops to 818 bytes, but
// every following bit test then rotates one register (ours dl,al,ecx,dx for the
// original cl,dl,al,cx), 72.9-73.3%. A nested block that ends the local's scope
// right after the has_c4 test and writing the test as `(fv >> 6) & 1` (which
// emits `test al,0x40`) behave the same way.
//
// RETRY of deepseek-v4.1-flash: 89.2%, 827 of 820 bytes (7 over). The one gain
// over the 88.8% below is the tail test: writing the FUN_004b5510 result into a
// local (`int r = FUN_004b5510(...); if (r != 0)`) makes MSVC emit the
// original's `cmp eax,ebx; jne` instead of `test eax,eax; jne`.
// What still differs at 89.2%:
//  * scheduling of the videoFlags load in the first flag block (`mov cx,[+0x202]`
//    between the scratch[0]/scratch[1] stores in the original, later in ours).
//  * the second flag block picks dx/cx for videoFlags/flag-word where the
//    original picks cx/ax, so ours reloads `mov al,[+0xf0]` and uses `or ecx,1`
//    in place of the original `or al,1` (about 15 bytes over in that block).
//  * `d->wc.style = 8` is emitted just before RegisterClassA (`mov [eax],8`)
//    instead of the original's `mov [esi+0x18],8` between the wndProc store and
//    the mode-copy stores. Moving style earlier in the source (five positions
//    tried) forces a 4-byte spill and swaps the width/height registers, 72.4%.
//    Rewriting the flags expression in five forms, a videoFlags local, and
//    direct startWidth/startHeight all compile identically or worse.
//
// Run of deepseek-v4.1-flash: 88.8%, 827 of 820 bytes (7 over). Change from
// the previous 86.0%: both writes to the flag word at +0xf0 now go through a
// single `Flags_4b5980* fl = &d->flags;` local (declared just before the
// no_video clear) instead of `d->flags` directly. That stops MSVC 5 from
// folding the bit-11 clear and the bit-10 update into one `and eax,0xf3ff`:
// it now emits the original's in-place `and word [esi+0xf0],0xf7ff`, then
// reloads `mov ax,[esi+0xf0]` for the second update. Declaring the pointer
// before the clear (not only before the second store, the 87.9% v20) also
// pins the RMW late, between the unknown_dc and hwnd stores, as in the
// original. A `Flags_4b5980*` local used for only the second write scored
// 88.4% and one used only for the first 87.9%.
//
// GPT-6.1-sol refinement: seven check.py invocations, including the final
// verification; best remains 88.8%. Splitting the final flag expression into
// locals compiled identically. Routing it through a second flag pointer moved
// the flag update and item zero stores ahead of the start-width/height loads,
// scoring 87.9%; using the existing pointer for that update also scored 87.9%.
// The 88.8% direct-field version is restored below.
//
// deepseek-v4.1 run: 88.8% base restored after two experiments. (a) An
// `unsigned int f = (d->flags.value & 0xfc03) | ((d->videoFlags & 0x1fe) << 1) | 1;`
// local does put the flag word in eax as the original has it, but MSVC then folds the
// mask to `and eax,0xfc02` (the `| 1` makes bit 0 of the mask dead) and the extra live
// eax rotates every later bit test, 79.5%. (b) Swapping the two `|` operands and
// testing `d->flags.value & 0x40` instead of the bitfield gives 77.1%.
// What still differs:
//  * scheduling of the videoFlags load in the first flag block: the original
//    `mov cx,[esi+0x202]` sits between the scratch[0] and scratch[1] stores,
//    ours sits just after the bit-11 RMW. Reading videoFlags into a local
//    before the block only hoists the load higher (86.3%), and after the RMW
//    compiles identically to the direct read.
//  * the second flag block (before the has_c4 test) uses dx for videoFlags
//    and cx for the flag word; the original uses cx for videoFlags and ax for
//    the flag word. Ours therefore reloads `mov al,[esi+0xf0]` for the test
//    (+2 bytes) and emits `or ecx,1` where the original has the 2-byte
//    `or al,1`. A videoFlags local there compiles identically.
//  * the tail tests `test eax,eax` where the original has `cmp eax,ebx`.
//
// GPT-6.1-sol retry: six checker invocations in this session, including two compile failures. Moving the style assignment after hInstance reduced the score to 72.7%; loading videoFlags just after scratch[0] scored 86.3%. The original 88.8% source is restored.
// Earlier 86.0% runs established:
//  * `d->wc.style = 8` sits just before RegisterClassA instead of at its
//    natural place after wc.lpfnWndProc. With the store early, MSVC 5
//    common-subexpressions `&d->wc` into edi and spills the width; with it
//    late the address is used once and stays in eax, so edi is free for the
//    width. That restores the original `mov edi,[esi+0x1fa]` /
//    `mov ebp,[esi+0x1fe]` and most of the downstream register picks.
//  * `unsigned int cls = RegisterClassA(...)` rather than `ATOM`, which makes
//    the original zero-extend and home-slot store: `and eax,0xffff` then
//    `mov [esp+0x4c],eax` before the test.
// The width local must be declared before the height (`int w` then `int h`)
// for edi to get the width and ebp the height.
//
// The struct needs `#pragma pack(2)`: the mode struct at +0x1ea and the two
// ints after it sit at 0x1ea, 0x1fa and 0x1fe, and videoFlags is a word at
// +0x202. WNDCLASSA has to be padded to +0x18 by hand for the same reason.
// IDC_ARROW is passed as the raw Win16 value 103 (0x67).
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
    d->flags.value = (d->flags.value & 0xfc03) | ((d->videoFlags & 0x1fe) << 1) | 1;
    int w = d->startWidth;
    int h = d->startHeight;
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
        d->wc.hInstance = d->hInstance;
        d->wc.lpszClassName = d->className;
        d->wc.hIcon = LoadIconA(d->hInstance, IDI_APPLICATION);
        d->wc.hCursor = LoadCursorA(d->hInstance, (LPCSTR)103);
        d->wc.lpszMenuName = (LPSTR)d->menuId;
        d->wc.cbClsExtra = 0;
        d->wc.cbWndExtra = 0;
        d->wc.hbrBackground = GetStockObject(BLACK_BRUSH);
        d->wc.style = 8;
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
