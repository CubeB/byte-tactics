// Decompiled by deepseek-v4.1-flash, rechecked by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Skirmish setup screen builder: for each player it fills two menu-object
// templates (rec1 = the name/side/colour/resource buttons, rec2 = the colour
// and allegiance buttons) and registers them with the menu.
//
// The one non-obvious detail is the explicit `rec1.entry = 0;` in the metal
// block, which the original really does (it stores entry twice: once here and
// once inside the inlined SetEntry). It is not redundant for matching: that
// extra use of the live zero is the allocator lever that puts the zero in EBX
// (the only callee-saved byte-addressable register, so the byte stores become
// `mov [..], bl` and the wsprintfA import lands in ESI and is reloaded after the
// inlined strcpy clobbers it). Without it the zero lands in ESI and the whole
// function is one register swap away (99.7%).
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
        rec1.h.x = 0x11e;
        rec1.h.w = 0x2d;
        rec1.h.h = 0x14;
        rec1.h.attr |= 0x10000;
        rec1.f136 = 0;
        rec1.f138 = 0;
        rec1.entry = 0;
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
