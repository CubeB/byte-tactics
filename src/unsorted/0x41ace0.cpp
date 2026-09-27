// Decompiled by Claude Opus 5.5. Names are provisional.
// Opens a unit's build menu GUI (guis\<name>.GUI, or the side's "<side>DL"
// menu when that file is missing) with FUN_0041aa00 as its click handler,
// fills the entries registered for this unit type and page, refreshes the
// PREV/NEXT buttons, the counts and the ONOFF button, and selects the unit.
// FUN_0041a920 and FUN_0041ac90, defined just before this function in the
// original file, are inlined here.
//
// PARTIAL (82.6%). The build list loop needs the list pointer read into a
// local inside the inner loop: only then does MSVC keep one induction
// variable for i * 0xbd + j * 0x25 (`mov ebp, esi; add ebp, 0x25`) and the
// frame, registers and spills all line up. What still differs:
// - The four build entry accesses come out as [ecx+ebp+K] instead of
//   [ebp+ecx+K] (base and index swapped). This is compiler state: with no
//   headers (sprintf and strcpy declared by hand) plus 192 to 204 dummy
//   extern declarations in a scratch copy, the whole loop matches. No header
//   set found by tools/headers.py reproduces it; without <windows.h> the
//   strcpy source `lea` is scheduled after the entry address instead.
// - The inlined FUN_0041ac90: the original keeps the table pointer in edi
//   and turns it into the walking pointer with `add edi, 2`; here it lands
//   in eax and gets `lea edi, [eax+2]`. Its out-of-line copy (0x41ac90)
//   also uses eax/lea, so this is register allocation in context. Writing a
//   separate walking pointer gets edi but moves the add before the loop
//   guard, and no dummy-declaration count changes it.

#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Owner_0041ace0 {
    char unknown_0[0x95];
    unsigned char playerIndex;           // +0x95
};

struct Player_0041ace0 {
    char unknown_0[0x27];
    Owner_0041ace0* owner;               // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct UnitType_0041ace0 {
    char unknown_0[0x21e];
    short id;                            // +0x21e
    char unknown_220[0x22e - 0x220];
    unsigned char field_22e;             // +0x22e
};

struct Unit_0041ace0 {
    char unknown_0[0x92];
    UnitType_0041ace0* type;             // +0x92
    Player_0041ace0* player;             // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short field_a6;             // +0xa6
    unsigned short field_a8;             // +0xa8
    char unknown_aa[0x104 - 0xaa];
    float field_104;                     // +0x104
    char unknown_108[0x10e - 0x108];
    unsigned char onOff : 1;             // +0x10e bit 0
    unsigned char bits_10e : 7;
    char unknown_10f;
    unsigned int flags;                  // +0x110
};

struct Entry_0041ace0 {                  // 0x15b bytes
    char unknown_0[2];
    char name[0x26];                     // +0x02
    unsigned char flags;                 // +0x28
    char unknown_29;
    unsigned char field_2a;              // +0x2a
    char unknown_2b[0xb4 - 0x2b];
    unsigned short shown : 1;            // +0xb4 bit 0
    unsigned short bits_b4 : 15;
    char unknown_b6[0x13c - 0xb6];
    unsigned short enabled : 1;          // +0x13c bit 0
    unsigned short bits_13c : 15;
    char unknown_13e[0x15b - 0x13e];
};

struct Table_0041ace0 {
    char unknown_0[0xb6];
    short count;                         // +0xb6
};

struct Layer_0041ace0 {
    int unknown_0;
    Entry_0041ace0* entries;             // +0x04
    void (__stdcall* handler)(void*);    // +0x08
    int field_c;                         // +0x0c
};

struct Menu_0041ace0 {
    char unknown_0[0x18];
    Layer_0041ace0* layer;               // +0x18
};

// A unit type's extra build menu entry (0x25 bytes).
struct BuildEntry_0041ace0 {
    short typeId;                        // +0x00
    unsigned char page;                  // +0x02
    unsigned char slot;                  // +0x03
    char name[0x21];                     // +0x04
};

struct BuildList_0041ace0 {              // 0xbd bytes
    int count;                           // +0x00
    BuildEntry_0041ace0 entries[5];      // +0x04
};

struct Game_0041ace0 {
    char unknown_0[0x519];
    Menu_0041ace0 menu;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_0041ace0 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;           // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short unitIndex;            // +0x37e9c
    unsigned short field_37e9e;          // +0x37e9e
    char unknown_37ea0[0x37f5b - 0x37ea0];
    char sideNames[8][0x232];            // +0x37f5b
    char unknown_390eb[0x391c7 - 0x390eb];
    int buildListCount;                  // +0x391c7
    BuildList_0041ace0* buildLists;      // +0x391cb
};
#pragma pack(pop)

extern Game_0041ace0* g_game;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bbc40(char* path);
Layer_0041ace0* __stdcall FUN_004aa8f0(Menu_0041ace0* menu, const char* name, int flags);
void __stdcall FUN_0041aa00(void* menu);
void __stdcall FUN_004a81e0(Menu_0041ace0* menu, int value);
void __stdcall FUN_004a0570(Menu_0041ace0* menu, char* name, int param_3);
void __stdcall FUN_0041a120(Unit_0041ace0* unit);
void __stdcall FUN_004199b0(Menu_0041ace0* menu, Unit_0041ace0* unit);
int __stdcall FUN_0049fe60(Entry_0041ace0* entries, char* name);
void __stdcall FUN_004a11c0(Menu_0041ace0* menu, int index, int value);
short __stdcall FUN_00488b10(Entry_0041ace0* entry);
void __stdcall FUN_004a1200(Menu_0041ace0* menu, int index, int flag);

// Inlined copy of FUN_0041a920.
static inline void SetPrevNext(Unit_0041ace0* unit)
{
    char buf[256];
    if (unit->type->field_22e < 2) {
        sprintf(buf, "%sPREV", g_game->sideNames[unit->player->owner->playerIndex]);
        FUN_004a0570(&g_game->menu, buf, 0);
        sprintf(buf, "%sNEXT", g_game->sideNames[unit->player->owner->playerIndex]);
        FUN_004a0570(&g_game->menu, buf, 0);
    }
}

static inline Entry_0041ace0* Entries(Table_0041ace0* t)
{
    return (Entry_0041ace0*)((char*)t + 2);
}

// Inlined copy of FUN_0041ac90.
static inline void UpdateCounts(Menu_0041ace0* menu)
{
    Table_0041ace0* t = (Table_0041ace0*)menu->layer->entries;
    int n = t->count;
    for (int i = 0; i < n; i++) {
        if (Entries(t)[i].flags & 4) {
            FUN_004a1200(menu, i, FUN_00488b10(&Entries(t)[i]) == 0);
        }
    }
}

// FUNCTION: 0x41ace0
void __stdcall FUN_0041ace0(Unit_0041ace0* unit, char* guiName, int page)
{
    if (unit->field_104 == 0.0f) {
        Player_0041ace0* player = &g_game->players[g_game->localPlayer];
        int found = 0;
        char path[256];
        char name[256];
        FUN_004290f0(path, "guis", guiName, "GUI");
        if (FUN_004bbc40(path) == 0)
            sprintf(name, "%sDL", g_game->sideNames[player->owner->playerIndex]);
        else
            strcpy(name, guiName);
        Layer_0041ace0* layer = FUN_004aa8f0(&g_game->menu, name, 0);
        if (layer != 0) {
            layer->handler = FUN_0041aa00;
            layer->field_c = 0;
            for (int i = 0; i < g_game->buildListCount; i++) {
                for (int j = 0; j < g_game->buildLists[i].count; j++) {
                    BuildList_0041ace0* lists = g_game->buildLists;
                    if (unit->type->id == lists[i].entries[j].typeId
                        && lists[i].entries[j].page - 1 == page) {
                        char* src = lists[i].entries[j].name;
                        Entry_0041ace0* e = &layer->entries[lists[i].entries[j].slot + 4];
                        e->enabled = 0;
                        e->field_2a = 4;
                        strcpy(e->name, src);
                        found = 1;
                        e->shown = 1;
                    }
                }
            }
            if (found) {
                FUN_004a81e0(&g_game->menu, 2);
                FUN_004a81e0(&g_game->menu, 1);
            }
            SetPrevNext(unit);
            FUN_0041a120(unit);
            FUN_004199b0(&g_game->menu, unit);
            if (unit->flags & 0x20000000) {
                int index = FUN_0049fe60(g_game->menu.layer->entries, "ONOFF");
                if (index != -1)
                    FUN_004a11c0(&g_game->menu, index, unit->onOff);
            }
            if (page != 0)
                UpdateCounts(&g_game->menu);
            FUN_004a81e0(&g_game->menu, 0x40);
            g_game->unitIndex = unit->field_a8;
            g_game->field_37e9e = unit->field_a6;
        }
    }
}
