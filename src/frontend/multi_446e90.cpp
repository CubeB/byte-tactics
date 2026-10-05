// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The include is needed for MSVC's base/index choice in the player addresses.
#include <string.h>

#pragma pack(push, 1)
struct PlayerInfo_00446e90 {
    char unknown_0[0x9d];
    unsigned short flags_9d;
};

struct Player_00446e90 {
    int active;
    int field_4;
    char unknown_8[0x27 - 0x8];
    PlayerInfo_00446e90* info;
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;
    char unknown_74[0x13f - 0x74];
    unsigned char alliance;
    char unknown_140[0x146 - 0x140];
    unsigned char field_146;
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00446e90 players[10];
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00452960(int, int, int, int);

static inline int IsSelectable(Player_00446e90* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && p->field_146 != 10;
}

// For every other player on the same side, marks the relation and clears
// bit 1 in player->info->flags_9d. The extra "g_game->players[i].type != 4"
// check is dead: IsSelectable already restricts type to 1, 2 or 3, so the
// condition can never be false; kept because the compiler emitted it.

// FUNCTION: 0x446e90
void __stdcall FUN_00446e90(Player_00446e90* player)
{
    if (player->alliance != 5) {
        for (int i = 0; i < 10; i++) {
            Player_00446e90* p = &g_game->players[i];
            if (g_game->players[i].active
                && IsSelectable(p)
                && g_game->players[i].type != 4
                && g_game->players[i].alliance == player->alliance
                && i != player->field_146) {
                FUN_00452960(player->field_4, p->field_4, 0, 1);
                player->info->flags_9d &= 0xfffd;
            }
        }
    }
}
