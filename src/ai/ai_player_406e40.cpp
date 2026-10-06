// Decompiled by Opus. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Player_00406e40 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00406e40 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;
extern int g_aiCommandsEnabled;
extern char DAT_005119b8[];

class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
    char* GetArg(int index, char* fallback);
};

// 0x40-byte set (512 bits).
class UnitTypeSet {
public:
    int bits[16];
    void AddTypeOrCategory(char* text, int* out);
};

void __stdcall SetUnitLimits(int player, UnitTypeSet* set, int value, int param_4);

// FUNCTION: 0x406e40
void __stdcall CmdLimit(CommandArgs* args)
{
    if (g_aiCommandsEnabled != 0) {
        int count;
        UnitTypeSet set;
        memset(&set, 0, sizeof(set));
        set.AddTypeOrCategory(args->GetArg(1, DAT_005119b8), &count);
        int value = args->GetIntArg(2, 0);
        // A narrow index: MSVC then counts the loop down in a separate
        // register instead of testing the player offset.
        for (char i = 0; i < 10; i++) {
            if (g_game->players[i].active != 0 && g_game->players[i].type == 2) {
                SetUnitLimits(i, &set, value, count);
            }
        }
    }
}
