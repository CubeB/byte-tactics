// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Announces that the player with the given id has left, unless the local
// player is flagged. The index search was an inlined helper and appears twice
// in the original (see 0x44fed0, which has the same shape).
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Player_00450380 {
    char unknown_0[4];
    int id;                            // +0x4
    char unknown_8[0x22 - 8];
    unsigned char field_22;            // +0x22
    char unknown_23[0x2b - 0x23];
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char flag_73;             // +0x73
    char unknown_74[0x146 - 0x74];
    char field_146;                    // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00450380 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;
extern char* g_leftGameTexts[8];

char* __stdcall Translate(char* text);
void __stdcall AddMessage(char* text, int param_2, int param_3, char param_4);

static inline int GetPlayerField_00450380(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00450380(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            if (GetPlayerField_00450380(i) == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x450380
void __stdcall AnnouncePlayerLeft(int id)
{
    char buf[200];
    if (g_game->players[g_game->localPlayer].field_22 == 1)
        return;
    Player_00450380* p;
    if (FindPlayerIndex_00450380(id) == 10)
        p = 0;
    else
        p = &g_game->players[FindPlayerIndex_00450380(id)];
    if (p == 0)
        return;
    sprintf(buf, "%s %s", p->name, Translate(g_leftGameTexts[rand() & 7]));
    AddMessage(buf, 4, 0, p->field_146);
}
