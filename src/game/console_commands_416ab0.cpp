// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_00416ab0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00416ab0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;          // +0x2a42
    unsigned char field_2a43;          // +0x2a43
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

// FUNCTION: 0x416ab0
void __stdcall CmdControl(Class_004b73e0* args)
{
    unsigned char i = args->FUN_004b73e0(1, 0);
    if (i < 10) {
        Player_00416ab0* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            g_game->field_2a42 = args->FUN_004b73e0(1, 0);
            g_game->field_2a43 = args->FUN_004b73e0(1, 0);
        }
    }
}
