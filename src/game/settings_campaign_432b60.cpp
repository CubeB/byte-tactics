// Decompiled by Opus. Names are provisional.
// Saves the "AllMissions" flag (bit 0 of g_game+0x38d7f) to the registry.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d7f];
    unsigned short flags_38d7f;        // +0x38d7f
};
#pragma pack(pop)

extern Game* g_game;
extern int __stdcall FUN_004b6a50(const char* param1, const char* param2, int param3);

// FUNCTION: 0x432b60
void SaveAllMissionsSetting()
{
    FUN_004b6a50("Total Annihilation", "AllMissions", g_game->flags_38d7f & 1);
}
