// Decompiled by Opus. Names are provisional.
// Console command: with one argument, stores it (0..100, else 0) in g_game.

extern char* g_game;

// Command arguments.
class CommandArgs {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    int GetIntArg(int index, int fallback);
};

// FUNCTION: 0x418c70
void __stdcall CmdSenderror(CommandArgs* args)
{
    if (args->count == 2) {
        int value = args->GetIntArg(1, 0);
        if (value < 0 || value > 100)
            value = 0;
        *(int*)(g_game + 0x37f35) = value;
    }
}
