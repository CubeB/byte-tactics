// Decompiled by Opus. Names are provisional.
// Chat command handler (compare 0x406e40): parses a set from argument 1 and
// a float from argument 2, then calls ScaleUnitWeights for every player whose
// field_74 is set.
#include <string.h>

#pragma pack(push, 1)
struct Player_00406db0 {
    int active;                        // +0x00
    char unknown_4[0x74 - 0x4];
    int field_74;                      // +0x74
    char unknown_78[0x14b - 0x78];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00406db0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;
extern int g_aiCommandsEnabled;
extern char DAT_005119b8[];

class Class_004b73c0 {
public:
    char* GetArg(int index, char* fallback);
};

class CommandArgs {
public:
    float GetFloatArg(int index, float default_val);
};

// 0x40-byte set (512 bits).
class UnitTypeSet {
public:
    int bits[16];
    void AddTypeOrCategory(char* text, int* out);
};

void __stdcall ScaleUnitWeights(int player, UnitTypeSet* set, float value, int count);

// FUNCTION: 0x406db0
void __stdcall CmdWeight(CommandArgs* args)
{
    if (g_aiCommandsEnabled != 0) {
        int count;
        UnitTypeSet set;
        memset(&set, 0, sizeof(set));
        set.AddTypeOrCategory(((Class_004b73c0*)args)->GetArg(1, DAT_005119b8), &count);
        float value = args->GetFloatArg(2, 0);
        // A narrow index, as in 0x406e40: MSVC then counts the loop down in a
        // separate register.
        for (char i = 0; i < 10; i++) {
            if (g_game->players[i].field_74 != 0) {
                ScaleUnitWeights(i, &set, value, count);
            }
        }
    }
}
