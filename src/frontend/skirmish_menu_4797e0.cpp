// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
// Cycles a player slot's controller (open, computer, player) in the setup
// screen and refreshes that row's menu entries; a newly occupied slot whose
// colour clashes with another active player gets the first free colour.
#include <windows.h>

#pragma pack(push, 1)

struct Entry_004797e0 { // 0x15b bytes
    char unknown_0[0xbe];
    int field_be; // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6; // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Holder_004797e0 {
    int unknown_0;
    Entry_004797e0* entries; // +0x04
};

struct Menu_004797e0 {
    char unknown_0[0x18];
};

struct Player_004797e0 { // 0x18 bytes
    int controller;      // +0x00
    int side;            // +0x04
    int allyGroup;       // +0x08
    int metal;           // +0x0c
    int energy;          // +0x10
    int color;           // +0x14
};

struct Game {
    char unknown_0[0x519];
    Menu_004797e0 menu;      // +0x519
    Holder_004797e0* holder; // +0x531
    char unknown_535[0x29a0 - 0x535];
    Player_004797e0* players; // +0x29a0
    char unknown_29a4[0x148db - 0x29a4];
    int field_148db; // +0x148db
    char unknown_148df[0x38d81 - 0x148df];
    int numPlayers; // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RefreshAllyIcons(void);
int __stdcall FindGadgetIndex(Entry_004797e0* entries, char* name, int type);
void __stdcall FUN_004a0570(Menu_004797e0* menu, char* name, int value);
void __stdcall FUN_004a0bf0(Menu_004797e0* menu, char* key, char* text, int flag);
char* __stdcall Translate(char* key);

// 0x479500: the number of players with controller 1.
int __cdecl CountHumanSlots()
{
    int n = 0;
    for (int i = 0; i < g_game->numPlayers; i++) {
        if (g_game->players[i].controller == 1)
            n++;
    }
    return n;
}

// 0x479590: whether another active player uses this colour.
int __stdcall IsColorTaken(int color, int skip)
{
    for (int i = 0; i < g_game->numPlayers; i++) {
        if (g_game->players[i].color == color && g_game->players[i].controller != 0 && i != skip) {
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
        for (i = 0; i < g_game->numPlayers; i++) {
            if (g_game->players[i].color == color)
                break;
        }
        if (i == g_game->numPlayers)
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
    Entry_004797e0* entries = g_game->holder->entries;
    g_game->players[playerIndex].color = FindFreeColor();
    wsprintfA(name, "Color%d", playerIndex);
    int index = FindGadgetIndex(entries, name, 6);
    if (index != -1) {
        Entry_004797e0* e = &entries[index];
        if (e != 0) {
            e->field_be = g_game->field_148db;
            e->field_c6 = (unsigned short)g_game->players[playerIndex].color;
        }
    }
}

// FUNCTION: 0x4797e0
void __stdcall CycleSlotController(int playerIndex)
{
    char buffer[64];

    wsprintfA(buffer, "Player%d", playerIndex);
    switch (g_game->players[playerIndex].controller) {
    case 0:
        g_game->players[playerIndex].controller = 2;
        FUN_004a0bf0(&g_game->menu, buffer, Translate("Computer"), 0);
        break;
    case 1:
        g_game->players[playerIndex].controller = 0;
        FUN_004a0bf0(&g_game->menu, buffer, Translate("Open"), 0);
        break;
    case 2:
        if (CountHumanSlots() == 0) {
            g_game->players[playerIndex].controller = 1;
            FUN_004a0bf0(&g_game->menu, buffer, Translate("Player"), 0);
        } else {
            g_game->players[playerIndex].controller = 0;
            FUN_004a0bf0(&g_game->menu, buffer, Translate("Open"), 0);
        }
        break;
    }

    if (g_game->players[playerIndex].controller == 0) {
        wsprintfA(buffer, "Player%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Side%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Allies%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Metal%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Energy%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 0);
        wsprintfA(buffer, "Color%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 0);
    } else {
        if (IsColorTaken(g_game->players[playerIndex].color, playerIndex))
            NewColour(playerIndex);
        wsprintfA(buffer, "Player%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Side%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Allies%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Metal%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Energy%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
        wsprintfA(buffer, "Color%d", playerIndex);
        FUN_004a0570(&g_game->menu, buffer, 1);
    }
    RefreshAllyIcons();
}
