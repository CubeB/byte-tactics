// Decompiled by Space Bunny Free. Names are provisional.
// Swaps two player slots, clears the first one (type 0, not active), then
// stamps field_146 of every playing slot with its own index, or 10 for the
// others.

#pragma pack(push, 1)
// A player slot: the same 0x14b byte record the game keeps in g_game->players
// (see 0x416ab0). SetType below writes this->type at +0x73.
struct Player_004453a0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

class Class_00463c60 {
public:
    void SetType(int param_1);
};

struct Game {
    char unknown_0[0x1b63];
    Player_004453a0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4453a0
void __stdcall FUN_004453a0(Player_004453a0* param_1, Player_004453a0* param_2)
{
    Player_004453a0 tmp = *param_2;
    *param_2 = *param_1;
    *param_1 = tmp;
    ((Class_00463c60*)param_1)->SetType(0);
    param_1->active = 0;
    // The original bound is i <= 10, not i < 10: the offset test is
    // "cmp eax, 0xcee; jle" (0xcee is 10 * 0x14b, the size of players), so
    // the last pass reads and writes players[10]. The table really has 11
    // slots (+0x1b63 to +0x299c), so this is the spare last slot, not an
    // overrun; this file declares only the first ten.
    for (int i = 0; i <= 10; i++) {
        Player_004453a0* p = &g_game->players[i];
        if (p->active != 0
            && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            p->field_146 = i;
        } else {
            // Re-derived: with the same pointer variable in both arms MSVC
            // keeps g_game in edx instead of ecx.
            g_game->players[i].field_146 = 10;
        }
    }
}
