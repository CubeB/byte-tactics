// Decompiled by Sonnet. Names are provisional.

struct Game;
extern Game* g_game;

class CommandArgs {
public:
    int GetIntArg(int, int);
};

class Class_004ceb60 {
public:
    void PlayCdTrack(int index, int flag);
};

// FUNCTION: 0x4167f0
void __stdcall CmdCDPlay(CommandArgs* param_1)
{
    (*(Class_004ceb60**)((char*)g_game + 0x10))->PlayCdTrack(param_1->GetIntArg(1, 0), 1);
}
