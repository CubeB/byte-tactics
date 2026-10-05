// Decompiled by Sonnet. Names are provisional.

struct Game;
extern Game* g_game;

class Class_004b73e0 {
public:
    int FUN_004b73e0(int, int);
};

class Class_004ceb60 {
public:
    void PlayCdTrack(int index, int flag);
};

// FUNCTION: 0x4167f0
void __stdcall CmdCDPlay(Class_004b73e0* param_1)
{
    (*(Class_004ceb60**)((char*)g_game + 0x10))->PlayCdTrack(param_1->FUN_004b73e0(1, 0), 1);
}
