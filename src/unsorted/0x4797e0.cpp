// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// STATUS (deepseek-v4.1-flash, issue #3686): best is 74.1% (1024 of 1034 bytes), no MATCH.
// What still differs (whole-thing register allocation, not structure): the original
// keeps g_game in EBP for the switch plus the duplicate-colour block and the scaled
// index (playerIndex*24) in EBX, spilling the scaled index to [esp+0x10] at its def
// (EBX is then reused for count, myColour and entries), and spills holder->entries
// to [esp+0x14]. This file instead spills g_game to [esp+0x1c], holds the scaled
// index in EBP and spills players/entries to [esp+0x10]/[esp+0x14]. Also two small
// spots: our count==0 branch emits `push ebx` where the original emits `push 0`, and
// our duplicate-colour scan loads myColour before the numPlayers<=0 test where the
// original sinks it after. All jump targets differ as a consequence. GPT-6.1-sol's
// retry pass for issue #3188 notes below still apply.
// GPT-6.1-sol retry pass for issue #3188: baseline and tested variants scored
// at most 74.1% (7 checker invocations, one returned no output, no MATCH).
// Keep the staged v4 below.
// Explicit controller if/else fell to 71.3%; pointer iteration and a while(1)
// free-colour loop tied the existing score. Remaining differences include
// broad register allocation shifts around the indexed player base and scan loops.
// deepseek-v4.1-flash round-6 adoption: staged candidate v4 (one of the
// g_game-spelling variants in build/scratch/0x4797e0/) scored 74.1% (1024 of
// 1034 bytes) and replaces the previous 58.9% base. Same-batch scores:
// v3 46.2, v5 72.1, v6 72.7, v7 52.6. The root-cause note below (lea vs reload
// of &players[playerIndex].color) explains why the g_game spellings win.
// deepseek-v4.1-flash pass (issue #2872): verified 58.9% (986 of 1034 bytes),
// stopped early per the fleet watchdog. The frame is add esp,0x48 vs the
// original add esp,0x88: the original has a second 64-byte buffer at fb+0x58
// (the "Color%d" string passed to FUN_0049fdf0) that our one-buffer version
// lacks; adding one has always regressed the score via spill differences
// (52.0 best with two buffers). Also ours spills game->players to [esp+0x10]
// and the address of players[playerIndex].color to [esp+0x14] where the
// original keeps the former in ecx (reloading) and rematerializes the latter.
// Best 58.9%, no MATCH. Original 1034 bytes, ours 986.
// Fixed the big register-role swap: never reassign the `game` local after the switch
// (a `game = g_game;` no-op split its live range and forced the scaled index into ebp,
// g_game into edx, and an extra stack temp). With one continuous `game` variable MSVC
// now emits ebp = g_game, ebx = 24*playerIndex, exactly like the original; 48.5 -> 58.9.
// Remaining differences:
//  1. Frame 0x48 vs original 0x88: the original has TWO 64-byte buffers (wsprintf at
//     [fb+0x18] and a second at [fb+0x58] used only for the "Color%d" passed to
//     FUN_0049fdf0). Ours has only the first. Adding a second buffer makes frame 0x8c
//     (52.0%) because ours spills three values rather than two.
//  2. Ours spills game->players to [esp+0x10] before the tail condition and keeps it
//     across the if-branch calls; the original keeps players in ecx and reloads it,
//     so no spil. Ours also materializes and spills the address of
//     players[playerIndex].color ([esp+0x14]) between the myColor read and the later
//     store; the original re-materializes that address instead.
// Tried this pass: game declared before wsprintf (36.5%), second 64-byte buffer (44.1%),
// buffer[128] with buffer+64 as the second (44.4%), v4 + second buffer (52.0%),
// tail via g_game directly (33.6%). The one-buffer core (this file) is the best.
// Earlier passes (see git history): no-alias g_game-only 36.9%, no-alias + myColor 44.1%,
// larger/smaller buffers 44-48%.
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
    char buffer2[64];

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
            int myColor = g_game->players[playerIndex].color;
            for (int j = 0; j < game->numPlayers; j++) {
                if (game->players[j].color == myColor && game->players[j].controller != 0 &&
                    j != playerIndex) {
                    Entry_004797e0* entries = game->holder->entries;
                    int free = FreeColour_4797e0(game);
                    game->players[playerIndex].color = free;
                    wsprintfA(buffer2, "Color%d", playerIndex);
                    int idx = FUN_0049fdf0(entries, buffer2, 6);
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
