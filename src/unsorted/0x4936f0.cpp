// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Opens the resource sharing screen (SHARE.GUI): it walks the ten player
// records, finds the local player's entry (flagged 0x40), points the METAL and
// ENERGY sliders at the local counts, and finishes with the menu setup calls.
// A resource's owner is flagged 0x40 in the player record; the two sliders are
// the "METAL#" and "ENERGY#" texts.
// PARTIAL, 97.1% (993 of 994 bytes; up from 94.2%). Two fixes, both real.
//
// 1. The loop over the player table must be a real indexed walk, not a manual
//    byte-offset one. The previous file wrote
//        for (int off = 0; off < 0xcee; off += 0x14b)
//            (Player*)((char*)g_game + off + 0x1b63)
//    which gives `mov eax, [ecx + esi + 0x1b63]`, while the original has
//    `mov eax, [esi + ecx + 0x1b63]`. Rewritten as
//        for (int i = 0; i < 10; i++) { Player_004936f0* p = &g_game->players[i]; ... }
//    with a struct-typed index, MSVC 5 strength-reduces the induction variable
//    itself and the SIB base and index come out in the original's order. This
//    made the whole loop byte identical. Note the direction: the manual form put
//    ecx (the base) first, and the indexed form puts esi first, so the fix is
//    not "swap the operands" but "stop computing the address by hand".
//
// 2. `menu` must be computed BEFORE the FUN_004a0200 call, not after. The
//    previous file assigned `Menu_004936f0* menu = &g_game->menu;` after the
//    call, which made the compiler reload `g_game` and cost 12 bytes. Moving
//    the assignment ahead of the call and giving the layer a local that is read
//    on both sides of it schedules the layer load before the `lea esi,
//    [eax+0x519]` and reproduces the original's ordering.
//
// A NOTE ON WHY THE SELF-COMPARISON IS NEEDED, measured again on this run.
// Deleting it (`FUN_004a0200(ents, "METAL")`, and also the fully inline
// `FUN_004a0200(lyr->entries, "METAL")`) makes the scheduler move the
// `lea esi, [eax + 0x519]` in front of the layer load, so the emitted order
// becomes lea / layer / entries where the original has layer / lea / entries.
// With the self-comparison the layer load is pinned to the argument
// evaluation and the order matches. It does NOT change the register choice:
// both forms still coalesce the entries load onto the dead layer register.
//
// Measured again on this run and no better: routing the two expressions
// through `__inline` accessors `layer_of(menu)` and `ents_of(lyr)` (97.1%,
// byte for byte the same tail), and computing `menu` before `lyr` in each
// block (96.7%, worse). The order layer / menu / entries is forced.
//
// A NOTE ON THE SELF-COMPARISON IN THE BODY, because it looks like a mistake.
// Both calls are written
//        e = FUN_004a0200(ents, ents == lyr->entries ? "METAL" : "METAL");
// and that tautology is deliberate. It emits no instructions and is not a
// runtime check; its only effect is to make the compiler read `lyr->entries`
// a second time, which pins the layer pointer live across the `lea` and
// changes the register assignment. It is a codegen device, not logic, and it
// should not be "simplified" to a plain "METAL" without re-running the check:
// the plain form is measurably worse. The same device is what made the layer
// load land before the `lea` in the first place.
//
// WHAT IS LEFT IS ONE PROBLEM, NOT THREE, and the 993/994 size gap proves it.
// An instruction-by-instruction size comparison (see
// build/scratch/0x4936f0/offs.py) shows that every instruction of the two
// tail blocks has the same length on both sides, with ONE exception, and it is
// a register allocation effect rather than a code shape effect:
//   0x493a43  original  mov edx, dword ptr [0x511de8]   6 bytes  (8B 15)
//   0x493a43  ours      mov eax, dword ptr [0x511de8]   5 bytes  (A1)
// A load from an absolute address is 5 bytes in the accumulator form (A1) and
// 6 bytes in the general form (8B /r), so the whole 1 byte this file is short
// is the price of putting g_game in eax instead of edx in the second block.
// Both remaining blocks therefore fail for the same reason, and the fix has to
// be one change that moves the layer, the menu, the entries and g_game into
// the original's four registers at once.
//
// NEW FINDING (deepseek-v4.1-flash), measured on build/scratch/0x4936f0/v3.cpp:
// the two tail blocks are NOT the same shape in the original. Writing block 2
// with `menu` computed first and the layer read through it,
//        menu = &g_game->menu;
//        lyr = menu->layer;
//        ents = lyr->entries;
//        e = FUN_004a0200(ents, ents == lyr->entries ? "ENERGY" : "ENERGY");
// makes block 2 emit the original's exact prologue
//        mov edx, dword ptr [0x511de8]   (g_game in edx, 6 bytes)
//        mov eax, dword ptr [edx + 0x531] (layer in eax)
//        lea esi, [edx + 0x519]           (menu in esi)
// so the 994-byte count is reached. Block 1 must keep the `lyr =
// g_game->menu.layer` shape (g_game in eax, layer in ecx). The only thing left
// in each block is that the entries load must NOT coalesce onto the layer
// register: original block 1 wants `mov edx, [ecx+4]` and block 2 wants
// `mov ecx, [eax+4]`, while every shape tried here emits the base register.
// With v3 (994 bytes) the score is 94.9% because the register deltas cascade
// into the calls after each block; the block-1-uniform base stays at 97.1%.
// Tried and no better for the entries register: second layer pointer (`lyr2`),
// inline `lyr->entries` as the argument, reusing the top-level layer/entries.
//
// The three instructions are, with identical mnemonics and sizes:
//   - `mov edx, [ecx + 4]` in block 1 where this file has `mov ecx, [ecx + 4]`
//   - `mov edx, dword ptr [g_game]` in block 2 against `mov eax, ...`
//   - and the resulting branch targets, off by one because this file is 993
//     bytes to the original's 994.
// The original keeps four distinct registers live across each block (eax =
// g_game, ecx = layer, esi = menu, edx = entries); this file reuses ecx for
// the entries load because its `lyr` local dies immediately after
// `lyr->entries`. So the missing ingredient is keeping the layer pointer live,
// and liveness alone is NOT the lever: a `char** ep = &lyr->entries` indirection
// does keep the layer live and still scores 97.1% with the same three
// instructions, as do a `(void)lyr;` at the end and a second read after the
// call. Roughly 60 shapes were measured: layer as a local or inline, entries as
// a local or inline, `menu` assigned per block or once, both blocks scoped
// separately, and eight rect-store orderings, all 97.1% or close. A
// `static __inline` helper, a by-value struct copy, and a `g_game` local all
// scored worse, at 88.5% to 89.4%, so the current inline shape is right.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004936f0 {                  // 0x15b bytes
    char unknown_0[0x17];
    short field_17;                      // +0x17
    short field_19;                      // +0x19
    char unknown_1b[0x136 - 0x1b];
    short field_136;                     // +0x136
    char unknown_138[0x13c - 0x138];
    int field_13c;                       // +0x13c
    short field_140;                     // +0x140
    short field_142;                     // +0x142
    void (__stdcall* handler)(void*, int); // +0x144
    char unknown_148[0x14a - 0x148];
    void* field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Layer_004936f0 {
    int unknown_0;
    char* entries;                       // +0x04
    void (__stdcall* handler)(void*);    // +0x08
    void* data;                          // +0x0c
    char unknown_10[0x24 - 0x10];
};

struct Menu_004936f0 {
    char unknown_0[0x18];
    Layer_004936f0* layer;               // +0x18
};

struct Owner_004936f0 {
    char unknown_0[0x9b];
    unsigned short pad : 6;              // +0x9b
    unsigned short bit6 : 1;
    unsigned short rest : 9;
};

struct Player_004936f0 {                 // 0x14b bytes
    int active;                          // +0x00
    int field_4;                         // +0x04
    char unknown_8[0x27 - 0x8];
    Owner_004936f0* owner;               // +0x27
    char name[0x73 - 0x2b];              // +0x2b
    unsigned char state;                 // +0x73
    char unknown_74[0x8c - 0x74];
    float metal;                         // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                        // +0x98
    char unknown_9c[0x140 - 0x9c];
    int field_140;                       // +0x140
    unsigned short field_144;            // +0x144
    unsigned char field_146;             // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_004936f0 {
    char unknown_0[0x519];
    Menu_004936f0 menu;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_004936f0 players[10];         // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;           // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;           // +0x2a42
    char unknown_2a43[0x37ebe - 0x2a43];
    unsigned short pad_37ebe : 6;        // +0x37ebe
    unsigned short bit6_37ebe : 1;
    unsigned short rest_37ebe : 9;
};
#pragma pack(pop)

extern Game_004936f0* g_game;
extern int DAT_0051e6d0[10];

Layer_004936f0* __stdcall FUN_004aa8f0(Menu_004936f0* menu, const char* name, int flags);
void __stdcall FUN_004934b0(void* gadget);
int __stdcall FUN_0049fdf0(char* entries, char* name, int type);
Entry_004936f0* __stdcall FUN_004a0200(char* entries, char* name);
void __stdcall FUN_00493340(void* entry, int param_2);
void __stdcall FUN_00493390(void* entry, int param_2);
void __stdcall FUN_0045b9b0(void* entry, int param_2);
void* FUN_004d83b0(char* name, unsigned int size);
void __stdcall FUN_004a9660(Menu_004936f0* menu);
void __stdcall FUN_004a32a0(Menu_004936f0* menu, char* name, char* text, int count, int flag);
int __stdcall FUN_0045ba20(Entry_004936f0* entry);
void __stdcall FUN_004a0bf0(Menu_004936f0* menu, char* name, char* text, int param_4);
void __stdcall FUN_0049fa90(Menu_004936f0* menu);
void __stdcall FUN_0049fb10(Menu_004936f0* menu, int value);
void __stdcall FUN_004a81e0(Menu_004936f0* menu, int value);

// FUNCTION: 0x4936f0
void FUN_004936f0()
{
    if (g_game->players[g_game->localPlayer].owner->bit6)
        return;
    Layer_004936f0* layer = FUN_004aa8f0(&g_game->menu, "SHARE.GUI", 0x800);
    g_game->bit6_37ebe = 1;
    char* entries = layer->entries;
    layer->handler = FUN_004934b0;
    layer->data = g_game;
    int idx = FUN_0049fdf0(entries, "METAL", 0xe);
    if (idx != -1) {
        Entry_004936f0* e = (Entry_004936f0*)(entries + idx * 0x15b);
        e->field_142 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_19;
        e->field_136 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].energy;
        e->handler = FUN_00493340;
        e->field_140 = 0;
        FUN_0045b9b0(e, 0);
        e->field_14a = g_game;
    }
    idx = FUN_0049fdf0(layer->entries, "ENERGY", 0xe);
    if (idx != -1) {
        Entry_004936f0* e = FUN_004a0200(layer->entries, "ENERGY");
        e->field_142 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_19;
        e->field_136 = ((Entry_004936f0*)(layer->entries + idx * 0x15b))->field_17 - e->field_142;
        e->field_13c = (int)g_game->players[g_game->localPlayer].metal;
        e->handler = FUN_00493390;
        e->field_140 = 0;
        FUN_0045b9b0(e, 0);
        e->field_14a = g_game;
    }

    char* names = (char*)FUN_004d83b0("PLAYERS", g_game->field_2a3c * 30);
    char* np = names;
    *np = 0;
    memset(DAT_0051e6d0, -1, sizeof(DAT_0051e6d0));
    int* ids = DAT_0051e6d0;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_004936f0* p = &g_game->players[i];
        if (p->active && (p->state == 1 || p->state == 2 || p->state == 3) && p->field_146 != 10 &&
            (p->field_144 != 0 || p->field_140 == 0) && p->state != 1 && !p->owner->bit6) {
            strcpy(np, p->name);
            np += strlen(p->name) + 1;
            *ids = p->field_4;
            count++;
            ids++;
        }
    }
    if (count == 0) {
        FUN_004a9660(&g_game->menu);
        return;
    }
    FUN_004a32a0(&g_game->menu, "PLYRLIST", names, count, 0);
    char text[0x34];
    Layer_004936f0* lyr = g_game->menu.layer;
    char* ents = lyr->entries;
    Menu_004936f0* menu = &g_game->menu;
    Entry_004936f0* e = FUN_004a0200(ents, ents == lyr->entries ? "METAL" : "METAL");
    if (e) {
        sprintf(text, "%d", FUN_0045ba20(e));
        FUN_004a0bf0(menu, "METAL#", text, 0);
    }
    lyr = g_game->menu.layer;
    ents = lyr->entries;
    menu = &g_game->menu;
    e = FUN_004a0200(ents, ents == lyr->entries ? "ENERGY" : "ENERGY");
    if (e) {
        sprintf(text, "%d", FUN_0045ba20(e));
        FUN_004a0bf0(menu, "ENERGY#", text, 0);
    }
    FUN_0049fa90(&g_game->menu);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_004a81e0(&g_game->menu, 0x40);
}
