// Decompiled by Opus. Names are provisional.
// For each active player of type 2, calls FUN_00409470 on its entry in
// DAT_005119c0, then calls FUN_004648e0.

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
    void FUN_00409470();
};

extern Class_00409470* DAT_005119c0[];

void FUN_004648e0();

// FUNCTION: 0x40a100
void FUN_0040a100()
{
    for (int i = 0; i < 10; i++) {
        if (g_game->players[i].active != 0 && g_game->players[i].type == 2) {
            DAT_005119c0[i]->FUN_00409470();
        }
    }
    FUN_004648e0();
}
