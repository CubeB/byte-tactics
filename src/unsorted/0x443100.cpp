// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash and
// Space Bunny Free. Names are provisional.
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


struct Len { unsigned int v; };

// PARTIAL, 99.6% (888 of 888 bytes, exact size; was 94.8%).
//
// The single remaining difference is one instruction order in the second
// FUN_0049ff90 call. The original is
//
//   push "ACCOUNTS"; mov ecx,[esp+0x20]; mov edx,[ecx+4]; push edx; call
//
// (the reload of `gadget` happens after the string push, so it uses the
// post-push offset), and this file is
//
//   mov ecx,[esp+0x1c]; push "ACCOUNTS"; mov edx,[ecx+4]; push edx; call
//
// Same two instructions, same total size, one slot earlier. The shape is
// unique in the whole exe: a byte scan for `push imm32; mov ecx,[esp+d8];
// mov edx,[ecx+4]; push edx` finds exactly this one site, and every source
// spelling tried that reaches the same two loads (a local `entries`, an
// `__inline` accessor, a member getter, comma/assignment expressions, casts,
// a nested block, and a separate loop pointer) still emits the reload before
// the push. A minimal `f(g->e, "ACCOUNTS")` with a stack-resident g shows the
// compiler always loads the base first, so the original's order looks like an
// allocator/scheduler coin-flip that this source cannot steer.
//
// A second pass (Space Bunny Free, #1060) scored about 95 further shapes and
// none moved it off 99.6%. The score is bimodal with a hard ceiling at 99.6
// for this source, so the family is not close to the boundary and sampling
// more of it is wasted budget. What was tried, all 99.6% unless noted:
//   * argument expression: plain, Table* cast, char* arithmetic cast, a
//     conditional, a comma expression (96.0%), an __inline accessor, an
//     accessor taking Gadget* by value, one taking Gadget**, one called twice
//   * the name argument: a literal, a macro, a static const array (needs a
//     const char* callee parameter), a non-static global char[9], a foldable
//     local `char* nm`, a helper returning the literal, one taking an unused
//     int
//   * `gadget` storage: plain, `const` (won't compile uninitialised), a one
//     field struct, a one field union, a struct with an inline method, a class
//     with operator=, a file-static, a struct-by-value inline parameter
//   * address-taken without code: `(void)&gadget`, `sizeof(&gadget)`,
//     `&gadget == &gadget`, `(&gadget)[0]`, a `void*` alias array (93.6%), a
//     pointer-to-pointer local. All leave the load hoistable, which is the
//     point: the reload is not pinned by aliasing, it is a plain frame read
//   * declaration order of all six locals (every size-preserving permutation
//     of addr/size/len/r/gadget/net), plus adding unused locals of each type
//   * local types: `addr` as void*, `size` as int, `r` as unsigned or long,
//     `len` as a plain unsigned int instead of `struct Len`
//   * call shape: the tail as one inline void helper, as a helper that also
//     stores the callback, an `if (acc != 0)` wrapper (97.3%), do/while(0),
//     a `for(;;)` body, a separate result variable, a block around the last
//     statement, the callback store before the final call (96.7%), an early
//     `return` (79.8%)
//   * `player` as short (89.7%), unsigned short (97.1%), moved into the call,
//     or declared at the top of the function
//   * handler/owner stores swapped (98.8%), or done in an inline helper, or in
//     a helper that also builds the gadget
//   * /Ob2 budget: adding a trivial inline call, and adding a large inline
//     candidate the optimiser rejects
//   * headers.py over all 128 sets and 768 with --cpp: all 99.6%
//
// Reading it as a scheduler tie-break matches the guide's note on 0xb6570
// (a reload that drifts around a call's argument pushes, with the
// displacement shifting by exactly 4 per push). The only structural fact
// that has moved the score on this function before was the declaration order
// and initialisation of the six locals, and all of those are already right.
//
// What the previous attempt had wrong, and what fixed the six offsets and the
// two missing prologue stores (all at once):
//
//   * `size` must be initialised: `unsigned long size = 0;`, declared AFTER
//     `addr`. Both are then zeroed in the prologue (`addr` at E-0x24 first,
//     then `size` at E-0x28), which is the original's order, and the two slot
//     assignments come out right (initialised locals take slots in declaration
//     order; the previous `size`/`addr` transposition was only the missing
//     initialiser, not a layout problem). This alone was 94.8% -> 96.5%.
//   * The `ACCOUNTNAMES` buffer needs TWO pointers: `buffer` for the stores,
//     and a separate `p` that runs the copy loop. Written with one pointer the
//     loop pointer is live from the call and goes straight into ebp; with the
//     second pointer the compiler keeps eax for `DAT_0051298c` and `*buffer`
//     and only moves to ebp at loop entry, which is what the original does.
//     96.5% -> 99.6%.
//
// Settled and kept: "frame pointer: yes" is a false positive (sub esp,0x28
// plus four pushes, ebp is a general register), every struct needs
// `#pragma pack(push,1)` or `menu` lands at g_game+0x51c, the DirectPlay local
// must be the 8-byte `{void* dp; void* dp3;}` rather than the big `Net` type,
// `g_game->menu` must be a struct member so MSVC emits `mov ecx,[g_game]; add
// ecx,0x519`, and each HRESULT must be assigned to a local before it is
// compared (`r = f(...); if (r >= 0)` gives `cmp eax,ebx; jl` where
// `if (f(...) >= 0)` gives `test eax,eax; jl`).
//
// FUNCTION: 0x443100
void FUN_00443100()
{
    char* addr = 0;
    unsigned long size = 0;
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
                        char* buffer = (char*)FUN_004d83b0("ACCOUNTNAMES", 0xa00);
                        DAT_0051298c = buffer;
                        *buffer = 0;
                        char* p = buffer;
                        for (i = 0; i < 20; i++) {
                            strcpy(p, DAT_00512988[i].name);
                            p += strlen(DAT_00512988[i].name) + 1;
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
