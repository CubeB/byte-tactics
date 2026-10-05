// Decompiled by Opus. Names are provisional.
// Sets the GUI's "WATCHING" and "GAMEOPEN" controls from the local player's
// flags and marks the GUI for redraw. FUN_004a1080's value is widened as an
// int here (its own file says char; the checker compares names only).
struct Class_004a1080;
struct Class_0049fa90;

int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_0049fa90(Class_0049fa90* obj);

#pragma pack(push, 1)
struct PlayerInfo_00446450 {
    char unknown_0[0x9b];
    unsigned short bits_9b : 7;        // +0x9b
    unsigned short watching : 1;       // bit 7
    unsigned short bits_9b_8 : 7;
    unsigned short closed : 1;         // bit 15
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_00446450 {
    char unknown_0[0x27];
    PlayerInfo_00446450* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_00446450 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x446450
void FUN_00446450()
{
    PlayerInfo_00446450* info = g_game->players[g_game->localPlayer].info;
    FUN_004a1080((Class_004a1080*)g_game->gui, "WATCHING", info->watching);
    FUN_004a1080((Class_004a1080*)g_game->gui, "GAMEOPEN", !info->closed);
    FUN_0049fa90((Class_0049fa90*)g_game->gui);
}
