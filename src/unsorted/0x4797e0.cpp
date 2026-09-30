// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and deepseek-v4.1-flash, finished
// by GPT-6. Names are provisional. Partial, 44.4%. Reloads the game after controller callbacks and
// colour lookup. Free-colour helper preserves the scan; frame and register allocation still differ.
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

struct Game_004797e0 {
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

extern Game_004797e0* g_game;

void __stdcall FUN_00479660(void);
int __stdcall FUN_0049fdf0(Entry_004797e0* entries, char* name, int type);
void __stdcall FUN_004a0570(Menu_004797e0* menu, char* name, int value);
void __stdcall FUN_004a0bf0(Menu_004797e0* menu, char* key, char* text, int flag);
char* __stdcall FUN_004c5740(char* key);

static int __stdcall FreeColour_4797e0(Game_004797e0* game) {
    int n = 0;
    do {
        int k = 0;
        for (; k < game->numPlayers; ++k)
            if (game->players[k].color == n)
                break;
        if (k == game->numPlayers)
            return n;
        ++n;
    } while (n < 10);
    return -1;
}

// FUNCTION: 0x4797e0
void __stdcall FUN_004797e0(int playerIndex) {
    struct {
        char name[64];
        char colorName[64];
    } buffers;

    wsprintfA(buffers.name, "Player%d", playerIndex);
    {
        Game_004797e0* game = g_game;
        switch (game->players[playerIndex].controller) {
        case 0:
            game->players[playerIndex].controller = 2;
            FUN_004a0bf0(&g_game->menu, buffers.name, FUN_004c5740("Computer"), 0);
            break;
        case 1:
            game->players[playerIndex].controller = 0;
            FUN_004a0bf0(&g_game->menu, buffers.name, FUN_004c5740("Open"), 0);
            break;
        case 2: {
            int count = 0;
            for (int j = 0; j < game->numPlayers; j++) {
                if (game->players[j].controller == 1)
                    count++;
            }
            if (count == 0) {
                game->players[playerIndex].controller = 1;
                FUN_004a0bf0(&g_game->menu, buffers.name, FUN_004c5740("Player"), 0);
            } else {
                game->players[playerIndex].controller = 0;
                FUN_004a0bf0(&g_game->menu, buffers.name, FUN_004c5740("Open"), 0);
            }
        } break;
        }

        game = g_game;
        if (game->players[playerIndex].controller == 0) {
            wsprintfA(buffers.name, "Player%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Side%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 0);
            wsprintfA(buffers.name, "Allies%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 0);
            wsprintfA(buffers.name, "Metal%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 0);
            wsprintfA(buffers.name, "Energy%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 0);
            wsprintfA(buffers.name, "Color%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 0);
        } else {
            int myColor = game->players[playerIndex].color;
            for (int j = 0; j < game->numPlayers; j++) {
                if (game->players[j].color == myColor && game->players[j].controller != 0 &&
                    j != playerIndex) {
                    Entry_004797e0* entries = game->holder->entries;
                    int free = FreeColour_4797e0(game);
                    game->players[playerIndex].color = free;
                    wsprintfA(buffers.colorName, "Color%d", playerIndex);
                    int idx = FUN_0049fdf0(entries, buffers.colorName, 6);
                    if (idx != -1) {
                        Entry_004797e0* e = &entries[idx];
                        if (e != 0) {
                            e->field_be = g_game->field_148db;
                            e->field_c6 = (unsigned short)g_game->players[playerIndex].color;
                        }
                    }
                    break;
                }
            }
            wsprintfA(buffers.name, "Player%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Side%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Allies%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Metal%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Energy%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
            wsprintfA(buffers.name, "Color%d", playerIndex);
            FUN_004a0570(&g_game->menu, buffers.name, 1);
        }
    }
    FUN_00479660();
}
