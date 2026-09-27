// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Player_00416bd0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00416bd0 {
    char unknown_0[0x1b63];
    Player_00416bd0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_00416bd0* g_game;
extern char DAT_005119b8[];

class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

class Class_004b73c0 {
public:
    char* FUN_004b73c0(int index, char* fallback);
};

void __stdcall FUN_00464c60(unsigned char a, int b, float c, int d);
void __stdcall FUN_00464b30(unsigned char a, int b, float c, int d);

// FUNCTION: 0x416bd0
void __stdcall FUN_00416bd0(Class_004b73e0* args)
{
    unsigned char i = args->FUN_004b73e0(1, 0);
    if (i < 10) {
        Player_00416bd0* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            if (_strcmpi(((Class_004b73c0*)args)->FUN_004b73c0(3, DAT_005119b8), "metal") == 0) {
                FUN_00464c60(g_game->localPlayer, args->FUN_004b73e0(1, 0),
                             (float)args->FUN_004b73e0(2, 0), 1);
            }
            if (_strcmpi(((Class_004b73c0*)args)->FUN_004b73c0(3, DAT_005119b8), "energy") == 0) {
                FUN_00464b30(g_game->localPlayer, args->FUN_004b73e0(1, 0),
                             (float)args->FUN_004b73e0(2, 0), 1);
            }
        }
    }
}
