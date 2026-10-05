// Decompiled by Opus. Names are provisional.

struct Game {
    char unknown_0[0x14];
    char field_14[0x2a44 - 0x14];
    unsigned short flags_2a44;
};

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

bool FUN_0046bf20();
void __stdcall FUN_004c9b70(void* param_1);

// FUNCTION: 0x450dd0
void FUN_00450dd0()
{
    if (g_game->flags_2a44 & 1) {
        if (DAT_00506dbc != 0) {
            DAT_00513000.FUN_004618a0(1);
        }
        if (!FUN_0046bf20()) {
            FUN_004c9b70(g_game->field_14);
        }
        g_game->flags_2a44 &= 0xfffe;
    }
}
