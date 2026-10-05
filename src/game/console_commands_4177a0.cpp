// Decompiled by Opus. Names are provisional.

extern void* g_game;

// Command arguments.
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

struct Flags_004177a0
{
    unsigned short b0 : 1;
    unsigned short rest : 15;
};

// FUNCTION: 0x4177a0
void __stdcall CmdDrop(CommandArgs* args)
{
    int value = args->GetIntArg(1, 0);
    Flags_004177a0* f = (Flags_004177a0*)((char*)g_game + 0x37f2f);
    f->b0 = value == 0;
}
