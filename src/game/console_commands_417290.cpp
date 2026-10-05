// Decompiled by Opus. Names are provisional.
// Console command: sets the brightness from the first argument (tenths).

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37f08];
    int brightness;                    // +0x37f08
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

void __stdcall FUN_004ba590(float value);
void FUN_00430f00();

// FUNCTION: 0x417290
void __stdcall FUN_00417290(Class_004b73e0* args)
{
    FUN_004ba590(args->FUN_004b73e0(1, 0) * 0.1f);
    g_game->brightness = args->FUN_004b73e0(1, 0);
    FUN_00430f00();
}
