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

struct Game {
    char unknown_0[0x1b63];
    Player_00416bd0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
    char* GetArg(int index, char* fallback);
};

void __stdcall TransferEnergy(unsigned char a, int b, float c, int d);
void __stdcall TransferMetal(unsigned char a, int b, float c, int d);

// FUNCTION: 0x416bd0
void __stdcall CmdGive(CommandArgs* args)
{
    unsigned char i = args->GetIntArg(1, 0);
    if (i < 10) {
        Player_00416bd0* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            if (_strcmpi(((CommandArgs*)args)->GetArg(3, DAT_005119b8), "metal") == 0) {
                TransferEnergy(g_game->localPlayer, args->GetIntArg(1, 0),
                             (float)args->GetIntArg(2, 0), 1);
            }
            if (_strcmpi(((CommandArgs*)args)->GetArg(3, DAT_005119b8), "energy") == 0) {
                TransferMetal(g_game->localPlayer, args->GetIntArg(1, 0),
                             (float)args->GetIntArg(2, 0), 1);
            }
        }
    }
}
