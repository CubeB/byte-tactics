// Decompiled by space-bunny-free. Names are provisional.
// Scans the ten player slots twice. The outer pass picks every slot that looks
// like a local player (active, type 1 or 2, field_140 set, field_22 clear); the
// inner pass then looks for a network slot (active, type 3) whose data->field_94
// is 1 and whose team (field_146) is still clear in the target's three per-team
// byte tables, and hands the pair to SendPlayerEconomy, returning 0 in that case.
// The three tables live at +0x11e, +0x129 and +0x134, eleven bytes each.
// Unused here, but changes how g_game is indexed in the loop heads.
#include <string.h>

struct PlayerData_004572a0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

#pragma pack(push, 1)
struct Player_004572a0 {
    int active;                        // +0x00
    char unknown_4[0x22 - 0x4];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    PlayerData_004572a0* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x11e - 0x74];
    unsigned char t0[11];              // +0x11e
    unsigned char t1[11];              // +0x129
    unsigned char t2[11];              // +0x134
    char unknown_13f;                  // +0x13f
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_004572a0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SleepMilliseconds(unsigned int param_1);
void __stdcall SendPlayerEconomy(Player_004572a0* from, Player_004572a0* to,
                            unsigned char param_3);

// FUNCTION: 0x4572a0
int FUN_004572a0()
{
    int result = 1;
    for (int i = 0; i < 10; i++) {
        Player_004572a0* pi = &g_game->players[i];
        if (pi->active == 0)
            continue;
        if (pi->type != 1 && pi->type != 2)
            continue;
        if (pi->field_140 == 0)
            continue;
        if (pi->field_22 != 0)
            continue;
        for (int j = 0; j < 10; j++) {
            Player_004572a0* pj = &g_game->players[j];
            if (pj->active != 0 && pj->type == 3 || pj->field_140 == 0
                || pj->field_22 != 0) {
                // Two sibling ifs, each with its own call: the jump layout and
                // registers follow the original.
                if (pj->active != 0 && pj->type == 3) {
                    if (pj->data->field_94 == 1
                        && (pi->t0[pj->field_146] == 0
                            || pi->t2[pj->field_146] == 0
                            || pi->t1[pj->field_146] == 0))
                        goto send;
                }
                if (pj->active != 0 && pj->type == 3) {
                    if (pi->t1[pj->field_146] == 0) {
                        SendPlayerEconomy(pi, pj, 1);
                        result = 0;
                    }
                }
                continue;
            send:
                SendPlayerEconomy(pi, pj, 1);
                result = 0;
            }
        }
    }
    SleepMilliseconds(0xfa);
    return result;
}
