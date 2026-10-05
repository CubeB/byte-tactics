// Decompiled by Opus. Names are provisional.

struct Game;
extern Game* g_game;

// Command arguments.
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

class CMemoryCache {
public:
    void FlushCache();
};

void __stdcall SetLightVector(int param_1, int param_2, int param_3);

// FUNCTION: 0x4166c0
void __stdcall CmdLight(CommandArgs* args)
{
    SetLightVector(args->GetIntArg(1, 0), args->GetIntArg(2, 0), args->GetIntArg(3, 0));
    (*(CMemoryCache**)((char*)g_game + 0x1437b))->FlushCache();
}
