// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a4b];
    unsigned short field_38a4b;        // +0x38a4b
    unsigned short field_38a4d;        // +0x38a4d
};
#pragma pack(pop)

extern Game* g_game;

char* __stdcall FUN_004c5740(char* text);
int GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall AddMessage(char* text, int param_2, int param_3, int param_4);

// FUNCTION: 0x490df0
void __stdcall SetGameSpeed(int speed, int param_2)
{
    if (speed > 0x14)
        speed = 0x14;
    if (speed < 1)
        speed = 1;
    if (speed != g_game->field_38a4b) {
        char buf[100];
        int d = speed - 10;
        if (d == 0) {
            strcpy(buf, FUN_004c5740("Game Speed Normal"));
        } else {
            sprintf(buf, "%s  %c%d\n", FUN_004c5740("Game Speed"),
                    (d > 0) ? '+' : ' ', d);
        }
        AddMessage(buf, 2, 0, 10);
    }
    g_game->field_38a4b = speed;
    g_game->field_38a4d = speed;
    if (param_2) {
        char data[3];
        data[0] = 0x19;
        data[1] = 1;
        data[2] = (char)speed;
        BroadcastPacket(GetLocalDpid(), data, 3);
    }
}
