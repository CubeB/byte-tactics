// Decompiled by Space Bunny Free. Names are provisional.
// Picks which of the two "side" subtitles is active from the game flag at
// +0x37ef2, marks the matching sub-object and asks FUN_004a1110 to show it.

#pragma pack(push, 1)
struct Side_00477360 {
    char unknown_0[0x18];
    void* link;                         // +0x18
};

struct Sub_00477360 {
    char unknown_0[0x95];
    unsigned char side;                 // +0x95
};

struct Game_00477360 {
    char unknown_0[0x519];
    Side_00477360 sides;                // +0x519
    char unknown_535[0x1b8a - 0x535];
    Sub_00477360* slot_1b8a;            // +0x1b8a
    char unknown_1b8e[0x1cd5 - 0x1b8e];
    Sub_00477360* slot_1cd5;            // +0x1cd5
    char unknown_1cd9[0x37ef2 - 0x1cd9];
    int flag_37ef2;                     // +0x37ef2
};
#pragma pack(pop)

extern Game_00477360* g_game;

int __stdcall FUN_004a1110(Side_00477360* sides, char* name, int value);

// FUNCTION: 0x477360
void __cdecl FUN_00477360()
{
    if (g_game->flag_37ef2 == 0) {
        g_game->slot_1b8a->side = 0;
        g_game->slot_1cd5->side = 1;
        FUN_004a1110(&g_game->sides, "Arm", 1);
        FUN_004a1110(&g_game->sides, "Side0", 1);
    } else {
        g_game->slot_1b8a->side = 1;
        g_game->slot_1cd5->side = 0;
        FUN_004a1110(&g_game->sides, "Core", 1);
        FUN_004a1110(&g_game->sides, "Side1", 1);
    }
}
