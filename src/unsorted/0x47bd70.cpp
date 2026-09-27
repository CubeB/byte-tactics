// Decompiled by Opus. Names are provisional.
// Announces that a player's forces were destroyed: "<Core|Arm> <random
// message>", using one of three (translated) messages.
#include <stdio.h>
#include <stdlib.h>

struct Side_0047bd70 {
    char unknown_0[0x95];
    unsigned char isCore;              // +0x95
};

#pragma pack(push, 1)
struct Player_0047bd70 {
    char unknown_0[0x27];
    Side_0047bd70* side;               // +0x27
    char unknown_2b[0x146 - 0x2b];
    unsigned char color;               // +0x146
};
#pragma pack(pop)

extern char* DAT_00507b88[];

char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_00463ca0(char* text, int param_2, int param_3, unsigned char param_4);

// FUNCTION: 0x47bd70
void __stdcall FUN_0047bd70(Player_0047bd70* player)
{
    char buf[200];
    const char* side = "Core";
    if (!player->side->isCore)
        side = "Arm";
    sprintf(buf, "%s %s", side, FUN_004c5740(DAT_00507b88[(unsigned int)rand() % 3]));
    FUN_00463ca0(buf, 4, 0, player->color);
}
