// Decompiled by Opus. Names are provisional.
// Console command: takes a positive number from the first argument.

#pragma pack(push, 1)
struct Game_004173e0 {
    char unknown_0[0x38c57];
    int value;                         // +0x38c57
    char unknown_38c5b[0x38c63 - 0x38c5b];
    int valueSet;                      // +0x38c63
};
#pragma pack(pop)

extern Game_004173e0* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

void FUN_00430f00();

// FUNCTION: 0x4173e0
void __stdcall FUN_004173e0(Class_004b73e0* args)
{
    if (args->count > 1 && args->FUN_004b73e0(1, 0) > 0) {
        g_game->value = args->FUN_004b73e0(1, 0);
        g_game->valueSet = 1;
        FUN_00430f00();
    }
}
