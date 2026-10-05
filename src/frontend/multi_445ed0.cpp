// Decompiled by Space Bunny Free. Names are provisional.
// Pushes the local player's status flags (commander, mapping, los type,
// watching, cheating, fixed position, game open) into the GUI by name.
// SetButtonStageByName's value parameter is widened to an int here, as in 0x446450.
struct Class_004a1080;

int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
unsigned char FindHostSlot();

#pragma pack(push, 1)
struct PlayerInfo_00445ed0 {
    char unknown_0[0x9b];
    unsigned short bits_9b : 6;        // +0x9b, bits 0 to 5
    unsigned short bit6 : 1;           // bit 6
    unsigned short watching : 1;       // bit 7
    unsigned short mapping : 1;        // bit 8
    unsigned short bit9 : 1;           // bit 9
    unsigned short bit10 : 1;          // bit 10
    unsigned short commander : 2;      // bits 11 and 12
    unsigned short cheating : 1;       // bit 13
    unsigned short fixedloc : 1;       // bit 14
    unsigned short closed : 1;         // bit 15
};

struct Player_00445ed0 {
    char unknown_0[0x27];
    PlayerInfo_00445ed0* info;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];          // +0x519
    Player_00445ed0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x445ed0
void UpdateBattleRoomFlags()
{
    int i = FindHostSlot();
    if (i == 10) {
        i = g_game->localPlayer;
    }
    PlayerInfo_00445ed0* info = g_game->players[i].info;

    SetButtonStageByName((Class_004a1080*)g_game->gui, "COMMANDER", info->commander);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "MAPPING", !info->mapping);
    int los;
    if (!info->bit9) {
        los = 2;
    } else {
        los = !info->bit10;
    }
    SetButtonStageByName((Class_004a1080*)g_game->gui, "LOSTYPE", los);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "WATCHING", info->watching);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "CHEATING", info->cheating);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "FIXEDLOC", info->fixedloc);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "GAMEOPEN", !info->closed);
}
