// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// 84.3% : the bitfield stores, the name setup, the sprintf/strcpy pair and the
// two message boxes match. Still differs, all register allocation of the same
// source shapes: (1) MSVC puts the second parameter (`flag`) in esi here but
// in edi in the original, which cascades through the loop-base and index
// registers; (2) our `i = 0` initialiser is emitted before the first call, the
// original emits it after; (3) the second (slot search) loop keeps the index
// in a register where the original writes it back to its stack slot on both
// exits. Struct fields, call sequence and statement order are otherwise the
// same.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00451220 {
    char unknown_0[0x8b];
    unsigned short field_8b;                 // +0x8b
    unsigned short field_8d;                 // +0x8d
    char unknown_8f[0x96 - 0x8f];
    unsigned char field_96;                  // +0x96
    unsigned short flag_97_0 : 1;            // +0x97
    unsigned short rest_97 : 15;
    char unknown_99[0x9b - 0x99];
    unsigned short field_9b;                 // +0x9b
    unsigned short flag_9d_0 : 1;            // +0x9d
    unsigned short rest_9d : 15;
    char unknown_9f[0xa5 - 0x9f];
    unsigned short field_a5;                 // +0xa5
    unsigned char field_a7;                  // +0xa7
    unsigned char field_a8;                  // +0xa8
};

class Class_00463c60 {
public:
    int field_0;                             // +0x0
    int field_4;                             // +0x4
    int field_8;                             // +0x8
    char unknown_c[0x21 - 0xc];
    unsigned char field_21;                  // +0x21
    char unknown_22[0x27 - 0x22];
    PlayerInfo_00451220* info;               // +0x27
    char name[16];                           // +0x2b
    char unknown_3b[0x73 - 0x3b];
    unsigned char field_73;                  // +0x73
    char unknown_74[0x14b - 0x74];
    void FUN_00463c60(int value);
};

struct Game_00451220 {
    char unknown_0[1];
    unsigned char field_1;                   // +0x1
    unsigned char field_2;                   // +0x2
    char unknown_3[0x14 - 3];
    char net_14[0x519 - 0x14];               // +0x14
    char menu[0x1b63 - 0x519];               // +0x519
    Class_00463c60 players[10];              // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    short field_2a3c;                        // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;               // +0x2a42
    char unknown_2a43[0x2bd2 - 0x2a43];
    char nickName[0x11];                     // +0x2bd2
    char passWord[0x11];                     // +0x2be3
    char unknown_2bf4[0x37f1b - 0x2bf4];
    short field_37f1b;                       // +0x37f1b
    char unknown_37f1d[0x37f1f - 0x37f1d];
    short field_37f1f;                       // +0x37f1f
};
#pragma pack(pop)

extern Game_00451220* g_game;

int __stdcall FUN_004ca6a0(void* net, unsigned long* id, char* shortName,
                           char* longName, char* name, short field_11, short field_13);
void __stdcall FUN_004abd90(void* menu, const char* text, int a, int b, int c);
char* __stdcall FUN_004c5740(const char* text);
int __cdecl FUN_004b6340();

// FUNCTION: 0x451220
int __stdcall FUN_00451220(unsigned char playerIndex, int flag)
{
    char buf[256];
    unsigned char i = 0;

    Class_00463c60* player = &g_game->players[playerIndex];
    player->FUN_00463c60(flag);

    for (; i < 10; i++) {
        if (g_game->players[i].field_73 != 0 &&
            g_game->players[i].info->flag_97_0)
            break;
    }
    int same = (i == playerIndex);

    if (flag == 1) {
        strcpy(buf, g_game->nickName);
    } else {
        sprintf(buf, "AI:%s", g_game->players[g_game->localPlayer].name);
        buf[16] = 0;
    }

    player->info->flag_97_0 = same;
    player->info->field_96 = 0xff;
    player->field_21 &= 0xfd;
    player->field_8 = FUN_004b6340();
    PlayerInfo_00451220* info = player->info;
    info->field_9b = (info->field_9b ^ ((g_game->field_2a3c ^ info->field_9b) & 0xf)) & 0x7fff;
    info->flag_9d_0 = (strlen(g_game->passWord) != 0);
    info->field_a5 = 0x64;
    info->field_8b = g_game->field_37f1b;
    info->field_8d = g_game->field_37f1f;
    info->field_a7 = g_game->field_1;
    info->field_a8 = g_game->field_2;

    int r = FUN_004ca6a0(g_game->net_14, (unsigned long*)&player->field_4,
                         buf, buf, g_game->passWord, 0, 0x50);
    if (r == 0) {
        g_game->players[playerIndex].FUN_00463c60(0);
        unsigned char j;
        for (j = 0; j < 10; j++) {
            if (g_game->players[j].field_73 != 0 &&
                g_game->players[j].info->flag_97_0)
                break;
        }
        Class_00463c60* slot = &g_game->players[j];
        if (slot->field_0 != 0 && (slot->field_73 == 1 || slot->field_73 == 2)) {
            FUN_004abd90(g_game->menu,
                FUN_004c5740("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-create the game session.\n"),
                500, 1, 1);
        }
        else {
            FUN_004abd90(g_game->menu,
                FUN_004c5740("Direct Play failed to add new player.\n\nRecommended you go to previous screen and re-join the game session.\n"),
                500, 1, 1);
        }
    }
    return r;
}
