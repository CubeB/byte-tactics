// Decompiled by Haiku. Names are provisional.

class CommandArgs {
public:
    int GetIntArg(int, int);
};

class Class_004ce690 {
public:
    void SetTrackCategory(int);
};

extern void* g_game;

// FUNCTION: 0x4175e0
void __stdcall CmdMusicMode(void* param_1)
{
    CommandArgs* obj = (CommandArgs*)param_1;
    int result = obj->GetIntArg(1, 0);
    void* p = *(void**)((char*)g_game + 0x10);
    ((Class_004ce690*)p)->SetTrackCategory(result);
}
