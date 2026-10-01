// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// mimo-v2.6-pro retry (issue #4060, 60 min box): base re-verified at 74.1%
// (1024 of 1034 bytes). Confirmed the residue is one register-allocation
// coin-flip, not compiler state: the original keeps g_game in EBP and the
// scaled index (playerIndex*24) in EBX (spilled to [esp+0x10]), while this
// file keeps the scaled index in EBP and spills g_game to [esp+0x1c]. Every
// variant this pass scored <= base: g_game-only body 44.1, cached me/players
// pointer 54.4, named int off=playerIndex*24 + me pointer 35.6, member-address
// int* ctl for the controller 37.2, register Game* game (no-op) 74.1, free
// colour written inline instead of the inlined helper 74.1. Compiler-state
// hammer (N unused extern decls, N=3..300) is byte-identical at 74.1 for every
// N, so the allocation is fixed by the source shape. The prior notes on the
// g_game/game spelling mix (local optimum) still hold: single-spelling swaps
// all regress. Likely natural construct still untried: a shape that forces
// MSVC to rematerialise the scaled index at the final colour store (splitting
// its live range) so g_game wins EBP, e.g. deriving players[playerIndex] only
// where needed rather than one kept scaled temp.
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
// deepseek-v4.1-flash pass (issue #3907, 10 min box): base re-verified at 74.1%
// (1024 of 1034 bytes), 17 check runs. Every single-spelling perturbation of the
// g_game/game mix regresses, so the kept mix is a local optimum: all body via
// game-> 30.0, else-block menus via game-> 27.8, inverted count branch 72.9,
// myColor read via game-> 52.0, final colour store via game-> 51.8, holder->entries
// via g_game-> 57.5, case-2 numPlayers via g_game-> 59.8, the four controller
// stores via g_game-> 52.4, switch/tail test/dup-loop-conditions via g_game->
// 55.3/48.0/46.2, FreeColour passed g_game 40.6, full g_game-only body with every
// local kept 44.1. Byte-neutral (all 74.1, 1024 bytes): hoisted `int mode = (count
// == 0)`, named switch selector `int ctl = ...; switch (ctl)`, and `register
// Game_004797e0* game`. The residue is therefore one allocator coin-flip (g_game
// vs the scaled index for ebp) that no single spelling reaches.
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
    int n;
    n = 0;
    do {
        int k;
        k = 0;
        while (k < game->numPlayers) { if (game->players[k].color == n) break; k = k + 1; }
        bool tmp1 = k == game->numPlayers;
        if (tmp1)
            return n;
        n = 1 + n;
    } while (10 > n);
    return -1;
}

static inline Menu_004797e0* inl0() { return &g_game->menu; }

static inline int inl1(Game_004797e0*game, int j, unsigned int myColor) { return (int)(game->players[j].color == myColor); }

// FUNCTION: 0x4797e0
void __stdcall FUN_004797e0(int playerIndex) {
    Game_004797e0* game;
    unsigned int myColor;
    Entry_004797e0* entries;
    int idx;
    Entry_004797e0* e;
    int count;
    char buffer2[64], buffer[64];

    wsprintfA(buffer, "Player%d", (int)playerIndex);
    {
        game = g_game;
        switch (game->players[playerIndex].controller) {
        case 0:
            game->players[playerIndex].controller = 2;
            FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Computer"), 0);
            break;
        case 1:
            game->players[((int)playerIndex)].controller = 0;
            FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Open"), 0);
            break;
        case 2: {
            int j;
            j = 0;
            count = 0;
            while (game->numPlayers > j) {
                if ((int)(game->players[j].controller == 1)) count = 1 + count;
                j++;
            }
            if (0 == count) {
                game->players[playerIndex].controller = 1;
                FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Player"), 0);
            } else {
                game->players[playerIndex].controller = 0;
                do FUN_004a0bf0(&g_game->menu, buffer, FUN_004c5740("Open"), 0); while (0);
            }
        } break;
        }

        if (!game->players[playerIndex].controller) {
            wsprintfA(buffer, "Player%d", ((int)playerIndex));
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
            do FUN_004a0570(&g_game->menu, buffer, 0); while (0);
        } else {
            int j = 0, same1 = j;
            j = ((int)same1);
            for (myColor = g_game->players[playerIndex].color; j < game->numPlayers; ) {
                if ((inl1(game, j, myColor)) && game->players[j].controller == 0) {
                } else {
                    if (j != ((int)playerIndex)) {
                                                                entries = game->holder->entries;
                                                                Entry_004797e0* same2 = entries;
                                                                entries = same2;
                                                                do {
                                                                    unsigned int free = FreeColour_4797e0(game);
                                                                    game->players[playerIndex].color = free;
                                                                } while (0);
                                                                wsprintfA(buffer2, "Color%d", playerIndex);
                                                                idx = FUN_0049fdf0(entries, buffer2, 6);
                                                                if (idx != -1) {
                                                                    do {
                                                                        e = &entries[idx];
                                                                        if (e != 0) {
                                                                            e->field_be = g_game->field_148db;
                                                                            e->field_c6 = (unsigned short)g_game->players[playerIndex].color;
                                                                        }
                                                                    } while (0);
                                                                }
                                                                break;
                                                            }
                }
                j = 1 + j;
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
            FUN_004a0570(inl0(), buffer, 1);
        }
    }
    FUN_00479660();
}
