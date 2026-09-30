// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 52.6%. Skirmish player-slot click handler. All calls, strings, struct
// offsets, the switch dispatch and the free-colour loop shape now match. What
// still differs is register allocation and the frame: the original keeps g_game
// in ebp and the playerIndex*0x18 byte offset in ebx (spilled to [esp+0x10]),
// while this version keeps the offset in ebp and rematerialises g_game out of
// the global address, so the two spill slots and every [esp+N] frame offset
// differ. Removing the `game` local (direct g_game->) shrank the frame to 0x90
// but still gave ebp to the index and edx to g_game, scoring 36.3%; hoisting
// `game` out of the inner block scored 40.9%. Keeping the block-scoped `game`
// local is the best of the three.
// deepseek-v4.1-flash retried a direct `g_game->` rewrite (no `game` local):
// frame shrank to 0x8c (one extra slot) but g_game still landed in edx, the
// index in ebp, and it scored 34.5%; the version below remains the best.
// Target needs g_game in ebp, the byte offset in ebx (spilled to [esp+0x10]),
// and only two scalar slots before the two 64-byte buffers.
#include <windows.h>

#pragma pack(push, 1)

struct Entry_004797e0 {             // 0x15b bytes
    char unknown_0[0xbe];
    int field_be;                   // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;        // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Holder_004797e0 {
    int unknown_0;
    Entry_004797e0* entries;        // +0x04
};

struct Menu_004797e0 {
    char unknown_0[0x18];
};

struct Player_004797e0 {            // 0x18 bytes
    int controller;                 // +0x00
    int side;                       // +0x04
    int allyGroup;                  // +0x08
    int metal;                      // +0x0c
    int energy;                     // +0x10
    int color;                      // +0x14
};

struct Game_004797e0 {
    char unknown_0[0x519];
    Menu_004797e0 menu;             // +0x519
    Holder_004797e0* holder;        // +0x531
    char unknown_535[0x29a0 - 0x535];
    Player_004797e0* players;       // +0x29a0
    char unknown_29a4[0x148db - 0x29a4];
    int field_148db;                // +0x148db
    char unknown_148df[0x38d81 - 0x148df];
    int numPlayers;                 // +0x38d81
};
#pragma pack(pop)

extern Game_004797e0* g_game;

void __stdcall FUN_00479660(void);
int __stdcall FUN_0049fdf0(Entry_004797e0* entries, char* name, int type);
void __stdcall FUN_004a0570(Menu_004797e0* menu, char* name, int value);
void __stdcall FUN_004a0bf0(Menu_004797e0* menu, char* key, char* text, int flag);
char* __stdcall FUN_004c5740(char* key);

// FUNCTION: 0x4797e0
void __stdcall FUN_004797e0(int playerIndex)
{
    char name[64];
    char colorName[64];

    wsprintfA(name, "Player%d", playerIndex);
    {
    Game_004797e0* game = g_game;
    switch (game->players[playerIndex].controller) {
    case 0:
        game->players[playerIndex].controller = 2;
        FUN_004a0bf0(&g_game->menu, name, FUN_004c5740("Computer"), 0);
        break;
    case 1:
        game->players[playerIndex].controller = 0;
        FUN_004a0bf0(&g_game->menu, name, FUN_004c5740("Open"), 0);
        break;
    case 2:
        {
            int count = 0;
            for (int j = 0; j < g_game->numPlayers; j++) {
                if (game->players[j].controller == 1)
                    count++;
            }
            if (count == 0) {
                game->players[playerIndex].controller = 1;
                FUN_004a0bf0(&g_game->menu, name, FUN_004c5740("Player"), 0);
            } else {
                game->players[playerIndex].controller = 0;
                FUN_004a0bf0(&g_game->menu, name, FUN_004c5740("Open"), 0);
            }
        }
        break;
    }

    if (game->players[playerIndex].controller == 0) {
        wsprintfA(name, "Player%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "Side%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 0);
        wsprintfA(name, "Allies%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 0);
        wsprintfA(name, "Metal%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 0);
        wsprintfA(name, "Energy%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 0);
        wsprintfA(colorName, "Color%d", playerIndex);
        FUN_004a0570(&g_game->menu, colorName, 0);
    } else {
        int myColor = game->players[playerIndex].color;
        for (int j = 0; j < g_game->numPlayers; j++) {
            if (game->players[j].color == myColor
                && game->players[j].controller != 0
                && j != playerIndex) {
                Entry_004797e0* entries = g_game->holder->entries;
                int free = -1;
                int n = 0;
                while (1) {
                    int k = 0;
                    for (; k < g_game->numPlayers; k++) {
                        if (game->players[k].color == n)
                            break;
                    }
                    if (k == g_game->numPlayers) {
                        free = n;
                        break;
                    }
                    n++;
                    if (n >= 10)
                        break;
                }
                game->players[playerIndex].color = free;
                wsprintfA(colorName, "Color%d", playerIndex);
                int idx = FUN_0049fdf0(entries, colorName, 6);
                if (idx != -1) {
                    Entry_004797e0* e = &entries[idx];
                    if (e != 0) {
                        e->field_be = g_game->field_148db;
                        e->field_c6 = (unsigned short)game->players[playerIndex].color;
                    }
                }
                break;
            }
        }
        wsprintfA(name, "Player%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "Side%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "Allies%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "Metal%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(name, "Energy%d", playerIndex);
        FUN_004a0570(&g_game->menu, name, 1);
        wsprintfA(colorName, "Color%d", playerIndex);
        FUN_004a0570(&g_game->menu, colorName, 1);
    }
    }
    FUN_00479660();
}
