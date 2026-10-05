// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Resets every player's "PLAYER%d" gadget and publishes a "LIVEPLYR%d" value
// for each occupied slot except the local player's: 1, whether the local
// player considers that slot an ally, or the flag at g_game+0x2bf1, depending
// on the chat mode at g_game+0x2bf0.
#include <stdio.h>

#pragma pack(push, 1)
// One 0x14b byte player record; the same layout other files use.
struct Player_00493ae0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x04];       // +0x04
    unsigned char state;               // +0x73
    char unknown_74[0x108 - 0x74];     // +0x74
    unsigned char allied[0x3e];        // +0x108
    char unknown_146[0x14b - 0x146];   // +0x146
};

struct Game_00493ae0 {
    char unknown_0[0x1b63];            // +0x0000
    Player_00493ae0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];// +0x2851
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bf0 - 0x2a43];// +0x2a43
    unsigned char mode;                // +0x2bf0
    unsigned char field_2bf1[10];      // +0x2bf1
};
#pragma pack(pop)

struct Object_004a0570;
struct Class_004a1080;

extern Game_00493ae0* g_game;

void __stdcall FUN_004a0570(Object_004a0570* obj, char* name, int value);
int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, char value);

// FUNCTION: 0x493ae0
void FUN_00493ae0(void)
{
    char buf[52];
    unsigned char* p = g_game->players[g_game->localPlayer].allied;
    for (int i = 0; i < 10; i++, p++) {
        sprintf(buf, "PLAYER%d", i);
        FUN_004a0570((Object_004a0570*)((char*)g_game + 0x519), buf, 0);
        unsigned char state = g_game->players[i].state;
        if (state != 0 && state != 4 && i != g_game->localPlayer) {
            sprintf(buf, "LIVEPLYR%d", i);
            int value = 0;
            unsigned char* flags = g_game->field_2bf1;
            switch (g_game->mode) {
            case 1:
                value = *p;
                break;
            case 2:
                value = (*p == 0);
                break;
            case 0:
                value = 1;
                break;
            case 3:
                value = flags[i];
                break;
            }
            FUN_004a1080((Class_004a1080*)((char*)g_game + 0x519), buf,
                         (char)value);
        }
    }
}
