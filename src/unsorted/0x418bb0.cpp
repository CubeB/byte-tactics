// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Console command: writes a player's save file. Only runs with three
// arguments, needs a valid active player slot, and dumps it through
// FUN_0040c250 into the file named by argument 2.
#include <stdio.h>

#pragma pack(push, 1)
struct Player_00418bb0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_00418bb0 {
    char unknown_0[0x1b63];
    Player_00418bb0 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game_00418bb0* g_game;
extern char DAT_005119b8[];

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

class Class_004b73c0 {
public:
    char* FUN_004b73c0(int index, char* fallback);
};

void __stdcall FUN_0040c250(int player, FILE* file);

// FUNCTION: 0x418bb0
void __stdcall FUN_00418bb0(Class_004b73e0* args)
{
    if (args->count == 3) {
        unsigned char i = args->FUN_004b73e0(1, 0);
        if (i < 10) {
            Player_00418bb0* p = &g_game->players[i];
            if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
                && p->field_146 != 10) {
                FILE* f = fopen(((Class_004b73c0*)args)->FUN_004b73c0(2, DAT_005119b8), "w+b");
                if (f != 0) {
                    FUN_0040c250(args->FUN_004b73e0(1, 0), f);
                    fclose(f);
                }
            }
        }
    }
}
