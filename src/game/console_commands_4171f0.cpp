// Decompiled by Opus. Names are provisional.
// Console command, twin of 0x417150 (which sets the float at +0x98): sets a
// player's float at +0x8c from the second argument. The player is the first
// argument, or the local player when there is only the command word.

#pragma pack(push, 1)
struct Player_004171f0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x8c - 0x74];
    float field_8c;                    // +0x8c
    char unknown_90[0x146 - 0x90];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_004171f0 {
    char unknown_0[0x1b63];
    Player_004171f0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_004171f0* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

// FUNCTION: 0x4171f0
void __stdcall FUN_004171f0(Class_004b73e0* args)
{
    unsigned char i;
    if (args->count == 1)
        i = g_game->localPlayer;
    else
        i = args->FUN_004b73e0(1, 0);
    if (i < 10) {
        Player_004171f0* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->players[i].field_8c = args->FUN_004b73e0(2, 0);
        }
    }
}
