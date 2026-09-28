// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

// DirectX 5's DPERR_BUFFERTOOSMALL, MAKE_DPHRESULT(30).
#define DPERR_BUFFERTOOSMALL_00443100 0x8877001e

#pragma pack(push, 1)

struct Guid_00443100 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

// The DirectPlay interfaces as laid out by the HAPINET_ wrappers.
struct Net_00443100 {
    void* dp;                          // +0x0
    void* dp3;                         // +0x4
    char unknown_8[0x4cd - 8];
    void* lobby;                       // +0x4cd
};

// 0x102-byte records: a name and a number string.
struct Entry_00443100 {
    char name[0x81];                   // +0x00
    char number[0x81];                 // +0x81
};

// GUI layout entry, as returned by FUN_0049ff90.
struct Layout_00443100 {
    char unknown_0[0xba];
    short player;                      // +0xba
    char unknown_bc[0xce - 0xbc];
    void (__stdcall* callback)(void*, Layout_00443100*);   // +0xce
};

// Object with the layout entry array at +4.
struct Table_00443100 {
    int unknown_0;
    Layout_00443100* entries;           // +0x4
};

// The MODEM.GUI gadget created by FUN_004aa8f0.
struct Gadget_00443100 {
    int unknown_0;
    Layout_00443100* entries;           // +0x4
    void (__stdcall* handler)(void*);   // +0x8
    void* owner;                        // +0xc
};

// The multiplayer menu; the layout entry array hangs off it at +0x18.
struct Gui_00443100 {
    char unknown_0[0x18];
};

struct Game_00443100 {
    char unknown_0[0x14];
    Net_00443100 net;                   // +0x14
    char unknown_4e5[0x519 - 0x4e5];
    Gui_00443100 menu;                  // +0x519
    Table_00443100* table;              // +0x531
};
#pragma pack(pop)

extern Game_00443100* g_game;
extern Guid_00443100 DAT_004fcdc8;
extern char* DAT_00512980;
extern int DAT_00512984;
extern Entry_00443100* DAT_00512988;
extern char* DAT_0051298c;

Gadget_00443100* __stdcall FUN_004aa8f0(void* gui, const char* name, int flags);
void __stdcall FUN_00442a30(void*);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
int __stdcall FUN_004ca490(Net_00443100* net);
int __stdcall FUN_004ca900(Guid_00443100* sp, Net_00443100* net);
int __stdcall FUN_004ca990(Net_00443100* net, unsigned long player, void* data, unsigned long* size);
int __stdcall FUN_004ca8c0(Net_00443100* net, void* callback, void* address, unsigned long size, void* context);
int __stdcall FUN_004ca940(Net_00443100* net);
void* FUN_004d83b0(const char* name, unsigned int size);
void FUN_004d85a0(void* p);
int __stdcall FUN_0042f980(const char* key, void* buf, unsigned int* size);
Layout_00443100* __stdcall FUN_0049ff90(Layout_00443100* entries, char* name);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int count, int flag);
void __stdcall FUN_004a2e40(void* menu, char* name, int index);
void __stdcall FUN_004428f0(void* menu, Layout_00443100* entry);
void __stdcall FUN_004a9660(void* gui);
void __stdcall FUN_00449fb10(void* gui, int value);
void __stdcall FUN_004a81e0(void* gui, int value);
void __stdcall FUN_00425730(char* text);
void __stdcall FUN_00425860(int state, int line, const char* file);
void __stdcall FUN_004257e0(char state, int line, char* file);
int __stdcall FUN_00443070(void* guid, unsigned long size, void* data, void* context);


struct Addr { char* p; };
struct Len { unsigned int v; };
struct Size { unsigned long v; };

// PARTIAL, 94.8% (888 of 888 bytes, exact size; up from 55.4% at first run).
//
// The one change that moved it: the registry-read length had to become a
// one-field struct, `struct Len { unsigned int v; } len;`, used as `len.v`.
// The slots did not move at all, so the gain is not the layout, and the reason
// is not yet understood. Measured at 93.4% without the wrapper and 94.8% with
// it, same six offsets either way.
//
// On the `size`/`addr` slot swap, which the previous attempt named as the
// single blocker. That model is wrong, and the correction matters more than
// the percentage, so it is worth setting out. The original wants
//
//   size=-40  addr=-36  len=-32  gadget=-28  net=-24  iid=-16
//
// and this file produces
//
//   addr=-40  size=-36  len=-32  gadget=-28  net=-24  iid=-16
//
// with only `size` and `addr` transposed. Wrapping BOTH of them in one-field
// structs, `struct Size { unsigned long v; }` and `struct Addr { char* p; }`,
// produces **exactly** the target layout, all six offsets correct. It scores
// **84.6%**, ten points worse. So the slot order is not what is costing the
// six instructions; the shape that gets the slots right costs ten points
// somewhere else, which is the real blocker and it is not the one described
// above.
//
// In the 84.6% variant the cost is visible: `xor ebx,ebx` and the two early
// zero stores of `addr` and `len` (the original has `xor ebx,ebx; push;
// push; mov [esp+0x20],ebx; mov [esp+0x1c],ebx` before the first call, with
// `ebx` holding the zero) move to *after* that call, and the two `jl`/`jne`
// targets shift by four bytes. Adding `int r = 0;` to try to pull the
// `xor ebx,ebx` back to the prologue does not move it (still 84.6%), and
// neither does reverting the `Len` wrapper alongside it.
//
// Full set measured, all with `check.py --sym` after `rm -rf build/obj`:
//   no wrappers                     93.4%   addr=-40 size=-36 len=-32
//   Len only                        94.8%   addr=-40 size=-36 len=-32   <- in the file
//   Size only                       94.8%   addr=-40 size=-36 len=-32
//   Addr only                       84.6%   size=-40 addr=-36 len=-32
//   Size and Addr                   84.6%   size=-40 addr=-36 len=-32   <- exact target layout
//   Size, Addr and r = 0            84.6%   size=-40 addr=-36 len=-32
//
// So: wrap the two out-of-order locals to get the layout, then find whatever
// costs the ten points, which is about where `r` is zeroed rather than what
// the offsets are. Everything else is settled and worth keeping: the
// "frame pointer: yes" flag is a false positive (`sub esp,0x28` plus four
// pushes, `ebp` a general register), every struct needs `#pragma pack(push,1)`
// or `menu` lands at `g_game+0x51c` instead of `+0x519`, the DirectPlay local
// must be the 8-byte `{void* dp; void* dp3;}` rather than the big `Net` type
// (which gives a 0x4f4-byte frame), `g_game->menu` must be a struct member
// rather than `void*` so MSVC emits `mov ecx,[g_game]; add ecx,0x519`, and
// each HRESULT must be assigned to a local before it is compared, since
// `r = f(...); if (r >= 0)` gives the original's `cmp eax,ebx; jl` where
// `if (f(...) >= 0)` gives `test eax,eax; jl`.
//
// FUNCTION: 0x443100
void FUN_00443100()
{
    unsigned long size;
    char* addr = 0;
    Len len;
    int r;
    Gadget_00443100* gadget;
    struct { void* dp; void* dp3; } net;
    Guid_00443100 iid = DAT_004fcdc8;

    gadget = FUN_004aa8f0(&g_game->menu, "MODEM.GUI", 0x800);
    gadget->handler = FUN_00442a30;
    gadget->owner = g_game;
    FUN_004288d0(0, 0, 0, 0);
    FUN_004ca490(&g_game->net);
    r = FUN_004ca900(&iid, (Net_00443100*)&net);
    if (r >= 0) {
        r = FUN_004ca990((Net_00443100*)&net, 0, 0, &size);
        if (r == DPERR_BUFFERTOOSMALL_00443100) {
            addr = (char*)FUN_004d83b0("MODEMADDR", size);
            if (addr != 0) {
                r = FUN_004ca990((Net_00443100*)&net, 0, addr, &size);
                if (r >= 0) {
                    DAT_00512980 = (char*)FUN_004d83b0("MODEMINFO", 0xc8);
                    memset(DAT_00512980, 0, 0xc8);
                    DAT_00512984 = 0;
                    r = FUN_004ca8c0(&g_game->net, (void*)FUN_00443070, addr, size, 0);
                    if (DAT_00512984 == 0) {
                        FUN_004a9660(&g_game->menu);
                        FUN_00425730("Unable to find any modems");
                        FUN_00425860(0xf, 0x4e4, "c:\\cavedog\\wargame\\multi.cpp");
                        FUN_004257e0(0, 0x4e5, "c:\\cavedog\\wargame\\multi.cpp");
                        FUN_004d85a0(DAT_00512980);
                        FUN_004d85a0(addr);
                        FUN_004ca940((Net_00443100*)&net);
                        return;
                    }
                    if (r >= 0) {
                        int i;
                        FUN_004a32a0(&g_game->menu, "MODEMS", DAT_00512980, DAT_00512984, 0);
                        DAT_00512988 = (Entry_00443100*)FUN_004d83b0("MODEMACCOUNTS", 0x1428);
                        len.v = 0x1428;
                        r = FUN_0042f980("MODEMNUMBERS", DAT_00512988, &len.v);
                        if (r == 0) {
                            for (i = 0; i < 20; i++) {
                                strcpy(DAT_00512988[i].name, "UNUSED");
                                DAT_00512988[i].number[0] = 0;
                            }
                        }
                        DAT_0051298c = (char*)FUN_004d83b0("ACCOUNTNAMES", 0xa00);
                        *DAT_0051298c = 0;
                        char* buffer = DAT_0051298c;
                        for (i = 0; i < 20; i++) {
                            strcpy(buffer, DAT_00512988[i].name);
                            buffer += strlen(DAT_00512988[i].name) + 1;
                        }
                        Layout_00443100* entry = FUN_0049ff90(g_game->table->entries, "ACCOUNTS");
                        int player = entry->player;
                        FUN_004a32a0(&g_game->menu, "ACCOUNTS", DAT_0051298c, 20, 0);
                        FUN_004a2e40(&g_game->menu, "ACCOUNTS", player);
                        entry = FUN_0049ff90(gadget->entries, "ACCOUNTS");
                        entry->callback = FUN_004428f0;
                        FUN_004428f0(&g_game->menu, entry);
                    }
                }
            }
        }
    }
    FUN_00449fb10(&g_game->menu, 1);
    FUN_004a81e0(&g_game->menu, 0x40);
    FUN_004ca940((Net_00443100*)&net);
    FUN_004d85a0(addr);
}
