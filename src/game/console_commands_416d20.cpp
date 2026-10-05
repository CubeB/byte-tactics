// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern void SaveSettings();

class CommandArgs {
public:
    void* GetIntArg(int a, int b);
};

// FUNCTION: 0x416d20
void __stdcall CmdIFace(void* param_1)
{
    void* result = ((CommandArgs*)param_1)->GetIntArg(1, 0);
    void* g = g_game;
    *(void**)((char*)g + 0x37efa) = result;
    SaveSettings();
}
