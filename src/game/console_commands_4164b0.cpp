// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Command callback: with no arguments (token count 1) resets everything
// (FUN_00486ed0); otherwise sets the local player (FUN_00486f10) from
// argument 1. Both paths then pump the object at g_game+0x391ed.
// Compare 0x4169d0 / 0x416a30.

#pragma pack(push, 1)
struct Class_004b73e0 {
    char unknown_0[0xd0];
    int field_d0;                      // +0xd0

    int FUN_004b73e0(int param_1, int param_2);
};

class Class_004904b0 {
public:
    void FUN_004904b0();
};

struct Game {
    char unknown_0[0x391ed];
    Class_004904b0* field_391ed;       // +0x391ed
};
#pragma pack(pop)

extern Game* g_game;

void FUN_00486ed0(void);
void __stdcall FUN_00486f10(unsigned char player);

// FUNCTION: 0x4164b0
void __stdcall FUN_004164b0(Class_004b73e0* args)
{
    if (args->field_d0 == 1) {
        FUN_00486ed0();
    } else {
        FUN_00486f10(args->FUN_004b73e0(1, 0));
    }
    g_game->field_391ed->FUN_004904b0();
}
