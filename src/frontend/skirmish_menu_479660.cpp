// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, GPT-6.1-sol, GPT-6-Luna, mimo-v2.6-pro, longcat-2.5-preview-free, Space Bunny Free, Claude Opus 5.5, Opus, Claude Sonnet 5.5, Sonnet, Haiku, DeepSeek V4.1 Flash, space-bunny-free and opus. Names are provisional.
//
// The skirmish setup screen: the per-player rows (name, side, allies, colour,
// metal, energy), the game-option buttons, the SELMAP.GUI map selector, the
// click and cheat-text handlers, and the "forces destroyed" announcement.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct GafEntry;
struct Gaf;
struct Menu;
struct Layer;
struct Cheat;
class Mission;

#pragma pack(push, 1)

struct Backdrop {
    char unknown_0[0xc0];
    Gaf* gaf;                          // +0xc0
};

struct Gadget {                        // GUI entry, 0x15b bytes
    char unknown_0[0x33];
    char text[0xb6 - 0x33];            // +0x33
    short field_b6;                    // +0xb6
    char unknown_b8[2];
    short selected;                    // +0xba
    char unknown_bc[2];
    void* field_be;                    // +0xbe
    char* items;                       // +0xc2
    unsigned short field_c6;           // +0xc6
    char unknown_c8[6];
    void (__stdcall* onSelect)(Menu*, int);   // +0xce
    char unknown_d2[0x137 - 0xd2];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Menu {
    char unknown_0[0x18];
    Layer* holder;                     // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

struct Data {                          // "SELECT MAP DATA", 0x20 bytes
    char unknown_0[0x14];
    char* items;                       // +0x14
};

struct Layer {
    int unknown_0;
    Gadget* entries;                   // +0x04
    void (__stdcall* handler)(Menu*);  // +0x08
    Data* data;                        // +0x0c
    char unknown_10[0x37 - 0x10];
    int clickMode;                     // +0x37
    void (__stdcall* textHandler)(Cheat*);   // +0x3b
};

struct Player {                        // 0x18 bytes
    int active;                        // +0x00
    int shade;                         // +0x04
    int team;                          // +0x08
    int metal;                         // +0x0c
    int energy;                        // +0x10
    int color;                         // +0x14
};

struct Table {
    Player players[11];                // +0x00 .. +0x108
    int field_108;                     // +0x108
    int field_10c;                     // +0x10c
    int field_110;                     // +0x110
    int field_114;                     // +0x114
    int field_118;                     // +0x118
    char mapName[0x220 - 0x11c];       // +0x11c
    int field_220;                     // +0x220
    int current;                       // +0x224
    int field_228;                     // +0x228
};

struct Unit {
    char unknown_0[0x95];
    unsigned char isCore;              // +0x95
    unsigned char slot;                // +0x96
};

struct Slot {                          // 0x14b bytes
    char unknown_0[0x27];
    Unit* unit;                        // +0x27
    char unknown_2b[0x108 - 0x2b];
    unsigned char marks[0x146 - 0x108];   // +0x108
    unsigned char color;               // +0x146
    char unknown_147[4];
};

struct Game {
    char unknown_0[0x519];
    Menu menu;                         // +0x519
    char unknown_57d[0x1b63 - 0x57d];
    Slot slots[10];                    // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Table* table;                      // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x2bc0 - 0x2a44];
    char frontendSubstateRequest;      // +0x2bc0
    char unknown_2bc1[0x148db - 0x2bc1];
    unsigned short* colorCount;        // +0x148db
    char unknown_148df[0x37eee - 0x148df];
    int difficulty;                    // +0x37eee
    char unknown_37ef2[0x37f39 - 0x37ef2];
    int sideCount;                     // +0x37f39
    char unknown_37f3d[0x38d81 - 0x37f3d];
    int playerCount;                   // +0x38d81
    char unknown_38d85[0x391e9 - 0x38d85];
    Mission* mission;                  // +0x391e9
};

#pragma pack(pop)

extern Game* g_game;

int __stdcall FindGadgetIndex(Gadget* entries, const char* name, int flag);
void __stdcall MarkChanged(Menu* menu);

// FUNCTION: 0x479660
void RefreshAllyIcons(void)
{
    int i = 0;
    Gadget* entries = g_game->menu.holder->entries;
    for (; i < g_game->playerCount; i++) {
        char name[64];
        wsprintfA(name, "Allies%d", i);
        int index = FindGadgetIndex(entries, name, 6);
        if (index != -1) {
            Gadget* gadget = &entries[index];
            if (gadget != 0) {
                int team = g_game->table->players[i].team;
                int count = 0;
                int j;
                for (j = 0; j < g_game->playerCount; j++) {
                    if (g_game->table->players[j].team == team &&
                        g_game->table->players[j].active != 0) {
                        count++;
                    }
                }
                switch (count) {
                case 0:
                    gadget->field_c6 = 10;
                    break;
                case 1:
                    gadget->field_c6 = team * 2 + 1;
                    break;
                default:
                    gadget->field_c6 = team * 2;
                    break;
                }
            }
        }
    }
    MarkChanged(&g_game->menu);
}

// Returns 1 when every active player (other than type 5) has the same type
// and at least one such player exists, else 0.
// The original calls this out of line from HandleSkirmishClick.
#pragma auto_inline(off)
// FUNCTION: 0x479760
int AreAllPlayersInOneAllyGroup(void)
{
    int type;
    int i;
    for (i = 0; i < g_game->playerCount; i++) {
        if (g_game->table->players[i].active != 0 && (type = g_game->table->players[i].team) != 5)
            break;
    }
    if (i == g_game->playerCount)
        return 0;
    for (i = 0; i < g_game->playerCount; i++) {
        if (g_game->table->players[i].team != type && g_game->table->players[i].active != 0)
            return 0;
    }
    return 1;
}
#pragma auto_inline(on)

void __stdcall SetGadgetActiveByName(Menu* menu, char* name, int value);
void __stdcall SetTranslatedTextByName(Menu* menu, char* key, char* text, int flag);
char* __stdcall Translate(char* key);

// Copies of the campaign_menu functions at 0x479500, 0x479590 and 0x4795e0:
// the original inlines them here, so they are defined in this unit.
// 0x479500: the number of players with controller 1.
int __cdecl CountHumanSlots()
{
    int n = 0;
    for (int i = 0; i < g_game->playerCount; i++) {
        if (g_game->table->players[i].active == 1)
            n++;
    }
    return n;
}

// 0x479590: whether another active player uses this colour.
int __stdcall IsColorTaken(int color, int skip)
{
    for (int i = 0; i < g_game->playerCount; i++) {
        if (g_game->table->players[i].color == color && g_game->table->players[i].active != 0 && i != skip) {
            return 1;
        }
    }
    return 0;
}

// 0x4795e0: the first colour no player uses.
int FindFreeColor()
{
    int color = 0;
    // do/while, not for: a for loop moves the return block to the function end.
    do {
        int i;
        for (i = 0; i < g_game->playerCount; i++) {
            if (g_game->table->players[i].color == color)
                break;
        }
        if (i == g_game->playerCount)
            return color;
        color++;
    } while (color < 10);
    return -1;
}

// Gives the player the first free colour and updates the "Color<n>" entry.
static void NewColour(int playerIndex)
{
    // Separate helper with its own name buffer: the players array is re-read.
    char name[64];
    Gadget* entries = g_game->menu.holder->entries;
    g_game->table->players[playerIndex].color = FindFreeColor();
    wsprintfA(name, "Color%d", playerIndex);
    int index = FindGadgetIndex(entries, name, 6);
    if (index != -1) {
        Gadget* e = &entries[index];
        if (e != 0) {
            e->field_be = g_game->colorCount;
            e->field_c6 = (unsigned short)g_game->table->players[playerIndex].color;
        }
    }
}

// Cycles a player slot's controller (open, computer, player) in the setup
// screen and refreshes that row's menu entries; a newly occupied slot whose
// colour clashes with another active player gets the first free colour.
// FUNCTION: 0x4797e0
void __stdcall CycleSlotController(int playerIndex)
{
    char buffer[64];

    wsprintfA(buffer, "Player%d", playerIndex);
    switch (g_game->table->players[playerIndex].active) {
    case 0:
        g_game->table->players[playerIndex].active = 2;
        SetTranslatedTextByName(&g_game->menu, buffer, Translate("Computer"), 0);
        break;
    case 1:
        g_game->table->players[playerIndex].active = 0;
        SetTranslatedTextByName(&g_game->menu, buffer, Translate("Open"), 0);
        break;
    case 2:
        if (CountHumanSlots() == 0) {
            g_game->table->players[playerIndex].active = 1;
            SetTranslatedTextByName(&g_game->menu, buffer, Translate("Player"), 0);
        } else {
            g_game->table->players[playerIndex].active = 0;
            SetTranslatedTextByName(&g_game->menu, buffer, Translate("Open"), 0);
        }
        break;
    }

    if (g_game->table->players[playerIndex].active == 0) {
        wsprintfA(buffer, "Player%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Side%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Allies%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Metal%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Energy%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Color%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 0);
    } else {
        if (IsColorTaken(g_game->table->players[playerIndex].color, playerIndex))
            NewColour(playerIndex);
        wsprintfA(buffer, "Player%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Side%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Allies%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Metal%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Energy%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Color%d", playerIndex);
        SetGadgetActiveByName(&g_game->menu, buffer, 1);
    }
    RefreshAllyIcons();
}

#pragma pack(push, 1)
struct Header {
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

struct Rec1 {                          // 0x13e bytes
    Header h;                          // +0x00
    char unknown_2a[0x2f - 0x2a];
    GafEntry* entry;                   // +0x2f
    char text[0x103];                  // +0x33
    unsigned char f136;                // +0x136
    char pad_137;
    short f138;                        // +0x138
    char pad_13a;
    unsigned char frame;               // +0x13b
    char pad_13c[2];
};

struct Rec2 {                          // 0xcc bytes
    Header h;                          // +0x00
    char unknown_2a[0x33 - 0x2a];
    char text[0x83];                   // +0x33
    short count;                       // +0xb6
    char unknown_b8[0xc8 - 0xb8];
    int flags;                         // +0xc8
};
#pragma pack(pop)

GafEntry* __stdcall FindGafEntry(Gaf* gaf, const char* name);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);

// FUNCTION: 0x479bf0
void __stdcall BindGadgetAnimSequence(Rec1* obj, char* name)
{
    Backdrop* h = (Backdrop*)g_game->menu.holder->entries;
    obj->entry = 0;
    if (h->gaf) {
        GafEntry* e = FindGafEntry(h->gaf, name);
        if (e) {
            obj->entry = e;
            short* f = (short*)GetGafFrame((unsigned short*)e, obj->frame);
            if (f) {
                obj->h.w = f[0];
                obj->h.h = f[1];
            }
        }
    }
}

int __stdcall AddButtonGadget(void* obj, void* record);
int __stdcall AddHotspotGadget(void* obj, void* record);

// Skirmish setup screen builder: for each player it fills two menu-object
// templates (rec1 = the name/side/colour/resource buttons, rec2 = the colour
// and allegiance buttons) and registers them with the menu.
// FUNCTION: 0x479c50
void BuildSkirmishPlayerRows(void)
{
    Rec1 rec1;
    Rec2 rec2;
    int step = 200 / g_game->playerCount;
    int y = (0xb4 - (g_game->playerCount - 1) * step) / 2 + 0x4f;
    memset(&rec1, 0, sizeof(rec1));
    memset(&rec2, 0, sizeof(rec2));
    rec1.h.flag = 1;
    rec2.h.flag = 1;
    rec2.flags |= 1;
    int i = 0;
    while (i < g_game->playerCount) {
        rec1.h.y = y;
        rec2.h.y = y;
        wsprintfA(rec1.h.name, "Player%d", i);
        rec1.h.x = 0x2d;
        rec1.h.w = 0x70;
        rec1.h.h = 0x14;
        rec1.h.attr = 2;
        rec1.text[0] = 0;
        BindGadgetAnimSequence(&rec1, "skirmname");
        AddButtonGadget(&g_game->menu, &rec1);

        wsprintfA(rec1.h.name, "Side%d", i);
        rec1.h.x = 0xa3;
        rec1.h.w = 0x2d;
        BindGadgetAnimSequence(&rec1, "SIDEx");
        rec1.frame = 0;
        rec1.f136 = 2;
        AddButtonGadget(&g_game->menu, &rec1);

        wsprintfA(rec2.h.name, "Color%d", i);
        rec2.h.x = 0xd6;
        rec2.h.w = 0x14;
        rec2.h.h = 0x14;
        rec2.text[0] = 0;
        AddHotspotGadget(&g_game->menu, &rec2);

        wsprintfA(rec2.h.name, "Allies%d", i);
        rec2.h.x = 0xf1;
        rec2.h.w = 0x28;
        rec2.h.h = 0x14;
        strcpy(rec2.text, Translate("Click to select an allegiance symbol."));
        AddHotspotGadget(&g_game->menu, &rec2);

        wsprintfA(rec1.h.name, "Metal%d", i);
        rec1.h.x = 0x11e;
        rec1.h.w = 0x2d;
        rec1.h.h = 0x14;
        rec1.h.attr |= 0x10000;
        rec1.f136 = 0;
        rec1.f138 = 0;
        // Explicit store, not redundant: the extra use of the zero keeps it in EBX.
        rec1.entry = 0;
        BindGadgetAnimSequence(&rec1, "skirmmet");
        strcpy(rec1.text, Translate("Left click to increase metal. Right click to decrease metal."));
        AddButtonGadget(&g_game->menu, &rec1);

        wsprintfA(rec1.h.name, "Energy%d", i);
        rec1.h.x = 0x151;
        rec1.h.w = 0x2d;
        BindGadgetAnimSequence(&rec1, "skirmmet");
        strcpy(rec1.text, Translate("Left click to increase energy. Right click to decrease energy."));
        AddButtonGadget(&g_game->menu, &rec1);
        y += step;
        ++i;
    }
}

int __stdcall SetButtonStageByName(Menu* menu, char* name, int value);

// Skirmish setup screen refresh: fills every player/game-option gadget with the
// current lobby state (player name, side, allies, metal, energy, colour, the
// start-location and commander-death rules, mapping and line of sight), then
// refreshes the menu and stores the map name.
// FUNCTION: 0x47a0e0
void RefreshSkirmishSetup()
{
    void* teamIcons;
    char buf[0x40];
    char num[0x40];
    int index;

    Gadget* entries = g_game->menu.holder->entries;
    g_game->table->field_220 = entries[0].field_b6;
    BuildSkirmishPlayerRows();

    int empty = 1;
    if (g_game->playerCount > 0) {
        Player* p = g_game->table->players;
        int n = g_game->playerCount;
        do {
            if (p->active != 0)
                empty = 0;
            p++;
        } while (--n != 0);
    }
    if (empty != 0) {
        g_game->table->players[0].active = 1;
        g_game->table->players[1].active = 2;
    }

    unsigned short* icons =
        (unsigned short*)FindGafEntry(((Backdrop*)entries)->gaf, "TEAMICONSx");
    teamIcons = icons;
    if (icons != 0) {
        int j = 0;
        if (*icons > 0) {
            do {
                short* f = (short*)GetGafFrame(icons, 0);
                f[3] = 0;
                f[2] = 0;
                j++;
            } while (j < (int)*icons);
        }
    }

    {
        int i;
        for (i = 0; i < g_game->playerCount; i++) {
            wsprintfA(buf, "Player%d", i);
            int type = g_game->table->players[i].active;
            switch (type) {
            case 2:
                SetTranslatedTextByName(&g_game->menu, buf, Translate("Computer"), 0);
                break;
            case 1:
                SetTranslatedTextByName(&g_game->menu, buf, Translate("Player"), 0);
                break;
            case 0:
                SetTranslatedTextByName(&g_game->menu, buf, "Open", 0);
                SetGadgetActiveByName(&g_game->menu, buf, 1);
                wsprintfA(buf, "Side%d", i);
                SetGadgetActiveByName(&g_game->menu, buf, 0);
                wsprintfA(buf, "Allies%d", i);
                SetGadgetActiveByName(&g_game->menu, buf, 0);
                wsprintfA(buf, "Metal%d", i);
                SetGadgetActiveByName(&g_game->menu, buf, 0);
                wsprintfA(buf, "Energy%d", i);
                SetGadgetActiveByName(&g_game->menu, buf, 0);
                wsprintfA(buf, "Color%d", i);
                SetGadgetActiveByName(&g_game->menu, buf, 0);
                break;
            }

            wsprintfA(buf, "Side%d", i);
            SetButtonStageByName(&g_game->menu, buf, g_game->table->players[i].shade);

            wsprintfA(buf, "Metal%d", i);
            _itoa(g_game->table->players[i].metal, num, 10);
            SetTranslatedTextByName(&g_game->menu, buf, num, 0);

            wsprintfA(buf, "Energy%d", i);
            _itoa(g_game->table->players[i].energy, num, 10);
            SetTranslatedTextByName(&g_game->menu, buf, num, 0);

            wsprintfA(buf, "Color%d", i);
            index = FindGadgetIndex(entries, buf, 6);
            if (index != -1) {
                Gadget* gadget = &entries[index];
                if (gadget != 0) {
                    gadget->field_be = g_game->colorCount;
                    gadget->field_c6 = g_game->table->players[i].color;
                }
            }

            wsprintfA(buf, "Allies%d", i);
            index = FindGadgetIndex(entries, buf, 6);
            if (index != -1) {
                Gadget* gadget = &entries[index];
                if (gadget != 0) {
                    gadget->field_c6 = 10;
                    gadget->field_be = teamIcons;
                }
            }
        }
    }

    RefreshAllyIcons();

    index = FindGadgetIndex(entries, "StartLocation", 1);
    {
        Gadget* g = &entries[index];
        if (g_game->table->field_118 != 0) {
            g->field_137 = 0;
            strcpy(g->text, Translate("Commanders are placed at pre-determined locations."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, Translate("Commanders are randomly placed on the battle field."));
        }
    }

    index = FindGadgetIndex(entries, "CommanderDeath", 1);
    {
        Gadget* g = &entries[index];
        if (g_game->table->field_108 != 0) {
            g->field_137 = 0;
            strcpy(g->text, Translate("Game ends when commander is destroyed."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, Translate("Game continues after Commander is destroyed."));
        }
    }

    index = FindGadgetIndex(entries, "Mapping", 1);
    {
        Gadget* g = &entries[index];
        if (g_game->table->field_10c != 0) {
            g->field_137 = 0;
            strcpy(g->text, Translate("Terrain is blacked out until explored."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, Translate("Terrain is visible."));
        }
    }

    index = FindGadgetIndex(entries, "LineOfSight", 1);
    {
        Gadget* g = &entries[index];
        if (g_game->table->field_110 == 0) {
            g->field_137 = 0;
            strcpy(g->text, Translate("All mapped terrain is visible."));
        } else if (g_game->table->field_114 == 1) {
            g->field_137 = 1;
            strcpy(g->text, Translate("Terrain elevations affect a unit's view."));
        } else {
            g->field_137 = 2;
            strcpy(g->text, Translate("Terrain elevations do not affect a unit's view."));
        }
    }

    SetTranslatedTextByName(&g_game->menu, "MapName", g_game->table->mapName, 0);
    MarkChanged(&g_game->menu);
}

// Searches the player table from index `start` for the first active player
// on the same side as `self` (type 5 excluded), stopping at `self` itself.
// Returns that index, or -1 when the table ends first.
// FUNCTION: 0x47a700
int __stdcall FindNextAllySlot(int self, int start)
{
    Player* players = g_game->table->players;
    int n = g_game->playerCount;
    if (start != n) {
        for (int i = start; i < n; i++) {
            if (players[i].team == players[self].team && players[i].active != 0 && players[i].team != 5)
                return i;
            if (i == self)
                return i;
        }
    }
    return -1;
}

void __stdcall SetupPlayerSlot(unsigned char player, unsigned char kind);

// Marks, for each active player (active 1 or 2), every player slot that shares
// its team type (or is the player itself), in the entry's `marks` array.
// FUNCTION: 0x47a760
void ApplySlotsToGamePlayers()
{
    for (int i = 0; i < g_game->playerCount; i++) {
        if (g_game->table->players[i].active == 1) {
            g_game->slots[i].unit->slot = g_game->table->players[i].color;
            g_game->slots[i].unit->isCore = g_game->table->players[i].shade;
            SetupPlayerSlot(i, 1);
            g_game->playerIndex = i;
            g_game->localPlayer = i;
        } else if (g_game->table->players[i].active == 2) {
            g_game->slots[i].unit->slot = g_game->table->players[i].color;
            g_game->slots[i].unit->isCore = g_game->table->players[i].shade;
            SetupPlayerSlot(i, 2);
        } else {
            SetupPlayerSlot(i, 0);
        }
        if (g_game->table->players[i].active == 1 || g_game->table->players[i].active == 2) {
            int j = 0;
            for (;;) {
                Player* players = g_game->table->players;
                int n = g_game->playerCount;
                int k;
                int ii;
                // Out-of-line found path via gotos: lays the found block after the ret.
                if (j != n) {
                    for (ii = j; ii < n; ii++) {
                        if (players[ii].team == players[i].team && players[ii].active != 0 && players[ii].team != 5)
                            goto found;
                        if (ii == i)
                            goto found;
                    }
                }
                k = -1;
                goto done;
            found:
                k = ii;
            done:
                if (k == -1)
                    break;
                g_game->slots[i].marks[k] = 1;
                j = k + 1;
            }
        }
    }
}

// Advances the current player's cyclic counter: counter = (counter + 1) % n.
// FUNCTION: 0x47a8e0
void __cdecl CycleCurrentPlayerSide()
{
    Table* base = g_game->table;
    int* p = &base->players[base->current].shade;
    *p = (*p + 1) % g_game->sideCount;
}

#include "../map/mission.h"

void __cdecl FUN_004d85a0(void* ptr);
void __stdcall PlaySoundByName(const char* name, int param_2);
int __stdcall IsCurrentGadgetNamed(Menu* menu, char* name);
Gadget* __stdcall FindGadgetChecked(Gadget* entries, char* name);
Gadget* __stdcall FUN_004a0280(Gadget* entries, char* name);
void __stdcall FUN_004a0e00(Menu* menu, char* name, char* value);
void __stdcall ClearSelectedGadget(Menu* menu);
char* __stdcall SkipTextLines(char* text, int line);

// Menu gadget callback. With no gadget selected it frees the MAPPIC bitmap and
// the map list object; for MAPNAMES or LOAD it clicks, copies the picked line
// out of the MAPNAMES list into the current player and refreshes "MapName".
// PREVMENU just clicks, anything else clears the selection.
// FUNCTION: 0x47a910
void __stdcall HandleSkirmishMapClick(Menu* menu)
{
    char buffer[0x100];
    Layer* holder = menu->holder;
    Data* list = holder->data;
    Gadget* entries = holder->entries;

    if (menu->current == -1) {
        Gadget* pic = FUN_004a0280(g_game->menu.holder->entries, "MAPPIC");
        if (pic->items != 0) {
            FUN_004d85a0(pic->items);
            pic->items = 0;
        }
        FUN_004d85a0(list->items);
        FUN_004d85a0(list);
        return;
    }
    if (!IsCurrentGadgetNamed(menu, "MAPNAMES") && !IsCurrentGadgetNamed(menu, "LOAD")) {
        if (IsCurrentGadgetNamed(menu, "PREVMENU")) {
            PlaySoundByName("Previous", 0);
        } else {
            ClearSelectedGadget(menu);
        }
        return;
    }
    if (IsCurrentGadgetNamed(menu, "LOAD")) {
        PlaySoundByName("SmallButton", 0);
    }
    Gadget* g = FindGadgetChecked(entries, "MAPNAMES");
    strncpy(g_game->table->mapName, SkipTextLines(g->items, g->selected), 0x100);
    g_game->mission->LoadMissionByName(g_game->table->mapName);
    strncpy(buffer, g_game->table->mapName, 0x100);
    FUN_004a0e00(menu, "MapName", buffer);
}

void ShowSelectedMapInfo();

// Like 0x444c40, without the MAPPIC update.
// FUNCTION: 0x47aaa0
void __stdcall UpdateSkirmishMapSelection(Menu* menu, int unused)
{
    Gadget* g = FindGadgetChecked(menu->holder->entries, "MAPNAMES");
    if (g_game->mission->LoadMissionByName(SkipTextLines(g->items, g->selected)) != 0) {
        ShowSelectedMapInfo();
    }
}

int __stdcall LoadMapList(char** out, int param_2, int param_3);
void __stdcall OpenMessageBox(Menu* menu, char* text, int width, int a, int b);
Layer* __stdcall LoadGuiLayer(Menu* menu, const char* name, int flags);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall SortFileList(char* items, int b, int c, int count);
void __stdcall ConfigureListBoxByName(Menu* menu, char* name, char* items, int count, int flag);
void __stdcall SetListBoxScrollByName(Menu* menu, char* name, int index);
void __stdcall SetKeyboardInput(Menu* menu, int value);
void __stdcall RenderLayer(Menu* menu, int value);
void __stdcall SetCursorMode(int value);

// Opens the skirmish map selector (SELMAP.GUI): counts the skirmish maps,
// fills the MAPNAMES list with them, selects the one the current player
// already has, then applies the selection the way the MAPNAMES callback does.
// FUNCTION: 0x47aaf0
void OpenSkirmishMapSelector()
{
    int n = LoadMapList(0, 0, 0);
    if (n == 0) {
        OpenMessageBox(&g_game->menu,
                     Translate("There are no skirmish maps to choose from"),
                     0x140, 1, 1);
        return;
    }
    Layer* layer = LoadGuiLayer(&g_game->menu, "SELMAP.GUI", 0x880);
    layer->handler = HandleSkirmishMapClick;
    Data* data = (Data*)FUN_004d83b0("SELECT MAP DATA", 0x20);
    layer->data = data;
    LoadPictureCached("DSELECTMAP2", 0, 0, 0);
    LoadMapList(&data->items, 0, 0);
    SortFileList(data->items, 0, 0, n);
    ConfigureListBoxByName(&g_game->menu, "MAPNAMES", data->items, n, 0);
    FindGadgetChecked(layer->entries, "MAPNAMES")->onSelect = UpdateSkirmishMapSelection;

    for (int i = 0; i < n; i++) {
        if (strcmp(g_game->table->mapName, SkipTextLines(data->items, i)) == 0) {
            SetListBoxScrollByName(&g_game->menu, "MAPNAMES", i);
            break;
        }
    }

    Gadget* g = FindGadgetChecked(g_game->menu.holder->entries, "MAPNAMES");
    if (g_game->mission->LoadMissionByName(SkipTextLines(g->items, g->selected)) != 0) {
        ShowSelectedMapInfo();
    }
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    SetCursorMode(0x13);
}

// FUNCTION: 0x47acb0
void __stdcall SelectLogosGadget(Menu* obj, void* unused)
{
    Layer* ptr = obj->holder;
    Gadget* val = ptr->entries;
    int result = FindGadgetIndex(val, "LOGOS", 2);
    obj->current = result;
}

// Steps the colour of the current player one step round the colour wheel (a
// non-zero argument walks backwards, the signed modulo can produce -1 which is
// then wrapped to the last colour) and keeps stepping while another active
// player still wears that colour, then refreshes the matching "Color<n>"
// gadget and marks the menu for redraw.
//
// The loop's test: an inlined helper keeps its two results in eax, which is
// what the original's `xor eax,eax` / `mov eax,1` merge shows.
static inline int color_taken(int me)
{
    int j;
    for (j = 0; j < g_game->playerCount; j++) {
        if (g_game->table->players[j].color == g_game->table->players[me].color &&
            g_game->table->players[j].active != 0 && j != me)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x47acd0
void __stdcall CyclePlayerColor(int param_1)
{
    char buf[0x40];
    Gadget* entries = g_game->menu.holder->entries;
    int current = g_game->table->current;

    do {
        g_game->table->players[current].color =
            (g_game->table->players[current].color + (param_1 ? -1 : 1)) % *g_game->colorCount;
        if (g_game->table->players[current].color == -1)
            g_game->table->players[current].color = *g_game->colorCount - 1;
    } while (color_taken(current));

    wsprintfA(buf, "Color%d", current);
    int index = FindGadgetIndex(entries, buf, 6);
    if (index != -1) {
        Gadget* gadget = &entries[index];
        if (gadget != 0) {
            gadget->field_be = g_game->colorCount;
            gadget->field_c6 = (unsigned short)g_game->table->players[current].color;
        }
    }
    MarkChanged(&g_game->menu);
}

// Advances the current player's cyclic counter (0..5), then refreshes.
// FUNCTION: 0x47ae30
void __cdecl CycleCurrentPlayerAllyGroup()
{
    Table* base = g_game->table;
    int* p = &base->players[base->current].team;
    *p = (*p + 1) % 6;
    RefreshAllyIcons();
}

// Frame is the click handler's stack block: two scratch strings, the mouse
// event and the gadget name.
#pragma pack(push, 1)
struct Frame {
    char sA[0xc];
    char sB[0xc];
    int ev[6];
    char bf[0x40];
};
#pragma pack(pop)

void __stdcall GetGadgetName(Gadget* entries, char* text, int id);
char __stdcall FindGameCdDrive(int param_1);
void RegisterDataArchives();
void InitMissionStatus();
void SaveSettings();
void __stdcall UpdateHelpText(Menu* param_1);
void __stdcall GetCurrentMouseEvent(int* out);

// FUNCTION: 0x47ae60
void __stdcall HandleSkirmishClick(Menu* menu)
{
    Frame frame;

    Gadget* entries = menu->holder->entries;
    int cmd = menu->current;
    if (cmd == -1) {
        return;
    }
    GetGadgetName(entries, frame.bf, cmd);

    int player = atoi(&frame.bf[strlen(frame.bf) - 1]);
    Table* table = g_game->table;
    table->current = player;
    frame.bf[strlen(frame.bf) - 1] = 0;

    if (IsCurrentGadgetNamed(menu, "Start")) {
        PlaySoundByName("BigButton", 0);
        if (!FindGameCdDrive(1)) {
            OpenMessageBox(&g_game->menu,
                         Translate("Please insert the Multiplayer CD (Disc 1) and try again"),
                         0xc8, 1, 1);
            ClearSelectedGadget(&g_game->menu);
        }
        RegisterDataArchives();

        int n = 0;
        int count = g_game->playerCount;
        if (count > 0) {
            Player* p = g_game->table->players;
            do {
                if (p->active == 2)
                    n++;
                p++;
            } while (--count);
        }
        g_game->numPlayers = n + 1;

        if (g_game->mission->LoadMissionByName(g_game->table->mapName) == 0) {
            OpenMessageBox(&g_game->menu,
                         Translate("The terrain for the selected map does not exist."),
                         0x1e0, 1, 1);
            ClearSelectedGadget(menu);
            return;
        }

        int n2 = g_game->playerCount;
        int c2 = 0;
        if (n2 > 0) {
            Player* p = g_game->table->players;
            for (int i = n2; i > 0; i--) {
                if (p->active == 2)
                    c2++;
                p++;
            }
        }
        if (c2 >= 1) {
            int c1 = 0;
            if (n2 > 0) {
                Player* p = g_game->table->players;
                for (; n2 > 0; n2--) {
                    if (p->active == 1)
                        c1++;
                    p++;
                }
            }
            if (c1 >= 1) {
                int maxPlayers = g_game->mission->CountStartPositions();
                if ((int)g_game->numPlayers > maxPlayers) {
                    OpenMessageBox(&g_game->menu,
                                 Translate("There are too many players enabled for this map"),
                                 0x1e0, 1, 1);
                    ClearSelectedGadget(menu);
                    return;
                }

                if (AreAllPlayersInOneAllyGroup() != 0) {
                    OpenMessageBox(&g_game->menu,
                                 Translate("All players may not be in the same allied group."),
                                 0x1e0, 1, 1);
                    ClearSelectedGadget(menu);
                    return;
                }

                int n3 = g_game->playerCount;
                c2 = 0;
                if (n3 > 0) {
                    Player* p = g_game->table->players;
                    for (int i = n3; i > 0; i--) {
                        if (p->active == 2)
                            c2++;
                        p++;
                    }
                }
                c1 = 0;
                if (n3 > 0) {
                    Player* p = g_game->table->players;
                    for (; n3 > 0; n3--) {
                        if (p->active == 1)
                            c1++;
                        p++;
                    }
                }
                g_game->numPlayers = c1 + c2;

                ApplySlotsToGamePlayers();
                InitMissionStatus();
                SaveSettings();
                g_game->frontendSubstateRequest = 2;
                SetCursorMode(0x14);
                return;
            }
        }
        OpenMessageBox(&g_game->menu,
                     Translate("There must be at least one player and one computer opponent"),
                     0x1e0, 1, 1);
        ClearSelectedGadget(menu);
        return;
    }

    if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
        PlaySoundByName("Previous", 0);
        SetCursorMode(0x14);
        g_game->frontendSubstateRequest = 3;
        return;
    }


    // One if/else-if chain falling through to a single ClearSelectedGadget at the end:
    // no per-arm return.
    if (strcmp(frame.bf, "Player") == 0) {
        PlaySoundByName("Skirmish", 0);
        CycleSlotController(player);
    } else if (strcmp(frame.bf, "Side") == 0) {
        PlaySoundByName("Skirmish", 0);
        Table* t = g_game->table;
        int* p = &t->players[t->current].shade;
        *p = (*p + 1) % g_game->sideCount;
    } else if (strcmp(frame.bf, "Allies") == 0) {
        PlaySoundByName("Skirmish", 0);
        Table* t = g_game->table;
        int* p = &t->players[t->current].team;
        *p = (*p + 1) % 6;
        RefreshAllyIcons();
    } else if (strcmp(frame.bf, "Color") == 0) {
        PlaySoundByName("Skirmish", 0);
        GetCurrentMouseEvent(frame.ev);
        if (menu->holder->clickMode == 1) {
            CyclePlayerColor(0);
        }
        if (menu->holder->clickMode == 2) {
            CyclePlayerColor(1);
        }
    } else if (strcmp(frame.bf, "Energy") == 0) {
        GetCurrentMouseEvent(frame.ev);
        if (menu->holder->clickMode == 1) {
            PlaySoundByName("Skirmish", 0);
            Table* t = g_game->table;
            int* p = &t->players[player].energy;
            *p = min(*p + 0x1f4, 0x2710);
            Table* t2 = g_game->table;
            int* q = &t2->players[player].energy;
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa(g_game->table->players[player].energy, frame.sA, 10);
            SetTranslatedTextByName(menu, frame.sB, frame.sA, 10);
        }
        if (menu->holder->clickMode == 2) {
            PlaySoundByName("Skirmish", 0);
            Table* t = g_game->table;
            int* p = &t->players[player].energy;
            // min()/max() from windef.h on the pointee; down clamp is *p + -0x1f4.
            *p = max(*p + -0x1f4, 0xc8);
            wsprintfA(frame.sB, "Energy%d", player);
            _itoa(g_game->table->players[player].energy, frame.sA, 10);
            SetTranslatedTextByName(menu, frame.sB, frame.sA, 10);
        }
    } else if (strcmp(frame.bf, "Metal") == 0) {
        if (menu->holder->clickMode == 1) {
            PlaySoundByName("Skirmish", 0);
            Table* t = g_game->table;
            int* p = &t->players[player].metal;
            *p = min(*p + 0x1f4, 0x2710);
            Table* t2 = g_game->table;
            int* q = &t2->players[player].metal;
            if (*q == 0x2bc)
                *q = 0x1f4;
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa(g_game->table->players[player].metal, frame.sB, 10);
            SetTranslatedTextByName(menu, frame.sA, frame.sB, 10);
        }
        if (menu->holder->clickMode == 2) {
            PlaySoundByName("Skirmish", 0);
            Table* t = g_game->table;
            int* p = &t->players[player].metal;
            *p = max(*p + -0x1f4, 0xc8);
            wsprintfA(frame.sA, "Metal%d", player);
            _itoa(g_game->table->players[player].metal, frame.sB, 10);
            SetTranslatedTextByName(menu, frame.sA, frame.sB, 10);
        }
    } else if (IsCurrentGadgetNamed(menu, "CommanderDeath")) {
        PlaySoundByName("Skirmish", 0);
        Table* t = g_game->table;
        t->field_108 ^= 1;
        int index = FindGadgetIndex(entries, "CommanderDeath", 1);
        Gadget* e = &entries[index];
        if (g_game->table->field_108 != 0)
            strcpy(e->text, Translate("Game ends when commander is destroyed."));
        else
            strcpy(e->text, Translate("Game continues after Commander is destroyed."));
        UpdateHelpText(&g_game->menu);
    } else if (IsCurrentGadgetNamed(menu, "StartLocation")) {
        PlaySoundByName("Skirmish", 0);
        Table* t = g_game->table;
        t->field_118 ^= 1;
        int index = FindGadgetIndex(entries, "StartLocation", 1);
        Gadget* e = &entries[index];
        if (g_game->table->field_118 != 0)
            strcpy(e->text, Translate("Commanders are placed at pre-determined locations."));
        else
            strcpy(e->text, Translate("Commanders are randomly placed on the battle field."));
        UpdateHelpText(&g_game->menu);
    } else if (IsCurrentGadgetNamed(menu, "Mapping")) {
        PlaySoundByName("Skirmish", 0);
        Table* t = g_game->table;
        t->field_10c ^= 1;
        int index = FindGadgetIndex(entries, "Mapping", 1);
        Gadget* e = &entries[index];
        if (g_game->table->field_10c != 0)
            strcpy(e->text, Translate("Terrain is blacked out until explored."));
        else
            strcpy(e->text, Translate("Terrain is visible."));
        UpdateHelpText(&g_game->menu);
    } else if (IsCurrentGadgetNamed(menu, "LineOfSight")) {
        PlaySoundByName("Skirmish", 0);
        int index = FindGadgetIndex(entries, "LineOfSight", 1);
        Gadget* e = &entries[index];
        Table* t = g_game->table;
        if (t->field_110 == 0) {
            t->field_110 = 1;
            // The field_114 stores precede the strcpy.
            g_game->table->field_114 = 1;
            strcpy(e->text, Translate("Terrain elevations affect a unit's view."));
        } else if (t->field_114 == 1) {
            t->field_114 = 0;
            strcpy(e->text, Translate("Terrain elevations do not affect a unit's view."));
        } else {
            t->field_110 = 0;
            g_game->table->field_114 = 1;
            strcpy(e->text, Translate("All mapped terrain is visible."));
        }
        UpdateHelpText(&g_game->menu);
    } else if (IsCurrentGadgetNamed(menu, "SelectMap")) {
        PlaySoundByName("Skirmish", 0);
        SetCursorMode(0x14);
        OpenSkirmishMapSelector();
    } else if (IsCurrentGadgetNamed(menu, "Difficulty")) {
        // The original's typo, "SKirmish", not "Skirmish".
        PlaySoundByName("SKirmish", 0);
        int d = g_game->difficulty;
        if (d == 0) {
            g_game->table->field_228 = 1;
            g_game->difficulty = 1;
        } else if (d == 1) {
            g_game->table->field_228 = 2;
            g_game->difficulty = 2;
        } else if (d == 2) {
            g_game->table->field_228 = 0;
            g_game->difficulty = 0;
        }
    }

    ClearSelectedGadget(menu);
}

#pragma pack(push, 1)
struct CheatText {
    char unknown_0[4];
    Gadget* target;
};

struct Cheat {
    char unknown_0[0x18];
    CheatText* text;
};
#pragma pack(pop)

extern const char g_cheatFourPlayers[];
extern const char g_cheatFivePlayers[];
extern const char g_cheatSixPlayers[];
extern const char g_cheatSevenPlayers[];
extern const char g_cheatEightPlayers[];
extern const char g_cheatThreePlayers[];
extern const char g_cheatNinePlayers[];
extern const char g_cheatTenPlayers[];
extern const char g_skirmishCheatSoundName[];

void SaveNumSkirmishPlayers();
void LoadSettings();
void __stdcall SelectAdjacentGadget(Cheat*, int);

// FUNCTION: 0x47b9f0
void __stdcall HandleSkirmishCheatText(Cheat* cheat)
{
    int zero = 0;
    int code = 0;
    if (strncmp((char*)cheat->text + 0x34, g_cheatFourPlayers, 3) == 0)
        code = 4;
    else if (strncmp((char*)cheat->text + 0x35, g_cheatFivePlayers, 2) == 0)
        code = 5;
    else if (strncmp((char*)cheat->text + 0x34, g_cheatSixPlayers, 3) == 0)
        code = 6;
    else if (strncmp((char*)cheat->text + 0x33, g_cheatSevenPlayers, 4) == 0)
        code = 7;
    else if (strncmp((char*)cheat->text + 0x32, g_cheatEightPlayers, 5) == 0)
        code = 8;
    else if (strncmp((char*)cheat->text + 0x33, g_cheatThreePlayers, 4) == 0)
        code = 3;
    else if (strncmp((char*)cheat->text + 0x34, g_cheatNinePlayers, 3) == 0)
        code = 9;
    else if (strncmp((char*)cheat->text + 0x35, g_cheatTenPlayers, 2) == 0)
        code = 10;

    if (code != zero) {
        if (code == 3 || code == 4 || code == 8 || code == 9 || code == 10) {
            for (int i = 0; i < 0xf; i++)
                ((char*)cheat->text)[i + 0x28] = (char)zero;
        }
        SaveSettings();
        g_game->playerCount = code;
        SaveNumSkirmishPlayers();
        LoadSettings();
        cheat->text->target->field_b6 = g_game->table->field_220;
        RefreshSkirmishSetup();
        PlaySoundByName(g_skirmishCheatSoundName, 0);
        SelectAdjacentGadget(cheat, 1);
        MarkChanged(&g_game->menu);
    }
}

extern char g_skirmishGuiName[];
extern char g_skirmishSetupPictureName[];
extern char DAT_00502a78[];
extern char g_easyGadgetName[];
extern char g_mediumGadgetName[];
extern char g_hardGadgetName[];

void BlankScreen();
Gadget* __stdcall FindGadgetOrNull(Gadget* entries, char* name);
void __stdcall SetGadgetStatusByName(Menu* menu, char* name, int value);

// FUNCTION: 0x47bbb0
void OpenSkirmishMenu(void)
{
    Gadget* difficulty;
    Layer* dialog;

    BlankScreen();
    dialog = LoadGuiLayer(&g_game->menu, g_skirmishGuiName, 0);
    dialog->handler = HandleSkirmishClick;
    dialog->data = (Data*)g_game;
    LoadPictureCached(g_skirmishSetupPictureName, 0, 0, 0);

    g_game->difficulty = g_game->table->field_228;
    difficulty = FindGadgetOrNull(g_game->menu.holder->entries, DAT_00502a78);
    if (g_game->difficulty == 0) {
        difficulty->field_137 = 0;
        SetGadgetStatusByName(&g_game->menu, g_easyGadgetName, 1);
    }
    if (g_game->difficulty == 1) {
        difficulty->field_137 = 1;
        SetGadgetStatusByName(&g_game->menu, g_mediumGadgetName, 1);
    }
    if (g_game->difficulty == 2) {
        difficulty->field_137 = 2;
        SetGadgetStatusByName(&g_game->menu, g_hardGadgetName, 1);
    }
    MarkChanged(&g_game->menu);

    if (!g_game->mission->LoadMissionByName(g_game->table->mapName)) {
        g_game->mission->RefreshMapList(0);
        strncpy(g_game->table->mapName, g_game->mission->GetMissionName(), 0x100);
    }

    RefreshSkirmishSetup();
    g_game->menu.holder->textHandler = HandleSkirmishCheatText;
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    SetCursorMode(0x13);
}

extern char* g_forcesDestroyedTexts[];

void __stdcall AddMessage(char* text, int param_2, int param_3, unsigned char param_4);

// Announces that a player's forces were destroyed: "<Core|Arm> <random
// message>", using one of three (translated) messages.
// FUNCTION: 0x47bd70
void __stdcall AnnounceForcesDestroyed(Slot* player)
{
    char buf[200];
    const char* side = "Core";
    if (!player->unit->isCore)
        side = "Arm";
    sprintf(buf, "%s %s", side, Translate(g_forcesDestroyedTexts[(unsigned int)rand() % 3]));
    AddMessage(buf, 4, 0, player->color);
}
