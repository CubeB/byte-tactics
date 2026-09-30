// Decompiled by deepseek-v4.1-flash, rechecked by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 82.5% (1154 of 1156 bytes). Everything below the entry block has the
// right fields, frame (0x214), memset sizes (0x13e + 0xcc) and call order; the
// whole gap is ONE register swap in the allocator:
//   original: ESI = the g_game temporary, then the cached _imp__wsprintfA
//             reloaded at the loop head; EBX = the live zero (cmp eax,ebx /
//             mov [..],ebx / mov [..],bl);
//   ours:     EBX = the g_game temporary, then the cached wsprintfA import
//             (hoisted out of the loop and never clobbered); ESI = the live
//             zero, re-materialised at the loop head (xor esi,esi) because the
//             inlined strcpy kills ESI. So ours emits `call ebx` where the
//             original reloads `call dword ptr [0x4fc2e8]` after the strcpy
//             clobbers ESI.
// Consequences of the swap, all visible in the checker diff: `mov [..],0` /
// `test eax,eax` in ours where the original uses bl/ebx, `mov esi,0x14` and
// `mov [..],si` where the original uses edi/di, and `mov ebx,[0x511de8]` at
// entry where the original has `mov esi,[0x511de8]`.
// The deciding block is the entry: ours gives the g_game temporary EBX and the
// zero ESI, the original the reverse. See build/scratch/0x479c50/best_diff.txt.
// What raised the score from 78.6% to 82.5%: reading the inlined SetEntry body
// as `Entries* entries = g_game->menu.holder->entries; obj->entry = 0;
// Gaf* gaf = entries->gaf;`, which reproduces the original's interleave of the
// entries load above the obj->entry = 0 store (the same body, phrased with a
// local `holder` first, puts the store before the entries load and scored
// 78.6%). Same for `short* f = FUN_004b7f30(e, obj->frame);` reading the frame
// byte late.
// Further attempts (all scored with check.py --sym, none above 82.5%):
//   headers.py: no header set matches; adding <string>, <vector>, <map>,
//   <iostream>, <stdio.h>, <stdlib.h> alone or together: all 82.5%.
//   N unused extern declarations 0..2000: flat at 82.5%, so the entry swap is
//   source shape, not compiler state. Defining the real neighbour 0x479bf0
//   above ours, or calling it as the shared helper, also stayed at 82.5%.
//   Statement order around step/y/the two memsets (12 permutations): best 82.5%
//   only when do the two memsets come after the y computation; every order with
//   the memsets first folds the numPlayers load into `idiv [reg+0x38d81]` and
//   drops to 56-69%.
//   inline GetGame()/NumPlayers() accessors, a named `n` local, explicit memset
//   sizes, `Rec1 rec1 = {0}` initialisers, non-static/__inline helper, flags
//   reordered, helper taking void*: none flipped the swap.
// The remaining lever is a g_game use that raises its priority above the live
// zero without emitting an extra instruction; not found in this session.
// GPT-6.1-sol refinement: an explicit Game* local spanning the function fell to
// 63.4%; limiting it to the step calculation, reordering the Rec declarations, and
// rewriting the outer for loop as while all returned 82.5%. The baseline source shape
// remains the best verified variant; residual difference is the EBX/ESI allocation
// swap around g_game, wsprintfA, and the zero value described above.
#include <windows.h>
#include <string.h>

struct GafEntry_004b8d40;
struct Gaf_004b8d40;
GafEntry_004b8d40* __stdcall FUN_004b8d40(Gaf_004b8d40* gaf, const char* name);
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004c5740(char* key);
int __stdcall FUN_004ab2b0(void* obj, void* record);
int __stdcall FUN_004ab310(void* obj, void* record);

#pragma pack(push, 1)
struct Header_00479c50 {
    unsigned char type;                // +0x00
    unsigned char group;               // +0x01
    char name[0x11];                   // +0x02
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int attr;                          // +0x1b
    int color;                         // +0x1f
    int color2;                        // +0x23
    char unknown_27[2];                // +0x27
    unsigned char flag;                // +0x29
};

struct Rec1_00479c50 {                 // 0x13e bytes
    Header_00479c50 h;                 // +0x00
    char unknown_2a[0x2f - 0x2a];
    void* entry;                       // +0x2f
    char text[0x103];                  // +0x33
    unsigned char f136;                // +0x136
    char pad_137;
    short f138;                        // +0x138
    char pad_13a;
    unsigned char frame;               // +0x13b
    char pad_13c[2];
};

struct Rec2_00479c50 {                 // 0xcc bytes
    Header_00479c50 h;                 // +0x00
    char unknown_2a[0x33 - 0x2a];
    char text[0x83];                   // +0x33
    short count;                       // +0xb6
    char unknown_b8[0xc8 - 0xb8];
    int flags;                         // +0xc8
};

struct Entries_00479c50 {
    char unknown_0[0xc0];
    Gaf_004b8d40* gaf;                 // +0xc0
};

struct Holder_00479c50 {
    char unknown_0[4];
    Entries_00479c50* entries;         // +0x04
};

struct Menu_00479c50 {
    char unknown_0[0x18];
    Holder_00479c50* holder;           // +0x18
};

struct Game_00479c50 {
    char unknown_0[0x519];
    Menu_00479c50 menu;                // +0x519
    char unknown_535[0x38d81 - 0x535];
    int numPlayers;                    // +0x38d81
};
#pragma pack(pop)

extern Game_00479c50* g_game;

static void __stdcall SetEntry_00479c50(Rec1_00479c50* obj, char* name)
{
    Entries_00479c50* entries = g_game->menu.holder->entries;
    obj->entry = 0;
    Gaf_004b8d40* gaf = entries->gaf;
    if (gaf != 0) {
        GafEntry_004b8d40* e = FUN_004b8d40(gaf, name);
        if (e != 0) {
            obj->entry = e;
            short* f = (short*)FUN_004b7f30((unsigned short*)e, obj->frame);
            if (f != 0) {
                obj->h.w = f[0];
                obj->h.h = f[1];
            }
        }
    }
}

// FUNCTION: 0x479c50
void FUN_00479c50(void)
{
    Rec1_00479c50 rec1;
    Rec2_00479c50 rec2;
    int step = 200 / g_game->numPlayers;
    int y = (0xb4 - (g_game->numPlayers - 1) * step) / 2 + 0x4f;
    memset(&rec1, 0, sizeof(rec1));
    memset(&rec2, 0, sizeof(rec2));
    rec1.h.flag = 1;
    rec2.h.flag = 1;
    rec2.flags |= 1;
    int i = 0;
    while (i < g_game->numPlayers) {
        rec1.h.y = y;
        rec2.h.y = y;
        wsprintfA(rec1.h.name, "Player%d", i);
        rec1.h.x = 0x2d;
        rec1.h.w = 0x70;
        rec1.h.h = 0x14;
        rec1.h.attr = 2;
        rec1.text[0] = 0;
        SetEntry_00479c50(&rec1, "skirmname");
        FUN_004ab2b0(&g_game->menu, &rec1);

        wsprintfA(rec1.h.name, "Side%d", i);
        rec1.h.x = 0xa3;
        rec1.h.w = 0x2d;
        SetEntry_00479c50(&rec1, "SIDEx");
        rec1.frame = 0;
        rec1.f136 = 2;
        FUN_004ab2b0(&g_game->menu, &rec1);

        wsprintfA(rec2.h.name, "Color%d", i);
        rec2.h.x = 0xd6;
        rec2.h.w = 0x14;
        rec2.h.h = 0x14;
        rec2.text[0] = 0;
        FUN_004ab310(&g_game->menu, &rec2);

        wsprintfA(rec2.h.name, "Allies%d", i);
        rec2.h.x = 0xf1;
        rec2.h.w = 0x28;
        rec2.h.h = 0x14;
        strcpy(rec2.text, FUN_004c5740("Click to select an allegiance symbol."));
        FUN_004ab310(&g_game->menu, &rec2);

        wsprintfA(rec1.h.name, "Metal%d", i);
        rec1.h.attr |= 0x10000;
        rec1.h.x = 0x11e;
        rec1.h.w = 0x2d;
        rec1.h.h = 0x14;
        rec1.f136 = 0;
        rec1.f138 = 0;
        SetEntry_00479c50(&rec1, "skirmmet");
        strcpy(rec1.text, FUN_004c5740("Left click to increase metal. Right click to decrease metal."));
        FUN_004ab2b0(&g_game->menu, &rec1);

        wsprintfA(rec1.h.name, "Energy%d", i);
        rec1.h.x = 0x151;
        rec1.h.w = 0x2d;
        SetEntry_00479c50(&rec1, "skirmmet");
        strcpy(rec1.text, FUN_004c5740("Left click to increase energy. Right click to decrease energy."));
        FUN_004ab2b0(&g_game->menu, &rec1);
        y += step;
        ++i;
    }
}
