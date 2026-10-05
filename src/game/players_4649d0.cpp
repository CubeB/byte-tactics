// Decompiled by Opus. Names are provisional.
// Calls FUN_0040b370 for each player whose field at +0x74 is set. A char
// loop counter gives the separate countdown register (ebx = 10), as in
// 0x464990.

#pragma pack(push, 1)
struct Player_004649d0 {
    char unknown_0[0x74];
    int field_74;                      // +0x74
    char unknown_78[0x14b - 0x78];
};

struct Game_004649d0 {
    char unknown_0[0x1b63];
    Player_004649d0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_004649d0* g_game;

void __stdcall FUN_0040b370(int param_1);

// FUNCTION: 0x4649d0
void FUN_004649d0()
{
    for (char i = 0; i < 10; i++) {
        if (g_game->players[i].field_74) {
            FUN_0040b370(i);
        }
    }
}
