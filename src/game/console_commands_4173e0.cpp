// Decompiled by Opus. Names are provisional.
// Console command: takes a positive number from the first argument.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38c57];
    int value;                         // +0x38c57
    char unknown_38c5b[0x38c63 - 0x38c5b];
    int valueSet;                      // +0x38c63
};
#pragma pack(pop)

extern Game* g_game;

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int GetIntArg(int index, int fallback);
};

void SaveSettings();

// FUNCTION: 0x4173e0
void __stdcall CmdFilmSpeed(CommandArgs* args)
{
    if (args->count > 1 && args->GetIntArg(1, 0) > 0) {
        g_game->value = args->GetIntArg(1, 0);
        g_game->valueSet = 1;
        SaveSettings();
    }
}
