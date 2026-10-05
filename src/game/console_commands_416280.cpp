// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Player_00416280 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00416280 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;          // +0x2a42
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

class Class_00463c60 {
public:
    void SetType(int param_1);
};

// FUNCTION: 0x416280
void __stdcall FUN_00416280(Class_004b73e0* args)
{
    unsigned char i = args->FUN_004b73e0(1, g_game->field_2a42);
    if (i < 10) {
        Player_00416280* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10) {
            // Re-derived pointer: MSVC then re-reads the fields instead of
            // reusing the values tested above.
            Player_00416280* q = &g_game->players[i];
            if (q->active != 0 && q->type == 1)
                ((Class_00463c60*)q)->SetType(2);
            else
                ((Class_00463c60*)q)->SetType(1);
        }
    }
}
