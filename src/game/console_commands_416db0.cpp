// Decompiled by Opus. Names are provisional.

class CommandArgs {
public:
    float GetFloatArg(int index, float def);
};

extern int DAT_00511dd0;
extern int DAT_00511dd4;

// FUNCTION: 0x416db0
void __stdcall CmdContour(CommandArgs* args)
{
    DAT_00511dd0 = (int)(args->GetFloatArg(1, 0.0f) * 256.0f);
    DAT_00511dd4 = (int)(args->GetFloatArg(2, 0.75f) * 256.0f);
}
