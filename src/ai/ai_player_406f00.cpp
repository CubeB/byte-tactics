// Decompiled by Opus. Names are provisional.
// Registers the "plan", "weight" and "limit" console commands.

typedef void (__stdcall* Command_00406f00)(char* args);

void __stdcall FUN_004b7620(const char* name, Command_00406f00 fn, int flags);
void __stdcall CmdPlan(char* args);
void __stdcall CmdWeight(char* args);
void __stdcall CmdLimit(char* args);

// FUNCTION: 0x406f00
void RegisterAICommands()
{
    FUN_004b7620("plan", CmdPlan, 8);
    FUN_004b7620("weight", CmdWeight, 8);
    FUN_004b7620("limit", CmdLimit, 8);
}
