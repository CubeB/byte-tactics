// Decompiled by Opus. Names are provisional.
// Chat command handler (compare 0x406e40): parses a set from argument 1 and
// a float from argument 2, then calls FUN_00409dc0 for every player whose
// field_74 is set.
#include <string.h>

#pragma pack(push, 1)
struct Player_00406db0 {
    int active;                        // +0x00
    char unknown_4[0x74 - 0x4];
    int field_74;                      // +0x74
    char unknown_78[0x14b - 0x78];
};

struct Game_00406db0 {
    char unknown_0[0x1b63];
    Player_00406db0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00406db0* g_game;
extern int DAT_00501774;
extern char DAT_005119b8[];

class Class_004b73c0 {
public:
    char* FUN_004b73c0(int index, char* fallback);
};

class Class_004b7410 {
public:
    float FUN_004b7410(int index, float default_val);
};

// 0x40-byte set (512 bits).
class Class_00488d30 {
public:
    int bits[16];
    void FUN_00488d30(char* text, int* out);
};

void __stdcall FUN_00409dc0(int player, Class_00488d30* set, float value, int count);

// FUNCTION: 0x406db0
void __stdcall FUN_00406db0(Class_004b7410* args)
{
    if (DAT_00501774 != 0) {
        int count;
        Class_00488d30 set;
        memset(&set, 0, sizeof(set));
        set.FUN_00488d30(((Class_004b73c0*)args)->FUN_004b73c0(1, DAT_005119b8), &count);
        float value = args->FUN_004b7410(2, 0);
        // A narrow index, as in 0x406e40: MSVC then counts the loop down in a
        // separate register.
        for (char i = 0; i < 10; i++) {
            if (g_game->players[i].field_74 != 0) {
                FUN_00409dc0(i, &set, value, count);
            }
        }
    }
}
