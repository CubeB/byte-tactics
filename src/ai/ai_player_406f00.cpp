// Decompiled by Opus. Names are provisional.
// Registers the "plan", "weight" and "limit" console commands.

typedef void (__stdcall* Command_00406f00)(char* args);

void __stdcall RegisterCommand(const char* name, Command_00406f00 fn, int flags);
void __stdcall CmdPlan(char* args);
void __stdcall CmdWeight(char* args);
void __stdcall CmdLimit(char* args);

// FUNCTION: 0x406f00
void RegisterAICommands()
{
    RegisterCommand("plan", CmdPlan, 8);
    RegisterCommand("weight", CmdWeight, 8);
    RegisterCommand("limit", CmdLimit, 8);
}
