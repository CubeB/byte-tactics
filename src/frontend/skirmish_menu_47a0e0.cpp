// Decompiled by deepseek-v4.1-flash, finished by longcat-2.5-preview-free. Names are provisional.
// Skirmish setup screen refresh: fills every player/game-option gadget with the
// current lobby state (player name, side, allies, metal, energy, colour, the
// start-location and commander-death rules, mapping and line of sight), then
// refreshes the menu and stores the map name.

#include <windows.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct Entry_0047a0e0 {                // GUI entry, 0x15b bytes
    char unknown_0[0x33];
    char text[0xb6 - 0x33];            // +0x33
    short field_b6;                    // +0xb6
    char unknown_b8[0xbe - 0xb8];
    void* field_be;                    // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;           // +0xc6
    char unknown_c8[0x137 - 0xc8];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Player_0047a0e0 {               // 0x18 bytes
    int active;                        // +0x00
    int shade;                         // +0x04
    int type;                          // +0x08
    int metal;                         // +0x0c
    int energy;                        // +0x10
    short color;                       // +0x14
    char unknown_16[2];
};

struct Table_0047a0e0 {
    Player_0047a0e0 players[11];       // +0x00 .. +0x108
    int field_108;                     // +0x108
    int field_10c;                     // +0x10c
    int field_110;                     // +0x110
    int field_114;                     // +0x114
    int field_118;                     // +0x118
    char mapName[0x220 - 0x11c];       // +0x11c
    int field_220;                     // +0x220
    int field_224;                     // +0x224
};

struct Holder_0047a0e0 {
    int unknown_0;
    Entry_0047a0e0* entries;           // +0x04
};

struct Menu_0047a0e0 {
    char unknown_0[0x18];
};

struct Game {
    char unknown_0[0x519];
    Menu_0047a0e0 menu;                // +0x519
    Holder_0047a0e0* holder;           // +0x531
    char unknown_535[0x29a0 - 0x535];
    Table_0047a0e0* table;             // +0x29a0
    char unknown_29a4[0x148db - 0x29a4];
    void* colorCount;                  // +0x148db
    char unknown_148df[0x38d81 - 0x148df];
    int playerCount;                   // +0x38d81
};

#pragma pack(pop)

extern Game* g_game;

struct Gaf_0047a0e0;

Gaf_0047a0e0* __stdcall FUN_004b8d40(Gaf_0047a0e0* gaf, const char* name);
int __stdcall FUN_004b7f30(unsigned short* entry, int frame);
int __stdcall FUN_0049fdf0(Entry_0047a0e0* entries, char* name, int type);
void __stdcall FUN_004a0bf0(Menu_0047a0e0* menu, char* key, char* value, int flag);
void __stdcall FUN_004a0570(Menu_0047a0e0* menu, char* name, int value);
int __stdcall FUN_004a1080(Menu_0047a0e0* menu, char* name, int value);
void __stdcall FUN_0049fa90(Menu_0047a0e0* menu);
char* __stdcall FUN_004c5740(char* text);
void FUN_00479c50();
void FUN_00479660();

// FUNCTION: 0x47a0e0
void FUN_0047a0e0()
{
    void* teamIcons;
    char buf[0x40];
    char num[0x40];
    int index;

    Entry_0047a0e0* entries = g_game->holder->entries;
    g_game->table->field_220 = entries[0].field_b6;
    FUN_00479c50();

    int empty = 1;
    if (g_game->playerCount > 0) {
        Player_0047a0e0* p = g_game->table->players;
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
        (unsigned short*)FUN_004b8d40(*(Gaf_0047a0e0**)((char*)entries + 0xc0), "TEAMICONSx");
    teamIcons = icons;
    if (icons != 0) {
        int j = 0;
        if (*icons > 0) {
            do {
                short* f = (short*)FUN_004b7f30(icons, 0);
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
                FUN_004a0bf0(&g_game->menu, buf, FUN_004c5740("Computer"), 0);
                break;
            case 1:
                FUN_004a0bf0(&g_game->menu, buf, FUN_004c5740("Player"), 0);
                break;
            case 0:
                FUN_004a0bf0(&g_game->menu, buf, "Open", 0);
                FUN_004a0570(&g_game->menu, buf, 1);
                wsprintfA(buf, "Side%d", i);
                FUN_004a0570(&g_game->menu, buf, 0);
                wsprintfA(buf, "Allies%d", i);
                FUN_004a0570(&g_game->menu, buf, 0);
                wsprintfA(buf, "Metal%d", i);
                FUN_004a0570(&g_game->menu, buf, 0);
                wsprintfA(buf, "Energy%d", i);
                FUN_004a0570(&g_game->menu, buf, 0);
                wsprintfA(buf, "Color%d", i);
                FUN_004a0570(&g_game->menu, buf, 0);
                break;
            }

            wsprintfA(buf, "Side%d", i);
            FUN_004a1080(&g_game->menu, buf, g_game->table->players[i].shade);

            wsprintfA(buf, "Metal%d", i);
            _itoa(g_game->table->players[i].metal, num, 10);
            FUN_004a0bf0(&g_game->menu, buf, num, 0);

            wsprintfA(buf, "Energy%d", i);
            _itoa(g_game->table->players[i].energy, num, 10);
            FUN_004a0bf0(&g_game->menu, buf, num, 0);

            wsprintfA(buf, "Color%d", i);
            index = FUN_0049fdf0(entries, buf, 6);
            if (index != -1) {
                Entry_0047a0e0* gadget = &entries[index];
                if (gadget != 0) {
                    gadget->field_be = g_game->colorCount;
                    gadget->field_c6 = g_game->table->players[i].color;
                }
            }

            wsprintfA(buf, "Allies%d", i);
            index = FUN_0049fdf0(entries, buf, 6);
            if (index != -1) {
                Entry_0047a0e0* gadget = &entries[index];
                if (gadget != 0) {
                    gadget->field_c6 = 10;
                    gadget->field_be = teamIcons;
                }
            }
        }
    }

    FUN_00479660();

    index = FUN_0049fdf0(entries, "StartLocation", 1);
    {
        Entry_0047a0e0* g = &entries[index];
        if (g_game->table->field_118 != 0) {
            g->field_137 = 0;
            strcpy(g->text, FUN_004c5740("Commanders are placed at pre-determined locations."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, FUN_004c5740("Commanders are randomly placed on the battle field."));
        }
    }

    index = FUN_0049fdf0(entries, "CommanderDeath", 1);
    {
        Entry_0047a0e0* g = &entries[index];
        if (g_game->table->field_108 != 0) {
            g->field_137 = 0;
            strcpy(g->text, FUN_004c5740("Game ends when commander is destroyed."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, FUN_004c5740("Game continues after Commander is destroyed."));
        }
    }

    index = FUN_0049fdf0(entries, "Mapping", 1);
    {
        Entry_0047a0e0* g = &entries[index];
        if (g_game->table->field_10c != 0) {
            g->field_137 = 0;
            strcpy(g->text, FUN_004c5740("Terrain is blacked out until explored."));
        } else {
            g->field_137 = 1;
            strcpy(g->text, FUN_004c5740("Terrain is visible."));
        }
    }

    index = FUN_0049fdf0(entries, "LineOfSight", 1);
    {
        Entry_0047a0e0* g = &entries[index];
        if (g_game->table->field_110 == 0) {
            g->field_137 = 0;
            strcpy(g->text, FUN_004c5740("All mapped terrain is visible."));
        } else if (g_game->table->field_114 == 1) {
            g->field_137 = 1;
            strcpy(g->text, FUN_004c5740("Terrain elevations affect a unit's view."));
        } else {
            g->field_137 = 2;
            strcpy(g->text, FUN_004c5740("Terrain elevations do not affect a unit's view."));
        }
    }

    FUN_004a0bf0(&g_game->menu, "MapName", g_game->table->mapName, 0);
    FUN_0049fa90(&g_game->menu);
}
