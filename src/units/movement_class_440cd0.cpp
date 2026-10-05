// Decompiled by Opus. Names are provisional.

class Class_004358f0 {
public:
    int FUN_004358f0();
};

class Class_004373a0 {
public:
    unsigned int FUN_004373a0();
};

#pragma pack(push, 1)
struct PlayerData_00440cd0 {
    char unknown_0[0xa7];
    unsigned char field_a7;            // +0xa7
    unsigned char field_a8;            // +0xa8
    unsigned int field_a9;             // +0xa9
};

struct Player_00440cd0 {
    char unknown_0[0x27];
    PlayerData_00440cd0* data;         // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00440cd0 players[10];       // +0x1b63
    char unknown_2851[0x391e9 - 0x2851];
    Class_004358f0* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

unsigned char FindHostSlot();

// FUNCTION: 0x440cd0
int CheckMapCrc()
{
    if (!g_game->field_391e9->FUN_004358f0()) {
        return 0;
    }
    unsigned char me = FindHostSlot();
    PlayerData_00440cd0* data = 0;
    int check = 0;
    if (me != 10) {
        data = g_game->players[me].data;
        if (data->field_a7 >= 2 || (data->field_a7 == 1 && data->field_a8 >= 2)) {
            check = 1;
        }
    }
    if (!check) {
        return 1;
    }
    return ((Class_004373a0*)g_game->field_391e9)->FUN_004373a0() == data->field_a9;
}
