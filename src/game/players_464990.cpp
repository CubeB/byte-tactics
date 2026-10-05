// Decompiled by Opus. Names are provisional.
// For each player whose type byte is set, calls FUN_00464700 on its entry,
// then calls FUN_004648e0 (compare 0x40a100).

#pragma pack(push, 1)
struct Player_00464990 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00464990 {
    char unknown_0[0x1b63];
    Player_00464990 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00464990* g_game;

void __stdcall FUN_00464700(Player_00464990* player);
void FUN_004648e0();

// A char loop counter gives the separate countdown register (edi = 10), as
// in 0x403100; the entry pointer is computed before the test.
// FUNCTION: 0x464990
void FUN_00464990()
{
    for (char i = 0; i < 10; i++) {
        Player_00464990* p = &g_game->players[i];
        if (p->type) {
            FUN_00464700(p);
        }
    }
    FUN_004648e0();
}
