// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38d81];
    int field_38d81;                   // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;
extern int __stdcall FUN_004b6a50(const char* param1, const char* param2, int param3);

// FUNCTION: 0x432b80
void SaveNumSkirmishPlayers()
{
    FUN_004b6a50("Total Annihilation", "NumSkirmishPlayers", g_game->field_38d81);
}
