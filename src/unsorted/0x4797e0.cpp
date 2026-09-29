// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 34.5%. Skirmish player-slot click handler. All calls, strings,
// struct offsets and the switch dispatch (sub ecx,0 / dec ecx / dec ecx
// chain) match. What still differs is register allocation throughout:
// the original keeps g_game in ebp (accessing players/num/screen as
// [ebp+0x29a0]/[ebp+0x38d81]/[ebp+0x531]) and the byte index
// playerIndex*0x18 in ebx, spilled to [esp+0x10]; ours keeps the index in
// ebp and g_game in edx, so nearly every line differs. Consequently our
// frame is 0x8c instead of 0x88 and MSVC spills the players pointer in the
// free-color block where the original reloads it via [ebp+0x29a0].
// Also, the original pushes a literal 0 as FUN_004a0bf0's 4th argument
// where ours reuses ebx (which holds count==0 there).
#include <windows.h>

#pragma pack(push, 1)
struct SkirmishPlayer_004797e0 {
    int controller;                 // +0x00
    int side;                       // +0x04
    int allyGroup;                  // +0x08
    int metal;                      // +0x0c
    int energy;                     // +0x10
    int color;                      // +0x14
};

struct Entry_004797e0 {             // 0x15b bytes
    char unknown_0[0xbe];
    int field_be;                   // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;        // +0xc6
    char unknown_c8[0x15b - 0xc8];
};

struct Layer_004797e0 {
    int unknown_0;
    Entry_004797e0* entries;        // +0x04
};

struct Game_004797e0 {
    char unknown_0[0x531];
    Layer_004797e0* layer;          // +0x531
    char unknown_535[0x29a0 - 0x535];
    SkirmishPlayer_004797e0* players; // +0x29a0
    char unknown_29a4[0x148db - 0x29a4];
    int field_148db;                // +0x148db
    char unknown_148df[0x38d81 - 0x148df];
    int numSkirmishPlayers;         // +0x38d81
};
#pragma pack(pop)

extern Game_004797e0* g_game;

void __stdcall FUN_00479660(void);
int __stdcall FUN_0049fdf0(Entry_004797e0* entries, char* name, int type);
void __stdcall FUN_004a0570(void* obj, char* name, int value);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int param_4);
char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4797e0
void __stdcall FUN_004797e0(int playerIndex)
{
    char name[64];
    char color[64];
    int ctrl;

    wsprintfA(name, "Player%d", playerIndex);
    ctrl = g_game->players[playerIndex].controller;
    switch (ctrl) {
    case 0:
        g_game->players[playerIndex].controller = 2;
        FUN_004a0bf0((char*)g_game + 0x519, name, FUN_004c5740("Computer"), 0);
        break;
    case 1:
        g_game->players[playerIndex].controller = 0;
        FUN_004a0bf0((char*)g_game + 0x519, name, FUN_004c5740("Open"), 0);
        break;
    case 2:
        int count = 0;
        for (int j = 0; j < g_game->numSkirmishPlayers; j++) {
            if (g_game->players[j].controller == 1)
                count++;
        }
        if (count == 0) {
            g_game->players[playerIndex].controller = 1;
            FUN_004a0bf0((char*)g_game + 0x519, name, FUN_004c5740("Player"), 0);
        } else {
            g_game->players[playerIndex].controller = 0;
            FUN_004a0bf0((char*)g_game + 0x519, name, FUN_004c5740("Open"), 0);
        }
        break;
    }

    if (g_game->players[playerIndex].controller == 0) {
        wsprintfA(name, "Player%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Side%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 0);
        wsprintfA(name, "Allies%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 0);
        wsprintfA(name, "Metal%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 0);
        wsprintfA(name, "Energy%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 0);
        wsprintfA(name, "Color%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 0);
    } else {
        Entry_004797e0* entries = g_game->layer->entries;
        int j = 0;
        for (; j < g_game->numSkirmishPlayers; j++) {
            if (g_game->players[j].color == g_game->players[playerIndex].color
                && g_game->players[j].controller != 0 && j != playerIndex)
                break;
        }
        if (j < g_game->numSkirmishPlayers) {
            int free = -1;
            for (int n = 0; n < 10; n++) {
                int k = 0;
                for (; k < g_game->numSkirmishPlayers; k++) {
                    if (g_game->players[k].color == n)
                        break;
                }
                if (k == g_game->numSkirmishPlayers) {
                    free = n;
                    break;
                }
            }
            g_game->players[playerIndex].color = free;
            wsprintfA(color, "Color%d", playerIndex);
            int idx = FUN_0049fdf0(entries, color, 6);
            if (idx != -1) {
                Entry_004797e0* e = &entries[idx];
                if (e != 0) {
                    e->field_be = g_game->field_148db;
                    e->field_c6 = g_game->players[playerIndex].color;
                }
            }
        }
        wsprintfA(name, "Player%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Side%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Allies%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Metal%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Energy%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
        wsprintfA(name, "Color%d", playerIndex);
        FUN_004a0570((char*)g_game + 0x519, name, 1);
    }
    FUN_00479660();
}
