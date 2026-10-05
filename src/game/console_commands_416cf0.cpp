// Decompiled by Haiku. Names are provisional.

extern void* g_game;

class CommandArgs {
public:
    unsigned char GetIntArg(int arg1, int arg2);
};

extern void SaveSettings(void);

// FUNCTION: 0x416cf0
void __stdcall CmdScrollSpeed(void* param_1)
{
    unsigned char result = ((CommandArgs*)param_1)->GetIntArg(1, 0);
    *(unsigned char*)((char*)g_game + 0x1434d) = result;
    SaveSettings();
}
