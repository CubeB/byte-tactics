// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_00417150 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x98 - 0x74];
    float field_98;                    // +0x98
    char unknown_9c[0x146 - 0x9c];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00417150 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

// FUNCTION: 0x417150
void __stdcall CmdNoMetal(Class_004b73e0* args)
{
    unsigned char i;
    if (args->count == 1)
        i = g_game->localPlayer;
    else
        i = args->FUN_004b73e0(1, 0);
    if (i < 10) {
        Player_00417150* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->players[i].field_98 = args->FUN_004b73e0(2, 0);
        }
    }
}
