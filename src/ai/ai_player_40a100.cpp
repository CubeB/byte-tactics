// Decompiled by Opus. Names are provisional.
// For each active player of type 2, calls InitUnitTables on its entry in
// g_playerAI, then calls FUN_004648e0.

#pragma pack(push, 1)
struct Player_0040a100 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_0040a100 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

class Class_00409470 {
public:
    void InitUnitTables();
};

extern Class_00409470* g_playerAI[];

void FUN_004648e0();

// FUNCTION: 0x40a100
void ResetAIPlayers()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0 && g_game->players[i].type == 2) {
            g_playerAI[i]->InitUnitTables();
        }
    }
    FUN_004648e0();
}
