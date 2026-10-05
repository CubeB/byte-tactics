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
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

void __stdcall SetBrightness(float value);
void SaveSettings();

// FUNCTION: 0x417290
void __stdcall CmdGamma(CommandArgs* args)
{
    SetBrightness(args->GetIntArg(1, 0) * 0.1f);
    g_game->brightness = args->GetIntArg(1, 0);
    SaveSettings();
}
