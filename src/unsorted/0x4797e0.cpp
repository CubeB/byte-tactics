// Decompiled by deepseek-v4.1. Names are provisional.
// The #2193 source started at 44.4% (body by deepseek-v4.1-flash, space-bunny-free, GPT-6).
// Earlier pass stopped at 44.4%; GPT-6.1-sol raised the best to 48.5% below.
// What still differs (original 1034 bytes, 0x88 frame; ours 1005 bytes, 0x8c frame):
//  1. Frame is 4 bytes too big: the anonymous `buffers` struct lands at frame+0xc with
//     three 4-byte temps below it (playerOffset, entries, players pointer), the original
//     has only two (offset at [esp+0x10], entries at [esp+0x14]) and the buffers at +0x8.
//  2. Register roles: original keeps ebp = g_game and ebx = playerIndex*0x18 (the scaled
//     index is spilled to [esp+0x10] and reloaded at 0x47984d/0x479894/0x479a71); ours
//     puts the scaled index in ebp, g_game in edx and reloads g_game per use, so the
//     switch dispatch (0x479810..0x47982d) and every [esp+N] after it differ.
//  3. The else-branch colour block: original loads the player's own colour into ebx with
//     `mov ebx,[ecx+ebx+0x14]` (ecx = players reused from the controller test) and later
//     recomputes the store address (`mov ecx,[ebp+0x29a0]; mov ebp,[esp+0x10];
//     mov [ebp+ecx+0x14],eax`); ours CSEs the colour address into a third temp and
//     spills the players pointer, then stores through `mov ecx,[esp+0x18]; mov [ecx],eax`.
//  4. In the controller==2 arm ours emits `push ebx` (ebx is the known-zero human count)
//     where the original emits the literal `push 0`.
// Tried: no local `game` alias (use g_game everywhere), a no-arg static inlined
// free-colour helper with `colour++/* return -1` shape (loop body itself matches), an
// explicit `int myColour` local instead of repeating players[playerIndex].color, both
// `<windows.h>`-only and `#pragma pack(1)` layouts, separate char[64] buffers instead of
// a struct. None removed the extra CSE slots or flipped ebp/ebx; 36.9% for the g_game-only
// rewrite and 44.1% for that rewrite plus myColour, versus 44.4% here.
// GPT-6.1-sol refinement: best is 48.5% after 13 checks, with no MATCH. Reusing
// one 64-byte buffer for all formatted names removes the separate color buffer
// and improves the baseline. Larger capacities (68/72) tie; 120-128 bytes return
// to 44.4-45.3%. The index/register and CSE-slot differences remain; this source
// now has a 0x48 frame versus the original 0x88.
// GPT-6.1-sol root retest: an 80-byte buffer kept 48.5%; restored the 64-byte best.
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
    char buffer[64];

    wsprintfA(buffer, "Player%d", playerIndex);
    {
        Game_004797e0* game = g_game;
        switch (game->players[playerIndex].controller) {
        case 0:
            game->players[playerIndex].controller = 2;
            FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Computer"), 0);
            break;
        case 1:
            game->players[playerIndex].controller = 0;
            FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Open"), 0);
            break;
        case 2: {
            int count = 0;
            for (int j = 0; j < game->numPlayers; j++) {
                if (game->players[j].controller == 1)
                    count++;
            }
            if (count == 0) {
                game->players[playerIndex].controller = 1;
                FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Player"), 0);
            } else {
                game->players[playerIndex].controller = 0;
                FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Open"), 0);
            }
        } break;
        }

        game = g_game;
        if (game->players[playerIndex].controller == 0) {
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
            int myColor = game->players[playerIndex].color;
            for (int j = 0; j < game->numPlayers; j++) {
                if (game->players[j].color == myColor && game->players[j].controller != 0 &&
                    j != playerIndex) {
                    Entry_004797e0* entries = game->holder->entries;
                    int free = FreeColour_4797e0(game);
                    game->players[playerIndex].color = free;
                    wsprintfA(buffer, "Color%d", playerIndex);
                    int idx = FUN_0049fdf0(entries, buffer, 6);
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
    }
    FUN_00479660();
}
