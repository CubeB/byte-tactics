// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

class Class_00435c30 {
public:
    int FUN_00435c30();
};

#pragma pack(push, 1)
struct PlayerInfo_451090 {
    char unknown_0[0x99];
    int field_99;
    int field_9d;
    int field_a1;
    int field_a5;
};

struct Player_451090 {
    char unknown_0[0x27];
    PlayerInfo_451090* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game_451090 {
    char unknown_0[1];
    unsigned char field_1;            // +0x1
    unsigned char field_2;            // +0x2
    char unknown_3[0x1b63 - 3];
    Player_451090 players[10];        // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;        // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;        // +0x2a42
    char unknown_2a43[0x2bc1 - 0x2a43];
    char gameName[0x10];              // +0x2bc1
    char unknown_2bd1[0x391e9 - 0x2bd1];
    Class_00435c30* field_391e9;      // +0x391e9
};
#pragma pack(pop)

extern Game_451090* g_game;

// FUNCTION: 0x451090
void __stdcall FUN_00451090(char* name, int* d, int* c, int* b, int* a)
{
    PlayerInfo_451090* info = g_game->players[g_game->localPlayer].info;
    char* p = (char*)info;
    p[0xa7] = g_game->field_1;
    p += 0x99;
    p[0xf] = g_game->field_2;
    unsigned short w = *(unsigned short*)(p + 2);
    p += 4;
    *(unsigned short*)(p - 2) = w ^ ((g_game->field_2a3c ^ w) & 0xf);
    p += 4;
    *d = *(int*)(p - 8);
    *c = *(int*)(p - 4);
    *b = *(int*)(p);
    *a = *(int*)(p + 4);
    memset(name, ' ', 0x20);
    name[0x1f] = 0;
    strncpy(name, g_game->gameName, 0x10);
    int src = g_game->field_391e9->FUN_00435c30();
    strncpy(name + 0x10, (char*)src, 0xf);
    char* q = name;
    int n = 0x20;
    do {
        if (*q == 0)
            *q = ' ';
        q++;
    } while (--n);
    name[0x1f] = 0;
}
