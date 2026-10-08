// Decompiled by Claude Opus 5.5. Names are provisional.
// <windows.h> is needed for the operand order of the player address
// ([eax + ebx + 0x1b63], g_game as the base); without it MSVC swaps them.
#include <windows.h>
#include <string.h>

// The aligned frame (`and esp, -8`) comes from the default flags, no /Op: the
// unsigned-to-float conversion needs an 8-byte stack temporary.

// The campaign object at g_game+0x391e9 (see 0x435da0.cpp).
#include "../map/mission.h"

#pragma pack(push, 1)
struct PlayerData_0046c2a0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
    unsigned char side;                // +0x95
    char unknown_96;
    unsigned char field_97;            // +0x97
    char unknown_98[0x9b - 0x98];
    unsigned short bits_9b_0 : 6;      // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;      // bit 6 (mask 0x40)
    unsigned short bits_9b_7 : 9;
};

struct Player_0046c2a0 {               // 0x14b bytes
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x27 - 0x8];
    PlayerData_0046c2a0* data;         // +0x27
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char type;                // +0x73
    char unknown_74[0xac - 0x74];
    double totalEnergyProduced;        // +0xac
    double totalMetalProduced;         // +0xb4
    char unknown_bc[0xcc - 0xbc];
    double energyWasted;               // +0xcc
    double metalWasted;                // +0xd4
    char unknown_dc[0xfc - 0xdc];
    short kills;                       // +0xfc
    short losses;                      // +0xfe
    char unknown_100[0x104 - 0x100];
    short commandersKilled;            // +0x104
    short commandersLost;              // +0x106
    char allied[10];                   // +0x108
    char unknown_112[0x140 - 0x112];
    int unitsCreated;                  // +0x140
    char unknown_144[0x146 - 0x144];
    char index;                        // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_0046c2a0 {
    char unknown_0[0x1b63];
    Player_0046c2a0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x38a47 - 0x2a43];
    unsigned int ticks;                // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* campaign;                 // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bits0_3923b : 2;    // +0x3923b
    unsigned short bit2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short bit4_3923b : 1;
    unsigned short bits5_3923b : 11;
};
#pragma pack(pop)

struct PlayerInfo_0046c2a0 {           // 0x18 bytes
    char* name;                        // +0x00
    int id;                            // +0x04
    int flags;                         // +0x08
    char* side;                        // +0x0c
    int numAllies;                     // +0x10
    PlayerInfo_0046c2a0** allies;      // +0x14
};

struct Score_0046c2a0 {                // 8 bytes
    char* name;                        // +0x0
    int value;                         // +0x4
};

struct ScoreBoard_0046c2a0 {           // 0xc bytes
    int score;                         // +0x0
    int count;                         // +0x4
    Score_0046c2a0** scores;           // +0x8
};

extern Game_0046c2a0* g_game;
extern PlayerInfo_0046c2a0** DAT_0051e574;
extern ScoreBoard_0046c2a0** DAT_0051e57c;
extern char* g_sideNames[2];           // "Arm", "Core"
extern char* g_scoreNames[9];          // "Kills", "Losses", ..., "I am Winner"

// Fills the player and score-board tables AllocScoreTables allocated (handed to
// the stats DLL by ReportGameEvent) for every player in the game, and returns
// how many there are.
// FUNCTION: 0x46c2a0
int FillScoreTables()
{
    // j declared first, so that p->allied[j] is addressed [p + j], and n
    // set before the allies pointer is read: both are needed for a match.
    int j;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player_0046c2a0* p = &g_game->players[i];
        if ((p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->index != 10) || p->unitsCreated || p->active) {
            count++;
            DAT_0051e574[i]->name = p->name;
            DAT_0051e574[i]->id = p->id;
            DAT_0051e574[i]->flags = 1;
            if ((p->active && p->type == 2) || (p->active && p->type == 3 && p->data->field_94 == 2))
                DAT_0051e574[i]->flags |= 2;
            if (p->active && p->data->flag_9b_6)
                DAT_0051e574[i]->flags |= 4;
            if (p->active && (p->data->field_97 & 1))
                DAT_0051e574[i]->flags |= 8;
            DAT_0051e574[i]->side = g_sideNames[p->data->side];
            int n = 0;
            PlayerInfo_0046c2a0** allies = DAT_0051e574[i]->allies;
            PlayerInfo_0046c2a0** a = allies;
            for (j = 0; j < 10; j++) {
                if (j != i && p->allied[j]) {
                    n++;
                    *a++ = DAT_0051e574[j];
                }
            }
            DAT_0051e574[i]->numAllies = n;
            for (; n < 10; n++)
                allies[n] = 0;
            Mission* c = g_game->campaign;
            int score = (int)(g_game->ticks / 60 * c->timeMul);
            score += (int)(p->kills * c->killMul);
            DAT_0051e57c[i]->score = score;
            DAT_0051e57c[i]->count = 9;
            Score_0046c2a0* s;
            s = DAT_0051e57c[i]->scores[0];
            s->name = g_scoreNames[0];
            s->value = p->kills;
            s = DAT_0051e57c[i]->scores[1];
            s->name = g_scoreNames[1];
            s->value = p->losses;
            s = DAT_0051e57c[i]->scores[2];
            s->name = g_scoreNames[2];
            s->value = (int)p->totalEnergyProduced;
            s = DAT_0051e57c[i]->scores[3];
            s->name = g_scoreNames[3];
            s->value = (int)p->totalMetalProduced;
            s = DAT_0051e57c[i]->scores[4];
            s->name = g_scoreNames[4];
            s->value = (int)p->energyWasted;
            s = DAT_0051e57c[i]->scores[5];
            s->name = g_scoreNames[5];
            s->value = (int)p->metalWasted;
            s = DAT_0051e57c[i]->scores[6];
            s->name = g_scoreNames[6];
            s->value = p->commandersKilled;
            s = DAT_0051e57c[i]->scores[7];
            s->name = g_scoreNames[7];
            s->value = p->commandersLost;
            s = DAT_0051e57c[i]->scores[8];
            s->name = g_scoreNames[8];
            s->value = (i == g_game->localPlayer || (p->active && p->type == 2)) ? g_game->bit4_3923b : 0;
        }
    }
    return count;
}
