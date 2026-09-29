// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 78.6%: builds the skirmish setup screen's GUI entries. Structure,
// record layouts, strings and control flow are right; the only remaining
// difference is one allocator choice. The original keeps the constant 0 in ebx
// (so the four byte stores are `mov byte ptr [esp+N], bl`) and the wsprintfA
// import in esi, reloading it every loop iteration because the inlined strcpy
// clobbers esi. This copy keeps g_game in ebx, so the wsprintfA import lives in
// ebx across the whole loop and the constant 0 is rematerialised into esi
// (`xor esi,esi` at loop top and mid-loop), which forces immediates for the
// byte stores and a direct `cmp ebp,[ecx+0x38d81]` at the latch. Both use all
// four callee-saved registers, so it is a straight swap of the two roles.
// ebx is the only callee-saved register with a byte sub-register in 32-bit
// x86; the original's zero must therefore hold ebx once it serves byte stores.
//
// Tried and did NOT flip the choice (all scored the same 78.6, byte-identical
// output): reordering y before/after the memsets, flags before the memsets,
// rec2 declared before rec1, `int i;` declared at top, for/while/do-while,
// while(1)+break, separate `int step; step = ...;` assignment, ZeroMemory for
// memset, `NULL` instead of 0, a StoreByte helper, a named `char zero` local,
// a dead `(void)g_game->numPlayers;`. Swapping the two memset calls (75.2) or
// caching numPlayers (58.5) or `unsigned step` (75.1) made it worse. The y
// computation before the memsets is what took it from 72.0 to 78.6.
//
// Retry also tried, all still 78.6 with byte-identical output: an explicit
// `char`/`unsigned char`/`int zero` local used in the byte and word stores,
// `i` and `zero` declared before `step`, records declared before `step`, a
// named `Game* g = g_game` used for the top block (declared first and after
// the records), a local `int n = g_game->numPlayers` for step/y, and
// `static __stdcall` helpers Step_/Y_ taking `g_game`. The memsets moved
// between step and y (v2) removed `push ebx` and flipped g_game to esi but
// shuffled the frame (y at 0x154) and dropped to 56.6. The zero constant
// keeps landing in esi because the anonymous g_game temporary takes ebx in
// the prologue; per the guide, constants only get the byte registers the
// variables leave free, so the fix is a source shape that puts g_game
// somewhere other than ebx.
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
    Holder_00479c50* holder = g_game->menu.holder;
    obj->entry = 0;
    Gaf_004b8d40* gaf = holder->entries->gaf;
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
    int step = 200 / g_game->numPlayers;
    Rec1_00479c50 rec1;
    Rec2_00479c50 rec2;
    int y = (0xb4 - (g_game->numPlayers - 1) * step) / 2 + 0x4f;
    memset(&rec1, 0, sizeof(rec1));
    memset(&rec2, 0, sizeof(rec2));
    rec1.h.flag = 1;
    rec2.h.flag = 1;
    rec2.flags |= 1;
    for (int i = 0; i < g_game->numPlayers; i++) {
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
    }
}
