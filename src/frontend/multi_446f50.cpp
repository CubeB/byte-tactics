// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_446f50 {
    char unknown_0[0x13f];
    unsigned char colour;            // +0x13f
    char unknown_140[0x14b - 0x140];
};

struct Game {
    char unknown_0[0x1b63];
    Player_446f50 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_00446e90(Player_446f50* player);
void __stdcall FUN_00452bd0(Player_446f50* player);
void FUN_00446c70();
void RefreshTeamIcons();

// FUNCTION: 0x446f50
void __stdcall CyclePlayerAlliance(int index)
{
    int colour = g_game->players[index].colour;
    Player_446f50* player = &g_game->players[index];
    FUN_00446e90(player);
    player->colour = (colour + 1) % 6;
    FUN_00452bd0(player);
    FUN_00446c70();
    RefreshTeamIcons();
}
